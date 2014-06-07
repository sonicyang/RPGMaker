#include "menu.h"
#include "engine.h"

Menu::Menu(Engine* eng, std::map< std::string, variant<paraVarType> >& b) :
    genericContorller(eng),
    varMap(b)
{

}

Menu::~Menu()
{
}

