#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPainter>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addVertex();
    void deleteVertex();
    void DFS(int vertex);

    QVector<int> getNeighbours(int vertex);
    void getMatrix();

    //coomi
    void commi();
    void rowReduction();
    void columnReduction();

    int findMinInRow(int i, int j);			//i, j - excepts
    int findMinInColumn(int i, int j);

    void setInfinity(int i = -1, int j = -1);
    void printPath();
    //endcommi

    virtual void paintEvent(QPaintEvent *event);
    void paintEllipse(QPainter &painter, int numberOfVerticies = 0);
    void bindEllipseWithDirection(QPainter &painter, int numberOfVerticies = 0);


private slots:
    void on_pushButtonAddVertex();
    void on_pushButtonDeleteVertex();
    void on_tempButtonClicked();
    void on_TSPButtonClicked();


private:
    Ui::MainWindow *ui;
    QVector<bool> isVisitedVertecies = QVector<bool>(1, 5);
    QPainter paint;

    QVector<QVector<int>> matrix;
    QVector<int> path;
    QString solution;
};
#endif // MAINWINDOW_H
