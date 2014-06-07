#include "startmenu.h"
#include "utils.h"
#include "enum.h"
#include "engine.h"

StartMenu::StartMenu(Engine* eng, std::map< std::string, variant<paraVarType> >& b) :
    Menu(eng, b)
{
    mOption.push_back("Start");
    mOption.push_back("Load");
    mOption.push_back("Help/Keys");
    mOption.push_back("Exit");

    currentPos = 0;
    varMap["StartMenuCurPos"].set<unsigned int>(currentPos);
}

StartMenu::~StartMenu()
{

}

int StartMenu::hKeyUp(){
    currentPos = (currentPos==0)? mOption.size() - 1 : currentPos - 1;
    return 0;
}

int StartMenu::hKeyDown(){
    currentPos = (currentPos == mOption.size() - 1)? 0 : currentPos + 1;
    return 0;
}

int StartMenu::hKeyZ(){
    varMap["StartMenuCurPos"].set<unsigned int>(currentPos);

    switch(currentPos){
            case 0:
                engine->engineCall(loadStack(svc::setStat, Stats::onMap));
                break;
            case 1:
                engine->engineCall(loadStack(svc::loadGame));
                engine->engineCall(loadStack(svc::setStat, Stats::onMap));
                break;
            case 2:
                engine->engineCall(loadStack(svc::setStat, Stats::inHelp));
                break;
            case 3:
                engine->engineCall(loadStack(svc::closeGame));
                break;
            }
    return 0;
}

int StartMenu::hRender(render& rdr){
    rdr.render_StartMenu(currentPos, mOption);
    return 0;
}

void StartMenu::init(){
    currentPos = 0;
    return;
}

