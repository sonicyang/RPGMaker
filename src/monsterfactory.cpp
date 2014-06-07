#include "monsterfactory.h"

#include <cstdlib>

#include "json/reader.h"
#include "json/value.h"
#include "utils.h"

MonsterFactory::MonsterFactory()
{
}

MonsterFactory::MonsterFactory(std::string monsterList)
{
    std::string in = get_file_contents(monsterList.c_str());

	Json::Value root;
	Json::Reader reader;
	if (reader.parse( in, root )){
	    _monsterCount = root.get("Count", 0 ).asInt();
	    for(unsigned int i = 0; i < _monsterCount; i++){
	            std::string in2 = get_file_contents(root["Path"][i].asCString());

	            Json::Value root2;
	            Json::Reader reader2;
	            if (reader.parse( in2, root2 )){
                    std::vector<Skill> skills;

                    for(unsigned int k = 0; k < root2["Skills"].size(); k++){
                        skills.push_back(Skill(root2["Skills"][k].asString()));
                    }

	            	Monster monster(root2["Name"].asString(),
                                    root2["HP"].asInt(), root2["MP"].asInt(),
                                    root2["ATK"].asInt(), root2["DEF"].asInt(), root2["EXP"].asInt(),
                                    skills,
                                    root2["Image"].asString());

	                _monsterCache.insert(_monsterCache.begin(), std::pair<std::string, Monster>(root2["Name"].asString(), monster));
	            }else{
	                std::cout << "Failed to parse configuration\n"  << reader.getFormatedErrorMessages();
	                exit(128);
	            }
	    }

	}else{
	    std::cout << "Failed to parse configuration\n"  << reader.getFormatedErrorMessages();
	    exit(128);
	}
}

MonsterFactory::~MonsterFactory()
{
    //dtor
}

Monster MonsterFactory::operator[](std::string val){
    if(_monsterCache.find(val) != _monsterCache.end())
        return _monsterCache[val];
    return _null;
}
