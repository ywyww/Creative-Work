#ifndef FORMBD_H
#define FORMBD_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QtSql>
#include <ENUM_SELECT.h>

namespace Ui {
class FormBD;
}

class FormBD : public QWidget
{
    Q_OBJECT

public:
    explicit FormBD(QWidget *parent = nullptr,
                    QSqlDatabase* db = nullptr);
    ~FormBD();
    void setMode(int);
    void acceptMode();  //hide registration

private:
    bool isValidRequestData();
    void do_addRequest();
    void do_deleteRequest();
    void authorize();
    void modeChanged();


private slots:
    void on_buttonPushRequest();

private:
    Ui::FormBD *ui;
    QSqlDatabase* db;
    QString readersName,
            readersAdress,
            readersPhone;
    int mode;

};

#endif // FORMBD_H
