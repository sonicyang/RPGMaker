#include "monster.h"

Monster::Monster()
{
    //ctor
}

Monster::Monster(std::string name) :
    _name(name)
{
    //ctor
}

Monster::~Monster()
{
    //dtor
}

void Monster::setHP(const int a){
    _hp = a;
    return;
}

int Monster::getHP() const{
    return this->_hp;
}

void Monster::setMP(int a){
    _mp = a;
    return;
}

int Monster::getMP() const{
    return this->_mp;
}

int Monster::getAttack(void) {
    return attack;
}

int Monster::getDefense(void) {
    return defense;
}
