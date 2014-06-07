#include "battle.h"

#include <unistd.h>
#include <cstdlib>

#include "enum.h"
#include "utf8.h"
#include "engine.h"
#include "render.h"

Battle::Battle(Engine* eng, std::map< std::string, variant<paraVarType> >* b) :
    genericContorller(eng),
    varMap(b)
{

}

Battle::Battle(std::string monsterList, Engine* eng, std::map< std::string, variant<paraVarType> >* b) :
    genericContorller(eng),
    varMap(b),
    _monsterCache(monsterList)
{
    team = NULL;
}

Battle::~Battle()
{
    //dtor
}

int Battle::loadBattle(Team* t, std::vector<std::string>& monsters, int chance){
    _chance = chance;
    _memberCount = t->getNameList().size();;
    _currentChara = 0;
    _monsters.clear();
    for(unsigned int i = 0; i < monsters.size(); i++){
        _monsters.push_back(_monsterCache[monsters[i]]);
    }
    _monstersBak = _monsters;
    charaAttackBuff.resize(_memberCount);
    charaDefenseBuff.resize(_memberCount);
    team = t;
    processStat = 0;
    return 0;
}

int Battle::hKeyUp(){
    if(processStat == BattleMenu){
        battleMenuCurrentPos = (battleMenuCurrentPos < 2)? battleMenuCurrentPos : battleMenuCurrentPos - 2;
    }
    return 0;
}

int Battle::hKeyDown(){
    if(processStat == BattleMenu){
        battleMenuCurrentPos = (battleMenuCurrentPos < 2)? battleMenuCurrentPos + 2 : battleMenuCurrentPos;
    }
    return 0;
}

int Battle::hKeyLeft(){
    if(processStat == BattleMenu){
        battleMenuCurrentPos = ((battleMenuCurrentPos % 2) != 1)? battleMenuCurrentPos : battleMenuCurrentPos - 1;
    }else if(processStat == MonsterMenu || processStat == skillMonsterMenu){
        MonsterMenuCurrentPos = (MonsterMenuCurrentPos==0)? _monsters.size() - 1 : MonsterMenuCurrentPos - 1;
    }

    return 0;
}

int Battle::hKeyRight(){
    if(processStat == BattleMenu){
        battleMenuCurrentPos = ((battleMenuCurrentPos % 2) != 1)? battleMenuCurrentPos + 1: battleMenuCurrentPos;
    }else if(processStat == MonsterMenu || processStat == skillMonsterMenu){
        MonsterMenuCurrentPos = (MonsterMenuCurrentPos == _monsters.size() - 1)? 0 : MonsterMenuCurrentPos + 1;
    }
    return 0;
}

int Battle::hKeyZ(){
    if(processStat == BattleMenu){
        switch(battleMenuCurrentPos){
            case 0://Attack
                processStat = MonsterMenu;
                break;
            case 1:
                engine->engineCall(loadStack(svc::loadSkillMenu, 1, _currentChara));
                engine->engineCall(loadStack(svc::setStat, Stats::inSkillMenu));
                if(varMap->at("SkillMenuCurPos").get<unsigned int>() != 0xffffffff){
                    Skill tmp = (*team)[team->getNameList()[_currentChara]].getSkillList()[varMap->at("SkillMenuCurPos").get<unsigned int>()];
                    if(tmp.geteTarget() != 0){
                        processStat = PlayerSkill;
                    }else{
                        processStat = skillMonsterMenu;
                    }
                }
                break;
            case 2:
                engine->engineCall(loadStack(svc::loadInvMenu, 0));
                engine->engineCall(loadStack(svc::setStat, Stats::inInvMenu));
                processStat = process::PostPlayer;
                break;
            case 3:
                int p = rand() % 100;
                if(p - _chance < 0){
                    engine->engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar("You Successfully Escaped!"), UTF8_to_WChar("System")));
                    engine->engineCall(loadStack(svc::restoreStat));
                }else{
                    engine->engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar("You Failed to Escape!"), UTF8_to_WChar("System")));
                    processStat = process::PostPlayer;
                }
                break;
        }
    }else if(processStat == MonsterMenu){
        int atk = (*team)[team->getNameList()[_currentChara]].getAttack();
        float rng = (rand() % 6) / 10 + 0.75;
        int dmg = (-1) * (rng) * (atk + charaAttackBuff[_currentChara]) + _monsters[MonsterMenuCurrentPos].getDefense();

        _monsters[MonsterMenuCurrentPos].varHP(dmg);
        char kk[100];
        if(_monsters[MonsterMenuCurrentPos].isDead()){
            sprintf(kk, "%s is Dead!", _monsters[MonsterMenuCurrentPos].getName().c_str());
            _monsters.erase(_monsters.begin() + MonsterMenuCurrentPos);
        }else{
            sprintf(kk, "%s received %d point of damage!", _monsters[MonsterMenuCurrentPos].getName().c_str(), (-1)*dmg);
        }
        engine->engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar(kk), UTF8_to_WChar("System")));

        processStat = PostPlayer;
    }else if(processStat == skillMonsterMenu){
        processStat = PlayerSkill;
    }

    return 0;
}

int Battle::hKeyX(){
    if(processStat == MonsterMenu || processStat == skillMonsterMenu)
        processStat = BattleMenu;
    return 0;
}

int Battle::hDoEvent(){
    if(processStat == prePlayer){
        if((*team)[team->getNameList()[_currentChara]].isDead())
            processStat = PostPlayer;
        else
            processStat = BattleMenu;
    }else if(processStat == PlayerSkill){
        Skill tmp = (*team)[team->getNameList()[_currentChara]].getSkillList()[varMap->at("SkillMenuCurPos").get<unsigned int>()];
        if(tmp.getfTarget() == 0){
            engine->engineCall(loadStack(svc::loadTeamMenu, 1));
            engine->engineCall(loadStack(svc::setStat, Stats::inTeamMenu));
            if(varMap->at("TeamMenuCurPos").get<unsigned int>() == 0xffffffff){
                processStat = process::BattleMenu;
                return -1;
            }
        }
        int p = engine->engineCall(loadStack(svc::useSkill, _currentChara, varMap->at("SkillMenuCurPos").get<unsigned int>(), MonsterMenuCurrentPos, varMap->at("TeamMenuCurPos").get<unsigned int>())).get<int>();
        if(!p){
            engine->engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar("Not Enough Mana!"), UTF8_to_WChar("System")));
            processStat = BattleMenu;
        }else{
           processStat = PostPlayer;

        }
    }else if(processStat == PostPlayer){
        if(isMonsterWipeOut()){
            processStat = process::PostBattle;
        }else{
             _currentChara++;
            if(_currentChara >= _memberCount){
                engine->engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar("It's Now Monster's Turn"), UTF8_to_WChar("System")));
                processStat = process::MonsterTurn;
            }else{
                processStat = process::prePlayer;
            }
        }
    }else if(processStat == MonsterTurn){
        for(unsigned int i = 0; i < _monsters.size(); i++){

            if(_monsters[i].isDead())
                continue;
            int us = rand() % _monsters[i].getSkillList().size() + 1;

            if(us == 0){//Normal Attack
                int target = rand() % _memberCount;
                float rng = (rand() % 6) / 10 + 0.75;

                int tardef = (*team)[team->getNameList()[target]].getDefense();
                engine->engineCall(loadStack(svc::varHP, target, ((-1) * (rng) * _monsters[i].getAttack() + tardef + charaDefenseBuff[target])));

            }else{//Skills
                Skill tmp = _monsters[i].getSkillList()[us - 1];

                if(tmp.getfTarget() == 0){
                    _monsters[i].varHP(tmp.getfHPv());
                    _monsters[i].varMP(tmp.getfMPv());
                }else if(tmp.getfTarget() == 1){
                    for(unsigned int m = 0; m < _monsters.size(); m++){
                        _monsters[m].varHP(tmp.getfHPv());
                        _monsters[m].varMP(tmp.getfMPv());
                    }
                }

                if(tmp.geteTarget() == 0){
                    unsigned int target = rand() %  _memberCount;
                    engine->engineCall(loadStack(svc::varHP, target, tmp.geteHPv()));
                    engine->engineCall(loadStack(svc::varMP, target, tmp.geteMPv()));
                }else if(tmp.geteTarget() == 1){
                    for(unsigned int m = 0; m < _memberCount; m++){
                        engine->engineCall(loadStack(svc::varHP, m, tmp.geteHPv()));
                        engine->engineCall(loadStack(svc::varMP, m, tmp.geteMPv()));
                    }
                }
            }
            usleep(1000000);
        }

        if(engine->engineCall(loadStack(svc::isTeamWipeOut)).get<int>()){
            engine->engineCall(loadStack(svc::gameOver));
        }else{
            engine->engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar("It's Now your Turn"), UTF8_to_WChar("System")));
            _currentChara = 0;
            processStat = process::prePlayer;
        }
    }else if(processStat == process::PostBattle){
        for(unsigned int i = 0; i < _monstersBak.size(); i++){
            for(unsigned int j = 0; j < _memberCount; j++){
                engine->engineCall(loadStack(svc::varExp, j, _monstersBak[i].getExp())).get<int>();
            }
        }
        engine->engineCall(loadStack(svc::restoreStat));
    }
    return 0;
}

int Battle::hRender(render& rdr){
    if(processStat == MonsterMenu || processStat == skillMonsterMenu)
        rdr.render_BattleScene(_monsters, MonsterMenuCurrentPos);
    else
        rdr.render_BattleScene(_monsters);
    rdr.render_BattleTeam(*team, _currentChara);
    rdr.render_BattleMenu(battleMenuCurrentPos);
    return 0;
}

bool Battle::isMonsterWipeOut(){
    bool flag = true;
    for(unsigned int i = 0; i < _monsters.size(); i++){
        flag &= _monsters[i].isDead();
    }
    return flag;
}
