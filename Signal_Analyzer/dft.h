#ifndef DFT_H
#define DFT_H
#include <QVector>

typedef struct{
    double real,
        imaginario;
}complex;

QVector<double>* DFTAmplitude(QVector<double> *v);
QVector<double>* Filter(QVector<double> *v, double cutPassLow, double cutPassHigh);
QVector<double>* DFTPowerSpectre(QVector<double> *v);
QVector<double>* Autocorrelation(QVector<double> *v);

#endif // DFT_H
