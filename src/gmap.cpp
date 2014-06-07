#include <cstdlib>
#include <cstring>
#include <string>
#include <string>
#include <curses.h>
#include "json/reader.h"
#include "json/value.h"
#include "gmap.h"
#include "point.h"
#include "utils.h"
#include "array2d.h"
#include "mapobject.h"
#include "utf8.h"

gmap::gmap()
{
    this->m_name = "NULL";

    m_size.Set_x(1);
    m_size.Set_y(1);

    m_data_x = Array2D<int>(m_size);
    m_data_y = Array2D<int>(m_size);
    m_data_state = Array2D<int>(m_size);
}



gmap::gmap(const char * filename)
{
    std::string in = get_file_contents(filename);

    Json::Value root;
    Json::Reader reader;
    bool stat = reader.parse( in, root );
    if (stat){
        this->m_name = root.get("Name", "NULL" ).asString();

        this->tile = root["Tile"].asString();

        m_size.Set_x(root["Size"]["x"].asInt());
        m_size.Set_y(root["Size"]["y"].asInt());

        m_data_x = Array2D<int>(m_size);
        for(unsigned int i = 0; i < m_size.m_y; i++){
            for(unsigned int j = 0; j < m_size.m_x; j++){
                m_data_x[i][j] = root["Floorx"][i * m_size.m_x + j].asInt();
            }
        }

        m_data_y = Array2D<int>(m_size);
        for(unsigned int i = 0; i < m_size.m_y; i++){
            for(unsigned int j = 0; j < m_size.m_x; j++){
                m_data_y[i][j] = root["Floory"][i * m_size.m_x + j].asInt();
            }
        }

        m_data_state = Array2D<int>(m_size);
        for(unsigned int i = 0; i < m_size.m_y; i++){
            for(unsigned int j = 0; j< m_size.m_x; j++){
                m_data_state[i][j] = root["Floors"][i * m_size.m_x + j].asInt();
            }
        }

        for(unsigned int i = 0; i < root["Blocks"].size(); i++){
            blocks.insert(UTF8_to_WChar(root["Blocks"][i].asCString())[0]);
        }

        for(unsigned int i = 0; i < root["Objects"].size(); i++){
            Point cord(root["Objects"][i]["x"].asInt(), root["Objects"][i]["y"].asInt());
            mapObject toAdd(root["Objects"][i]["Name"].asString(), i,
                            root["Objects"][i]["Type"].asInt(),
                            cord.m_x, cord.m_y,
                            root["Objects"][i]["Tile"].asString(),
                            Point(root["Objects"][i]["Icon"]["x"].asInt(), root["Objects"][i]["Icon"]["y"].asInt()),
                            root["Objects"][i]["Trigger"].asString(),
                            root["Objects"][i]["canStep"].asInt(),
                            root["Objects"][i]["trigType"].asInt(),
                            root["Objects"][i]["Facing"].asInt());
            objects.insert(objects.begin(),std::pair<Point,mapObject>(cord, toAdd));
        }

        Background = root["Background"].asString();

    }else{
        std::cout  << "Failed to parse configuration\n"  << reader.getFormatedErrorMessages();
        exit(128);
    }
}

gmap::~gmap()
{
}

gmap::gmap(const gmap& other)
{
    m_name = other.m_name;
    m_size = other.m_size;

    tile = other.tile;

    m_data_x = other.m_data_x;
    m_data_y = other.m_data_y;
    m_data_state = other.m_data_state;

    blocks = other.blocks;

    objects = other.objects;

    Background = other.Background;
}

gmap& gmap::operator=(const gmap& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
        m_name = rhs.m_name;
        m_size = rhs.m_size;

        tile = rhs.tile;

        m_data_x = rhs.m_data_x;
        m_data_y = rhs.m_data_y;
        m_data_state = rhs.m_data_state;

        blocks = rhs.blocks;
        objects = rhs.objects;

        Background = rhs.Background;
    return *this;
}

bool gmap::isObstacle(Point a){
    if(m_data_state[a.m_y][a.m_x] != 0)
        return true;
    if(objects.find(a) != objects.end())
        if(!objects.find(a)->second.canStepOn())
            return true;
    return false;
}

bool gmap::isOutOfBound(Point a){
    if(a.m_x >= m_size.m_x || a.m_y >= m_size.m_y)
        return true;
    return false;
}

mapObject* gmap::getObject(Point a){
    if(objects.find(a) != objects.end())
        return &(objects[a]);
    return NULL;
}


