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
    explicit monsterForm(std::map<std::string, Monster>& m, std::map<std::string, Skill>& s, QWidget *parent = 0);
    ~monsterForm();

    void updateData();

private slots:
    void on_lst_Monsters_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_btn_IMG_clicked();

    void on_btn_Add_clicked();

    void on_btn_Del_clicked();

    void on_btn_addSkill_clicked();

    void on_btn_rmSkill_clicked();

private:
    Ui::monsterForm *ui;

    std::map<std::string, Monster>& monsterList;
    std::map<std::string, Skill>& skillList;
};

#endif // MONSTERFORM_H
