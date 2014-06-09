#include "itemform.h"
#include "ui_itemform.h"

#include <QInputDialog>
#include <QMessageBox>

itemForm::itemForm(std::map<std::string, Item>& i, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::itemForm),
    itemList(i)
{
    ui->setupUi(this);
    update();
}

itemForm::~itemForm()
{
    delete ui;
}

void itemForm::update(){
    for(auto it : itemList){
        if(ui->lst_Items->findItems(QString::fromStdString(it.first), Qt::MatchFixedString).size() == 0)
            ui->lst_Items->addItem(QString::fromStdString(it.first));
    }
    ui->lst_Items->setCurrentRow(0);
}

void itemForm::on_btn_Add_clicked()
{
    QString newItemName = QInputDialog::getText(this, tr("Insert Item"),
            tr("Input name for the new item:"));

    if (newItemName.isNull()){
        QMessageBox::critical(this, "Error", "You must enter a name for the new Item!");
        return;
    }

    if(ui->lst_Items->findItems(newItemName, Qt::MatchExactly).size() > 0){
        QMessageBox::critical(this, "Error", "Items should have unique names!");
        return;
    }

    //Following Order does Matter
    itemList.insert(itemList.begin(), std::pair<std::string, Item>(newItemName.toStdString(), Item(newItemName.toStdString())));
    ui->lst_Items->addItem(newItemName);
    ui->lst_Items->setCurrentItem(ui->lst_Items->findItems(newItemName, Qt::MatchExactly).front());

    return;
}

void itemForm::on_btn_Del_clicked()
{
    itemList.erase(ui->lst_Items->item(ui->lst_Items->currentRow())->text().toStdString());
    ui->lst_Items->takeItem(ui->lst_Items->currentRow());

    return;
}

void itemForm::on_lst_Items_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(previous != nullptr){
        auto it = itemList.find(previous->text().toStdString());

        it->second.setName(ui->txt_Name->text().toStdString());
        it->second.setDescription(ui->txt_Description->toPlainText().toStdString());
        it->second.setComsumable(ui->chk_Comsumable->isChecked());
        it->second.setHPVarient(ui->num_HPv->value());
        it->second.setMPVarient(ui->num_MPv->value());
        it->second.setATKVarient(ui->num_ATKv->value());
        it->second.setDEFVarient(ui->num_DEFv->value());

        if(ui->rad_null->isChecked())
            it->second.setType(-1);
        if(ui->rad_Potion->isChecked())
            it->second.setType(0);
        if(ui->rad_TPotion->isChecked())
            it->second.setType(1);
        if(ui->rad_Head->isChecked())
            it->second.setType(2);
        if(ui->rad_Armor->isChecked())
            it->second.setType(3);
        if(ui->Rad_Legs->isChecked())
            it->second.setType(4);
        if(ui->rad_Shoes->isChecked())
            it->second.setType(5);
        if(ui->rad_Weapon->isChecked())
            it->second.setType(6);
    }

    if(current == nullptr){
        ui->txt_Name->setText("");
        ui->txt_Description->setPlainText("");
        ui->chk_Comsumable->setChecked(false);
        ui->num_HPv->setValue(0);
        ui->num_MPv->setValue(0);
        ui->num_ATKv->setValue(0);
        ui->num_DEFv->setValue(0);
        return;
    }

    auto it = itemList.find(current->text().toStdString());

    if(it == itemList.end())
        return;

    ui->txt_Name->setText(QString::fromStdString(it->second.getName()));
    ui->txt_Description->setPlainText(QString::fromStdString(it->second.getDescription()));
    ui->chk_Comsumable->setChecked(it->second.isComsumable());
    ui->num_HPv->setValue(it->second.getHPVarient());
    ui->num_MPv->setValue(it->second.getMPVarient());
    ui->num_ATKv->setValue(it->second.getATKVarient());
    ui->num_DEFv->setValue(it->second.getDEFVarient());

    ui->rad_null->setChecked(false);
    ui->rad_Potion->setChecked(false);
    ui->rad_TPotion->setChecked(false);
    ui->rad_Head->setChecked(false);
    ui->rad_Armor->setChecked(false);
    ui->Rad_Legs->setChecked(false);
    ui->rad_Shoes->setChecked(false);
    ui->rad_Weapon->setChecked(false);

    switch(it->second.getType()){
        case -1:
            ui->rad_null->setChecked(true);
            break;
        case 0:
            ui->rad_Potion->setChecked(true);
            break;
        case 1:
            ui->rad_TPotion->setChecked(true);
            break;
        case 2:
            ui->rad_Head->setChecked(true);
            break;
        case 3:
            ui->rad_Armor->setChecked(true);
            break;
        case 4:
            ui->Rad_Legs->setChecked(true);
            break;
        case 5:
            ui->rad_Shoes->setChecked(true);
            break;
        case 6:
            ui->rad_Weapon->setChecked(true);
            break;
    }
    return;
}
