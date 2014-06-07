#include <string>
#include <cstring>
#include "point.h"
#include "array2d.h"

template<class T>
Array2D<T>::Array2D()
{
    m_size = Point(2,2);
    m_data = new T*[m_size.m_y];
    m_data[0] = new T[m_size.m_x * m_size.m_y];
    for(unsigned int i = 1; i < m_size.m_y; i++)
        m_data[i] = m_data[0] + i * m_size.m_x;
}

template<class T>
Array2D<T>::Array2D(Point s)
{
    m_size = s;
    m_data = new T*[m_size.m_y];
    m_data[0] = new T[m_size.m_x * m_size.m_y];
    for(unsigned int i = 1; i < m_size.m_y; i++)
        m_data[i] = m_data[0] + i * m_size.m_x;
}

/*template<class T>
Array2D<T>::Array2D(Point s, void* loading)
{
    m_size = s;
    m_data = new T*[m_size.m_y];
    m_data[0] = new T[m_size.m_x * m_size.m_y];
    for(unsigned int i = 1; i < m_size.m_y; i++)
        m_data[i] = m_data[0] + i * m_size.m_x;
    memcpy(m_data[0], loading, sizeof(T) * (m_size.m_x) * (m_size.m_y));
}*/

template<class T>
Array2D<T>::~Array2D()
{
    delete [] m_data[0];
    delete [] m_data;
}

template<class T>
Array2D<T>::Array2D(const Array2D& other)
{
    m_size = other.m_size;
    m_data = new T*[m_size.m_y];
    m_data[0] = new T[m_size.m_x * m_size.m_y];
    for(unsigned int i = 1; i < m_size.m_y; i++)
        m_data[i] = m_data[0] + i * m_size.m_x;

    for(unsigned int i = 0; i < m_size.m_y; i++){
        for(unsigned int j = 0; j < m_size.m_x; j++){
            m_data[i][j] = other.m_data[i][j];
        }
    }
}

template<class T>
Array2D<T>& Array2D<T>::operator=(const Array2D& rhs)
{
    if (this == &rhs) return *this; // handle self assignment

    delete [] m_data[0];
    delete [] m_data;

    m_size = rhs.m_size;
    m_data = new T*[m_size.m_y];
    m_data[0] = new T[m_size.m_x * m_size.m_y];
    for(unsigned int i = 1; i < m_size.m_y; i++)
        m_data[i] = m_data[0] + i * m_size.m_x;
    for(unsigned int i = 0; i < m_size.m_y; i++){
        for(unsigned int j = 0; j < m_size.m_x; j++){
            m_data[i][j] = rhs.m_data[i][j];
        }
    }

    return *this;
}

/*template<class T>
int Array2D<T>::operator=(const std::string& rhs)
{
    if(rhs.size() != (m_size.m_x * m_size.m_y))
        return -1;
    memcpy(m_data[0], rhs.c_str(), sizeof(T) * (m_size.m_x) * (m_size.m_y));
    return 0;
}*/

template<class T>
T* Array2D<T>::operator[](unsigned int subscript)
{
    if(subscript >= m_size.m_y)
        return 0;
    else
        return m_data[subscript];
}

template<class T>
Point Array2D<T>::size()
{
    return m_size;
}

template class Array2D<char>;
template class Array2D<int>;
template class Array2D<wchar_t>;
template class Array2D<Point>;

