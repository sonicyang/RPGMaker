#include "objplayer.h"

objPlayer::objPlayer() : mapObject("NULL", 0, 0 , 0, 0, '@', "", false, 0)
{
    icon_list = L"^>v<";
    facing = 0;
}

objPlayer::objPlayer(std::string n, int i, int t, int x, int y, std::wstring ic, int f) : mapObject(n, i, t, x, y, '@', "", false, 0)
{
    icon_list = ic;
    facing = f;
}

objPlayer::~objPlayer()
{
    //dtor
}

objPlayer::objPlayer(const objPlayer& other) : mapObject(other)
{
    icon_list = other.icon_list;
    facing = other.facing;
}

objPlayer& objPlayer::operator=(const objPlayer& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
        mapObject::operator=(rhs);
        icon_list = rhs.icon_list;
        facing = rhs.facing;
    return *this;
}

void objPlayer::setFacing(int f){
    if(f < 0 || f > 3)
        return;
    facing = f;
    return;
}
