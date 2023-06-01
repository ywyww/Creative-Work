#ifndef FORMGETBORROW_H
#define FORMGETBORROW_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QtSql>
#include <ENUM_SELECT.h>

namespace Ui {
class FormGetBorrow;
}

class FormGetBorrow : public QWidget
{
    Q_OBJECT

public:
    explicit FormGetBorrow(QWidget *parent = nullptr, QSqlDatabase* db = nullptr);
    ~FormGetBorrow();
    void setMode(int);
    void acceptMode();

private:
    QString getUserID(QString username);
    QString getBookID(QString bookname);

    int getBookInUse(QString bookID);
    int getBookQuantity(QString bookID);
    void increaseBookInUse(QString bookID);
    void decreaseBookInUse(QString bookID);

    void pushRequest();
    void deleteRequest();
    void extenseRequest();

private slots:
     void on_buttonPushRequest();
     void modeChanged();

private:
    Ui::FormGetBorrow *ui;
    QSqlDatabase* db;
    int mode;
    QSqlQueryModel* modelComboBoxUser;
    QSqlQueryModel* modelComboBoxBook;

};

#endif // FORMGETBORROW_H
