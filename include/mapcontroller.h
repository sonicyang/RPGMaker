#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H

#include "gmap.h"
#include "mapobject.h"
#include "variant.h"
#include "point.h"
#include "genericcontorller.h"

class Engine;

class mapController : public genericContorller
{
    public:
        mapController(Engine* eng);
        mapController(std::string maplist, Engine* eng);
        ~mapController();

        int hKeyUp();
        int hKeyDown();
        int hKeyLeft();
        int hKeyRight();
        int hKeyZ();
        int hKeyX();
        int hKeyQ();

        int hDoEvent();

        int hRender(render&);

        const gmap& getCurrentMap();
        void setCurrentMap(std::string);

        int setPlayerPosition(Point a);
        Point getPlayerPosition();
        int movePlayer(Point a);
        const mapObject& getPlayer() { return player; }

        bool isPlayerFacingObject();
        mapObject& getPlayerFacingObject();

        bool isPlayerOnObject();
        mapObject& getPlayerOnObject();

        std::string save();
        void load(std::string);
    protected:
    private:
        std::string currentMap;
        std::string defaultMap;
        std::map<std::string, gmap> map_list;
        unsigned int map_count;

        mapObject player;

        Point getPlayerFacing();
};

#endif // MAPCONTROLLER_H
