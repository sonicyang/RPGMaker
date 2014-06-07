#include <cstdlib>
#include <string>
#include <cstring>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "gmap.h"
#include "point.h"
#include "utils.h"
#include "json/reader.h"
#include "json/value.h"
#include "render.h"
#include "engine.h"
#include "variant.h"
#include "menu.h"
#include "itemexec.h"
#include "utf8.h"

Engine::Engine(std::string manifest) :
    rdr(manifest),
    mapCtl(this),
    evtCtl(this, &varMap),
    inv(),
    team(),
    prom(this),
    battle(this, &varMap),
    mainmenu(this, varMap),
    teammenu(this, varMap),
    invmenu(this, varMap),
    charmenu(this, varMap),
    skillmenu(this, varMap),
    vendor(this, varMap),
    startmenu(this, varMap),
    helpmenu(this, varMap)
{
    std::string in = get_file_contents(manifest.c_str());

    Json::Value root;
    Json::Reader reader;
    bool stat = reader.parse( in, root );
    if (stat){
        mapCtl = mapController(root["MapList"].asString(), this);
        mapList = root["MapList"].asString();
        evtCtl = eventController(root["EventList"].asString(), this, &varMap);
        team = Team(root["TeamList"].asString(), root["RoleList"].asString());
        teamList = root["TeamList"].asString();
        roleList = root["RoleList"].asString();
        battle = Battle(root["MonsterList"].asString(), this, &varMap);
    }else{
        std::cout << "Failed to parse manifest\n"  << reader.getFormatedErrorMessages();
        exit(128);
    }
}

Engine::~Engine()
{
}

void Engine::parseUserInput(genericContorller& controller){
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        switch(e.type){
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym){
                    case SDLK_UP:
                        controller.hKeyUp();
                        break;
                    case SDLK_DOWN:
                        controller.hKeyDown();
                        break;
                    case SDLK_LEFT:
                        controller.hKeyLeft();
                        break;
                    case SDLK_RIGHT:
                        controller.hKeyRight();
                        break;
                    case SDLK_z:
                    case SDLK_RETURN:
                        controller.hKeyZ();
                        break;
                    case SDLK_x:
                        controller.hKeyX();
                        break;
                    case SDLK_q:
                    case SDLK_ESCAPE:
                        controller.hKeyQ();
                        break;
                }
                break;
            case SDL_QUIT:
                fullstop = 1;
                stop = -1;
                break;
        }
    }

}

void Engine::excute(){
    for(;!fullstop;){
        try{
            parseUserInput(startmenu);
            startmenu.hDoEvent();
            startmenu.hRender(rdr);
        }catch(int){
            stop = 0;
            mapCtl = mapController(mapList, this);
            team = Team(teamList, roleList);
            inv = inventory();
        }
    }
}

void Engine::excute(genericContorller& controller){
    for(;!stop;){
        parseUserInput(controller);
        controller.hDoEvent();
        controller.hRender(rdr);
    }
    if(stop != 1) //Nested stop signal
        throw(0);
    else
       stop = 0;
}

variant<paraVarType> Engine::engineCall(std::vector< variant<paraVarType> > params){
    char tmp[100];

    variant<paraVarType> ret;
    ret.set<int>(1);

    int commd = params[0].get<svc>();
    switch(commd){
        case svc::loadEvent:
            evtCtl.pushEvent(params[1].get<std::string>());
            break;
        case svc::popEvent:
            evtCtl.popEventStack();
            break;
        case svc::restoreStat:
            stop = 1;
            break;
        case svc::setStat:
            this->setStat(params[1].get<Stats>());
            break;
        case svc::loadPrompt:
            prom.loadMessaage(params[1].get<wchar_t*>(), params[2].get<wchar_t*>());
            this->setStat(Stats::inPrompt);
            break;
        case svc::clearPrompt:
            prom.discardMessage();
            break;
        case svc::changeMap:
            mapCtl.setCurrentMap(params[1].get<std::string>());
            mapCtl.setPlayerPosition(Point(params[2].get<int>(), params[3].get<int>()));
            break;
        case svc::addItem:
            inv.addItem(params[1].get<std::string>(), params[2].get<int>());
            sprintf(tmp, "You get %d %s", params[2].get<int>(), params[1].get<std::string>().c_str());
            engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar(tmp), UTF8_to_WChar("System")));
            break;
        case svc::removeItem:
            ret.set<int>(inv.removeItem(params[1].get<std::string>(), params[2].get<int>()));
            if(ret.get<int>() != -1){
                sprintf(tmp, "You lost %d %s", params[2].get<int>(), params[1].get<std::string>().c_str());
                engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar(tmp), UTF8_to_WChar("System")));
            }
            break;
        case svc::incItem:
            inv.incItem(params[1].get<std::string>());
            sprintf(tmp, "You get a %s", params[1].get<std::string>().c_str());
            engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar(tmp), UTF8_to_WChar("System")));
            break;
        case svc::decItem:
            ret.set<int>(inv.decItem(params[1].get<std::string>()));
            if(ret.get<int>() != -1){
                sprintf(tmp, "You lost a %s", params[1].get<std::string>().c_str());
                engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar(tmp), UTF8_to_WChar("System")));
            }
            break;
        case svc::setMoney:
            inv.setMoney(params[1].get<int>());
            sprintf(tmp, "You Currently Have $%d", inv.getMoney());
            break;
        case svc::addMoney:
            ret.set<int>(inv.addMoney(params[1].get<int>()));

            if(ret.get<int>() != -1){
                if(params[1].get<int>() > 0)
                    sprintf(tmp, "You gained $%d", params[1].get<int>());
                else if(params[1].get<int>() < 0)
                    sprintf(tmp, "You lost $%d", (-1)* params[1].get<int>());
                if(params[1].get<int>() != 0)
                    engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar(tmp), UTF8_to_WChar("System")));
            }
            break;
        case svc::addCharToTeam:
            team.addCharToTeam(params[1].get<std::string>());
            sprintf(tmp, "%s joined the Team", params[1].get<std::string>().c_str());
            engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar(tmp), UTF8_to_WChar("System")));
            break;
        case svc::removeCharFromTeam:
            team.removeCharFromTeam(params[1].get<std::string>());
            sprintf(tmp, "%s leaved the Team", params[1].get<std::string>().c_str());
            engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar(tmp), UTF8_to_WChar("System")));
            break;
        case svc::battle:
            battle.loadBattle(&team, params[1].get< std::vector< std::string> >());
            engineCall(loadStack(svc::setStat, Stats::inBattle));
            break;
        case svc::loadMainMenu:
            mainmenu.init();
            break;
        case svc::loadTeamMenu:
            teammenu.init(params[1].get<int>(), &team);
            break;
        case svc::loadInvMenu:
            invmenu.init(params[1].get<int>(), &inv);
            break;
        case svc::loadCharMenu:
            charmenu.init(params[1].get<int>(), &team[team.getNameList()[params[2].get<unsigned int>()]]);
            break;
        case svc::loadSkillMenu:
            skillmenu.init(params[1].get<int>(), &team[team.getNameList()[params[2].get<unsigned int>()]]);
            break;
        case svc::invEnableNull:
            inv.enableNull();
            break;
        case svc::invDisableNull:
            inv.disableNull();
            break;
        case svc::changeEquip:
            ret.set<int>(ItemExec::changeItem(inv, params[1].get<unsigned int>(), team, params[2].get<unsigned int>(), params[3].get<unsigned int>()));

            if(ret.get<int>() == -1){
                engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar("You Can't Wear a Shoe on Your Head!"), UTF8_to_WChar("System")));
            }
            break;
        case svc::isItemUsable:
            ret.set<int>(inv[inv.getNameList(params[1].get<unsigned int>())[0]].item.isUsable());
            break;
        case svc::getItemType:
            ret.set<int>(inv[inv.getNameList(params[1].get<unsigned int>())[0]].item.getType());
            break;
        case svc::useItem:
            ItemExec::Exec(inv, params[1].get<unsigned int>(), team, params[2].get<unsigned int>());
            sprintf(tmp, "Used %s", inv[inv.getNameList(params[1].get<unsigned int>())[0]].item.getName().c_str());
            engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar(tmp), UTF8_to_WChar("System")));
            break;
        case svc::isCharDead:
            ret.set<int>(team[team.getNameList()[params[1].get<unsigned int>()]].isDead());
            break;
        case svc::isTeamWipeOut:
            ret.set<int>(team.isWipeOut());
            break;
        case svc::queryAttack:
            ret.set<int>(team[team.getNameList()[params[1].get<unsigned int>()]].getAttack());
            break;
        case svc::queryDefense:
            ret.set<int>(team[team.getNameList()[params[1].get<unsigned int>()]].getDefense());
            break;
        case svc::varHP:
            team[team.getNameList()[params[1].get<unsigned int>()]].varHP(params[2].get<int>());
            if(params[2].get<int>() > 0)
                sprintf(tmp, "%s HP recovered by %d points", team.getNameList()[params[1].get<unsigned int>()].c_str(), params[2].get<int>());
            else if(params[2].get<int>() < 0)
                sprintf(tmp, "%s received damage by %d points", team.getNameList()[params[1].get<unsigned int>()].c_str(), (-1)* params[2].get<int>());
            if(params[2].get<int>() != 0)
                engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar(tmp), UTF8_to_WChar("System")));
            break;
        case svc::varMP:
            team[team.getNameList()[params[1].get<unsigned int>()]].varMP(params[2].get<int>());
            if(params[2].get<int>() > 0)
                sprintf(tmp, "%s MP increased by %d points", team.getNameList()[params[1].get<unsigned int>()].c_str(), params[2].get<int>());
            else if(params[2].get<int>() < 0)
                sprintf(tmp, "%s MP decreased by %d points", team.getNameList()[params[1].get<unsigned int>()].c_str(), (-1)* params[2].get<int>());
            if(params[2].get<int>() != 0)
                engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar(tmp), UTF8_to_WChar("System")));
            break;
        case svc::varExp:
            ret.set<int>(team[team.getNameList()[params[1].get<unsigned int>()]].varExp(params[2].get<int>()));
            if(params[2].get<int>() > 0)
                sprintf(tmp, "%s experience increased by %d points", team.getNameList()[params[1].get<unsigned int>()].c_str(), params[2].get<int>());
            else if(params[2].get<int>() < 0)
                sprintf(tmp, "%s experience decreased by %d points", team.getNameList()[params[1].get<unsigned int>()].c_str(), (-1)* params[2].get<int>());
            if(params[2].get<int>() != 0)
                engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar(tmp), UTF8_to_WChar("System")));
            if(ret.get<int>() > 0){
                sprintf(tmp, "%s level up by %d level(s)", team.getNameList()[params[1].get<unsigned int>()].c_str(), ret.get<int>());
                engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar(tmp), UTF8_to_WChar("System")));
            }
            break;
        case svc::moveVar:
            varMap[params[1].get<std::string>()].set<int>(varMap[params[2].get<std::string>()].get<int>());
            break;
        case svc::qureySkill:
            ret.set<Skill>(team[team.getNameList()[params[1].get<unsigned int>()]].getSkillList()[params[2].get<unsigned int>()]);
            break;
        case svc::useSkill:
            ret.set<int>(ItemExec::skillExec(team, params[1].get<unsigned int>(), params[2].get<unsigned int>(), battle.getMonsters(), params[3].get<unsigned int>(), params[4].get<unsigned int>()));
            break;
        case svc::setupVender:
            vendor.setUp(params[1].get<std::vector<std::string> >());
            break;
        case svc::loadVenderInvMenu:
            invmenu.init(params[1].get<int>(), &(vendor.getVenderInv()));
            vendor.getVenderInv().setMoney(inv.getMoney());
            break;
        case svc::sellItem:
            engineCall(loadStack(svc::addMoney, inv[inv.getNameList(params[1].get<unsigned int>())[0]].item.getSellPrice()));
            engineCall(loadStack(svc::decItem, inv.getNameList(params[1].get<unsigned int>())[0]));
            break;
        case svc::gameOver:
            rdr.render_gameOver();
            stop = -1;
            ret.set<int>(-1);
            break;
        case svc::endGame:
            stop = -1;
            ret.set<int>(0);
            break;
        case loadGame:
            mapCtl.load(get_file_contents("data/map.sav"));
            inv.load(get_file_contents("data/inv.sav"));
            team.load(get_file_contents("data/team.sav"));

            engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar("Game Loaded!"), UTF8_to_WChar("System")));
            break;
        case saveGame:{
            std::ofstream map_s("data/map.sav");
            std::ofstream inv_s("data/inv.sav");
            std::ofstream team_s("data/team.sav");
            map_s << mapCtl.save();
            inv_s << inv.save();
            team_s << team.save();
            map_s.close();
            inv_s.close();
            team_s.close();

            engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar("Game Saved!"), UTF8_to_WChar("System")));
            break;
            }
        case svc::closeGame:
            fullstop = 1;
            break;
    }

    varMap["ret"] = ret;
    return ret;
}

void Engine::setStat(int s){
    switch(s){
        case onMap:
            excute(mapCtl);
            break;
        case inEvent:
            excute(evtCtl);
            break;
        case inMainMenu:
            excute(mainmenu);
            break;
        case inTeamMenu:
            excute(teammenu);
            break;
        case inInvMenu:
        case inVendorInvMenu:
            excute(invmenu);
            break;
        case inCharMenu:
            excute(charmenu);
            break;
        case inSkillMenu:
            excute(skillmenu);
            break;
        case inBattle:
            excute(battle);
            break;
        case inPrompt:
            excute(prom);
            break;
        case inVender:
            excute(vendor);
            break;
        case inHelp:
            excute(helpmenu);
            break;
    }
    return;
}


