#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "monsterform.h"
#include "ui_monsterform.h"

monsterForm::monsterForm(std::map<std::string, Monster>& m, std::map<std::string, Skill>& s, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::monsterForm),
    monsterList(m),
    skillList(s)
{
    ui->setupUi(this);
}

monsterForm::~monsterForm()
{
    delete ui;
}

void monsterForm::update(){
    for(auto it : monsterList){
        if(ui->lst_Monsters->findItems(QString::fromStdString(it.first), Qt::MatchFixedString).size() == 0)
            ui->lst_Monsters->addItem(QString::fromStdString(it.first));
    }
    ui->lst_Monsters->setCurrentRow(0);
}

void monsterForm::on_lst_Monsters_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(previous != nullptr){
        auto it = monsterList.find(previous->text().toStdString());

        it->second.setName(ui->txt_Name->text().toStdString());
        it->second.setHP(ui->num_HP->value());
        it->second.setMP(ui->num_MP->value());
        it->second.setExp(ui->num_EXP->value());
        it->second.setAttack(ui->num_ATK->value());
        it->second.setDefense(ui->num_DEF->value());
        it->second.setImg(ui->txt_IMG->text().toStdString());

    }

    if(current == nullptr){
        ui->txt_Name->setText("");
        ui->txt_IMG->setText("");
        ui->num_HP->setValue(0);
        ui->num_MP->setValue(0);
        ui->num_EXP->setValue(0);
        ui->num_ATK->setValue(0);
        ui->num_DEF->setValue(0);
        ui->lst_Mskills->clear();
        return;
    }

    auto it = monsterList.find(current->text().toStdString());

    if(it == monsterList.end())
        return;

    ui->txt_Name->setText(QString::fromStdString(it->second.getName()));
    ui->txt_IMG->setText(QString::fromStdString(it->second.getImg()));
    ui->num_EXP->setValue(it->second.getExp());
    ui->num_HP->setValue(it->second.getHP());
    ui->num_MP->setValue(it->second.getMP());
    ui->num_ATK->setValue(it->second.getAttack());
    ui->num_DEF->setValue(it->second.getDefense());

    ui->lst_Mskills->clear();
    for(auto sit : it->second.getSkillList()){
        ui->lst_Mskills->addItem(QString::fromStdString(sit.getName()));
    }

    return;
}

void monsterForm::on_btn_IMG_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open Image File"),"",tr("Image Files (*.jpg, png, bmp)"));
    ui->txt_IMG->setText(file);
}

void monsterForm::on_btn_Add_clicked()
{
    QString newMonsterName = QInputDialog::getText(this, tr("Insert Monster"),
            tr("Input name for the new monster:"));

    if (newMonsterName.isNull()){
        QMessageBox::critical(this, "Error", "You must enter a name for the new Monster!");
        return;
    }

    if(ui->lst_Monsters->findItems(newMonsterName, Qt::MatchExactly).size() > 0){
        QMessageBox::critical(this, "Error", "Monster should have unique names!");
        return;
    }

    //Following Order does Matter
    monsterList.insert(monsterList.begin(), std::pair<std::string, Monster>(newMonsterName.toStdString(), Monster(newMonsterName.toStdString())));
    ui->lst_Monsters->addItem(newMonsterName);
    ui->lst_Monsters->setCurrentItem(ui->lst_Monsters->findItems(newMonsterName, Qt::MatchExactly).front());

    return;
}

void monsterForm::on_btn_Del_clicked()
{
    monsterList.erase(ui->lst_Monsters->item(ui->lst_Monsters->currentRow())->text().toStdString());
    ui->lst_Monsters->takeItem(ui->lst_Monsters->currentRow());

    return;
}



void monsterForm::on_btn_addSkill_clicked()
{
    QString sName = QInputDialog::getText(this, tr("Insert Skill"),
            tr("Input name of the Skill:"));

    if (sName.isNull()){
        QMessageBox::critical(this, "Error", "You must enter a name of Skill!");
        return;
    }

    if(skillList.find(sName.toStdString()) == skillList.end()){
        QMessageBox::critical(this, "Error", "No Such Skill");
        return;
    }

    monsterList[ui->lst_Monsters->currentItem()->text().toStdString()].getSkillList().emplace_back(skillList[sName.toStdString()]);

    ui->lst_Mskills->addItem(sName);

    return;
}

void monsterForm::on_btn_rmSkill_clicked()
{
    monsterList[ui->lst_Monsters->currentItem()->text().toStdString()].getSkillList().erase(
                monsterList[ui->lst_Monsters->currentItem()->text().toStdString()].getSkillList().begin() +
                ui->lst_Mskills->currentRow());
    ui->lst_Mskills->takeItem(ui->lst_Mskills->currentRow());

    return;
}
