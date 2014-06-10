#ifndef ROLEFORM_H
#define ROLEFORM_H

#include <QListWidgetItem>
#include <QWidget>
#include "role.h"

namespace Ui {
class roleForm;
}

class roleForm : public QWidget
{
    Q_OBJECT

public:
    explicit roleForm(std::map<std::string, Role>& r, std::map<std::string, Skill>& s, QWidget *parent = 0);
    ~roleForm();

    void updateData();

private slots:
    void on_btn_Add_clicked();

    void on_btn_Del_clicked();

    void on_lst_Roles_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_btn_addSkill_clicked();

    void on_btn_rmSkill_clicked();

private:
    Ui::roleForm *ui;

    std::map<std::string, Role>& roleList;
    std::map<std::string, Skill>& skillList;
};

#endif // ROLEFORM_H
