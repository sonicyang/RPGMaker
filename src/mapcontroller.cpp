#include <cstdlib>
#include <deque>
#include "mapcontroller.h"
#include "utils.h"
#include "json/reader.h"
#include "json/value.h"
#include "json/writer.h"
#include "engine.h"
#include "utf8.h"
#include "render.h"

using namespace std;

mapController::mapController(Engine* eng) : genericContorller(eng)
{

}

mapController::mapController(std::string maplist, Engine* eng) : genericContorller(eng)
{
    std::string in = get_file_contents(maplist.c_str());

    Json::Value root;
    Json::Reader reader;
    bool stat = reader.parse( in, root );
    if (stat){
        map_count = root.get("Count", 0 ).asInt();

        for(unsigned int i = 0; i < map_count; i++){
            gmap tmp(root["Path"][i].asString().c_str());
            map_list.insert (map_list.begin(), std::pair<std::string,gmap>(tmp.Getname(),tmp));
        }
        defaultMap = currentMap = root["Default"].asString();

        player = mapObject("Player", 0, 0, 1, 1, root["pTile"].asString(), Point(root["pIcon"]["x"].asInt(), root["pIcon"]["y"].asInt()), "", 0, 0, faceing::EAST);
    }else{
        std::cout << "Failed to parse configuration\n"  << reader.getFormatedErrorMessages();
        exit(128);
    }
}

mapController::~mapController()
{
    //dtor
}

void mapController::setCurrentMap(std::string a){
    if(map_list.find(a) != map_list.end())
        currentMap = a;
}

int mapController::hKeyUp(){
    movePlayer(Point(0,-1));
    return 0;
}

int mapController::hKeyDown(){
    movePlayer(Point(0,1));
    return 0;
}

int mapController::hKeyLeft(){
    movePlayer(Point(-1,0));
    return 0;
}

int mapController::hKeyRight(){
    movePlayer(Point(1,0));
    return 0;
}

int mapController::hKeyZ(){
    if(isPlayerFacingObject()){
        if(getPlayerFacingObject().getTriggerType() == buttonTrig){
            engine->engineCall(loadStack(svc::loadEvent, getPlayerFacingObject().getTrigger()));
            engine->engineCall(loadStack(svc::setStat, Stats::inEvent));
        }
    }
    return 0;
}

int mapController::hKeyX(){
    return 0;
}

int mapController::hKeyQ(){
    engine->engineCall(loadStack(svc::loadMainMenu));
    engine->engineCall(loadStack(svc::setStat, Stats::inMainMenu));
    return 0;
}

int mapController::hDoEvent(){
    return 0;
}

int mapController::hRender(render& rdr){
    rdr.render_map(map_list[currentMap], player);
    return 0;
}

const gmap& mapController::getCurrentMap(){
    return map_list[currentMap];
}

int mapController::setPlayerPosition(Point a){
    if(map_list[currentMap].isOutOfBound(a))
        return -1;
    if(map_list[currentMap].isObstacle(a))
        return -1;
    player.SetCord(a);

    if(isPlayerOnObject()){
        if(getPlayerOnObject().getTriggerType() == stepOnTrig){
            engine->engineCall(loadStack(svc::loadEvent, getPlayerOnObject().getTrigger()));
            engine->engineCall(loadStack(svc::setStat, Stats::inEvent));
        }
    }

    return 1;
}

int mapController::movePlayer(Point a){
    Point curr = player.GetCord();
    if(a.m_y != 0)
        player.setFacing(a.m_y + 1);
    else
        player.setFacing(a.m_x * -1 + 2);
    return setPlayerPosition(curr + a);
}

Point mapController::getPlayerPosition(){
    return player.GetCord();
}

Point mapController::getPlayerFacing(){
    Point cordFacing = player.GetCord();
    switch(player.getFacing()){
        case 0:
            cordFacing += Point(0,-1);
            break;
        case 1:
            cordFacing += Point(1,0);
            break;
        case 2:
            cordFacing += Point(0,1);
            break;
        case 3:
            cordFacing += Point(-1,0);
            break;
    }
    return cordFacing;
}

bool mapController::isPlayerFacingObject(){
    mapObject* obj = map_list[currentMap].getObject(getPlayerFacing());
    if(!obj)
        return false;
    return true;
}

mapObject& mapController::getPlayerFacingObject(){
    return *map_list[currentMap].getObject(getPlayerFacing());
}

bool mapController::isPlayerOnObject(){
    mapObject* obj = map_list[currentMap].getObject(player.GetCord());
    if(!obj)
        return false;
    return true;
}

mapObject& mapController::getPlayerOnObject(){
    return *map_list[currentMap].getObject(player.GetCord());
}

std::string mapController::save(){
    Json::Value root;
    root["Cord"]["x"] = player.GetCord().m_x;
    root["Cord"]["y"] = player.GetCord().m_y;
    root["Map"] = getCurrentMap().Getname();

    Json::StyledWriter writer;

    return writer.write(root);
}

void mapController::load(std::string data){
    Json::Reader reader;
    Json::Value root;
    reader.parse(data, root);

    player.SetCord(Point(root["Cord"]["x"].asInt(), root["Cord"]["y"].asInt()));
    setCurrentMap(root["Map"].asString());

    return;
}
