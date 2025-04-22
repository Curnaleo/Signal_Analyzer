#include "dft.h"

QVector<double>* DFT(QVector<double> *v){
    QVector<double> *v2;
    v2 = new QVector<double>;

    complex aux;

    double real, angulo, imaginario, omega;

    int n, k;
    int N = v->size();

    omega = (2 * M_PI)/(N);


    for(n=0; n<(N/2); n++){
        aux.real = 0;
        aux.imaginario = 0;

        for(k=0 ;k<N; k++){
            angulo = k * omega * n;
            aux.real = aux.real + (v->at(k) * cos(angulo))/N;
            aux.imaginario = aux.imaginario + (v->at(k) * sin(angulo))/N;
        }

        v2->append(sqrt(pow(aux.real,2)+pow(aux.imaginario,2)));
    }

    return v2;
}
