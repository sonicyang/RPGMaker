#include <algorithm>
#include <cstring>
#include "prompt.h"
#include "utils.h"
#include "eventcontroller.h"
#include "json/reader.h"
#include "json/value.h"
#include "utf8.h"
#include "engine.h"

using namespace std;

eventController::eventController(Engine* eng, std::map< std::string, variant<paraVarType> >* m) :
    genericContorller(eng),
    varMap(m)
{
}

eventController::eventController(std::string eventlist, Engine* eng, std::map< std::string, variant<paraVarType> >* m) :
    genericContorller(eng),
    varMap(m),
    userInputRequired(0)
{
	std::string in = get_file_contents(eventlist.c_str());

	Json::Value root;
	Json::Reader reader;
	if (reader.parse( in, root )){
	    event_count = root.get("Count", 0 ).asInt();
	    for(unsigned int i = 0; i < event_count; i++){
	            std::string in2 = get_file_contents(root["Path"][i].asCString());

	            Json::Value root2;
	            Json::Reader reader2;
	            if (reader.parse( in2, root2 )){
	            	event trigger;

	                trigger.stk.resize(root2["Command"].size());
	                for(unsigned int i = 0; i < root2["Command"].size(); i++){
	                    trigger.stk[i] = root2["Command"][i].asString();
	                }

	                trigger.trigBy = root2["Name"].asString();

	                trigger.triggerType = root2["triggerType"].asInt();

	                trigger.pc = 0;

	                event_list.insert (event_list.begin(), std::pair<std::string, event>(trigger.trigBy, trigger));
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

eventController::~eventController()
{
    //dtor
}

int eventController::hDoEvent(){
    execTopEvent();
    return 0;
}

void eventController::popEventStack(){
    if(eventStack.size() > 0){
        eventStack.pop_back();
    }
    return;
}

int eventController::execTopEvent(){
    struct event& currentEvent = eventStack.back();

    if(currentEvent.stk.size() == currentEvent.pc + 1){
        engine->engineCall(loadStack(svc::popEvent));
        engine->engineCall(loadStack(svc::restoreStat));
        return -1;
    }

    std::vector<std::string> ss = split(currentEvent.stk[currentEvent.pc], '|');

    int commd = atoi(ss[0].c_str());
    switch(commd){
        case 0:
            /*currBattle = battle(ss[1]);
            currBattle.exec();
            return 0;*/
        case eventCode::showPrompt:
            engine->engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar(ss[2].c_str()), UTF8_to_WChar(ss[1].c_str())));
            userInputRequired = 0;
            break;
        case eventCode::endEvent:
            engine->engineCall(loadStack(svc::clearPrompt));
            engine->engineCall(loadStack(svc::popEvent));
            engine->engineCall(loadStack(svc::restoreStat));
            break;
        case eventCode::transferMap:
            engine->engineCall(loadStack(svc::changeMap, ss[1], atoi(ss[2].c_str()), atoi(ss[3].c_str())));
            break;
        case eventCode::addItem:
            engine->engineCall(loadStack(svc::addItem, ss[1], atoi(ss[2].c_str())));
            break;
        case eventCode::removeItem:
            engine->engineCall(loadStack(svc::removeItem, ss[1], atoi(ss[2].c_str())));
            break;
        case eventCode::setMoney:
            engine->engineCall(loadStack(svc::setMoney, atoi(ss[1].c_str())));
            break;
        case eventCode::addMoney:
            engine->engineCall(loadStack(svc::addMoney, atoi(ss[1].c_str())));
            break;
        case eventCode::jner:
            if(varMap->at(ss[1]).get<int>() != atoi(ss[2].c_str())){
                currentEvent.pc += atoi(ss[3].c_str());
                if(eventStack.back().pc >= currentEvent.stk.size())
                    throw(64);
            }
            break;
        case eventCode::jer:
            if(varMap->at(ss[1]).get<int>() == atoi(ss[2].c_str())){
                currentEvent.pc += atoi(ss[3].c_str());
                if(currentEvent.pc >= currentEvent.stk.size())
                    throw(64);
            }
            break;
        case eventCode::jne:
            if(varMap->at(ss[1]).get<int>() != atoi(ss[2].c_str())){
                currentEvent.pc = atoi(ss[3].c_str());
                if(eventStack.back().pc >= currentEvent.stk.size())
                    throw(64);
            }
            break;
        case eventCode::je:
            if(varMap->at(ss[1]).get<int>() == atoi(ss[2].c_str())){
                currentEvent.pc = atoi(ss[3].c_str());
                if(currentEvent.pc >= currentEvent.stk.size())
                    throw(64);
            }
            break;
        case eventCode::mov:
            varMap->at(ss[1]).set<int>(varMap->at(ss[2]).get<int>());
            break;
        case eventCode::set:
            varMap->at(ss[1]).set<int>(atoi(ss[2].c_str()));
            break;
        case eventCode::addCharToTeam:
            engine->engineCall(loadStack(svc::addCharToTeam, ss[1]));
            break;
        case eventCode::removeCharFromTeam:
            engine->engineCall(loadStack(svc::removeCharFromTeam, ss[1]));
            break;
        case eventCode::battle:{
            std::vector<std::string> mons;
            for(unsigned int i = 1; i < ss.size(); i++)
                mons.push_back(ss[i]);
            engine->engineCall(loadStack(svc::battle, mons));
            break;
            }
        case eventCode::vendor:{
            std::vector<std::string> its;
            for(unsigned int i = 1; i < ss.size(); i++)
                its.push_back(ss[i]);
            engine->engineCall(loadStack(svc::setupVender, its));
            engine->engineCall(loadStack(svc::setStat, Stats::inVender));
            break;
            }
    }
    currentEvent.pc++;
    return 1;
}

int eventController::pushEvent(std::string name){
	eventStack.push_back(event_list[name]);
	eventStack.back().pc = 0;
	return 0;
}
