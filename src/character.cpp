#include "character.h"
#include "enum.h"

Character::Character (RoleFactory* rf, std::string n, int lvl, Role role) : _roleCache(rf), _name(n), _exp(0), _role(role)
{
    _role.setLevel(lvl);
    _level = lvl;
    recoverHP();
    recoverMP();
}

Character::Character ()
{
    //dtor
}


Character::~Character (){
    //dtor
}

void Character::setHP(const int a){
    if(a >= 0 && a <= getMaxHP()){
        this->_hp = a;
    }else if(a > getMaxHP()){
        this->_hp = getMaxHP();
    }else{
        this->_hp = 0;
    }
    return;
}

int Character::getHP() const{
    return this->_hp;
}

void Character::varHP(const int val){
    setHP(getHP() + val);
    return;
}

void Character::setMP(int a){
    if(a >= 0 && a <= getMaxMP()){
        this->_mp = a;
    }else if(a > getMaxMP()){
        this->_mp = getMaxMP();
    }else{
        this->_mp = 0;
    }
    return;
}

int Character::getMP() const{
    return this->_mp;
}

void Character::varMP(const int val){
    setMP(getMP() + val);
    return;
}

int Character::LevelUP(){
    _level += 1;
    _role.setLevel(_level);
    return 0;
}

void Character::setLevel(int val){
    _level = val;
    _role.setLevel(_level);
    return;
}

int Character::setExp(const int a){
    if(a >= 0 && a < getLevelUpExp()){
        this->_exp = a;
    }else if(a >= 0 && a >= getLevelUpExp()){

        int cc = setExp(a - getLevelUpExp()) + 1;
        LevelUP();
        return cc;
    }
    return 0;
}

int Character::getExp(void) const {
    return this->_exp;
}

int Character::varExp(const int val){
    return setExp(getExp() + val);
}

int Character::getMaxHP(void) {
    return getBaseHP() + getAdditionalHP();
}

int Character::getMaxMP(void) {
    return getBaseMP() + getAdditionalMP();
}

int Character::getLevelUpExp(void) {
    return _role.getLevelUpExp();
}

int Character::getAttack(void) {
    return getBaseAttack() + getAdditionalAttack();
}

int Character::getDefense(void) {
    return getBaseDefense() + getAdditionalDefense();
}

int Character::getBaseHP(void){
    return _role.getMaxHP();
}

int Character::getBaseMP(void){
    return _role.getMaxMP();
}

int Character::getBaseAttack(void){
    return _role.getAttack();
}

int Character::getBaseDefense(void){
    return _role.getDefense();
}

int Character::getAdditionalHP(void){
    return _weapon.getHPVarient() + _armor.getHPVarient() + _head.getHPVarient() + _legs.getHPVarient() + _shoes.getHPVarient();
}

int Character::getAdditionalMP(void){
    return  + _weapon.getMPVarient() + _armor.getMPVarient() + _head.getMPVarient() + _legs.getMPVarient() + _shoes.getMPVarient();
}
int Character::getAdditionalAttack(void){
    return  _weapon.getATKVarient() + _armor.getATKVarient() + _head.getATKVarient() + _legs.getATKVarient() + _shoes.getATKVarient();
}

int Character::getAdditionalDefense(void){
    return  _weapon.getDEFVarient() + _armor.getDEFVarient() + _head.getDEFVarient() + _legs.getDEFVarient() + _shoes.getDEFVarient();
}


int Character::setWeapon(Item item){
    if(item.getType() != itemTypes::weapon)
        return -1;
    _weapon = item;
    return 1;
}
int Character::setHead(Item item){
    if((item.getType() != itemTypes::head) && (item.getType() != itemTypes::null))
        return -1;
    _head = item;
    return 1;
}

int Character::setArmor(Item item){
    if((item.getType() != itemTypes::armor) && (item.getType() != itemTypes::null))
        return -1;
    _armor = item;
    return 1;
}

int Character::setLegs(Item item){
    if((item.getType() != itemTypes::legs) && (item.getType() != itemTypes::null))
        return -1;
    _legs = item;
    return 1;
}

int Character::setShoes(Item item){
    if((item.getType() != itemTypes::shoes) && (item.getType() != itemTypes::null))
        return -1;
    _shoes = item;
    return 1;
}

std::string Character::getRoleName(void) {
    return _role.getName();
}

void Character::setRole(std::string val){
    Role tmpRole = (*_roleCache)[val];
    if(tmpRole.getName() != "NULL")
        _role = tmpRole;
    _role.setLevel(_level);
    return;
}

void Character::recoverHP(void){
    setHP(_role.getMaxHP());
    return;
}

void Character::recoverMP(void){
    setMP(_role.getMaxMP());
    return;
}

bool Character::isDead(){
    return _hp == 0;
}
