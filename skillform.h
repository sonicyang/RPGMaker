#ifndef SKILLFORM_H
#define SKILLFORM_H

#include <QWidget>
#include <QListWidgetItem>
#include "skill.h"

namespace Ui {
class skillForm;
}

class skillForm : public QWidget
{
    Q_OBJECT

public:
    explicit skillForm(std::map<std::string, Skill>& s, QWidget *parent = 0);
    ~skillForm();

    void updateData();

private slots:

    void on_btn_Add_clicked();

    void on_btn_Del_clicked();

    void on_lst_Skills_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::skillForm *ui;

    std::map<std::string, Skill>& skillList;
};

#endif // SKILLFORM_H
