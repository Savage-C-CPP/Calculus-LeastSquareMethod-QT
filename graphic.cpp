#include "graphic.h"
#include "ui_graphic.h"
#include <cmath>
#include <QDialog>
#include <QMessageBox>

using namespace std;

graphic::graphic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::graphic)
{
    ui->setupUi(this);
    pictHeight = 370;
    pictWidth = 540;
    step = 0.1;
    leftX = -100; rightX = 100;
    leftY = -100; rightY = 100;
    drawGraph();
}

graphic::~graphic()
{
    delete ui;
}

void graphic::getData()
{
    leftX = ui->inputLeftX->text().toDouble();
    rightX = ui->inputRightX->text().toDouble();
    leftY = ui->inputLeftY->text().toDouble();
    rightY = ui->inputRightY->text().toDouble();
    step = 1.0/1000;
}

//sin(x)
double graphic::f(double x)
{
    return sin(x);
}

double graphic::dots(double x)
{
    return 2 + 0.4 * x - 1.2 * pow(x, 2);
}

void graphic::initData(int k, int n, QVector<double> x, QVector<double> y, QVector<double> a)
{
    K = k; N = n;
    X = x; Y = y; A = a;
}


void graphic::recountPixels()
{
    onePixelX = 540.0/(rightX-leftX);
    onePixelY = 370.0/(rightY-leftY);
    Ox = fabs(leftX); Oy = rightY;
}

void graphic::drawGraph(bool notEmpty)
{
    QPixmap graph(540,370);
    QPainter paint;
    paint.begin(&graph);
    paint.eraseRect(0,0,540,370);
    paint.drawLine(Ox*onePixelX,0,Ox*onePixelX,pictHeight);
    paint.drawLine(0,Oy*onePixelY,pictWidth,Oy*onePixelY);

    paint.setPen(QPen(Qt::black,3));
    for(double i = leftX;i<=rightX;i+=10.0)
        paint.drawPoint((i+Ox)*onePixelX,Oy*onePixelY);
    for(double i = leftY;i<=rightY;i+=10.0)
        paint.drawPoint(Ox*onePixelX,(i+Oy)*onePixelY);

    if(!notEmpty) {
        paint.end();
        ui->outputGraph->setPixmap(graph);
        return;
    }

    paint.setPen(QPen(Qt::green,1,Qt::SolidLine));
    paint.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path,p[3];
    bool first[4] = {true,true,true,true};
    //Отрисовка Sin(x) на интервалах x [leftX ; rightX], y [leftY ; rightY]
    for(double i = (double)leftX+step;i<=(double)rightX;i+=step) {
        if(!isnan(f(i))) {
            if(first[0]) {
                path.moveTo((i+Ox)*onePixelX,(f(i)+Oy)*onePixelY);
                first[0] = false;
            }
            else
                path.lineTo((i+Ox)*onePixelX,(f(i)+Oy)*onePixelY);
        }
    }
    for(double i = (double)leftX+step;i<=(double)rightX;i+=step) {
        if(!isnan(dots(i))) {
            if(first[0]) {
                path.moveTo((i+Ox)*onePixelX,(dots(i)+Oy)*onePixelY);
                first[0] = false;
            }
            else
                path.lineTo((i+Ox)*onePixelX,(dots(i)+Oy)*onePixelY);
        }
    }
    if(ui->sin->isChecked()) {
        paint.setPen(QPen(Qt::blue,1,Qt::SolidLine));
        paint.drawPath(path);
    }
     paint.setPen(QPen(Qt::green,1,Qt::SolidLine));
     paint.drawPath(p[0]);
    

    paint.end();
    ui->outputGraph->setPixmap(graph);



    return;
}

void graphic::on_exit_clicked()
{
    this->close();
}

void graphic::on_clear_clicked()
{
    recountPixels();
    drawGraph();
}

void graphic::on_draw_clicked()
{

    getData();
    recountPixels();
    drawGraph(1);
}
