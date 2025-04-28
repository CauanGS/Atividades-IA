
#include <stdlib.h>
#include <stdio.h>
#include "particles.h"

#ifndef SWARM_H
#define SWARM_H

typedef struct{
    int N; //número de partículas
    particle* P; //conjunto de partículas
    int dim; //dimensão
    double* x_opt; //posição ótima
    //Vetores-coeficientes
    double* c1;
    double* c2;
} swarm;

void initSwarm(swarm* S, int N, int dim, double* c1, double* c2);
void freeSwarm(swarm* S);
void updateSwarm(swarm* S, double (*cost)(double*));
void updateSwarmWeight(swarm* S, double (*cost)(double*), double w_max, double w_min, int t, int t_max);
void updateSwarmCut(swarm* S, double (*cost)(double*), double v_max);
void updateSwarmCutCharged(swarm* S, double (*cost)(double*), double v_max, double Dnuc, double Drep);
void updateSwarmWeightCharged(swarm* S, double (*cost)(double*), double w_max, double w_min, int t, int t_max, double Dnuc, double Drep);
void updateSwarmCutPreyPredator(swarm* S, double (*cost)(double*), double v_max, int numPred, double* A, double lambda, double alpha);
void updateSwarmWeightPreyPredator(swarm* S, double (*cost)(double*), double w_max, double w_min, int t, int t_max, int numPred, double* A, double lambda, double alpha);

#endif