#include "teammenu.h"
#include "utils.h"
#include "enum.h"
#include "engine.h"

TeamMenu::TeamMenu(Engine* eng, std::map< std::string, variant<paraVarType> >& b) :
    Menu(eng, b)
{
    currentPos = 0;
    varMap["TeamMenuCurPos"].set<unsigned int>(currentPos);
}

TeamMenu::~TeamMenu()
{
    //dtor
}

int TeamMenu::hKeyUp(){
    currentPos = (currentPos == 0)? 0 : currentPos - 1;
    return 0;
}

int TeamMenu::hKeyDown(){
    currentPos = (currentPos == _limiter - 1 )? _limiter - 1 : currentPos + 1;
    return 0;
}

int TeamMenu::hKeyZ(){
    varMap["TeamMenuCurPos"].set<unsigned int>(currentPos);
    if(mode == 0){
        engine->engineCall(loadStack(svc::loadCharMenu, 0, currentPos));
        engine->engineCall(loadStack(svc::setStat, Stats::inCharMenu));
    }else{
        engine->engineCall(loadStack(svc::restoreStat));
    }
    return 0;
}

int TeamMenu::hKeyX(){
    varMap["TeamMenuCurPos"].set<unsigned int>(0xffffffff);
    engine->engineCall(loadStack(svc::restoreStat));
    return 0;
}

int TeamMenu::hKeyQ(){
    varMap["TeamMenuCurPos"].set<unsigned int>(0xffffffff);
    engine->engineCall(loadStack(svc::restoreStat));
    return 0;
}

int TeamMenu::hRender(render& rdr){
    rdr.render_TeamMenu(*team, currentPos);
    return 0;
}

void TeamMenu::init(int m, Team* t){
    currentPos = 0;
    varMap["TeamMenuCurPos"].set<unsigned int>(currentPos);
    _limiter = t->getNameList().size();
    team = t;
    mode = m;
}
