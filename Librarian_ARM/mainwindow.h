#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <dialog.h>
#include <formbd.h>
#include <formgetborrow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void aboutInfo();
    void on_buttonUserDebtors();
    void on_buttonUserInfo();
    void on_buttonUserAdd();
    void on_buttonUserDelete();

    void on_buttonBookCondition();
    void on_buttonBookGet();
    void on_buttonBookBorrow();
    void on_buttonBookExtense();
    void on_buttonBookOrder();

private:
    Ui::MainWindow *ui;
    //windows:
    Dialog* dialog;
    FormBD* formBD;
    FormGetBorrow* formGetBorrow;
    //endwindows
    QSqlDatabase db;

};
#endif // MAINWINDOW_H
