#include "approx.h"

Approximate::Matrix Approximate::makeMatrix(QVector<double> X, int K)
{
    //Создаем объект QMatrix размером K*K
        Matrix M = Matrix(K, QVector<double>(K, 0));

        int step;
        for (int i = 0; i < K; i++) {
            step = i;
            for (int j = 0; j < K; j++) {
                M[i][j] = sum(X, step++);
            }
        }
        return  M;
}

QVector<double> Approximate::makeVector(QVector<double> X, QVector<double> Y, int K)
{
    QVector<double> V = QVector<double>(K, 0);
    //Записываем вектор свободных членов в крайний правый столбец матрицы
    for (int i = 0; i < K; i++) {
        V[i] = sum(X, Y, i);
    }
    return V;
}

double Approximate::sum(QVector<double> v, int deg)
{
    double s = 0;
    foreach (auto val, v) {
       s += pow(val, deg);
    }
    return s;
}

double Approximate::sum(QVector<double> x, QVector<double> y, int deg)
{
    double s = 0;
    for (int i = 0; i < x.size(); i++) {
        s += pow(x[i], deg) * y[i];
    }
    return s;
}

QVector<double> Approximate::Gauss(Approximate::Matrix M, QVector<double> B, int K)
{
    QVector<double> res;    //вектор решения
    double max;
    int uravn, index;
    const double eps = 0.00001;
    uravn = 0;
    while (uravn < K) {
        //Ищем строку с максимальным элементом в текущем столбце
       max = abs(M[uravn][uravn]);
       index = uravn;
       for(int i = uravn + 1; i < K; i++)
       {
           if(abs(M[i][uravn]) > max)
           {
               max = abs(M[i][uravn]);
               index = i;
           }
       }
       if(max < eps)
       {
           //НУЛЕВОЙ СТОЛБЕЦ
           return res;
       }
       //Свап строк
       for (int i = 0; i < K; i++) {
           double temp = M[uravn][i];
           M[uravn][i] = M[index][i];
           M[index][i] = temp;
       }
       double temp = B[uravn];
          B[uravn] = B[index];
          B[index] = temp;
       //Нормализация строк
       for (int i = uravn; i < K; i++) {
           double temp = M[i][uravn];
           if(abs(temp) < eps) continue;
           for (int j = 0; j < K; j++)
                M[i][j] /= temp;
           B[i] /= temp;
           if(i == uravn) continue;
           for(int j = 0; j < K; j++)
               M[i][j] -= M[uravn][j];
           B[i] -= B[uravn];

       }
       uravn+=1;     //Переходим к работе со следующей строкой
    }
    //Обратный ход
    res = QVector<double>(K, 0);
    for (uravn = K - 1; uravn >= 0; uravn--) {
        res[uravn] = B[uravn];
        for (int i = 0; i < uravn; i++) {
            B[i] -= M[i][uravn] * res[uravn];
        }
    }
    return res;
}

double Approximate::polynom(double x)
{
    double y = A[0];
    for (int i = 1; i < N; i++) {
        y += A[i] * pow(x, i);
    }
    return y;
}

double Approximate::leastSquareMethod(bool model)   //1 полином 0 степень
{
    double mnk = 0;
    QVector<double> Fi = QVector<double>(K, 0);
    if(model) {
        for (int i = 0; i < K; i++)
            Fi[i] = (polynom(X[i]));
        for (int i = 0; i < K; i++)
            mnk += pow((Y[i] - Fi[i]), 2);
    }
    else {
        for (int i = 0; i < K; i++)
            Fi[i] = (A[0] * pow(i, A[1]));
        for (int i = 0; i < K; i++)
            mnk += pow((Y[i] - Fi[i]), 2);
    }
    return mnk;

}

QVector<double> Approximate::lnVector(QVector<double> X)
{
    QVector<double> lnX = QVector<double>(X.size());
    for (int i = 0; i < lnX.size(); i++) {
        lnX[i] = qLn(X[i]);
    }
    return lnX;
}

double Approximate::degreeFunc(double x)
{
    return pow(M_E, A[0]) * pow(x, A[1]); // y = e^a0 * x^a1
}
