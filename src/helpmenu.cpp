#include "helpmenu.h"
#include "utils.h"
#include "enum.h"
#include "engine.h"

HelpMenu::HelpMenu(Engine* eng, std::map< std::string, variant<paraVarType> >& b) :
    Menu(eng, b)
{
}

HelpMenu::~HelpMenu()
{

}

int HelpMenu::hKeyZ(){
    engine->engineCall(loadStack(svc::restoreStat));
    return 0;
}

int HelpMenu::hKeyX(){
    engine->engineCall(loadStack(svc::restoreStat));
    return 0;
}

int HelpMenu::hRender(render& rdr){
    rdr.render_HelpMenu();
    return 0;
}
