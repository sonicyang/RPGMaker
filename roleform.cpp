#include <QInputDialog>
#include <QMessageBox>
#include "roleform.h"
#include "ui_roleform.h"

roleForm::roleForm(std::map<std::string, Role>& r, std::map<std::string, Skill>& s, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::roleForm),
    roleList(r),
    skillList(s)
{
    ui->setupUi(this);
}

roleForm::~roleForm()
{
    delete ui;
}

void roleForm::updateData(){
    for(auto it : roleList){
        if(ui->lst_Roles->findItems(QString::fromStdString(it.first), Qt::MatchFixedString).size() == 0)
            ui->lst_Roles->addItem(QString::fromStdString(it.first));
    }
    ui->lst_Roles->setCurrentRow(0);
}

void roleForm::on_btn_Add_clicked()
{
    QString newRoleName = QInputDialog::getText(this, tr("Insert Role"),
            tr("Input name for the new Role:"));

    if (newRoleName.isNull()){
        QMessageBox::critical(this, "Error", "You must enter a name for the new Role!");
        return;
    }

    if(ui->lst_Roles->findItems(newRoleName, Qt::MatchExactly).size() > 0){
        QMessageBox::critical(this, "Error", "Roles should have unique names!");
        return;
    }

    //Following Order does Matter
    roleList.insert(roleList.begin(), std::pair<std::string, Role>(newRoleName.toStdString(), Role(newRoleName.toStdString())));
    ui->lst_Roles->addItem(newRoleName);
    ui->lst_Roles->setCurrentItem(ui->lst_Roles->findItems(newRoleName, Qt::MatchExactly).front());

    return;
}

void roleForm::on_btn_Del_clicked()
{
    roleList.erase(ui->lst_Roles->item(ui->lst_Roles->currentRow())->text().toStdString());
    ui->lst_Roles->takeItem(ui->lst_Roles->currentRow());
}

void roleForm::on_lst_Roles_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(previous != nullptr){
        auto it = roleList.find(previous->text().toStdString());

        it->second.setName(ui->txt_Name->text().toStdString());
        it->second.setHPc(ui->num_HPc->value());
        it->second.setMPc(ui->num_MPc->value());
        it->second.setATKc(ui->num_ATKc->value());
        it->second.setDEFc(ui->num_DEFc->value());
        it->second.setHPx(ui->num_HPx->value());
        it->second.setMPx(ui->num_MPx->value());
        it->second.setATKx(ui->num_ATKx->value());
        it->second.setDEFx(ui->num_DEFx->value());

    }

    if(current == nullptr){
        ui->txt_Name->setText("");
        ui->num_HPx->setValue(0);
        ui->num_MPx->setValue(0);
        ui->num_ATKx->setValue(0);
        ui->num_DEFx->setValue(0);
        ui->num_HPc->setValue(0);
        ui->num_MPc->setValue(0);
        ui->num_ATKc->setValue(0);
        ui->num_DEFc->setValue(0);
        ui->lst_Mskills->clear();
        return;
    }

    auto it = roleList.find(current->text().toStdString());

    if(it == roleList.end())
        return;

    ui->txt_Name->setText(QString::fromStdString(it->second.getName()));
    ui->num_HPc->setValue(it->second.getHPc());
    ui->num_MPc->setValue(it->second.getMPc());
    ui->num_ATKc->setValue(it->second.getATKc());
    ui->num_DEFc->setValue(it->second.getDEFc());
    ui->num_HPx->setValue(it->second.getHPx());
    ui->num_MPx->setValue(it->second.getMPx());
    ui->num_ATKx->setValue(it->second.getATKx());
    ui->num_DEFx->setValue(it->second.getDEFx());

    ui->lst_Mskills->clear();
    for(auto sit : it->second.getSkillList()){
        ui->lst_Mskills->addItem(QString::number(sit.first) + "|" + QString::fromStdString(sit.second.getName()));
    }

    return;
}

void roleForm::on_btn_addSkill_clicked()
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

    QString sLevel = QInputDialog::getText(this, tr("Which Level?"),
            tr("Input Level:"));

    if (sLevel.isNull()){
        QMessageBox::critical(this, "Error", "You must enter a Level of Skill!");
        return;
    }

    bool stat;
    int ssLevel = sLevel.toInt(&stat);

    if(!stat){
        QMessageBox::critical(this, "Error", "Please Enter a Number for Level");
        return;
    }

    if(roleList[ui->lst_Roles->currentItem()->text().toStdString()].getSkillList().find(ssLevel) != roleList[ui->lst_Roles->currentItem()->text().toStdString()].getSkillList().end()){
        QMessageBox::critical(this, "Error", "One Skill, One Level");
        return;
    }

    roleList[ui->lst_Roles->currentItem()->text().toStdString()].getSkillList().emplace(ssLevel, skillList[sName.toStdString()]);

    ui->lst_Mskills->addItem(sLevel + "|" + sName);

    return;
}

void roleForm::on_btn_rmSkill_clicked()
{
    roleList[ui->lst_Roles->currentItem()->text().toStdString()].getSkillList().erase(ui->lst_Mskills->currentItem()->text().split('|')[0].toInt());
    ui->lst_Mskills->takeItem(ui->lst_Mskills->currentRow());
}
