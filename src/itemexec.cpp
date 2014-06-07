#include "itemexec.h"

#include "menu.h"
#include "render.h"
#include "prompt.h"

int ItemExec::Exec(inventory& inv, int item, Team& team, int chara){

    std::string name = inv.getNameList(item)[0];

        switch(inv[name].item.getType()){
            case itemTypes::potion:{
                team[team.getNameList()[chara]].varHP(inv[name].item.getHPVarient());
                team[team.getNameList()[chara]].varMP(inv[name].item.getMPVarient());

                break;
            }
            case itemTypes::allPotion:{
                    std::vector<std::string> memberList = team.getNameList();
                    for(unsigned int i = 0; i < memberList.size(); i++){
                        team[team.getNameList()[i]].varHP(inv[name].item.getHPVarient());
                        team[team.getNameList()[i]].varMP(inv[name].item.getMPVarient());
                    }
                    break;
            }
        }

        if(inv[name].item.isComsumable())
            inv.decItem(name);
    return 0;
}

int ItemExec::changeItem(inventory& inv, int item, Team& team, int chara, int slot){
    Item selectedItem = inv[inv.getNameList(item)[0]].item;
    Item currItemName;

    int ret;
    std::string cname = team.getNameList()[chara];

    switch(slot){
        case 0:
            currItemName = team[cname].getHead();
            ret = team[cname].setHead(selectedItem);
            break;
        case 1:
            currItemName = team[cname].getArmor();
            ret = team[cname].setArmor(selectedItem);
            break;
        case 2:
            currItemName = team[cname].getLegs();
            ret = team[cname].setLegs(selectedItem);
            break;
        case 3:
            currItemName = team[cname].getShoes();
            ret = team[cname].setShoes(selectedItem);
            break;
        case 4:
            currItemName = team[cname].getWeapon();
            ret = team[cname].setWeapon(selectedItem);
            break;
        }

    if(ret != -1){
        inv.decItem(inv.getNameList(item)[0]);
        if(currItemName.getName() != "Empty")
            inv.incItem(currItemName.getName());
    }

    return ret;
}

int ItemExec::skillExec(Team& _team, int chara, int skill, std::vector<Monster>& _monsters, int monsterId, int friendlyId){
    std::vector<std::string> memberList = _team.getNameList();
    Skill tmp = _team[memberList[chara]].getSkillList()[skill];
    
    if(_team[memberList[chara]].getMP() < tmp.getMPC())
        return 0;

    if(tmp.geteTarget() == 0){
        _monsters[monsterId].varHP(tmp.geteHPv());
        _monsters[monsterId].varMP(tmp.geteMPv());
    }else if(tmp.geteTarget() == 1){
        for(unsigned int m = 0; m < _monsters.size(); m++){
            _monsters[m].varHP(tmp.geteHPv());
            _monsters[m].varMP(tmp.geteMPv());
        }
    }
    if(tmp.getfTarget() == 0){
        _team[memberList[friendlyId]].varHP(tmp.getfHPv());
        _team[memberList[friendlyId]].varMP(tmp.getfMPv());
    }else if(tmp.getfTarget() == 1){
        for(unsigned int m = 0; m < memberList.size(); m++){
            _team[memberList[m]].varHP(tmp.getfHPv());
            _team[memberList[m]].varMP(tmp.getfMPv());
        }
    }


    _team[memberList[chara]].varMP((-1) * tmp.getMPC());
    return 1;
}
