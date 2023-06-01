#include "formbd.h"
#include "ui_formbd.h"

FormBD::FormBD(QWidget *parent, QSqlDatabase *db) :
    QWidget(parent),
    ui(new Ui::FormBD),
    db(db)
{
    ui->setupUi(this);
    connect(ui->buttonPushRequest, &QPushButton::clicked,
            this, &FormBD::on_buttonPushRequest);
    connect(ui->buttonLogIn, &QPushButton::clicked,
            this, &FormBD::authorize);
    connect(ui->mode, &QComboBox::currentTextChanged,
            this, &FormBD::modeChanged);

    readersName = readersAdress = readersPhone = "";
    mode = USER_ADD;
}

FormBD::~FormBD()
{
    delete ui;
}

bool FormBD::isValidRequestData() {
    readersName = ui->line_name->text();
    readersAdress = ui->line_adress->text();
    readersPhone = ui->line_phone->text();

    return (readersName != "") && (readersAdress != "") && (readersPhone != "");
}

void FormBD::on_buttonPushRequest() {
    if (isValidRequestData() && ui->mode->currentText() == "add") {
        do_addRequest();
    }
    else if ( ui->mode->currentText() == "delete") {
        do_deleteRequest();
    }
}

void FormBD::do_addRequest() {
    QString request;
    request = QString("INSERT INTO readers (readersName, readersAdress, readersPhone) "
                      "VALUES ('%1', '%2', '%3')").arg(readersName, readersAdress, readersPhone);

    //qDebug() << request;
    QSqlQuery query(*db);
    if (isValidRequestData() && !query.exec(request)) {
        qDebug() << query.lastError();
    }
}

void FormBD::do_deleteRequest() {
    QString request;
    request = QString("DELETE FROM readers WHERE readersName = '%1' AND readersAdress = '%2' AND readersPhone = '%3'").arg(readersName, readersAdress, readersPhone);

    //qDebug() << request;
    QSqlQuery query(*db);
    if (isValidRequestData() && !query.exec(request)) {
        qDebug() << query.lastError();
    }
}

void FormBD::setMode(int mode) {
    this->mode = mode;
}

void FormBD::acceptMode() {
    if (mode == USER_ADD) {
        ui->authorizationGroup->hide();
        ui->mode->setCurrentIndex(USER_ADD);
        ui->buttonPushRequest->show();
    }
    else if (mode == USER_DELETE) {
        ui->authorizationGroup->show();
        ui->mode->setCurrentIndex(USER_DELETE);
        ui->buttonPushRequest->hide();
    }
}

void FormBD::authorize() {
    QString login = ui->line_login->text();
    QString password = ui->line_password->text();

    if (login == "admin" && password == "admin") {
        ui->buttonPushRequest->show();
    }
}

void FormBD::modeChanged() {
    if (ui->mode->currentIndex() == USER_ADD) {
        setMode(USER_ADD);
    }
    else if (ui->mode->currentIndex() == USER_DELETE) {
        setMode(USER_DELETE);
    }
    acceptMode();
}
