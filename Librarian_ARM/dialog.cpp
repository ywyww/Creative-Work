#include "dialog.h"
#include "ui_dialog.h"

#include <QDebug>
#include "ENUM_SELECT.h"

Dialog::Dialog(QWidget *parent, QSqlDatabase* db, int mode) :
    QDialog(parent),
    ui(new Ui::Dialog),
    db(db),
    mode(mode)
{
    ui->setupUi(this);

    connect(ui->userInfoPushButton, &QPushButton::clicked,
            this, &Dialog::on_userInfoPushButton);


    userSearchText = "";

    model = new QSqlRelationalTableModel(this, *db);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->tableView->setModel(model);

}

Dialog::~Dialog()
{
    delete ui;
    delete model;
}

void Dialog::on_buttonBox_accepted()
{
    model->submitAll();
    //qDebug() << model->lastError().text();
}


void Dialog::on_buttonBox_rejected()
{
    model->revertAll();
}

void Dialog::on_userInfoPushButton() {
    QString text = ui->userInfoLineEdit->text();
    userSearchText = text;
    if (ui->userInfoComboBox->currentText() == "name" && text != "") {
        QString request = QString("readersName='%1'").arg(userSearchText);
        userSearchText = request;
    }
    else if (ui->userInfoComboBox->currentText() == "id" && text != "") {
        QString request = QString("readersID='%1'").arg(userSearchText);
        userSearchText = request;
    }

    this->setMode(SELECTION_ENUM::SELECT_USERINFO);
}

void Dialog::showHideElements(int flag) {
    if (!flag) {
        ui->userInfoLineEdit->hide();
        ui->userInfoPushButton->hide();
        ui->userInfoComboBox->hide();
        ui->label->hide();
    }
    else {
        ui->userInfoLineEdit->show();
        ui->userInfoPushButton->show();
        ui->userInfoComboBox->show();
        ui->label->show();
    }
}

void Dialog::setMode(int mode) {
    switch (mode) {
    case SELECTION_ENUM::SELECT_DEBTORS:
        model->setTable("issuance");
        model->setFilter("dateBORROW <= date()");
        model->setRelation(model->fieldIndex("bookID"),
                           QSqlRelation("book", "bookID", "name"));
        model->setRelation(model->fieldIndex("readersID"),
                           QSqlRelation("readers", "readersID", "readersName"));
        model->select();
        model->setHeaderData(0, Qt::Horizontal, "ID");
        model->setHeaderData(1, Qt::Horizontal, "Взял");
        model->setHeaderData(2, Qt::Horizontal, "Должен вернуть");
        model->setHeaderData(3, Qt::Horizontal, "Название книги");
        model->setHeaderData(4, Qt::Horizontal, "Читатель");

        showHideElements(0);

        break;
    case SELECTION_ENUM::SELECT_USERINFO:
        model->setTable("readers");
        if (userSearchText != "") {
            model->setFilter(userSearchText);    //установить коннект
            qDebug() << userSearchText;
        }
        else {
            model->setFilter(QString());
        }
        model->select();
        model->setHeaderData(0, Qt::Horizontal, "ID читателя");
        model->setHeaderData(1, Qt::Horizontal, "Имя");
        model->setHeaderData(2, Qt::Horizontal, "Адрес");
        model->setHeaderData(3, Qt::Horizontal, "Телефон");

        showHideElements(1);
        break;
    case SELECTION_ENUM::SELECT_BOOKINFO:
        model->setTable("book");
        model->setFilter(QString());
        model->setRelation(model->fieldIndex("authorID"),
                                             QSqlRelation("author", "authorID", "authorName"));
        model->select();
        showHideElements(0);

        model->setHeaderData(0, Qt::Horizontal, "ID книги");
        model->setHeaderData(1, Qt::Horizontal, "Название");
        model->setHeaderData(2, Qt::Horizontal, "Год");
        model->setHeaderData(3, Qt::Horizontal, "Количество");
        model->setHeaderData(4, Qt::Horizontal, "Автор");
        model->setHeaderData(5, Qt::Horizontal, "Используется");
        ui->tableView->hideColumn(0);
        break;

    default:
        qDebug() << "SELECTION ERROR";
    }
    ui->tableView->hideColumn(0);
}
