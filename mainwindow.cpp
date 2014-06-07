#include <QInputDialog>
#include <QMessageBox>
#include <Qt>
#include <QFileDialog>

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_Add_clicked()
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

void MainWindow::on_btn_Del_clicked()
{
    itemList.erase(ui->lst_Items->item(ui->lst_Items->currentRow())->text().toStdString());
    ui->lst_Items->takeItem(ui->lst_Items->currentRow());

    return;
}

void MainWindow::on_lst_Items_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
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

void MainWindow::on_txt_Description_textChanged()
{

}

void MainWindow::on_btn_Save_clicked()
{
    ui->lst_Items->setCurrentRow(0);

    QString fileName = QFileDialog::getSaveFileName(this, tr("Open File"), "", tr("Files (*.lst)"));

    if(fileName == "")
        return;

    Json::Value root;
    root["Count"] = itemList.size();
    int i = 0;
    for(auto it : itemList){
        root["Items"][i]["Name"] = it.second.getName();
        root["Items"][i]["Description"] = it.second.getDescription();
        root["Items"][i]["Comsumable"] = it.second.isComsumable();
        root["Items"][i]["Type"] = it.second.getType();
        root["Items"][i]["HPv"] = it.second.getHPVarient();
        root["Items"][i]["MPv"] = it.second.getMPVarient();
        root["Items"][i]["ATKv"] = it.second.getATKVarient();
        root["Items"][i]["DEFv"] = it.second.getDEFVarient();
        i++;
    }

    std::ofstream file(fileName.toStdString());
    Json::StyledWriter writer;
    file << writer.write(root);
    file.close();

    return;
}


void MainWindow::on_btn_Open_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.lst)"));

    if(fileName == "")
        return;

    std::string data = get_file_contents(fileName.toStdString().c_str());

    Json::Reader read;
    Json::Value root;
    read.parse(data, root);

    for(int i = 0; i < root["Count"].asInt(); i++){
        itemList.insert(itemList.begin(), std::pair<std::string, Item>(root["Items"][i]["Name"].asString(), Item(root["Items"][i]["Name"].asString())));
        ui->lst_Items->addItem(QString::fromStdString(root["Items"][i]["Name"].asString()));


        itemList[root["Items"][i]["Name"].asString()].setDescription(root["Items"][i]["Description"].asString());
        itemList[root["Items"][i]["Name"].asString()].setComsumable(root["Items"][i]["Comsumable"].asBool());
        itemList[root["Items"][i]["Name"].asString()].setType(root["Items"][i]["Type"].asInt());
        itemList[root["Items"][i]["Name"].asString()].setHPVarient(root["Items"][i]["HPv"].asInt());
        itemList[root["Items"][i]["Name"].asString()].setMPVarient(root["Items"][i]["MPv"].asInt());
        itemList[root["Items"][i]["Name"].asString()].setATKVarient(root["Items"][i]["ATKv"].asInt());
        itemList[root["Items"][i]["Name"].asString()].setDEFVarient(root["Items"][i]["DEFv"].asInt());
    }
    ui->lst_Items->setCurrentRow(0);

    return;
}