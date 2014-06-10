#include <QInputDialog>
#include <QMessageBox>
#include <Qt>
#include <QFileDialog>
#include <QMdiSubWindow>

#include <fstream>

#include "utils.h"
#include "json/reader.h"
#include "json/writer.h"
#include "json/value.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    dataPath = QFileDialog::getExistingDirectory(this, tr("Open Game"),
                                                 "./",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if(dataPath == ""){
        QMessageBox::critical(this, "Error", "You Must Open a Game manifest file!");
        throw(122);
    }

    QString fileName = dataPath + "/data/items/itemlist.lst";

    Json::Reader read;
    Json::Value root;
    read.parse(get_file_contents(fileName.toStdString().c_str()), root);

    for(int i = 0; i < root["Count"].asInt(); i++){
        Json::Value root2;
        read.parse(get_file_contents((dataPath.toStdString() + "/" + root["Items"][i].asString()).c_str()), root2);
        itemList.insert(itemList.begin(), std::pair<std::string, Item>(root2["Name"].asString(), Item(root2["Name"].asString())));
        itemList[root2["Name"].asString()].setDescription(root2["Description"].asString());
        itemList[root2["Name"].asString()].setComsumable(root2["Comsumable"].asBool());
        itemList[root2["Name"].asString()].setType(root2["Type"].asInt());
        itemList[root2["Name"].asString()].setHPVarient(root2["HPv"].asInt());
        itemList[root2["Name"].asString()].setMPVarient(root2["MPv"].asInt());
        itemList[root2["Name"].asString()].setATKVarient(root2["ATKv"].asInt());
        itemList[root2["Name"].asString()].setDEFVarient(root2["DEFv"].asInt());
    }

    fileName = dataPath + "/data/skills/skilllist.lst";
    root.clear();
    read.parse(get_file_contents(fileName.toStdString().c_str()), root);

    for(int i = 0; i < root["Count"].asInt(); i++){
        Json::Value root2;
        read.parse(get_file_contents((dataPath.toStdString() + "/" + root["Skills"][i].asString()).c_str()), root2);
        skillList.insert(skillList.begin(), std::pair<std::string, Skill>(root2["Name"].asString(), Skill(root2["Name"].asString())));
        skillList[root2["Name"].asString()].setDescription(root2["Description"].asString());
        skillList[root2["Name"].asString()].setMPC(root2["Comsumable"].asInt());
        skillList[root2["Name"].asString()].seteTarget(root2["eTarget"].asInt());
        skillList[root2["Name"].asString()].seteHPv(root2["eHPv"].asInt());
        skillList[root2["Name"].asString()].seteMPv(root2["eMPv"].asInt());
        skillList[root2["Name"].asString()].seteATKv(root2["eATKv"].asInt());
        skillList[root2["Name"].asString()].seteDEFv(root2["eDEFv"].asInt());
        skillList[root2["Name"].asString()].setfTarget(root2["fTarget"].asInt());
        skillList[root2["Name"].asString()].setfHPv(root2["fHPv"].asInt());
        skillList[root2["Name"].asString()].setfMPv(root2["fMPv"].asInt());
        skillList[root2["Name"].asString()].setfATKv(root2["fATKv"].asInt());
        skillList[root2["Name"].asString()].setfDEFv(root2["fDEFv"].asInt());
    }

    fileName = dataPath + "/data/monsters/monsterlist.lst";
    root.clear();
    read.parse(get_file_contents(fileName.toStdString().c_str()), root);

    for(int i = 0; i < root["Count"].asInt(); i++){
        Json::Value root2;
        read.parse(get_file_contents((dataPath.toStdString() + "/" + root["Path"][i].asString()).c_str()), root2);
        monsterList.insert(monsterList.begin(), std::pair<std::string, Monster>(root2["Name"].asString(), Monster(root2["Name"].asString())));
        monsterList[root2["Name"].asString()].setExp(root2["EXP"].asInt());
        monsterList[root2["Name"].asString()].setHP(root2["HP"].asInt());
        monsterList[root2["Name"].asString()].setMP(root2["MP"].asInt());
        monsterList[root2["Name"].asString()].setAttack(root2["ATK"].asInt());
        monsterList[root2["Name"].asString()].setDefense(root2["DEF"].asInt());
        monsterList[root2["Name"].asString()].setImg(root2["Image"].asString());
        for(int k = 0; k < root2["Skills"].size(); k++){
            monsterList[root2["Name"].asString()].getSkillList().emplace_back(skillList[root2["Skills"][k].asString()]);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = dataPath + "/data/items/itemlist.lst";

    Json::StyledWriter writer;
    Json::Value root;
    root["Count"] = itemList.size();
    int i = 0;
    for(auto it : itemList){
        root["Items"][i] = "data/items/" + it.second.getName() + ".item";

        Json::Value root2;
        root2["Name"] = it.second.getName();
        root2["Description"] = it.second.getDescription();
        root2["Comsumable"] = (int)it.second.isComsumable();
        root2["Type"] = it.second.getType();
        root2["HPv"] = it.second.getHPVarient();
        root2["MPv"] = it.second.getMPVarient();
        root2["ATKv"] = it.second.getATKVarient();
        root2["DEFv"] = it.second.getDEFVarient();

        std::ofstream file(dataPath.toStdString() + "/data/items/" + it.second.getName() + ".item");
        file << writer.write(root2);
        file.close();

        i++;
    }

    std::ofstream file(fileName.toStdString());
    file << writer.write(root);
    file.close();

    fileName = dataPath + "/data/skills/skilllist.lst";

    root.clear();
    root["Count"] = skillList.size();
    i = 0;
    for(auto it : skillList){
        root["Skills"][i] = "data/skills/" + it.second.getName() + ".skl";

        Json::Value root2;
        root2["Name"] = it.second.getName();
        root2["Description"] = it.second.getDescription();
        root2["MPC"] = it.second.getMPC();
        root2["eTarget"] = it.second.geteTarget();
        root2["eHPv"] = it.second.geteHPv();
        root2["eMPv"] = it.second.geteMPv();
        root2["eATKv"] = it.second.geteATKv();
        root2["eDEFv"] = it.second.geteDEFv();
        root2["fTarget"] = it.second.getfTarget();
        root2["fHPv"] = it.second.getfHPv();
        root2["fMPv"] = it.second.getfMPv();
        root2["fATKv"] = it.second.getfATKv();
        root2["fDEFv"] = it.second.getfDEFv();

        std::ofstream file(dataPath.toStdString() + "/data/skills/" + it.second.getName() + ".skl");
        file << writer.write(root2);
        file.close();

        i++;
    }

    std::ofstream file2(fileName.toStdString());
    file2 << writer.write(root);
    file2.close();

    fileName = dataPath + "/data/monsters/monsterlist.lst";

    root.clear();
    root["Count"] = monsterList.size();
    i = 0;
    for(auto it : monsterList){
        root["Path"][i] = "data/monsters/" + it.second.getName() + ".mst";

        Json::Value root2;
        root2["Name"] = it.second.getName();
        root2["EXP"] = it.second.getExp();
        root2["HP"] = it.second.getHP();
        root2["MP"] = it.second.getMP();
        root2["ATK"] = it.second.getAttack();
        root2["DEF"] = it.second.getDefense();
        root2["Image"] = it.second.getImg();

        int k = 0;
        for(auto sit : it.second.getSkillList()){
            root2["Skills"][k] = sit.getName();
            k++;
        }

        std::ofstream file(dataPath.toStdString() + "/data/monsters/" + it.second.getName() + ".mst");
        file << writer.write(root2);
        file.close();

        i++;
    }

    std::ofstream file3(fileName.toStdString());
    file3 << writer.write(root);
    file3.close();

    return;
}

void MainWindow::on_actionItem_triggered()
{
    ui->mdiArea->removeSubWindow(iF);
    delete iF;

    iF = new itemForm(itemList);
    QMdiSubWindow* sub = ui->mdiArea->addSubWindow(iF);
    sub->show();
    iF->update();
}

void MainWindow::on_actionSkills_triggered()
{
    ui->mdiArea->removeSubWindow(sF);
    delete sF;

    sF = new skillForm(skillList);
    QMdiSubWindow* sub = ui->mdiArea->addSubWindow(sF);
    sub->show();
    sF->update();
}

void MainWindow::on_actionMonster_triggered()
{
    ui->mdiArea->removeSubWindow(mF);
    delete mF;

    mF = new monsterForm(monsterList, skillList);
    QMdiSubWindow* sub = ui->mdiArea->addSubWindow(mF);
    sub->show();
    mF->update();
}
