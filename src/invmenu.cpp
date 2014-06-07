#include <string>

#include "invmenu.h"
#include "utils.h"
#include "enum.h"
#include "utf8.h"
#include "engine.h"

InvMenu::InvMenu(Engine* eng, std::map< std::string, variant<paraVarType> >& b) :
    Menu(eng, b)
{
    currentPos = 0;
    varMap["InvMenuCurPos"].set<unsigned int>(currentPos);   //ctor
    inv = NULL;
}

int InvMenu::hKeyUp(){
    currentPos = (currentPos == 0)? 0 : currentPos - 1;
    return 0;
}

int InvMenu::hKeyDown(){
    currentPos = (currentPos == _limiter - 1 )? _limiter - 1 : currentPos + 1;
    return 0;
}

int InvMenu::hKeyZ(){
    varMap["InvMenuCurPos"].set<unsigned int>(currentPos);
    if(_limiter > 0){
        if(mode == 0){
            variant<paraVarType> ret;

            ret = engine->engineCall(loadStack(svc::isItemUsable, currentPos));
            if(ret.get<int>()){
                ret = engine->engineCall(loadStack(svc::getItemType, currentPos));
                if(ret.get<int>() == 0){
                    engine->engineCall(loadStack(svc::loadTeamMenu, 1));
                    engine->engineCall(loadStack(svc::setStat, Stats::inTeamMenu));
                }
                engine->engineCall(loadStack(svc::useItem, currentPos, varMap["TeamMenuCurPos"].get<unsigned int>()));
            }else{
                engine->engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar("This Item is not Comsumable"), UTF8_to_WChar("System")));
            }
        }else{
            engine->engineCall(loadStack(svc::restoreStat));
        }
    }else{
        varMap["InvMenuCurPos"].set<unsigned int>(0xffffffff);
        engine->engineCall(loadStack(svc::restoreStat));
    }
    return 0;
}

int InvMenu::hKeyX(){
    varMap["InvMenuCurPos"].set<unsigned int>(0xffffffff);
    engine->engineCall(loadStack(svc::restoreStat));
    return 0;
}

int InvMenu::hKeyQ(){
    varMap["InvMenuCurPos"].set<unsigned int>(0xffffffff);
    engine->engineCall(loadStack(svc::restoreStat));
    return 0;
}

int InvMenu::hRender(render& rdr){
    rdr.render_InvMenu(*inv, currentPos);
    return 0;
}

InvMenu::~InvMenu()
{
    //dtor
}

void InvMenu::init(int m, inventory* i){
    currentPos = 0;
    varMap["InvMenuCurPos"].set<unsigned int>(currentPos);
    _limiter = i->getNumOfItems();
    mode = m;
    inv = i;
}
