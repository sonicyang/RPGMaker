#include <cstdlib>
#include "team.h"
#include "json/value.h"
#include "json/reader.h"
#include "json/writer.h"
#include "utils.h"

Team::Team() : roleCache(), null()
{

}

Team::Team(std::string teamList, std::string roleList) : roleCache(roleList), null()
{
    std::string in = get_file_contents(teamList.c_str());

	Json::Value root;
	Json::Reader reader;
	if (reader.parse( in, root )){
	    _cacheCount = root.get("Count", 0 ).asInt();
	    for(unsigned int i = 0; i < _cacheCount; i++){
            Character chara(&roleCache, root["All"][i]["Name"].asString(), root["All"][i]["Level"].asInt(), roleCache[root["All"][i]["Role"].asString()]);
            _cache.insert(_cache.begin(), std::pair<std::string, Character>(root["All"][i]["Name"].asString(), chara));
	    }

        for(unsigned int i = 0; i < root["Default"].size(); i++){
            _team.insert(_team.begin(), std::pair<std::string, Character>(root["Default"][i].asString(), _cache[root["Default"][i].asString()]));
	    }


	}else{
	    std::cout << "Failed to parse configuration\n"  << reader.getFormatedErrorMessages();
	    exit(128);
	}
}

Team::~Team()
{
    //dtor
}

std::vector<std::string> Team::getNameList() const{
    std::vector<std::string> tmp;

    for (auto it = _team.cbegin(); it != _team.cend(); it++){
        tmp.push_back((*it).first);
    }
    return tmp;
}

Character& Team::operator[](std::string subscript){
    if(_team.find(subscript) != _team.end())
        return _team[subscript];
    return null;
}

void Team::addCharToTeam(std::string name){
    if(_cache.find(name) == _cache.end())
        return;
    _team.insert(_team.begin(), std::pair<std::string, Character>(name, _cache[name]));
    return;
}

void Team::removeCharFromTeam(std::string name){
    if(_team.find(name) == _team.end())
        return;
    _team.erase(name);
    return;
}

bool Team::isWipeOut(){
    bool flag = true;
    for (auto it = _team.begin(); it != _team.end(); it++){
        flag &= it->second.isDead();
    }
    return flag;
}

std::string Team::save(){
    Json::Value root;
    int i = 0;
    for(auto it = _team.begin(); it != _team.end(); it++, i++){
        root["Member"][i]["Name"] = it->second.getName();
        root["Member"][i]["HP"] = it->second.getHP();
        root["Member"][i]["MP"] = it->second.getMP();
        root["Member"][i]["EXP"] = it->second.getExp();
        root["Member"][i]["Level"] = it->second.getLevel();
        root["Member"][i]["Head"] = it->second.getHead().getName();
        root["Member"][i]["Armor"] = it->second.getArmor().getName();
        root["Member"][i]["Legs"] = it->second.getLegs().getName();
        root["Member"][i]["Shoes"] = it->second.getShoes().getName();
        root["Member"][i]["Weapon"] = it->second.getWeapon().getName();
        root["Member"][i]["Role"] = it->second.getRoleName();
    }

    Json::StyledWriter writer;

    return writer.write(root);
}

void Team::load(std::string data){
    Json::Reader reader;
    Json::Value root;
    reader.parse(data, root);

    _team.clear();
    for(unsigned int i = 0; i < root["Slot"].size(); i++){
        addCharToTeam(root["Member"][i]["Name"].asString());
        _team[root["Member"][i]["Name"].asString()].setRole(root["Member"][i]["Role"].asString());
        _team[root["Member"][i]["Name"].asString()].setLevel(root["Member"][i]["Level"].asInt());
        _team[root["Member"][i]["Name"].asString()].setHead(Item(root["Member"][i]["Head"].asString()));
        _team[root["Member"][i]["Name"].asString()].setArmor(Item(root["Member"][i]["Armor"].asString()));
        _team[root["Member"][i]["Name"].asString()].setLegs(Item(root["Member"][i]["Legs"].asString()));
        _team[root["Member"][i]["Name"].asString()].setShoes(Item(root["Member"][i]["Shoes"].asString()));
        _team[root["Member"][i]["Name"].asString()].setWeapon(Item(root["Member"][i]["Weapon"].asString()));
        _team[root["Member"][i]["Name"].asString()].setHP(root["Member"][i]["HP"].asInt());
        _team[root["Member"][i]["Name"].asString()].setMP(root["Member"][i]["MP"].asInt());
        _team[root["Member"][i]["Name"].asString()].setExp(root["Member"][i]["EXP"].asInt());
    }
}
