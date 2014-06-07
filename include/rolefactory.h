#ifndef ROLEFACTORY_H
#define ROLEFACTORY_H

#include <string>
#include <map>
#include "role.h"

class RoleFactory
{
    public:
        RoleFactory();
        RoleFactory(std::string roleList);
        virtual ~RoleFactory();

        Role operator[](std::string val);
    protected:
    private:

        unsigned int _roleCount;
        std::map<std::string, Role> _roleCache;

        Role _null;
};

#endif // ROLEFACTORY_H
