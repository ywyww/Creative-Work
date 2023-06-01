#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSql>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QDebug>
#include <QSqlError>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr, QSqlDatabase* db = nullptr, int mode=0);
    ~Dialog();

    void setMode(int mode);

private:
    void showHideElements(int);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_userInfoPushButton();

private:
    Ui::Dialog *ui;
    QSqlDatabase* db;
    QSqlRelationalTableModel* model;

    int mode;
    QString userSearchText;
};

#endif // DIALOG_H
