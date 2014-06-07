#ifndef POINT_H
#define POINT_H

class Point
{
    public:
        Point();
        Point(int, int);
        virtual ~Point();

        Point(const Point& other);
        Point& operator=(const Point& other);

        unsigned int Get_x() { return m_x; }
        void Set_x(unsigned int val) { m_x = val; }

        unsigned int Get_y() { return m_y; }
        void Set_y(unsigned int val) { m_y = val; }

        unsigned int m_x;
        unsigned int m_y;

        bool operator<(const Point&) const;
        bool operator<=(const Point&) const;
        bool operator>(const Point&) const;
        bool operator>=(const Point&) const;
        bool operator==(const Point&) const;
        bool operator!=(const Point&) const;

        Point operator+(const Point&);
        const Point& operator+=(const Point&);
    protected:
    private:

};

#endif // POINT_H
