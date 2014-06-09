#include "skill.h"

#include <cstdlib>
#include "json/reader.h"
#include "json/value.h"
#include "utils.h"

Skill::Skill()
{
    //ctor
}

Skill::Skill(std::string name) :
    _name(name)
{

}

Skill::~Skill()
{
    //dtor
}
