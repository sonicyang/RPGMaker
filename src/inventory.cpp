#include <json/value.h>
#include <json/writer.h>
#include <json/reader.h>

#include "inventory.h"

inventory::inventory() : _inventorySlots()
{
    null.count = -1;
    null.item = Item();
}

inventory::~inventory()
{
    //dtor
}

int inventory::incItem(std::string id){
    return addItem(id, 1);
}

int inventory::addItem(std::string id, unsigned int a){
    setItemAmount(id, getItemAmount(id) + a);
    return a;
}

int inventory::decItem(std::string id){
    return removeItem(id, 1);
}

int inventory::removeItem(std::string id, unsigned int a){
    unsigned int curr = getItemAmount(id);
    if(curr < a)
        return -1;
    setItemAmount(id, getItemAmount(id) - a);
    return a;
}

void inventory::setItemAmount(std::string id, unsigned int c){
    if(c == 0){
        _inventorySlots.erase(id);
        return;
    }
    if(_inventorySlots.find(id) == _inventorySlots.end())
        _inventorySlots[id].item = Item(id);
    _inventorySlots[id].count = c;
    return;
}
unsigned int inventory::getItemAmount(std::string id){
    if(_inventorySlots.find(id) == _inventorySlots.end())
        return 0;
    return _inventorySlots[id].count;
}

void inventory::enableNull(void){
    inventorySlot tmp;
    tmp.count = 1;
    tmp.item = Item();
    _inventorySlots.insert(_inventorySlots.begin(), std::pair<std::string, inventorySlot>("empty", tmp));
}

void inventory::disableNull(void){
    _inventorySlots.erase("empty");
}

unsigned int inventory::getNumOfItems(){
    return _inventorySlots.size();
}

std::vector<std::string> inventory::getNameList(unsigned int start) const{
    std::vector<std::string> tmp;

    auto it = _inventorySlots.cbegin();
    for(unsigned int i = 0; i < start; i++, it++);

    for (int i = 0; it != _inventorySlots.cend() && i < 10; it++, i++){
        tmp.push_back((*it).first);
    }

    return tmp;
}

const inventorySlot& inventory::operator[](std::string subscript){
    if(_inventorySlots.find(subscript) == _inventorySlots.end()){
        return null;
    }
    return _inventorySlots[subscript];
}

int inventory::addMoney(int val){
    if(val > 0){
        _money += val;
    }else{
        if(_money >= (-1) * val)
            _money += val;
        else
            return -1;
    }
    return 0;
}

std::string inventory::save(){
    Json::Value root;
    root["Money"] = _money;
    int i = 0;
    for(auto it = _inventorySlots.begin(); it != _inventorySlots.end(); it++, i++){
        root["Slot"][i]["Name"] = it->second.item.getName();
        root["Slot"][i]["Count"] = it->second.count;
    }

    Json::StyledWriter writer;

    return writer.write(root);
}

void inventory::load(std::string data){
    Json::Reader reader;
    Json::Value root;
    reader.parse(data, root);

    _money = root["Money"].asInt();
    for(unsigned int i = 0; i < root["Slot"].size(); i++){
        addItem(root["Slot"][i]["Name"].asString(), root["Slot"][i]["Count"].asInt());
    }
}
