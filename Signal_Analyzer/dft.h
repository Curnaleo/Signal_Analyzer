#ifndef DFT_H
#define DFT_H
#include <QVector>
#include <math.h>

typedef struct{
    double real,
        imaginario;
}complex;

QVector<double>* DFT(QVector<double> *v);

#endif // DFT_H
