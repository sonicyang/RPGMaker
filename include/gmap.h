#ifndef GMAP_H
#define GMAP_H

#include <string>
#include <set>
#include <map>
#include "mapobject.h"
#include "array2d.h"
#include "point.h"

class gmap
{
    public:
        gmap();
        gmap(const char * filename);
        ~gmap();
        gmap(const gmap& other);
        gmap& operator=(const gmap& other);

        Point Getsize() { return m_size; }

        std::string Getname() const { return m_name; }
        void Setname(std::string val) { m_name = val; }

        std::string getTile() { return tile; }

        Array2D<int> Getdata_x() { return m_data_x; }
        Array2D<int> Getdata_y() { return m_data_y; }

        bool isObstacle(Point);
        bool isOutOfBound(Point);

        bool isPlayerFacingObject();

        mapObject* getObject(Point);
        const std::map<Point,mapObject>& getObjects() { return objects; };

        std::string getBackground() { return Background; };

    protected:
    private:
        Point m_size;
        std::string m_name;
        std::string tile;
        Array2D<int> m_data_x;
        Array2D<int> m_data_y;
        Array2D<int> m_data_state;
        std::set<wchar_t> blocks;

        std::map<Point, mapObject> objects;

        std::string Background;
};

#endif // GMAP_H
