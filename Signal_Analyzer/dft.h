#ifndef DFT_H
#define DFT_H
#include <QVector>

typedef struct{
    double real,
        imaginario;
}complex;

QVector<double>* DFT(QVector<double> *v);
QVector<double>* Filter(QVector<double> *v, double cutPassLow, double cutPassHigh);

#endif // DFT_H
