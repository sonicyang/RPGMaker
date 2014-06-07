#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "item.h"

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

    void on_btn_Add_clicked();

    void on_btn_Del_clicked();

    void on_lst_Items_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_txt_Description_textChanged();

    void on_btn_Save_clicked();

    void on_btn_Open_clicked();

private:
    Ui::MainWindow *ui;

    std::map<std::string, Item> itemList;
};

#endif // MAINWINDOW_H
