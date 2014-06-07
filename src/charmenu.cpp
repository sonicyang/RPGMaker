#include "charmenu.h"
#include "utils.h"
#include "enum.h"
#include "engine.h"

CharMenu::CharMenu(Engine* eng, std::map< std::string, variant<paraVarType> >& b) :
    Menu(eng, b)
{
    currentPos = 0;
    varMap["CharMenuCurPos"].set<unsigned int>(currentPos);
    currChara = NULL;
}

CharMenu::~CharMenu()
{
    //dtor
}

int CharMenu::hKeyUp(){
    currentPos = (currentPos==0)? 5 : currentPos - 1;
    return 0;
}

int CharMenu::hKeyDown(){
    currentPos = (currentPos == 5)? 0 : currentPos + 1;
    return 0;
}

int CharMenu::hKeyZ(){
    varMap["CharMenuCurPos"].set<unsigned int>(currentPos);
    if(mode == 0){
        if(currentPos < 5){
            engine->engineCall(loadStack(svc::invEnableNull));
            engine->engineCall(loadStack(svc::loadInvMenu, 1));
            engine->engineCall(loadStack(svc::setStat, Stats::inInvMenu));
            if(varMap["InvMenuCurPos"].get<unsigned int>() != 0xffffffff)
                engine->engineCall(loadStack(svc::changeEquip, varMap["InvMenuCurPos"].get<unsigned int>(), varMap["TeamMenuCurPos"].get<unsigned int>(), currentPos));
            engine->engineCall(loadStack(svc::invDisableNull));
        }else{
            engine->engineCall(loadStack(svc::loadSkillMenu, 1, varMap["TeamMenuCurPos"].get<unsigned int>()));
            engine->engineCall(loadStack(svc::setStat, Stats::inSkillMenu));
        }
    }else{
        engine->engineCall(loadStack(svc::restoreStat));
    }
    return 0;
}

int CharMenu::hKeyX(){
    varMap["CharMenuCurPos"].set<unsigned int>(0xffffffff);
    engine->engineCall(loadStack(svc::restoreStat));
    return 0;
}

int CharMenu::hKeyQ(){
    varMap["CharMenuCurPos"].set<unsigned int>(0xffffffff);
    engine->engineCall(loadStack(svc::restoreStat));
    return 0;
}

int CharMenu::hDoEvent(){
    return 0;
}

int CharMenu::hRender(render& rdr){
    rdr.render_CharMenu((*currChara), currentPos);
    return 0;
}

void CharMenu::init(int m, Character* chara){
    currentPos = 0;
    varMap["CharMenuCurPos"].set<unsigned int>(currentPos);
    mode = m;
    currChara = chara;
}
