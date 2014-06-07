#include <cstdlib>
#include <string>
#include "item.h"
#include "utils.h"
#include "json/value.h"
#include "json/reader.h"

Item::Item()
{
    //ctor
}

Item::Item(std::string name) :
    _name(name)
{

}

Item::~Item()
{
    //dtor
}

bool Item::isUsable() const{
    return (_type == itemTypes::potion) || (_type == itemTypes::allPotion);
}

/*Item::Item(const Item& other)
{
    _name = other._name;
    _description = other._description;
    _price = other._price;
    _iscomsumable = other._iscomsumable;
}

Item& Item::operator=(const Item& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
        _name = rhs._name;
        _description = rhs._description;
        _price = rhs._price;
        _iscomsumable = rhs._iscomsumable;
    return *this;
}*/

bool Item::operator<(const Item& a) const{
    return _id < a._id;
}
bool Item::operator<=(const Item& a) const{
    return operator==(a) || operator<(a);
}
bool Item::operator>(const Item& a) const{
    return _id > a._id;
}
bool Item::operator>=(const Item& a) const{
    return operator==(a) || operator>(a);
}
bool Item::operator==(const Item& a) const{
    return _id == a._id;
}
bool Item::operator!=(const Item& a) const{
    return !operator==(a);
}
