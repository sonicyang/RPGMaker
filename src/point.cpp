#include "point.h"

Point::Point() : m_x(0), m_y(0){
}

Point::Point(int x, int y) : m_x(x), m_y(y){
}

Point::~Point()
{
}

Point::Point(const Point& other)
{
    this->m_x = other.m_x;
    this->m_y = other.m_y;
}

Point& Point::operator=(const Point& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
        this->m_x = rhs.m_x;
        this->m_y = rhs.m_y;
    return *this;
}

bool Point::operator<(const Point& a) const{
    if(m_x == a.m_x)
        return m_y < a.m_y;
    return m_x < a.m_x;
}
bool Point::operator<=(const Point& a) const{
    return operator==(a) || operator<(a);
}
bool Point::operator>(const Point& a) const{
    if(m_x == a.m_x)
        return m_y > a.m_y;
    return m_x > a.m_x;
}
bool Point::operator>=(const Point& a) const{
    return operator==(a) || operator>(a);
}
bool Point::operator==(const Point& a) const{
    if((m_x == a.m_x) && (m_y == a.m_y))
        return true;
    return false;
}
bool Point::operator!=(const Point& a) const{
    return !operator==(a);
}

Point Point::operator+(const Point& a){
    return Point(m_x + a.m_x, m_y + a.m_y);
}

const Point& Point::operator+=(const Point& a){
    m_x += a.m_x;
    m_y += a.m_y;
    return *this;
}
