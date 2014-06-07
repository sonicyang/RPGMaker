#ifndef ENUM_H_INCLUDED
#define ENUM_H_INCLUDED

#include <string>
#include <vector>

struct inventorySlot{
    Item item;
    int count = -1;
};

enum itemTypes{
    potion = 0,
    allPotion,
    head,
    armor,
    legs,
    shoes,
    weapon,
    null = -1
};

enum Stats{
    pending = 0,
    onMap,
    inEvent,
    inMainMenu,
    inTeamMenu,
    inInvMenu,
    inVendorInvMenu,
    inCharMenu,
    inSkillMenu,
    inBattle,
    inPrompt,
    inVender,
    inHelp
};

struct event{
    std::vector<std::string> stk;
    std::string trigBy;
    int triggerType;
    unsigned int pc;
};

enum trigType{
    buttonTrig = 1,
    stepOnTrig
};

enum faceing{
    NORTH = 0,
    EAST,
    SOUTH,
    WEST
};

enum svc{
    popEvent = -2,
    loadEvent,     // EventName
    restoreStat,        //              0
    setStat,            // stat Number
    loadPrompt,         // data, whom
    clearPrompt,        //
    changeMap,          // mapName, x, y
    addItem,            // id, amount   5
    removeItem,         //id, amount
    incItem,            //id
    decItem,            //id
    setMoney,           //Amount
    addMoney,           //Amount        10
    addCharToTeam,      //Name
    removeCharFromTeam, //Name
    battle,
    loadMainMenu,
    loadTeamMenu,       //Mode          15
    loadInvMenu,        //Mode
    loadCharMenu,       //Mode
    loadSkillMenu,      //Mode
    invEnableNull,
    invDisableNull,     //              20
    changeEquip,        //itemIndex, memberIndex, slotIndex
    isItemUsable,       //itemIndex
    getItemType,        //itemIndex
    useItem,            //itemIndex, memberIndex
    isCharDead,         //memberIndex
    isTeamWipeOut,
    queryAttack,        //memberIndex   26
    queryDefense,       //memberIndex
    varHP,              //memberIndex, Amount
    varMP,              //memberIndex, Amount
    varExp,             //memberIndex, Amount
    moveVar,            //dis, src      31
    qureySkill,         //memberIndex, skillIndex
    useSkill,           //memberIndex, skillIndex
    setupVender,        //List of Item
    loadVenderInvMenu,
    sellItem,           //itemIndex
    gameOver,
    endGame = 255,       //
    loadGame,
    saveGame,
    closeGame
};

#endif // ENUM_H_INCLUDED
