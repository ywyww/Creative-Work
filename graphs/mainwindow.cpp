#define _USE_MATH_DEFINES
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(ui->pushButton_addVertex, &QPushButton::clicked,
            this, &MainWindow::on_pushButtonAddVertex);

    connect(ui->pushButton_deleteVertex, &QPushButton::clicked,
            this, &MainWindow::on_pushButtonDeleteVertex);

    connect(ui->tempButton, &QPushButton::clicked,
            this, &MainWindow::on_tempButtonClicked);

    connect(ui->pushButton_TSP, &QPushButton::clicked,
            this, &MainWindow::on_TSPButtonClicked);

    solution = "";

}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    paint.begin(this);
    int numberOfNodes = ui->matrix->rowCount();
    paintEllipse(paint, numberOfNodes);
    bindEllipseWithDirection(paint, numberOfNodes);
    paint.end();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addVertex() {
    ui->matrix->setColumnCount(ui->matrix->columnCount() + 1);
    ui->matrix->setRowCount(ui->matrix->rowCount() + 1);
    isVisitedVertecies.resize(isVisitedVertecies.size() + 1);
    isVisitedVertecies = QVector<bool>(isVisitedVertecies.size(), 0);
}

void MainWindow::deleteVertex() {
    ui->matrix->setColumnCount(ui->matrix->columnCount() - 1);
    ui->matrix->setRowCount(ui->matrix->rowCount() - 1);
    isVisitedVertecies.resize(isVisitedVertecies.size() - 1);
    isVisitedVertecies = QVector<bool>(isVisitedVertecies.size(), 0);
}

void MainWindow::on_pushButtonAddVertex() {
    addVertex();
    ui->matrix->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->matrix->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::on_pushButtonDeleteVertex() {
    deleteVertex();
    ui->matrix->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->matrix->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

QVector<int> MainWindow::getNeighbours(int vertex) {
    QVector<int> rVect;
    for (int i = 0; i < ui->matrix->rowCount(); i++) {
        if (ui->matrix->item(i, vertex) != nullptr && !ui->matrix->item(i, vertex)->text().isEmpty()) {        // здесь возникает ошибка
            rVect.append(i);
        }
    }
    return rVect;
}

void MainWindow::DFS(int vertex) {
    QVector<int> neighbours = getNeighbours(vertex);
    if (neighbours.size() == 0) {
        qDebug() << "Нечего посещать";
        return;
    }
    qDebug() << "Вершина " << vertex + 1<< " посещена";// << neighbours.size() << " соседей";


    isVisitedVertecies[vertex] = 1;
    for (int i = 0; i < neighbours.size(); i++) {
        if (isVisitedVertecies[neighbours[i]] == 0) {
            DFS(neighbours[i]);
        }
    }
}

void MainWindow::on_tempButtonClicked() {
    update();
}

void MainWindow::paintEllipse(QPainter &paint, int numberOfVerticies) {

    paint.setPen(QColor(0, 0, 0));
    paint.setBrush(Qt::Dense7Pattern);

    int shiftX = 700,
        shiftY = 300;
    int radiusCircle = 160,
        radiusEllipse = 40;

    for (int i = 0; i < numberOfVerticies; i++) {
        int xCoord = shiftX + radiusCircle*cos(2*M_PI/numberOfVerticies * i);
        int yCoord = shiftY + -radiusCircle*sin(2*M_PI/numberOfVerticies * i);
        paint.drawText(xCoord, yCoord, QString("%1").arg(i + 1));
        paint.drawEllipse(xCoord, yCoord, radiusEllipse, radiusEllipse);
    }
}

void MainWindow::bindEllipseWithDirection(QPainter &paint, int numberOfVerticies) {
    paint.setPen(QColor(0, 0, 0));
    paint.setBrush(Qt::Dense7Pattern);

    int shiftX = 700,
        shiftY = 300;
    int radiusCircle = 160,
        radiusEllipse = 40;

    for (int vertexCount = 0; vertexCount < numberOfVerticies; vertexCount++) {
        QVector<int> neighbours = getNeighbours(vertexCount);

        int xCoordVertexFirst = shiftX + radiusCircle*cos(2*M_PI/numberOfVerticies*vertexCount) + radiusEllipse/2;
        int yCoordVertexFirst = shiftY + -radiusCircle*sin(2*M_PI/numberOfVerticies*vertexCount) + radiusEllipse/2;

        for (int i = 0; i < neighbours.size(); i++) {

            int xCoordVertexSecond = shiftX + radiusCircle*cos(2*M_PI/numberOfVerticies*neighbours[i]) + radiusEllipse/2;
            int yCoordVertexSecond = shiftY + -radiusCircle*sin(2*M_PI/numberOfVerticies*neighbours[i]) + radiusEllipse/2;

            paint.drawLine(xCoordVertexFirst, yCoordVertexFirst, xCoordVertexSecond, yCoordVertexSecond);
        }
    }
}

void MainWindow::getMatrix() {
    path.clear();
    matrix.clear();
    for (int i = 0; i < ui->matrix->rowCount(); i++) {
        QVector<int> qvect = QVector<int>(ui->matrix->rowCount(), 0);
        for (int j = 0; j < ui->matrix->rowCount(); j++) {
            if ((ui->matrix->item(i, j) != nullptr && !ui->matrix->item(i, j)->text().isEmpty())) {
                qvect[j] = ui->matrix->item(i, j)->data(0).toInt();
            }
            else qvect[j] = 0;
        }
        matrix.push_back(qvect);
    }
}

void MainWindow::commi()
{
    int i = 0;
    while (i < matrix.size()) {
        setInfinity();
        rowReduction();
        columnReduction();

        int i_pos = -1, j_pos = -1;
        int max = 0;
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix.size(); j++) {
                if (matrix[i][j] == 0) {
                    int minInRow = findMinInRow(i, j);
                    int minInColumn = findMinInColumn(i, j);
                    if (minInRow == INT_MAX) minInRow = 0;
                    if (minInColumn == INT_MAX) minInColumn = 0;

                    int value = minInRow + minInColumn;
                    if (value >= max) {
                        max = value;
                        i_pos = i;
                        j_pos = j;
                    }
                }
            }
        }
        path.push_back(i_pos);
        path.push_back(j_pos);
        setInfinity(i_pos, j_pos);
        matrix[j_pos][i_pos] = INT_MAX;

        i++;
    }
}

void MainWindow::rowReduction()
{
    //минимумы cтрок
    QVector<int> minimums;
    for (int i = 0; i < matrix.size(); i++) {
        int minimum = INT_MAX;
        for (int j = 0; j < matrix.size(); j++) {
            if (matrix[i][j] < minimum) minimum = matrix[i][j];
        }
        minimums.push_back(minimum);
    }
    //редукция
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix.size() && minimums[i] != INT_MAX; j++) {
            if (minimums[i] < 10000) matrix[i][j] = matrix[i][j] - minimums[i];
        }
    }
}

void MainWindow::columnReduction()
{
    QVector<int> minimumsST;
    for (int i = 0; i < matrix.size(); i++) {
        int minimum = INT_MAX;
        for (int j = 0; j < matrix.size(); j++) {
            if (matrix[j][i] < minimum) minimum = matrix[j][i];
        }

        minimumsST.push_back(minimum);
    }
    //редукция
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix.size() && minimumsST[i] != INT_MAX; j++) {
            if (minimumsST[i] < 10000) matrix[j][i] = matrix[j][i] - minimumsST[i];
        }
    }
}

int MainWindow::findMinInRow(int i_p, int j_p)
{
    int minimum = INT_MAX;
    for (int j = 0; j < matrix.size(); j++) {
        if (j == j_p) continue;
        else if (matrix[i_p][j] < minimum) minimum = matrix[i_p][j];
    }
    return minimum;
}

int MainWindow::findMinInColumn(int i_p, int j_p)
{
    int minimum = INT_MAX;
    for (int i = 0; i < matrix.size(); i++) {
        if (i == i_p) continue;
        else if (matrix[i][j_p] < minimum) minimum = matrix[i][j_p];
    }
    return minimum;
}

void MainWindow::setInfinity(int i_p, int j_p)
{
    for (int i = 0; i < matrix.size(); i++) {
        matrix[i][i] = INT_MAX;
    }

    if (i_p != -1 && j_p != -1) {
        for (int i = 0; i < matrix.size(); i++) {
            matrix[i_p][i] = INT_MAX;
            matrix[i][j_p] = INT_MAX;
        }
    }
}

void MainWindow::printPath()
{
    solution = "";
    QVector<int> newPath;

    int begin = 0;
    while (newPath.size() < path.size()) {
        bool flag = true;
        for (int i = 0; i < path.size() && flag; i++) {
            if (path[i] == begin && i % 2 == 0) {
                newPath.push_back(path[i]);
                newPath.push_back(path[i + 1]);
                begin = newPath[newPath.size() - 1];

                flag = false;
            }
        }
    }

    for (int i = 0; i < newPath.size() - 1; i++) {
        if (newPath[i] == newPath[i + 1]) {
            newPath.erase(newPath.begin() + i);
            i--;
        }
    }
    for (int i = 0; i < newPath.size(); i++) {
        int k = newPath[i] + 1;
        solution += QString::fromStdString(std::to_string(k)) + QString("->");
    }
}

void MainWindow::on_TSPButtonClicked() {
    getMatrix();
    commi();
    printPath();
    QMessageBox msgBox;
    msgBox.setWindowTitle("TSP");

    msgBox.setText(solution);
    msgBox.exec();
}
