#include "role.h"
#include <cmath>

Role::Role():
    _name("NULL"),
    _hp_base(0),
    _hp_mpx(0),
    _mp_base(0),
    _mp_mpx(0),
    _attack_base(0),
    _attack_mpx(0),
    _defense_base(0),
    _defense_mpx(0),
    _skills()
{
    setLevel(1);
}


Role::Role(std::string name, int hb, int hm, int mb, int mm, int ab, int am, int db, int dm, std::map<int, Skill> skills) :
    _name(name),
    _hp_base(hb),
    _hp_mpx(hm),
    _mp_base(mb),
    _mp_mpx(mm),
    _attack_base(ab),
    _attack_mpx(am),
    _defense_base(db),
    _defense_mpx(dm),
    _skillsCache(skills)
{
    setLevel(1);
}

Role::Role(std::string name, int hb, int hm, int mb, int mm, int ab, int am, int db, int dm, std::map<int, Skill> skills, int val) :
    _name(name),
    _hp_base(hb),
    _hp_mpx(hm),
    _mp_base(mb),
    _mp_mpx(mm),
    _attack_base(ab),
    _attack_mpx(am),
    _defense_base(db),
    _defense_mpx(dm),
    _skillsCache(skills)
{
    setLevel(val);
}

Role::~Role()
{
    //dtor
}

void Role::setLevel(int val){
    max_hp = _hp_base + _hp_mpx * 10;
    max_mp = _mp_base + _mp_mpx * 5;
    attack = _attack_base + _attack_mpx *5;
    defense = _defense_base + _defense_mpx *5;
    lvup_exp = ceil(pow(10, log2(val + 1)));

    for(int i = 0; i < val; i++){
        if(_skillsCache.find(i) != _skillsCache.end()){
            _skills.push_back(_skillsCache[i]);
        }
    }


    return;
}
