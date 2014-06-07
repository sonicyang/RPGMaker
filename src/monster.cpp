#include "monster.h"

Monster::Monster()
{
    //ctor
}

Monster::Monster(std::string name, int hp, int mp, int atk, int def, int exp, std::vector<Skill> skills, std::string img) :
    _name(name),
    max_hp(hp),
    max_mp(mp),
    attack(atk),
    defense(def),
    _hp(max_hp),
    _mp(max_mp),
    _exp(exp),
    _skills(skills),
    _img(img)
{
    //ctor
}

Monster::~Monster()
{
    //dtor
}

void Monster::setHP(const int a){
    if(a >= 0 && a <= getMaxHP()){
        this->_hp = a;
    }else if(a > getMaxHP()){
        this->_hp = getMaxHP();
    }else{
        this->_hp = 0;
    }
    return;
}

int Monster::getHP() const{
    return this->_hp;
}

void Monster::varHP(const int val){
    setHP(getHP() + val);
    return;
}

void Monster::setMP(int a){
    if(a >= 0 && a <= getMaxMP()){
        this->_mp = a;
    }else if(a > getMaxMP()){
        this->_mp = getMaxMP();
    }else{
        this->_mp = 0;
    }
    return;
}

int Monster::getMP() const{
    return this->_mp;
}

void Monster::varMP(const int val){
    setMP(getMP() + val);
    return;
}

int Monster::getMaxHP(void) {
    return max_hp;
}

int Monster::getMaxMP(void) {
    return max_mp;
}

int Monster::getAttack(void) {
    return attack;
}

int Monster::getDefense(void) {
    return defense;
}

void Monster::recoverHP(void){
    setHP(max_hp);
    return;
}

void Monster::recoverMP(void){
    setMP(max_mp);
    return;
}

bool Monster::isDead(){
    return _hp == 0;
}

void varAttack(const int val){

}

void varDefense(const int val){

}
