#include "skillmenu.h"
#include "utils.h"
#include "enum.h"
#include "render.h"
#include "engine.h"

SkillMenu::SkillMenu(Engine* eng, std::map< std::string, variant<paraVarType> >& b) :
    Menu(eng, b)
{
    currentPos = 0;
    varMap["SkillMenuCurPos"].set<unsigned int>(currentPos);
}

SkillMenu::~SkillMenu()
{
    //dtor
}
/*

int SkillMenu::processInput(int c){
    switch (c) {
        case KEY_UP:
            currentPos = (currentPos==0)? 0 : currentPos - 1;
            break;
        case KEY_DOWN:
            currentPos = (currentPos == _limiter - 1 )? _limiter - 1 : currentPos + 1;
            currentPos = (_limiter > 0)? currentPos : 0;
            break;
        case 'z':
            if(mode == 0){

            }else{
            }
            break;
        case 'x':
        case 'q':
            currentPos = 0xffffffff;
            ctlCallStack.push_back(loadStack(svc::restoreStat));
            break;
    }

    varMap["SkillMenuCurPos"].set<unsigned int>(currentPos);

    return 0;
}
*/

int SkillMenu::hKeyUp(){
    currentPos = (currentPos == 0)? 0 : currentPos - 1;
    return 0;
}

int SkillMenu::hKeyDown(){
    currentPos = (currentPos == _limiter - 1 )? _limiter - 1 : currentPos + 1;
    return 0;
}

int SkillMenu::hKeyZ(){
    varMap["SkillMenuCurPos"].set<unsigned int>(currentPos);
    engine->engineCall(loadStack(svc::restoreStat));
    return 0;
}

int SkillMenu::hKeyX(){
    varMap["SkillMenuCurPos"].set<unsigned int>(0xffffffff);
    engine->engineCall(loadStack(svc::restoreStat));
    return 0;
}

int SkillMenu::hKeyQ(){
    varMap["SkillMenuCurPos"].set<unsigned int>(0xffffffff);
    engine->engineCall(loadStack(svc::restoreStat));
    return 0;
}

int SkillMenu::hRender(render& rdr){
    rdr.render_SkillMenu(*currChara, currentPos);
    return 0;
}

void SkillMenu::init(int m, Character* c){
    currentPos = 0;
    varMap["SkillMenuCurPos"].set<unsigned int>(currentPos);
    mode = m;
    _limiter = c->getSkillList().size();
    currChara = c;
}
