#include <string>
#include <cstdlib>
#include "json/value.h"
#include "json/reader.h"
#include "utils.h"
#include "mapobject.h"

mapObject::mapObject() : name("NULL"), ID(0), type(0), _canStep(false), triggerType(0)
{
    Cord.m_x = 0;
    Cord.m_y = 0;
}

mapObject::mapObject(std::string n, int i, int t, int x, int y, std::string ti, Point ic, std::string trig, bool cs, int trigType, int fac) :
    name(n),
    ID(i),
    type(t),
    tile(ti),
    icon(ic),
    _canStep(cs),
    trigger(trig),
    triggerType(trigType),
    facing(fac)
{
    Cord.m_x = x;
    Cord.m_y = y;
}

mapObject::~mapObject()
{
}

void mapObject::setFacing(int f){
    if(f < 0 || f > 3)
        return;
    facing = f;
    return;
}
