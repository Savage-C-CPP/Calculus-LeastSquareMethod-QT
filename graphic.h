#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QMainWindow>
#include <QtGui>
#include <QDialog>

namespace Ui {
class graphic;
}

class graphic : public QDialog
{
    Q_OBJECT

public:
    explicit graphic(QWidget *parent = nullptr);
    ~graphic();
    void drawGraph(bool notEmpty = 0);
    void getData();
    void recountPixels();
    double f(double x);
    double dots(double x);
    void initData(int k, int n, QVector<double> x, QVector<double> y, QVector<double> a);

private slots:
    void on_exit_clicked();

    void on_clear_clicked();

    void on_draw_clicked();

private:
    Ui::graphic *ui;
    double leftX, rightX;
    double leftY, rightY;
    int pictWidth, pictHeight;
    double step;
    double onePixelX,onePixelY;
    double Ox,Oy;
    //Исходные данные
    int K, N;
    QVector<double> X;
    QVector<double> Y;
    QVector<double> A;

};

#endif // GRAPHIC_H
