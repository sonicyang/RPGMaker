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
        Role(std::string);
        virtual ~Role();

        int getHPc(void) const { return _hp_base; }
        int getHPx(void) const { return _hp_mpx; }
        int getMPc(void) const { return _mp_base; }
        int getMPx(void) const { return _mp_mpx; }
        int getATKc(void) const { return _attack_base; }
        int getATKx(void) const { return _attack_mpx; }
        int getDEFc(void) const { return _defense_base; }
        int getDEFx(void) const { return _defense_mpx; }

        void setHPc(int val) { _hp_base = val; }
        void setHPx(int val) { _hp_mpx = val; }
        void setMPc(int val) { _mp_base = val; }
        void setMPx(int val) { _mp_mpx = val; }
        void setATKc(int val) { _attack_base = val; }
        void setATKx(int val) { _attack_mpx = val; }
        void setDEFc(int val) { _defense_base = val; }
        void setDEFx(int val) { _defense_mpx = val; }

        void setName(std::string val) { _name = val; }
        std::string getName() { return _name; }

        std::map<int, Skill>& getSkillList() { return _skillsCache; }
    protected:
    private:
        std::string _name;

        int _hp_base = 0;
        int _hp_mpx = 0;
        int _mp_base = 0;
        int _mp_mpx = 0;
        int _attack_base = 0;
        int _attack_mpx = 0;
        int _defense_base = 0;
        int _defense_mpx = 0;

        std::map<int, Skill> _skillsCache;
};

#endif // ROLE_H
