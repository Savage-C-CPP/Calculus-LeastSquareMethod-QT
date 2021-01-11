#ifndef APPROX_H
#define APPROX_H
#include <QVector>
#include <QMatrix>
#include <QtMath>
#include <algorithm>

class Approximate
{
    typedef QVector<QVector<double>> Matrix;
public:
    int K, N;                   //Количество входных данных (0..k), N - степень полинома
    QVector<double>X;           //Вектор абсцисс
    QVector<double>Y;           //Вектор ординат
    QVector<double>A;           //Вектор коэффициентов (a0..an)
    QVector<double>lnX;         //Вектор натуральных логарифмов от исходного вектора X
    QVector<double>lnY;         //Вектор натуральных логарифмов полученных в ходе решения Yi = a0 + a1 * Xi
    QVector<double>lnA;         //Вектор натуральных логарифмов от вектора коэффициентов (a0..an)
    Matrix SummsA;              //Матрица сумм
    QVector<double>SummsB;      //Вектор свободных членов
    double Sigma;               //Значение функции F(a0..an) = s((Yi - f(xi)^2)

    Matrix makeMatrix(QVector<double> X, int K);                             //Создание матрицы сумм
    QVector<double> makeVector(QVector<double> X, QVector<double> Y, int K); //Создание вектора свободных членов
    double sum(QVector<double> v, int deg);                                  //Функции нахождения сумм
    double sum(QVector<double> x, QVector<double> y, int deg);               //
    QVector<double> Gauss(Matrix M, QVector<double> B, int K);               //Метод Гаусса. Возвращает вектор решения системы
    double polynom(double x);                                                //Функция реализующая полином степени N
    double leastSquareMethod(bool model);                                    //Реализация метода наименьших квадратов
    QVector<double> lnVector(QVector<double> X);                             //Функция возвращвет вектор натуральных логарифмов от вектора X
    double degreeFunc(double x);                                             //Функция реализующая степенную функцию

};

#endif // APPROX_H

/*  Общий вид матрицы решения задачи аппроксимации
 *
 *  K       s(x)    s(x^2)  ..  s(x^N)  | s(y)
 *  s(x)    s(x^2)  s(x^3)  ..  s(x^N+1)| s(x*y)
 *  s(x^2)  s(x^3)  s(x^4)  ..  s(x^N+2)| s(x^2*y)
 *      .       .       .       .
 *  s(x^N)  s(x^N+1)   ..   ..  s(x^2N) | s(x^N*y)
 *
 *  s(x) - функция суммы эл-тов вектора x, K - кол-во входных точек, N - степень полинома
*/
