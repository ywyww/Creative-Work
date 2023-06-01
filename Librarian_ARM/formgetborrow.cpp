#include "formgetborrow.h"
#include "ui_formgetborrow.h"

FormGetBorrow::FormGetBorrow(QWidget *parent, QSqlDatabase* db) :
    QWidget(parent),
    ui(new Ui::FormGetBorrow),
    db(db)
{
    ui->setupUi(this);

    modelComboBoxBook = new QSqlQueryModel();
    modelComboBoxBook->setQuery("SELECT name FROM book");
    ui->comboBookName->setModel(modelComboBoxBook);

    modelComboBoxUser = new QSqlQueryModel();
    modelComboBoxUser->setQuery("SELECT readersName FROM readers");
    ui->comboUserName->setModel(modelComboBoxUser);

    connect(ui->buttonPushRequest, &QPushButton::clicked,
            this, &FormGetBorrow::on_buttonPushRequest);

    connect(ui->modeRequest, &QComboBox::currentTextChanged,
            this, &FormGetBorrow::modeChanged);

    QSqlQuery query(*db);
    query.exec();
}

FormGetBorrow::~FormGetBorrow()
{
    delete ui;
    delete modelComboBoxBook;
    delete modelComboBoxUser;
}

void FormGetBorrow::setMode(int mode) {
    this->mode = mode;
}

void FormGetBorrow::acceptMode() {
    if (mode == FORMGB_ENUM::BOOK_GET) {
        ui->modeRequest->setCurrentIndex(FORMGB_ENUM::BOOK_GET);
    }
    else if (mode == FORMGB_ENUM::BOOK_BORROW) {
        ui->modeRequest->setCurrentIndex(FORMGB_ENUM::BOOK_BORROW);
    }
    else if (mode == FORMGB_ENUM::BOOK_EXTENSE) {
        ui->modeRequest->setCurrentIndex(FORMGB_ENUM::BOOK_EXTENSE);
    }
}

void FormGetBorrow::modeChanged() {
// меняется автоматом, можно не делать проверку
    setMode(ui->modeRequest->currentIndex());
    acceptMode();
}

void FormGetBorrow::on_buttonPushRequest() {
    switch(ui->modeRequest->currentIndex()) {
    case BOOK_GET:
    {
        pushRequest();
        break;
    }
    case BOOK_BORROW:
    {
        deleteRequest();
        break;
    }
    case BOOK_EXTENSE:
    {
        extenseRequest();
        break;
    }
    }//endswitch
}

void FormGetBorrow::pushRequest() {
    QString userID = getUserID(ui->comboUserName->currentText());
    QString bookID = getBookID(ui->comboBookName->currentText());

    QSqlQuery query = QSqlQuery(*db);
    int bookInUse = getBookInUse(bookID);
    int bookQuantity = getBookQuantity(bookID);

    if (bookID != -1 && userID != -1 && bookInUse < bookQuantity) {
        QString request = QString("INSERT into issuance (dateGET, dateBORROW, bookID, readersID) ");
                request += QString("VALUES (date(), date(date(), '30 days'), ");
                request += QString(QString("'") + bookID + QString("', '") + userID + QString("')"));
        if (query.exec(request)) {
            increaseBookInUse(bookID);
        }
    }
    else {
        if (bookInUse >= bookQuantity) qDebug() << "pushRequest: bookInUse >= bookQuantity";
        qDebug() << query.lastError().text();
    }
}

void FormGetBorrow::deleteRequest() {
    QString userID = getUserID(ui->comboUserName->currentText());
    QString bookID = getBookID(ui->comboBookName->currentText());

    int bookInUse = getBookInUse(bookID);
    QSqlQuery query = QSqlQuery(*db);

    if (bookID != -1 && userID != -1 && bookInUse > 0) {
        QString request = QString("DELETE FROM issuance "
                                  "WHERE  issuanceID = (SELECT issuanceID from issuance "
                                  "WHERE bookID = '%1' AND readersID = '%2' "
                                  "LIMIT 1)").arg(bookID, userID);
        if (query.exec(request)) {
            decreaseBookInUse(bookID);
        }
        else {
            qDebug() << "Ошибка deleteRequest: " << query.lastError();
        }
    }
    else {
        if (bookInUse <= 0) qDebug() << "deleteRequest: bookInUse <= 0";
        qDebug() << query.lastError().text();
    }

}

void FormGetBorrow::extenseRequest() {
    deleteRequest();
    pushRequest();
}

QString FormGetBorrow::getBookID(QString bookname) {
    QSqlQuery query = QSqlQuery(*db);
    QString request = QString("SELECT bookID from book WHERE name = '%1'").arg(bookname);
    QString bookID = "";

    query.exec(request);
    if (query.next()) {
        bookID = query.value(0).toString();
    }
    else {
        qDebug() << "Ошибка getBookID";
    }
    return bookID;
}

QString FormGetBorrow::getUserID(QString username) {
    QSqlQuery query = QSqlQuery(*db);
    QString request = QString("SELECT readersID from readers WHERE readersName = '%1'").arg(username);
    QString userID = "";

    query.exec(request);
    if (query.next()) {
        userID = query.value(0).toString();
    }
    else {
        qDebug() << "Ошибка getUserID";
    }
    return userID;
}

int FormGetBorrow::getBookInUse(QString bookID) {
    QSqlQuery query = QSqlQuery(*db);
    QString request = QString("SELECT inUse from book "
                       "where bookID = %1").arg(bookID);
    int bookInUse = -1;
    if (query.exec(request) && query.next()) {
        bookInUse = query.value(0).toInt();
    }
    else {
        qDebug() << "ошибка getBookInUse: " << query.lastError().text();
    }
    return bookInUse;
}

int FormGetBorrow::getBookQuantity(QString bookID) {
    QSqlQuery query = QSqlQuery(*db);
    QString request = QString("SELECT quantity from book "
                       "where bookID = %1").arg(bookID);
    int bookQuantity = -1;
    if (query.exec(request) && query.next()) {
        bookQuantity = query.value(0).toInt();
    }
    else {
        qDebug() << "ошибка getBookQuantity: " << query.lastError().text();
    }
    return bookQuantity;

}

void FormGetBorrow::increaseBookInUse(QString bookID) {
    QSqlQuery query = QSqlQuery(*db);
    QString request = QString("UPDATE book "
                      "SET inUse = inUse + 1 "
                      "WHERE bookID = %1").arg(bookID);

    if (!query.exec(request)) qDebug() << query.lastError().text();
}

void FormGetBorrow::decreaseBookInUse(QString bookID) {
    QSqlQuery query = QSqlQuery(*db);
    QString request = QString("UPDATE book "
                      "SET inUse = inUse - 1 "
                      "WHERE bookID = %1").arg(bookID);
    if (!query.exec(request)) qDebug() << query.lastError().text();
}
