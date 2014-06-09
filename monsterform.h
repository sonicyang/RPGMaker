#ifndef MONSTERFORM_H
#define MONSTERFORM_H

#include <QWidget>
#include <QListWidgetItem>
#include "monster.h"

namespace Ui {
class monsterForm;
}

class monsterForm : public QWidget
{
    Q_OBJECT

public:
    explicit monsterForm(std::map<std::string, Monster>& m, QWidget *parent = 0);
    ~monsterForm();

    void update();

private slots:
    void on_lst_Monsters_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_btn_IMG_clicked();

    void on_btn_Add_clicked();

    void on_btn_Del_clicked();

private:
    Ui::monsterForm *ui;

    std::map<std::string, Monster>& monsterList;
};

#endif // MONSTERFORM_H
