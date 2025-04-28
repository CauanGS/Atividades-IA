#include <stdlib.h>
#include <stdio.h>

#ifndef PARTICLES_H
#define PARTICLES_H

typedef struct {
    double* x; //posição atual
    double* x_opt; //posição ótima
    double* v; //velocidade
    int dim; //dimensão
} particle;

void initParticle(particle* p, int dim, double* x_0);
void freeParticle(particle* p);
void updateVelocity(particle* p, double* c1, double* c2, double* swarm_opt);
void updatePosition(particle* p, double (*cost)(double*));
void updateVelocityWeight(particle* p, double* c1, double* c2, double* swarm_opt, double W);
void updateVelocityCut(particle* p, double* c1, double* c2, double* swarm_opt, double v_max);
void updateVelocityPrey(particle* p, double* c1, double* c2, double* swarm_opt, double WorVmax, double* Xpred, void (*updateVelocityControl)(particle*, double*, double*, double*, double), double* A, double lambda);
void updateVelocityPredator(particle* p, double* prey_opt, double alpha);
void updateVelocityChargedParticles(particle* p, double* c1, double* c2, double WorVmax, int swarmSize, double* swamXOpt, particle* particles, void (*updateVelocityControl)(particle*, double*, double*, double*, double) , double Dnuc, double Drep, int arrayPos);
#endif