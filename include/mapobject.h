#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <string>
#include <vector>
#include "eventcontroller.h"
#include "utils.h"
#include "point.h"

class mapObject
{
    public:
        friend class gmap;
        friend class mapController;

        mapObject();
        mapObject(std::string, int, int, int, int, std::string, Point, std::string, bool, int, int);
        virtual ~mapObject();

        Point GetCord() const { return Cord; }
        std::string Getname() { return name; }
        int GetID() { return ID; }
        int Gettype() { return type; }
        bool canStepOn() { return _canStep; }

        std::string getTile() const { return tile; }
        virtual Point Geticon() const { return icon; }

        int getTriggerType() { return triggerType; };
        std::string getTrigger() { return trigger; };

        int getFacing() const { return facing; }
        void setFacing(int);
    protected:
        Point Cord;
        void SetCord(Point val) { Cord = val; }
    private:

        std::string name;
        int ID;
        int type;
        std::string tile;
        Point icon;
        bool _canStep;
        std::string trigger;
        int triggerType;

        int facing = faceing::SOUTH;
};

#endif // MAPOBJECT_H
