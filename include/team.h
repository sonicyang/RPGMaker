#ifndef TEAM_H
#define TEAM_H

#include <map>
#include <string>
#include <vector>
#include "character.h"
#include "rolefactory.h"

class Team
{
    public:
        Team();
        Team(std::string teamList, std::string roleList);
        virtual ~Team();

        std::vector<std::string> getNameList() const;

        Character& operator[](std::string subscript);

        void addCharToTeam(std::string);
        void removeCharFromTeam(std::string);

        bool isWipeOut();

        std::string save();
        void load(std::string);
    protected:
    private:
        RoleFactory roleCache;

        std::map<std::string, Character> _team;

        unsigned int _cacheCount;
        std::map<std::string, Character> _cache;

        Character null;
};

#endif // TEAM_H
