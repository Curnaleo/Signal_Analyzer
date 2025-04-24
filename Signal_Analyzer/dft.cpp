#include "dft.h"
#include <cmath>
#define M_PI 3.14159265358979323846

QVector<double>* DFT(QVector<double> *v){
    QVector<double> *v2;
    v2 = new QVector<double>;

    complex aux;

    double real, angulo, imaginario, omega;

    int n, k;
    int N = v->size();

    omega = (2.0 * M_PI)/(N);


    for(n=0; n<(N); n++){
        aux.real = 0;
        aux.imaginario = 0;

        for(k=0 ;k<N; k++){
            angulo = k * omega * n;
            aux.real = aux.real + (v->at(k) * cos(angulo));
            aux.imaginario = aux.imaginario + (v->at(k) * sin(angulo));
        }

        v2->append(sqrt(pow(aux.real,2)+pow(aux.imaginario,2)));
    }

    return v2;
}

QVector<double>* Filter(QVector<double> *v, double cutPassLow, double cutPassHigh){
    QVector<complex> *v2;
    v2 = new QVector<complex>;

    QVector<double> *v3;
    v3 = new QVector<double>;

    complex aux;

    double real, angulo, imaginario, omega;

    int n, k;
    int N = v->size();

    omega = (2.0 * M_PI)/(N);


    for(n=0; n<N; n++){
        aux.real = 0;
        aux.imaginario = 0;

        for(k=0 ;k<N; k++){
            angulo = k * omega * n;
            aux.real = aux.real + (v->at(k) * cos(angulo));
            aux.imaginario = aux.imaginario + (v->at(k) * sin(angulo));
        }

        aux.imaginario = aux.imaginario * (-1);

        v2->append(aux);
    }

    for(n=0; n<N; n++){
        aux.real = 0;
        aux.imaginario = 0;

        for(k=0 ;k<N; k++){
            if(k < (cutPassLow+1) && k >= (cutPassHigh) || k > (N-cutPassLow-1) && k <= (N-cutPassHigh))
            {
                angulo = k * omega * n;
                aux.real = aux.real + (v2->at(k).real * cos(angulo))/N;
                aux.imaginario = aux.imaginario + (v2->at(k).imaginario * sin(angulo))/N;
            }
        }

        v3->append(aux.real-aux.imaginario);
    }

    delete v2;

    return v3;
}
