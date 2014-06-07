#include <cstdlib>
#include <iostream>

#include "rolefactory.h"
#include "json/reader.h"
#include "json/value.h"
#include "utils.h"

RoleFactory::RoleFactory()
{

}

RoleFactory::RoleFactory(std::string roleList)
{
    std::string in = get_file_contents(roleList.c_str());

	Json::Value root;
	Json::Reader reader;
	if (reader.parse( in, root )){
	    _roleCount = root.get("Count", 0 ).asInt();
	    for(unsigned int i = 0; i < _roleCount; i++){
	            std::string in2 = get_file_contents(root["Path"][i].asCString());

	            Json::Value root2;
	            Json::Reader reader2;
	            if (reader.parse( in2, root2 )){
                    std::map<int, Skill> skills;

                    for(unsigned int k = 0; k < root2["Skills"].size(); k++){
                        skills.insert(skills.begin(), std::pair<int, Skill>(root2["Skills"][k]["Level"].asInt(), root2["Skills"][k]["Name"].asString()));
                    }

	            	Role role(root2["Name"].asString(), root2["HP"][(unsigned int)0].asInt(), root2["HP"][(unsigned int)1].asInt(),
                                                        root2["MP"][(unsigned int)0].asInt(), root2["MP"][(unsigned int)1].asInt(),
                                                        root2["Attack"][(unsigned int)0].asInt(), root2["Attack"][(unsigned int)1].asInt(),
                                                        root2["Defense"][(unsigned int)0].asInt(), root2["Defense"][(unsigned int)1].asInt(),
                                                        skills);

	                _roleCache.insert(_roleCache.begin(), std::pair<std::string, Role>(root2["Name"].asString(), role));
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

RoleFactory::~RoleFactory()
{
    //dtor
}

Role RoleFactory::operator[](std::string val){
    if(_roleCache.find(val) != _roleCache.end())
        return _roleCache[val];
    return _null;
}
