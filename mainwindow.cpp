#include "mainwindow.h"
#include "ui_mainwindow.h"
//==========================================
#include <QtCharts/QtCharts>    //Класс виджета чарта
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    inputModel = new QStandardItemModel (0, 2, this);
    ui->tableView->setModel(inputModel);
    ui->tableView->setColumnWidth(0, 88);
    ui->tableView->setColumnWidth(1, 88);
    inputModel->setHeaderData(0, Qt::Horizontal, "X");
    inputModel->setHeaderData(1, Qt::Horizontal, "Y");
}

MainWindow::~MainWindow()
{
    delete ui;
}

//===========================================
///Обработка нажатия на кнопку открытия файла
void MainWindow::on_openFileBtn_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/home",tr("*.txt"));
    QFile file(fileName);
    approx.X.clear();
    approx.Y.clear();
    approx.A.clear();
    if ((file.exists())&&(file.open(QIODevice::ReadOnly))) {
        while(!file.atEnd()) {
            QString data = file.readLine();
            QStringList lst = data.split(' ');
            approx.X.push_back(lst.at(0).toDouble());
            approx.Y.push_back(lst.at(1).toDouble());
        }
        file.close();
    }
    inputModel->setRowCount(approx.X.size());
    QModelIndex index;
       for(int i = 0; i < inputModel->rowCount(); ++i){
           index = inputModel->index(i, 0);
           inputModel->setData(index, approx.X[i]);
           index = inputModel->index(i, 1);
           inputModel->setData(index, approx.Y[i]);
       }
    if(inputModel->rowCount() >= 1) {
        ui->calculateBtn->setEnabled(true);
        ui->polynomDegreeBox->setMaximum(approx.K = inputModel->rowCount() - 1);
    }
    else QMessageBox::information(this, "Недостаточно данных", "Файл пуст  или содержит недостаточно исходных данных!");

}

void MainWindow::on_calculateBtn_clicked()
{
    if(ui->polynomRadioBtn->isChecked()) {
        approx.N = ui->polynomDegreeBox->value() + 1;
        approx.SummsA = approx.makeMatrix(approx.X, approx.N);
        approx.SummsB = approx.makeVector(approx.X, approx.Y, approx.N);
        approx.A = approx.Gauss(approx.SummsA, approx.SummsB, approx.N);    //Получили решение
        //Вычисляем степень отклонения полинома от реальных значений по МНК
        approx.Sigma = approx.leastSquareMethod(1);
        QString polynomInfoOutput;
        for(int i = 0; i < approx.N; i++){
           polynomInfoOutput += "(" + QString::number(approx.A[i]) + ") " + "*X^" + QString::number(i) + " + ";
        }
        polynomInfoOutput.chop(2);
        QMessageBox::information(this, "Успех!", "Полином приближения: \n" + polynomInfoOutput);
    }
    else if(ui->DegreeRadioBtn->isChecked()){
        approx.N = 2;
        approx.lnX = approx.lnVector(approx.X);
        approx.lnY = approx.lnVector(approx.Y);
        approx.SummsA = approx.makeMatrix(approx.lnX, approx.N);
        approx.SummsB = approx.makeVector(approx.lnX, approx.lnY, approx.N);
        approx.lnA = approx.Gauss(approx.SummsA, approx.SummsB, approx.N);    //Получили решение
        approx.A = approx.lnA;
        approx.A[0] = pow(M_E, approx.lnA[0]);
        approx.A[1] = approx.lnA[1];
        //Вычисляем степень отклонения полинома от реальных значений по МНК
        approx.Sigma = approx.leastSquareMethod(0);
        QString degreeOutput;
        degreeOutput = QString::number(approx.A[0]) + " * x^" + QString::number(approx.lnA[1]);
        QMessageBox::information(this, "Успех!", "Функция приближения: \n" + degreeOutput);
    }
    else {
        QMessageBox::information(this, "Внимание!", "Выберите модель приближения:");
        return;
    }

    //Записываем полученные коэффициенты в таблицу результатов
    outputModel = new QStandardItemModel (0, 1, this);
    ui->tableSolution->setModel(outputModel);
    ui->tableSolution->setColumnWidth(0, 95);
    outputModel->setRowCount(approx.N);
    outputModel->setHeaderData(0, Qt::Horizontal, "A");
    QModelIndex index;
       for(int i = 0; i < approx.N; ++i){
           index = outputModel->index(i, 0);
           outputModel->setData(index, approx.A[i]);
       }
     ui->sigmaEdit->setText(QString::number(approx.Sigma));

     //Отрисовка исходных точек и аппроксимирующей функции
       left = *std::min_element(approx.X.constBegin(), approx.X.constEnd());
       right = *std::max_element(approx.X.constBegin(), approx.X.constEnd());
       down = *std::min_element(approx.Y.constBegin(), approx.Y.constEnd());
       up = *std::max_element(approx.Y.constBegin(), approx.Y.constEnd());

       chrt = new QChart;
       ui->ChartWidget->setChart(chrt);

       QValueAxis *axisX = new QValueAxis;
       axisX->setRange(left, right);
       //axisX->setTickCount(10);
       axisX->setLabelFormat("%.2f");

       QValueAxis *axisY = new QValueAxis;
       axisY->setRange(down,  up);
       //axisY->setTickCount(10);
       axisY->setLabelFormat("%g");
       //Исходные точки
       QScatterSeries *inputDataSeries = new QScatterSeries();
       inputDataSeries->setPointLabelsFormat("(@xPoint, @yPoint)");
       inputDataSeries->setPointLabelsVisible(true);

       for (int i = 0; i < approx.X.size(); ++i) {
          *inputDataSeries << QPointF(approx.X[i], approx.Y[i]);
       }

       chrt->addSeries(inputDataSeries);

       //Аппроксимирующая функция
       QLineSeries *approxSeries = new QLineSeries();
       if(ui->polynomRadioBtn->isChecked()) {
           for(double i = left; i < right; i+=1) {
               *approxSeries << QPointF(i, approx.polynom(i));
           }
       }
       else{
           for(double i = left; i < right; i+=1) {
               *approxSeries << QPointF(i, approx.A[0] * pow(i, approx.A[1]));
           }
       }
       chrt->addSeries(approxSeries);
       chrt->setAxisX(axisX, inputDataSeries);
       chrt->setAxisY(axisY, inputDataSeries);
}

void MainWindow::on_quitBtn_clicked()
{
    this->close();
}
void MainWindow::on_DegreeRadioBtn_clicked()
{
    ui->polynomDegreeBox->setEnabled(false);
}

void MainWindow::on_polynomRadioBtn_clicked()
{
    ui->polynomDegreeBox->setEnabled(true);
}
