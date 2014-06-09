#ifndef MONSTER_H
#define MONSTER_H

#include <string>
#include <vector>
#include "skill.h"

class Monster
{
    public:
        Monster();
        Monster(std::string);
        virtual ~Monster();

        /* setters and getters */
        void setName(const std::string& val) { _name = val; }
        std::string getName(void) const { return _name; }

        void setHP(const int);
        int getHP(void) const;
        void setMP(const int);
        int getMP(void) const;

        int getExp(void) const { return _exp; }
        void setExp(int val) { _exp = val; }

        int getAttack(void);
        void setAttack(int val) { attack = val; }
        int getDefense(void);
        void setDefense(int val) { defense = val; }

        std::vector<Skill> getSkillList() { return _skills; }

        std::string getImg() { return _img; }
        void setImg(std::string val) { _img = val; }
    protected:
    private:

        std::string _name; // Stores the name of the player
        int attack = 0; // stores attack of the player
        int defense = 0; // stores defense of the playe

        int _hp = 0; // Stores the HP of the player, >= 0
        int _mp = 0; // Stores the MP of the player, >= 0
        int _exp = 0; // Stores the experience of the player, >= 0. Will not reset to zero after level-up

        std::vector<Skill> _skills;

        std::string _img;

};

#endif // MONSTER_H
