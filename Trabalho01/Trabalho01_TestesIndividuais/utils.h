
#ifndef UTILS_H
#define UTILS_H


/*
MEMBROS
Alexandre Jun Hayasaka
Cauan Gabriel Souza
Enzo Picelli Stevanato
Felipe Pereira de Souza
Gabriel Sanches Pinto
Hannah Caroline Cavalcanti Santos Andrade
*/

double* arrayMul(double* u, double* v, int len);
double* constMul(double* u, double w, int len);
double* arraySum(double* u, double* v, int len);
double* arraySub(double* u, double* v, int len);
void copyArray(double* to, double* from, int len);
double norm(double* v, int len);

#endif