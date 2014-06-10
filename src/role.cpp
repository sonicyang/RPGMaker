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
    _defense_mpx(0)
{
}

Role::Role(std::string name) :
    _name(name)
{
}

Role::~Role()
{
    //dtor
}
