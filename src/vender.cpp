#include "vender.h"

#include "utils.h"
#include "menu.h"
#include "engine.h"
#include "utf8.h"

Vender::Vender(Engine* eng, std::map< std::string, variant<paraVarType> >& b) :
    genericContorller(eng),
    varMap(b)
{
    mOptions.push_back("Sell");
    mOptions.push_back("Buy");
    mOptions.push_back("Exit");
}

Vender::~Vender()
{
    //dtor
}

int Vender::setUp(std::vector<std::string> nameList){
    venderInv.clearInv();
    for(unsigned int i = 0; i < nameList.size(); i++){
        venderInv.incItem(nameList[i]);
    }
    topCurPos = 0;
    varMap["VenderTopCurPos"].set<unsigned int>(topCurPos);
    return 0;
}

int Vender::hKeyUp(){
    topCurPos = (topCurPos==0)? 2 : topCurPos - 1;
    return 0;
}

int Vender::hKeyDown(){
    topCurPos = (topCurPos == 2)? 0 : topCurPos + 1;
    return 0;
}

int Vender::hKeyZ(){
    switch(topCurPos){
        case 0:
            engine->engineCall(loadStack(svc::loadInvMenu, 1));
            engine->engineCall(loadStack(svc::setStat, Stats::inInvMenu));
            while(varMap["InvMenuCurPos"].get<unsigned int>() != 0xffffffff){
                engine->engineCall(loadStack(svc::sellItem, varMap["InvMenuCurPos"].get<unsigned int>()));
                engine->engineCall(loadStack(svc::loadInvMenu, 1));
                engine->engineCall(loadStack(svc::setStat, Stats::inInvMenu));
            }
            break;
        case 1:
            engine->engineCall(loadStack(svc::loadVenderInvMenu, 1));
            engine->engineCall(loadStack(svc::setStat, Stats::inVendorInvMenu));
            while(varMap["InvMenuCurPos"].get<unsigned int>() != 0xffffffff){
                unsigned int p = varMap["InvMenuCurPos"].get<unsigned int>();
                if(engine->engineCall(loadStack(svc::addMoney, (-1) * venderInv[venderInv.getNameList(p)[0]].item.getBuyPrice())).get<int>() != -1){
                    engine->engineCall(loadStack(svc::incItem, venderInv.getNameList(p)[0]));
                }else{
                    engine->engineCall(loadStack(svc::loadPrompt, UTF8_to_WChar("You Don't Have Engough Money!"), UTF8_to_WChar("System")));
                }
                engine->engineCall(loadStack(svc::loadVenderInvMenu, 1));
                engine->engineCall(loadStack(svc::setStat, Stats::inVendorInvMenu));
            }
            break;
        case 2:
            engine->engineCall(loadStack(svc::restoreStat));
            break;
    }
    return 0;
}

int Vender::hKeyX(){
    engine->engineCall(loadStack(svc::restoreStat));
    return 0;
}

int Vender::hRender(render& rdr){
    rdr.render_VenderMenu(topCurPos, mOptions);
    return 0;
}
