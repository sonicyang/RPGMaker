#ifndef ARRAY2D_H
#define ARRAY2D_H

#include "point.h"
#include <string>

template<class T>
class Array2D
{
    public:
        Array2D();
        Array2D(Point);
        //Array2D(Point, void*);
        ~Array2D();
        Array2D(const Array2D& other);
        Array2D& operator=(const Array2D& other);
        //int operator=(const std::string&);
        T* operator[](unsigned int subscript);

        Point size();
    protected:
    private:
        T** m_data;
        Point m_size;
};

#endif // 2DCHARARRAY_H
