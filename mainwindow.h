#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QtCharts>
#include "approx.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void makechart();

private slots:
    void on_openFileBtn_clicked();

    void on_calculateBtn_clicked();

    void on_quitBtn_clicked();

    void on_DegreeRadioBtn_clicked();

    void on_polynomRadioBtn_clicked();

private:
    Ui::MainWindow *ui;

    Approximate approx;              //Объект хранения и обработки входных данных
    QStandardItemModel *inputModel;  //Модель таблицы входных данных
    QStandardItemModel *outputModel; //Модель таблицы выходных данных
    QChart *chrt;                    //Объект графика
    double left, right;              //Границы графика
    double down, up;
};

#endif // MAINWINDOW_H
