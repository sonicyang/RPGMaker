#define _XOPEN_SOURCE_EXTENDED

#include <iostream>
#include <unistd.h>
#include <locale.h>
#include <SDL2/SDL.h>
#include <json/reader.h>
#include <json/value.h>
#include "render.h"
#include "gmap.h"
#include "point.h"
#include "skill.h"
#include "prompt.h"
#include "gmap.h"

render::render()
{
}

render::render(std::string manifest)
{


    std::string in = get_file_contents(manifest.c_str());

    Json::Value root;
    Json::Reader reader;
    bool stat = reader.parse( in, root );
    if (stat){
        FONT_NAME_ARIAL = root["FontArial"].asString();
        FONT_NAME_COMIC = root["FontComic"].asString();

        WINDOW_SIZE_X = root["WindowSize"]["x"].asInt();
        WINDOW_SIZE_Y = root["WindowSize"]["y"].asInt();

        TILE_SIZE = root["TileSize"].asInt();

        BATTLEBACKGROUND = root["BattleBackground"].asString();

        FRAME = root["Frame"].asString();
        FRAME_SIDE = root["FrameSide"].asString();
        FRAME_TITLE = root["FrameTitle"].asString();
        CHAR_MENU = root["CharMenuOption"].asString();
        CHAR_MENU_SELECTED = root["CharMenuSelect"].asString();
        PROMPT = root["Prompt"].asString();
        BATTLEMENU = root["BattleMenu"].asString();
    }else{
        std::cout << "Failed to parse manifest\n"  << reader.getFormatedErrorMessages();
        exit(128);
    }


    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        throw(122);
    }

    win = SDL_CreateWindow("RPG", 100, 100, WINDOW_SIZE_X, WINDOW_SIZE_Y,
                SDL_WINDOW_SHOWN);
    if (win == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        throw(122);
    }

    ren = SDL_CreateRenderer(win, -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr){
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        throw(122);
    }

    TTF_Init();

    font_arial16 = TTF_OpenFont(FONT_NAME_ARIAL.c_str(), 16);
    font_comic16 = TTF_OpenFont(FONT_NAME_COMIC.c_str(), 16);
    font_comic32 = TTF_OpenFont(FONT_NAME_COMIC.c_str(), 32);
    font_comic50 = TTF_OpenFont(FONT_NAME_COMIC.c_str(), 50);

    if (font_arial16 == nullptr){
        std::cout << "SDL_CreateFont Error: " << SDL_GetError() << std::endl;
        throw(122);
    }

    if (font_comic16 == nullptr){
        std::cout << "SDL_CreateFont Error: " << SDL_GetError() << std::endl;
        throw(122);
    }

    if (font_comic32 == nullptr){
        std::cout << "SDL_CreateFont Error: " << SDL_GetError() << std::endl;
        throw(122);
    }

    if (font_comic50 == nullptr){
        std::cout << "SDL_CreateFont Error: " << SDL_GetError() << std::endl;
        throw(122);
    }
}

render::~render()
{
    TTF_Quit();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void render::render_map(gmap toRender, mapObject mo){
    clear();

    Point offset;
    offset.m_x = (getmaxx() - toRender.Getsize().m_x * TILE_SIZE) / 2;
    offset.m_y = (getmaxy() - toRender.Getsize().m_y * TILE_SIZE) / 2;

    std::string tmp2 = toRender.getBackground();

    if(tmp2.size() > 0){
        texture.loadFromFile(toRender.getBackground(), ren, false);
        texture.render(ren, 0, 0);
    }


    texture.loadFromFile(toRender.getTile(), ren, true);

    for(unsigned int i = 0; i < toRender.Getsize().m_y; i++){
       for(unsigned int j = 0; j < toRender.Getsize().m_x; j++){
           SDL_Rect ROI = {(int)toRender.Getdata_x()[i][j] * TILE_SIZE, (int)toRender.Getdata_y()[i][j] * TILE_SIZE, TILE_SIZE , TILE_SIZE};
           texture.render(ren, j * TILE_SIZE + offset.m_x, i * TILE_SIZE + offset.m_y, &ROI);
        }
    }

    std::map<Point,mapObject>::const_iterator it = toRender.getObjects().begin();
    for(; it != toRender.getObjects().end(); it++){
        texture.loadFromFile(it->second.getTile(), ren, true);
        SDL_Rect ROI = {(int)it->second.Geticon().m_x * TILE_SIZE + (int)it->second.getFacing() * TILE_SIZE, (int)it->second.Geticon().m_y * TILE_SIZE, TILE_SIZE , TILE_SIZE};
        texture.render(ren, it->second.GetCord().m_x * TILE_SIZE + offset.m_x, it->second.GetCord().m_y * TILE_SIZE + offset.m_y, &ROI);
    }

    texture.loadFromFile(mo.getTile(), ren, true);
    SDL_Rect ROI = {(int)mo.Geticon().m_x * TILE_SIZE + (int)mo.getFacing() * TILE_SIZE, (int)mo.Geticon().m_y * TILE_SIZE, TILE_SIZE , TILE_SIZE};
    texture.render(ren, mo.GetCord().m_x * TILE_SIZE + offset.m_x, mo.GetCord().m_y * TILE_SIZE + offset.m_y, &ROI);

    char tmp[100];
    sprintf(tmp, "Current Map : %s", toRender.Getname().c_str());

    SDL_Color textColor = { 255, 255, 255  };
    texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
    texture.render(ren, 5, 5);

    update();
    return;
}

void render::render_prompt(prompt& P){
    texture.loadFromFile(PROMPT, ren);
    texture.render(ren, 0, getmaxy() - 180);

    SDL_Color textColor = { 0xF4, 0xF0, 0xDD };
    texture.loadFromRenderedText(P.getWhom().c_str(), textColor, ren, font_arial16);
    texture.render(ren, 6, getmaxy() - 180 + 20);

    unsigned int i;
    for(i = ((getmaxx()) / 13); P.getMessage().size() > i; i += ((getmaxx()) / 13)){
        texture.loadFromRenderedText(P.getMessage().substr(i - ((getmaxx()) / 13), ((getmaxx()) / 13)).c_str(), textColor, ren, font_arial16);
        texture.render(ren, 6, getmaxy() - 180 + 60 + (i / ((getmaxx()) / 13) - 1) * 23);
    }

    texture.loadFromRenderedText(P.getMessage().substr(i - ((getmaxx()) / 13), P.getMessage().size()).c_str(), textColor, ren, font_arial16);
    texture.render(ren, 6, getmaxy() - 180 + 60 + (i / ((getmaxx()) / 13) - 1) * 23);

    update();
    return;
}

void render::render_MainMenu(int curPos, std::vector<std::string> options){
    clear();

    texture.loadFromFile(FRAME, ren);
    texture.render(ren, 0, 0);

    Point offset;
    offset.m_x = getmaxx() / 2;
    offset.m_y = getmaxy() / 2;

    SDL_Color textColor = {0xD8, 0xC6, 0x91};

    //Print Title
    texture.loadFromRenderedText("MENU", textColor, ren, font_comic50);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, 8);

    for(unsigned int i = 0; i < options.size(); i++){
        texture.loadFromRenderedText(options[i].c_str(), textColor, ren, font_comic32);
        texture.render(ren, offset.m_x - texture.getWidth() / 2, offset.m_y - options.size() / 2 * texture.getHeight() + texture.getHeight() * i);
    }

    //Print Selected Options
    textColor = { 0xF4, 0xF0, 0xDD };
    texture.loadFromRenderedText(options[curPos].c_str(), textColor, ren, font_comic32);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, offset.m_y - options.size() / 2 * texture.getHeight() + texture.getHeight() * curPos);

    update();
    return;
}

void render::render_TeamMenu(Team& team, unsigned int curPos){
    clear();

    std::vector<std::string> memberList = team.getNameList();

    clear();
    texture.loadFromFile(FRAME, ren);
    texture.render(ren, 0, 0);

    Point offset;
    offset.m_x = getmaxx() / 2;
    offset.m_y = getmaxy() / 2;

    SDL_Color textColor = {0xD8, 0xC6, 0x91};

    //Print Title
    texture.loadFromRenderedText("TEAM", textColor, ren, font_comic50);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, 8);

    for (unsigned int i = 0; i < memberList.size(); i++){
        if(i == curPos)
            texture.loadFromFile("data/menu/charmenu_selected.png", ren);
        else
            texture.loadFromFile("data/menu/charmenu.png", ren);

        texture.render(ren, 10, 80 + i * 130);

        char tmp[100];

        //Print Informations
        sprintf(tmp, "Name : %s", team[memberList[i]].getName().c_str());
        texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
        texture.render(ren, 20, 100 + i * 130);

        sprintf(tmp, "Level : %d", team[memberList[i]].getLevel());
        texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
        texture.render(ren, 350, 100 + i * 130);

        sprintf(tmp, "Role : %s", team[memberList[i]].getRoleName().c_str());
        texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
        texture.render(ren, 20, 130 + i * 130);

        sprintf(tmp, "HP : %d/%d", team[memberList[i]].getHP(), team[memberList[i]].getMaxHP());
        texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
        texture.render(ren, 20, 160 + i * 130);

        sprintf(tmp, "MP : %d/%d", team[memberList[i]].getMP(), team[memberList[i]].getMaxMP());
        texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
        texture.render(ren, 300, 160 + i * 130);

        sprintf(tmp, "EXP : %d/%d", team[memberList[i]].getExp(), team[memberList[i]].getLevelUpExp());
        texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
        texture.render(ren, 600, 160 + i * 130);
    }

    update();

}

void render::render_InvMenu(inventory& inv, int curPos){
    std::vector<std::string> nameList = inv.getNameList(curPos);

    clear();
    texture.loadFromFile(FRAME_SIDE, ren);
    texture.render(ren, 0, 0);

    Point offset;
    offset.m_x = getmaxx() / 2;
    offset.m_y = getmaxy() / 2;

    SDL_Color textColor = {0xD8, 0xC6, 0x91};

    //Print Title
    texture.loadFromRenderedText("INVENTORY", textColor, ren, font_comic50);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, 8);

    char mString[40];
    sprintf(mString, "Money: $%d", inv.getMoney());
    texture.loadFromRenderedText(mString, textColor, ren, font_comic16);
    texture.render(ren, 300, 560);

    for (unsigned int i = 0; i < nameList.size(); i++){
        texture.loadFromRenderedText(nameList[i].c_str(), textColor, ren, font_comic16);
        texture.render(ren, 20, 100 + i * texture.getHeight());
    }

    if(!nameList.empty()){
        //Print Selected Options
        textColor = { 0xF4, 0xF0, 0xDD };
        texture.loadFromRenderedText(nameList[0].c_str(), textColor, ren, font_comic16);
        texture.render(ren, 20, 100);
        textColor = {0xD8, 0xC6, 0x91};

        char tmp[100];
        //Print Informations
        sprintf(tmp, "Name : %s", inv[nameList[0]].item.getName().c_str());
        texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
        texture.render(ren, getmaxx() * 0.35, 100);

        sprintf(tmp, "Currently Have : %d", inv[nameList[0]].count);
        texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
        texture.render(ren, getmaxx() * 0.35, 130);

        texture.loadFromRenderedText("Description : ", textColor, ren, font_comic16);
        texture.render(ren, 300, 160);

        unsigned int xoff = (getmaxx() - 260) / 13;
        unsigned int i;
        for(i = xoff; inv[nameList[0]].item.getDescription().size() > i; i += xoff){
            texture.loadFromRenderedText(inv[nameList[0]].item.getDescription().substr(i - xoff, xoff).c_str(), textColor, ren, font_comic16);
            texture.render(ren, getmaxx() * 0.35 + 20, 180 + (i / xoff - 1) * 20);
        }

        texture.loadFromRenderedText(inv[nameList[0]].item.getDescription().substr(i - xoff, inv[nameList[0]].item.getDescription().size()).c_str(), textColor, ren, font_comic16);
        texture.render(ren, getmaxx() * 0.35 + 20, 180 + (i / xoff - 1) * 20);

    }

    update();
}

void render::render_CharMenu(Character& chara, int curPos){
    clear();
    texture.loadFromFile(FRAME_SIDE, ren);
    texture.render(ren, 0, 0);

    Point offset;
    offset.m_x = getmaxx() / 2;
    offset.m_y = getmaxy() / 2;

    SDL_Color textColor = {0xD8, 0xC6, 0x91};

    //Print Title
    texture.loadFromRenderedText(chara.getName().c_str(), textColor, ren, font_comic50);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, 5);

    char tmp[100];
    //Print Informations
    sprintf(tmp, "Name : %s", chara.getName().c_str());
    texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
    texture.render(ren, getmaxx() * 0.35, 100);

    sprintf(tmp, "Level : %d", chara.getLevel());
    texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
    texture.render(ren, getmaxx() * 0.35, 130);

    sprintf(tmp, "Role : %s", chara.getRoleName().c_str());
    texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
    texture.render(ren, getmaxx() * 0.35, 160);

    sprintf(tmp, "HP : %d + %d", chara.getBaseHP(), chara.getAdditionalHP());
    texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
    texture.render(ren, getmaxx() * 0.35, 190);

    sprintf(tmp, "MP : %d + %d", chara.getBaseMP(), chara.getAdditionalMP());
    texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
    texture.render(ren, getmaxx() * 0.35, 220);

    sprintf(tmp, "Attack : %d + %d", chara.getBaseAttack(), chara.getAdditionalAttack());
    texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
    texture.render(ren, getmaxx() * 0.35, 250);

    sprintf(tmp, "Defense : %d + %d", chara.getBaseDefense(), chara.getAdditionalDefense());
    texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
    texture.render(ren, getmaxx() * 0.35, 280);

    if(curPos == 0)textColor = { 0xF4, 0xF0, 0xDD };{
        texture.loadFromRenderedText("Head : ", textColor, ren, font_comic16);
        texture.render(ren, 20, 100);
        texture.loadFromRenderedText(chara.getHead().getName().c_str(), textColor, ren, font_comic16);
        texture.render(ren, 40, 120);
    }textColor = {0xD8, 0xC6, 0x91};

    if(curPos == 1)textColor = { 0xF4, 0xF0, 0xDD };{
        texture.loadFromRenderedText("Armor : ", textColor, ren, font_comic16);
        texture.render(ren, 20, 150);
        texture.loadFromRenderedText(chara.getArmor().getName().c_str(), textColor, ren, font_comic16);
        texture.render(ren, 40, 170);
    }textColor = {0xD8, 0xC6, 0x91};

    if(curPos == 2)textColor = { 0xF4, 0xF0, 0xDD };{
        texture.loadFromRenderedText("Legs : ", textColor, ren, font_comic16);
        texture.render(ren, 20, 200);
        texture.loadFromRenderedText(chara.getLegs().getName().c_str(), textColor, ren, font_comic16);
        texture.render(ren, 40, 220);
    }textColor = {0xD8, 0xC6, 0x91};

    if(curPos == 3)textColor = { 0xF4, 0xF0, 0xDD };{
        texture.loadFromRenderedText("Shoes : ", textColor, ren, font_comic16);
        texture.render(ren, 20, 250);
        texture.loadFromRenderedText(chara.getShoes().getName().c_str(), textColor, ren, font_comic16);
        texture.render(ren, 40, 270);
    }textColor = {0xD8, 0xC6, 0x91};

    if(curPos == 4)textColor = { 0xF4, 0xF0, 0xDD };{
        texture.loadFromRenderedText("Weapon : ", textColor, ren, font_comic16);
        texture.render(ren, 20, 300);
        texture.loadFromRenderedText(chara.getWeapon().getName().c_str(), textColor, ren, font_comic16);
        texture.render(ren, 40, 320);
    }textColor = {0xD8, 0xC6, 0x91};

    if(curPos == 5)textColor = { 0xF4, 0xF0, 0xDD };{
        texture.loadFromRenderedText("Show Skills", textColor, ren, font_comic16);
        texture.render(ren, 20, 350);
    }textColor = {0xD8, 0xC6, 0x91};

    update();
}

void render::render_SkillMenu(Character& chara, int curPos){
    std::vector<Skill> skills = chara.getSkillList();

    clear();
    texture.loadFromFile(FRAME_SIDE, ren);
    texture.render(ren, 0, 0);

    Point offset;
    offset.m_x = getmaxx() / 2;
    offset.m_y = getmaxy() / 2;

    SDL_Color textColor = {0xD8, 0xC6, 0x91};

    //Print Title
    texture.loadFromRenderedText("Skills", textColor, ren, font_comic50);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, 5);

    for (unsigned int i = 0; i < skills.size(); i++){
        texture.loadFromRenderedText(skills[i].getName().c_str(), textColor, ren, font_comic16);
        texture.render(ren, 20, 100 + i * texture.getHeight());
    }

    if(!skills.empty()){
        //Print Selected Options
        textColor = { 0xF4, 0xF0, 0xDD };
        texture.loadFromRenderedText(skills[0].getName().c_str(), textColor, ren, font_comic16);
        texture.render(ren, 20, 100);
        textColor = {0xD8, 0xC6, 0x91};

        char tmp[100];
        //Print Informations
        sprintf(tmp, "Name : %s", skills[0].getName().c_str());
        texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
        texture.render(ren, getmaxx() * 0.35, 100);

        sprintf(tmp, "Description :");
        texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
        texture.render(ren, getmaxx() * 0.35, 130);

        unsigned int xoff = (getmaxx() - 260) / 13;
        unsigned int i;
        for(i = xoff; skills[0].getDescription().size() > i; i += xoff){
            texture.loadFromRenderedText(skills[0].getDescription().substr(i - xoff, xoff).c_str(), textColor, ren, font_comic16);
            texture.render(ren, getmaxx() * 0.35 + 20, 150 + (i / xoff - 1) * 20);
        }

        texture.loadFromRenderedText(skills[0].getDescription().substr(i - xoff, skills[0].getDescription().size()).c_str(), textColor, ren, font_comic16);
        texture.render(ren, getmaxx() * 0.35 + 20, 150 + (i / xoff - 1) * 20);
    }

    update();
}

void render::render_BattleScene(std::vector<Monster> m, int tag){
    clear();

    texture.loadFromFile(BATTLEBACKGROUND, ren);
    texture.render(ren, 0, 0);

    SDL_Color textColor = {0xD8, 0xC6, 0x91};

    int segment = getmaxx() / (m.size() + 1);
    for(unsigned int i = 0; i < m.size(); i++){

        if((int)i == tag)
            textColor = { 0xF4, 0xF0, 0xDD };

        texture.loadFromRenderedText(m[i].getName().c_str(), textColor, ren, font_comic16);
        texture.render(ren, (i + 1) * segment - texture.getWidth()/2, getmaxy()/2 - 208);

        char tmp[50];
        sprintf(tmp, "%d/%d", m[i].getHP(), m[i].getMaxHP());
        texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
        texture.render(ren, (i + 1) * segment - texture.getWidth()/2, getmaxy()/2 - 188);

        textColor = {0xD8, 0xC6, 0x91};

        texture.loadFromFile(m[i].getImg().c_str(), ren);
        texture.render(ren, (i + 1) * segment - texture.getWidth()/2, getmaxy()/2 - 168);
    }

    //update();
}

void render::render_BattleTeam(Team& team, unsigned int turn){
    texture.loadFromFile(BATTLEMENU, ren);
    texture.render(ren, 0, getmaxy() - 128);

    SDL_Color textColor = {0xD8, 0xC6, 0x91};

    std::vector<std::string> nameList = team.getNameList();
    for(unsigned int i = 0; i < nameList.size(); i++){

        if(i == turn)textColor = { 0xF4, 0xF0, 0xDD };{
            //Print Informations
            char tmp[100];

            texture.loadFromRenderedText(team[nameList[i]].getName().c_str(), textColor, ren, font_comic16);
            texture.render(ren, 20, getmaxy() - 128 + 15 * (i + 1));

            sprintf(tmp, "%d/%d", team[nameList[i]].getHP(), team[nameList[i]].getMaxHP());
            texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
            texture.render(ren, 150, getmaxy() - 128 + 15 * (i + 1));

            sprintf(tmp, "%d/%d", team[nameList[i]].getMP(), team[nameList[i]].getMaxMP());
            texture.loadFromRenderedText(tmp, textColor, ren, font_comic16);
            texture.render(ren, 250, getmaxy() - 128 + 15 * (i + 1));

        }textColor = {0xD8, 0xC6, 0x91};
    }

    //update();
}

void render::render_BattleMenu(unsigned int curPos){
    SDL_Color textColor = {0xD8, 0xC6, 0x91};

    if(curPos == 0)textColor = { 0xF4, 0xF0, 0xDD };{
        texture.loadFromRenderedText("Attack", textColor, ren, font_comic16);
        texture.render(ren, getmaxx() * 0.5 + 20, getmaxy() - 128 + 15);
    }textColor = {0xD8, 0xC6, 0x91};

    if(curPos == 1)textColor = { 0xF4, 0xF0, 0xDD };{
        texture.loadFromRenderedText("Skills", textColor, ren, font_comic16);
        texture.render(ren, getmaxx() * 0.5 + 120, getmaxy() - 128 + 15);
        mvaddstr(getmaxy() - 6, 42, "Skills");
    }textColor = {0xD8, 0xC6, 0x91};

    if(curPos == 2)textColor = { 0xF4, 0xF0, 0xDD };{
        texture.loadFromRenderedText("Inventory", textColor, ren, font_comic16);
        texture.render(ren, getmaxx() * 0.5 + 20, getmaxy() - 128 + 50);
    }textColor = {0xD8, 0xC6, 0x91};

    if(curPos == 3)textColor = { 0xF4, 0xF0, 0xDD };{
        texture.loadFromRenderedText("Escape", textColor, ren, font_comic16);
        texture.render(ren, getmaxx() * 0.5 + 120, getmaxy() - 128 + 50);
    }textColor = {0xD8, 0xC6, 0x91};

    update();
}

void render::render_VenderMenu(int curPos, std::vector<std::string> options){
    clear();
    texture.loadFromFile(FRAME_TITLE, ren);
    texture.render(ren, 0, 0);

    Point offset;
    offset.m_x = getmaxx() / 2;
    offset.m_y = getmaxy() / 2;

    SDL_Color textColor = {0xD8, 0xC6, 0x91};

    texture.loadFromRenderedText("Vendor", textColor, ren, font_comic32);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, 100);

    //Print All Options
    for(unsigned int i = 0; i < options.size(); i++){
        texture.loadFromRenderedText(options[i].c_str(), textColor, ren, font_comic16);
        texture.render(ren, offset.m_x - texture.getWidth() / 2, offset.m_y + (i - 1) * 40);
    }

    //Print Selected Options
    textColor = { 0xF4, 0xF0, 0xDD };
    texture.loadFromRenderedText(options[curPos].c_str(), textColor, ren, font_comic16);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, offset.m_y + (curPos - 1) * 40);

    update();
    return;
}

void render::render_StartMenu(int curPos, std::vector<std::string> options){
    clear();
    texture.loadFromFile(FRAME_TITLE, ren);
    texture.render(ren, 0, 0);

    Point offset;
    offset.m_x = getmaxx() / 2;
    offset.m_y = getmaxy() / 2;

    SDL_Color textColor = {0xD8, 0xC6, 0x91};

    texture.loadFromRenderedText("This is a Mini RPG Game", textColor, ren, font_comic50);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, 100);

    //Print All Options
    for(unsigned int i = 0; i < options.size(); i++){
        texture.loadFromRenderedText(options[i].c_str(), textColor, ren, font_comic32);
        texture.render(ren, offset.m_x - texture.getWidth() / 2, offset.m_y + (i - 1) * 40);
    }

    //Print Selected Options
    textColor = { 0xF4, 0xF0, 0xDD };
    texture.loadFromRenderedText(options[curPos].c_str(), textColor, ren, font_comic32);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, offset.m_y + (curPos - 1) * 40);

    update();
    return;
}

void render::render_gameOver(){
    clear();

    texture.loadFromFile(FRAME_TITLE, ren);
    texture.render(ren, 0, 0);

    Point offset;
    offset.m_x = getmaxx() / 2;
    offset.m_y = getmaxy() / 2;

    SDL_Color textColor = { 0xF4, 0xF0, 0xDD };
    texture.loadFromRenderedText("GameOver", textColor, ren, font_comic32);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, offset.m_y - texture.getHeight() / 2);

    usleep(2000000);
    update();
    return;
}

void render::render_HelpMenu(){
    clear();

    texture.loadFromFile(FRAME, ren);
    texture.render(ren, 0, 0);

    Point offset;
    offset.m_x = getmaxx() / 2;
    offset.m_y = getmaxy() / 2;

    SDL_Color textColor = {0xD8, 0xC6, 0x91};

    //Print Title
    texture.loadFromRenderedText("HELP", textColor, ren, font_comic50);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, 5);

    texture.loadFromRenderedText("This is a Mini RPG Game", textColor, ren, font_comic50);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, 100);

    texture.loadFromRenderedText("Key Mapping:", textColor, ren, font_comic32);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, 200);
    texture.loadFromRenderedText("Select -- z or Enter", textColor, ren, font_comic32);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, 250);
    texture.loadFromRenderedText("Cancel -- x", textColor, ren, font_comic32);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, 300);
    texture.loadFromRenderedText("Menu -- q or ESC", textColor, ren, font_comic32);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, 350);
    texture.loadFromRenderedText("Move -- Arrow Keys", textColor, ren, font_comic32);
    texture.render(ren, offset.m_x - texture.getWidth() / 2, 400);

    update();
}

void render::update(){
    SDL_RenderPresent(ren);
    return;
}

void render::clear(){
    SDL_RenderClear(ren);
    return;
}

void render::mvaddstr(int y, int x, const char* str){
    _currX = x;
    _currY = y;

    SDL_Color textColor = { 255, 255, 255  };
    texture.loadFromRenderedText(str, textColor, ren, font_comic16);

    texture.render(ren, x * 10, y * 10);
    return;
}

void render::addstr (const char*){

}

void render::mvaddch(int y, int x, char){

}

void render::mvaddch(int y, int x, wchar_t){

}

void render::mvaddwstr(int y, int x, const wchar_t*){

}
