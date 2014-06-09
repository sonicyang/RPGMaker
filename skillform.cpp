#include <QInputDialog>
#include <QMessageBox>

#include "skillform.h"
#include "ui_skillform.h"

skillForm::skillForm(std::map<std::string, Skill>& s, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::skillForm),
    skillList(s)
{
    ui->setupUi(this);
}

skillForm::~skillForm()
{
    delete ui;
}

void skillForm::update(){
    for(auto it : skillList){
        if(ui->lst_Skills->findItems(QString::fromStdString(it.first), Qt::MatchFixedString).size() == 0)
            ui->lst_Skills->addItem(QString::fromStdString(it.first));
    }
    ui->lst_Skills->setCurrentRow(0);
}

void skillForm::on_btn_Add_clicked()
{
    QString newSkillName = QInputDialog::getText(this, tr("Insert Skill"),
            tr("Input name for the new skill:"));

    if (newSkillName.isNull()){
        QMessageBox::critical(this, "Error", "You must enter a name for the new Skill!");
        return;
    }

    if(ui->lst_Skills->findItems(newSkillName, Qt::MatchExactly).size() > 0){
        QMessageBox::critical(this, "Error", "Skills should have unique names!");
        return;
    }

    //Following Order does Matter
    skillList.insert(skillList.begin(), std::pair<std::string, Skill>(newSkillName.toStdString(), Skill(newSkillName.toStdString())));
    ui->lst_Skills->addItem(newSkillName);
    ui->lst_Skills->setCurrentItem(ui->lst_Skills->findItems(newSkillName, Qt::MatchExactly).front());

    return;
}

void skillForm::on_btn_Del_clicked()
{
    skillList.erase(ui->lst_Skills->item(ui->lst_Skills->currentRow())->text().toStdString());
    ui->lst_Skills->takeItem(ui->lst_Skills->currentRow());

    return;
}

void skillForm::on_lst_Skills_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(previous != nullptr){
        auto it = skillList.find(previous->text().toStdString());

        it->second.setName(ui->txt_Name->text().toStdString());
        it->second.setDescription(ui->txt_Description->toPlainText().toStdString());
        it->second.setMPC(ui->num_MPC->value());
        it->second.seteHPv(ui->num_eHPv->value());
        it->second.seteMPv(ui->num_eMPv->value());
        it->second.seteATKv(ui->num_eATKv->value());
        it->second.seteDEFv(ui->num_eDEFv->value());
        it->second.setfHPv(ui->num_fHPv->value());
        it->second.setfMPv(ui->num_fMPv->value());
        it->second.setfATKv(ui->num_fATKv->value());
        it->second.setfDEFv(ui->num_fDEFv->value());

        if(ui->rad_eNone->isChecked())
            it->second.seteTarget(-1);
        if(ui->rad_eAll->isChecked())
            it->second.seteTarget(1);
        if(ui->rad_eSelf->isChecked())
            it->second.seteTarget(0);

        if(ui->rad_fNone->isChecked())
            it->second.setfTarget(-1);
        if(ui->rad_fAll->isChecked())
            it->second.setfTarget(1);
        if(ui->rad_fSelf->isChecked())
            it->second.setfTarget(0);
    }

    if(current == nullptr){
        ui->txt_Name->setText("");
        ui->txt_Description->setPlainText("");
        ui->num_MPC->setValue(0);
        ui->num_eHPv->setValue(0);
        ui->num_eMPv->setValue(0);
        ui->num_eATKv->setValue(0);
        ui->num_eDEFv->setValue(0);
        ui->num_fHPv->setValue(0);
        ui->num_fMPv->setValue(0);
        ui->num_fATKv->setValue(0);
        ui->num_fDEFv->setValue(0);
        return;
    }

    auto it = skillList.find(current->text().toStdString());

    if(it == skillList.end())
        return;

    ui->txt_Name->setText(QString::fromStdString(it->second.getName()));
    ui->txt_Description->setPlainText(QString::fromStdString(it->second.getDescription()));
    ui->num_MPC->setValue(it->second.getMPC());
    ui->num_eHPv->setValue(it->second.geteHPv());
    ui->num_eMPv->setValue(it->second.geteMPv());
    ui->num_eATKv->setValue(it->second.geteATKv());
    ui->num_eDEFv->setValue(it->second.geteDEFv());
    ui->num_fHPv->setValue(it->second.getfHPv());
    ui->num_fMPv->setValue(it->second.getfMPv());
    ui->num_fATKv->setValue(it->second.getfATKv());
    ui->num_fDEFv->setValue(it->second.getfDEFv());

    ui->rad_eNone->setChecked(false);
    ui->rad_eAll->setChecked(false);
    ui->rad_eSelf->setChecked(false);
    ui->rad_fAll->setChecked(false);
    ui->rad_fNone->setChecked(false);
    ui->rad_fSelf->setChecked(false);

    switch(it->second.geteTarget()){
        case -1:
            ui->rad_eNone->setChecked(true);
            break;
        case 0:
            ui->rad_eSelf->setChecked(true);
            break;
        case 1:
            ui->rad_eAll->setChecked(true);
            break;
    }

    switch(it->second.getfTarget()){
        case -1:
            ui->rad_fNone->setChecked(true);
            break;
        case 0:
            ui->rad_fSelf->setChecked(true);
            break;
        case 1:
            ui->rad_fAll->setChecked(true);
            break;
    }
    return;
}
