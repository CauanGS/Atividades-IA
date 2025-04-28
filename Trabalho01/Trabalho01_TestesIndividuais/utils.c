#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

/*
MEMBROS
Alexandre Jun Hayasaka
Cauan Gabriel Souza
Enzo Picelli Stevanato
Felipe Pereira de Souza
Gabriel Sanches Pinto
Hannah Caroline Cavalcanti Santos Andrade
*/

double* arrayMul(double* u, double* v, int len){
    double *out = malloc(len*sizeof(double));
    assert(out);
    for (int i=0; i<len; i++){
        out[i] = u[i]*v[i];
    }
    return out;
}
//função auxiliar para multiplicar constante por array
double* constMul(double* u, double w, int len){
    double *out = malloc(len*sizeof(double));
    assert(out);
    for (int i=0; i<len; i++){
        out[i] = u[i]*w;
    }
    return out;
}


double* arraySum(double* u, double* v, int len){
    double *out = malloc(len*sizeof(double));
    assert(out);
    for (int i=0; i<len; i++){
        out[i] = u[i]+v[i];
    }
    return out;
}

double* arraySub(double* u, double* v, int len){
    double *out = malloc(len*sizeof(double));
    assert(out);
    for (int i=0; i<len; i++){
        out[i] = u[i]-v[i];
    }
    return out;
}

void copyArray(double* to, double* from, int len){
    for (int i=0; i<len; i++)
        to[i] = from[i];
}

double norm(double* v, int len){
    double out = 0;
    for (int i=0; i<len; i++)
        out += v[i]*v[i];
    return sqrt(out);    
}

