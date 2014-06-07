#ifndef ROLE_H
#define ROLE_H

#include <string>
#include <map>
#include <vector>
#include "skill.h"

class Role
{
    public:
        Role();
        Role(std::string, int, int, int, int, int, int, int, int, std::map<int, Skill>);
        Role(std::string, int, int, int, int, int, int, int, int, std::map<int, Skill>, int);
        virtual ~Role();

        int getMaxHP(void) const { return max_hp; };
        int getMaxMP(void) const { return max_mp; };
        int getLevelUpExp(void) const { return lvup_exp; };
        int getAttack(void) const { return attack; };
        int getDefense(void) const { return defense; };

        void setLevel(int);

        std::string getName() { return _name; };

        std::vector<Skill> getSkillList() { return _skills; };
    protected:
    private:
        std::string _name;

        int max_hp; // stores max. HP of the player
        int max_mp;  // stores max. MP of the player
        int lvup_exp; // stores needed experience to level-up
        int attack; // stores attack of the player
        int defense; // stores defense of the player

        int _hp_base, _hp_mpx;
        int _mp_base, _mp_mpx;
        int _attack_base, _attack_mpx;
        int _defense_base, _defense_mpx;

        std::vector<Skill> _skills;
        std::map<int, Skill> _skillsCache;
};

#endif // ROLE_H
