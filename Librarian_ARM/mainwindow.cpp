#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "ENUM_SELECT.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->menubar_about_info, &QAction::triggered,
            this, &MainWindow::aboutInfo);

    connect(ui->menubar_user_debtors, &QAction::triggered,
            this, &MainWindow::on_buttonUserDebtors);

    connect (ui->menubar_user_add, &QAction::triggered,
             this, &MainWindow::on_buttonUserAdd);

    connect (ui->menubar_user_delete, &QAction::triggered,
             this, &MainWindow::on_buttonUserDelete);

    connect (ui->menubar_user_information, &QAction::triggered,
             this, &MainWindow::on_buttonUserInfo);

    connect(ui->menubar_book_information, &QAction::triggered,
            this, &MainWindow::on_buttonBookCondition);

    connect(ui->menubar_book_get, &QAction::triggered,
            this, &MainWindow::on_buttonBookGet);

    connect(ui->menubar_book_borrow, &QAction::triggered,
            this, &MainWindow::on_buttonBookBorrow);

    connect(ui->menubar_book_extense, &QAction::triggered,
            this, &MainWindow::on_buttonBookExtense);



    connect(ui->mainPage_buttonUserDebtors, &QPushButton::clicked,
            this, &MainWindow::on_buttonUserDebtors);

    connect(ui->mainPage_buttonUserInfo, &QPushButton::clicked,
            this, &MainWindow::on_buttonUserInfo);

    connect(ui->mainPage_buttonUserAdd, &QPushButton::clicked,
            this, &MainWindow::on_buttonUserAdd);

    connect(ui->mainPage_buttonUserDelete, &QPushButton::clicked,
            this, &MainWindow::on_buttonUserDelete);

    connect(ui->bookPage_buttonBookCondition, &QPushButton::clicked,
             this, &MainWindow::on_buttonBookCondition);

    connect(ui->bookPage_buttonBookGet, &QPushButton::clicked,
            this, &MainWindow::on_buttonBookGet);

    connect(ui->bookPage_buttonBookBorrow, &QPushButton::clicked,
            this, &MainWindow::on_buttonBookBorrow);

    connect(ui->bookPage_buttonBookExtension, &QPushButton::clicked,
            this, &MainWindow::on_buttonBookExtense);

    connect(ui->bookPage_buttonBookOrder, &QPushButton::clicked,
            this, &MainWindow::on_buttonBookOrder);


    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Users\\tokma\\Documents\\SQL\\sqlite\\mydatabase.db");
    bool flag = db.open();
    if (!flag) {
        qDebug() << db.lastError().text();
    }
    else {
        qDebug() << "Success";
    }

    formGetBorrow = nullptr;
    dialog = nullptr;
    formBD = nullptr;

}

MainWindow::~MainWindow()
{
    delete ui;
    if (dialog != nullptr) delete dialog;
    if (formBD != nullptr) delete formBD;
    if (formGetBorrow != nullptr) delete formGetBorrow;
    db.close();
    QSqlDatabase::removeDatabase(QString());
}

void MainWindow::aboutInfo() {
    QMessageBox::about(this, tr("О программе"),
                       tr("<h1>Librarian</h1>"
                          "<p>Librarian - программа, гарантирующая"
                          " автоматизацию рабочего места библиотекаря"
                          "</p>"
                          "<p>Изготовитель: Токмаков Герман</p>"
                          "<p>Группа: ИВТ-22-2б</p>"
                          "<p>Пермский Национальный Исследовательский Политехнический Университет</p>"
                          "<p>2023</p>"
                          ));
}

void MainWindow::on_buttonUserDebtors() {
    if (dialog != nullptr) delete dialog;
    dialog = new Dialog(nullptr, &db, SELECTION_ENUM::SELECT_DEBTORS);
    dialog->setMode(SELECTION_ENUM::SELECT_DEBTORS);
    dialog->show();
}

void MainWindow::on_buttonUserInfo() {
    if (dialog != nullptr) delete dialog;
    dialog = new Dialog(nullptr, &db, SELECTION_ENUM::SELECT_DEBTORS);
    dialog->setMode(SELECTION_ENUM::SELECT_USERINFO);
    dialog->show();
}

void MainWindow::on_buttonBookCondition() {
    if (dialog != nullptr) delete dialog;
    dialog = new Dialog(nullptr, &db, SELECTION_ENUM::SELECT_DEBTORS);
    dialog->setMode(SELECTION_ENUM::SELECT_BOOKINFO);
    dialog->show();
}

void MainWindow::on_buttonUserAdd() {
    if (formBD) delete formBD;
    formBD = new FormBD(nullptr, &db);
    formBD->setMode(USER_ADD);
    formBD->acceptMode();
    formBD->show();
}

void MainWindow::on_buttonUserDelete() {
    if (formBD) delete formBD;
    formBD = new FormBD(nullptr, &db);
    formBD->setMode(USER_DELETE);
    formBD->acceptMode();
    formBD->show();
}

void MainWindow::on_buttonBookGet() {
    if (formGetBorrow != nullptr) delete formGetBorrow;
    formGetBorrow = new FormGetBorrow(nullptr, &db);
    formGetBorrow->setMode(FORMGB_ENUM::BOOK_GET);
    formGetBorrow->acceptMode();
    formGetBorrow->show();
}

void MainWindow::on_buttonBookBorrow() {
    if (formGetBorrow != nullptr) delete formGetBorrow;
    formGetBorrow = new FormGetBorrow(nullptr, &db);
    formGetBorrow->setMode(FORMGB_ENUM::BOOK_BORROW);
    formGetBorrow->acceptMode();
    formGetBorrow->show();
}

void MainWindow::on_buttonBookExtense() {
    if (formGetBorrow != nullptr) delete formGetBorrow;
    formGetBorrow = new FormGetBorrow(nullptr, &db);
    formGetBorrow->setMode(FORMGB_ENUM::BOOK_EXTENSE);
    formGetBorrow->acceptMode();
    formGetBorrow->show();
}

void MainWindow::on_buttonBookOrder() {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("Обратитесь к системному администратору");
    msgBox.exec();
}
