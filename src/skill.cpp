#include "skill.h"

#include <cstdlib>
#include "json/reader.h"
#include "json/value.h"
#include "utils.h"

Skill::Skill()
{
    //ctor
}

Skill::Skill(std::string path){
    char filename[256];
    sprintf(filename, "data/skills/%s.skl", path.c_str());
    std::string in = get_file_contents(filename);

    Json::Value root;
    Json::Reader reader;
    bool stat = reader.parse( in, root );
    if (stat){
        _name = root["Name"].asString();
        _description = root["Description"].asString();

        _MPC = root["MPC"].asInt();
        _eTarget = root["eTarget"].asInt();
        _eHPv = root["eHPv"].asInt();
        _eMPv = root["eMPv"].asInt();
        _eATKv = root["eATKv"].asInt();
        _eDEFv = root["eDEFv"].asInt();
        _fTarget = root["fTarget"].asInt();
        _fHPv = root["fHPv"].asInt();
        _fMPv = root["fMPv"].asInt();
        _fATKv = root["fATKv"].asInt();
        _fDEFv = root["fDEFv"].asInt();

    }else{
        std::cout << "Failed to parse configuration\n"  << reader.getFormatedErrorMessages();
        exit(128);
    }
}

Skill::~Skill()
{
    //dtor
}
