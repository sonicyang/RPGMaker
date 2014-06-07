#ifndef MENU_H
#define MENU_H

#include <map>
#include <vector>
#include <deque>
#include "utils.h"
#include "genericcontorller.h"
#include "render.h"

class Engine;

class Menu : public genericContorller
{
    public:

        Menu(Engine*, std::map< std::string, variant<paraVarType> >&);
        ~Menu();

    protected:
        std::map< std::string, variant<paraVarType> >& varMap;
    private:


};

#endif // MENU_H
