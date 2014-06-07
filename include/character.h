#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "role.h"
#include "rolefactory.h"
#include "item.h"

class Character
{
    public:
        Character();
        Character(RoleFactory*, std::string, int, Role);
        virtual ~Character();

        /* setters and getters */
        void setName(const std::string& val) { _name = val; };
        std::string getName(void) const { return _name; };

        void setHP(const int);
        int getHP(void) const;
        void varHP(const int);
        void setMP(const int);
        int getMP(void) const;
        void varMP(const int);
        int setExp(const int);
        int getExp(void) const;
        int varExp(const int);

        void recoverHP(void);
        void recoverMP(void);

        int getMaxHP(void);
        int getMaxMP(void);
        int getLevelUpExp(void);
        int getAttack(void);
        int getDefense(void);

        int getBaseHP(void);
        int getBaseMP(void);
        int getBaseAttack(void);
        int getBaseDefense(void);

        int getAdditionalHP(void);
        int getAdditionalMP(void);
        int getAdditionalAttack(void);
        int getAdditionalDefense(void);

        int setWeapon(Item);
        int setHead(Item);
        int setArmor(Item);
        int setLegs(Item);
        int setShoes(Item);

        Item getWeapon(void) { return _weapon; };
        Item getHead(void) { return _head; };
        Item getArmor(void) { return _armor; };
        Item getLegs(void) { return _legs; };
        Item getShoes(void) { return _shoes; };

        std::string getRoleName(void);
        void setRole(std::string);
        //Role getRole(void) const { return _role; };

        void setLevel(const int); // call setAttr(int) to set attributes
        int getLevel(void) const { return _level; };

        bool isDead();

        std::vector<Skill> getSkillList() { return _role.getSkillList(); };
    protected:
    private:
        RoleFactory* _roleCache;

        std::string _name; // Stores the name of the player
        int _hp; // Stores the HP of the player, >= 0
        int _mp; // Stores the MP of the player, >= 0
        int _exp; // Stores the experience of the player, >= 0. Will not reset to zero after level-up
        int _level; // Stores the level of the player, >= 1

        Role _role;

        Item _weapon;
        Item _head;
        Item _armor;
        Item _legs;
        Item _shoes;

        int LevelUP();
};

#endif // CHARACTER _H
