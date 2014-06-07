#include "mainmenu.h"
#include "utils.h"
#include "enum.h"
#include "engine.h"

MainMenu::MainMenu(Engine* eng, std::map< std::string, variant<paraVarType> >& b) :
    Menu(eng, b)
{
    mOption.push_back("Team");
    mOption.push_back("Inventory");
    mOption.push_back("Save");
    mOption.push_back("Exit");

    currentPos = 0;
    varMap["MainMenuCurPos"].set<unsigned int>(currentPos);
}

MainMenu::~MainMenu()
{

}

int MainMenu::hKeyUp(){
    currentPos = (currentPos==0)? mOption.size() - 1 : currentPos - 1;
    return 0;
}

int MainMenu::hKeyDown(){
    currentPos = (currentPos == mOption.size() - 1)? 0 : currentPos + 1;
    return 0;
}

int MainMenu::hKeyZ(){
    varMap["MainMenuCurPos"].set<unsigned int>(currentPos);

    switch(currentPos){
            case 0:
                engine->engineCall(loadStack(svc::loadTeamMenu, 0));
                engine->engineCall(loadStack(svc::setStat, Stats::inTeamMenu));
                break;
            case 1:
                engine->engineCall(loadStack(svc::loadInvMenu, 0));
                engine->engineCall(loadStack(svc::setStat, Stats::inInvMenu));
                break;
            case 2:
                engine->engineCall(loadStack(svc::saveGame));
                break;
            case 3:
                engine->engineCall(loadStack(svc::endGame));
                break;
            }
    return 0;
}

int MainMenu::hKeyX(){
    engine->engineCall(loadStack(svc::restoreStat));
    return 0;
}

int MainMenu::hKeyQ(){
    engine->engineCall(loadStack(svc::restoreStat));
    return 0;
}

int MainMenu::hRender(render& rdr){
    rdr.render_MainMenu(currentPos, mOption);
    return 0;
}

void MainMenu::init(void){
    currentPos = 0;
}

