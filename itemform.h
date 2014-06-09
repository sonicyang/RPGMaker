#ifndef ITEMFORM_H
#define ITEMFORM_H

#include <map>
#include <string>
#include <QWidget>
#include <QListWidgetItem>
#include "item.h"

namespace Ui {
class itemForm;
}

class itemForm : public QWidget
{
    Q_OBJECT

public:
    explicit itemForm(std::map<std::string, Item>& i, QWidget *parent = 0);
    ~itemForm();

    void update();

private slots:
    void on_btn_Add_clicked();

    void on_btn_Del_clicked();

    void on_lst_Items_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::itemForm *ui;

    std::map<std::string, Item>& itemList;
};

#endif // ITEMFORM_H
