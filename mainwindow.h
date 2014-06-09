#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "item.h"
#include "itemform.h"
#include "skill.h"
#include "skillform.h"
#include "monster.h"
#include "monsterform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_actionSave_triggered();

    void on_actionItem_triggered();

    void on_actionSkills_triggered();

    void on_actionMonster_triggered();

private:
    Ui::MainWindow *ui;

    QString dataPath;

    std::map<std::string, Item> itemList;
    std::map<std::string, Skill> skillList;
    std::map<std::string, Monster> monsterList;

    itemForm* iF = nullptr;
    skillForm* sF = nullptr;
    monsterForm* mF = nullptr;
};

#endif // MAINWINDOW_H
