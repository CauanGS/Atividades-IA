#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "swarm.h"

#define PI 3.141592684

/*
MEMBROS
Alexandre Jun Hayasaka
Cauan Gabriel Souza
Enzo Picelli Stevanato
Felipe Pereira de Souza
Gabriel Sanches Pinto
Hannah Caroline Cavalcanti Santos Andrade
*/


double rastrigin(double* x){
    double out = 2;
    out += x[0]*x[0]-cos(2*PI*x[0]);
    out += x[1]*x[1]-cos(2*PI*x[1]);
    return out;
}

double rosenbrock(double* x){ 
    double out = 0;
        //100*(y-x^2)^2 + (1-x)^2
    out = 100*pow(x[1] - pow(x[0],2),2) + pow(1-x[0],2);
    return out;
}

void writeArray(char* fname, double* out, int n){
    FILE* fp = fopen(fname,"w");
    for (int i=0; i<n; i++){
        fprintf(fp,"%.17g \n",out[i]);
    }
    fclose(fp);
}

int main(){

    //Ótimo individual é priorizado no cálculo da velocidade
    double c1[2] = {0.05,0.05};
    double c2[2] = {0.01,0.01};
/*
    //Ótimo do enxame é priorizado no cálculo da velocidade
    double c1[2] = {0.01,0.01};
    double c2[2] = {0.05,0.05};
*/
    //atualmente 5 particulas, permitir alteração
    int n_particles = 0;
    printf("Insira número de partículas: ");
    scanf("%d",&n_particles);

    printf("\nInsira c1: ");
    scanf("%lf %lf",&c1[0],&c1[1]);
    
    printf("\nInsira c2: ");
    scanf("%lf %lf",&c2[0],&c2[1]);


    swarm S; initSwarm(&S,n_particles,2,c1,c2);

    int tmax = 1000;
    printf("\nNúmero de iterações: ");
    scanf("%d",&tmax);

    double out[tmax];

    int choice = -1;
    printf("Escolha qual controle de velocidade, swarm e funcao deseja:\n 1 - Velocity Weight, Swarm Prey/Predator, Rastrigin \n 2 - Velocity Weight, Swarm Prey/Predator, Rosenbrock \n 3 - Velocity Weight, Swarm Charged Particles, Rastrigin\n");
    printf(" 4 - Velocity Weight,Swarm Charged Particles, Rosenbrock \n 5 - Velocity Cut, Swarm Prey/Predator, Rastrigin \n 6 - Velocity Cut, Swarm Prey/Predator, Rosenbrock \n 7 - Velocity Cut, Swarm Charged Particles, Rastrigin \n 8 - Velocity Cut, Swarm Charged Particles, Rosenbrock\n");
    
    scanf("%d",&choice);

    if(choice == 1){
        double wmax;
        double wmin;
        double numPred;
        double lambda;
        double alpha;
        double* A = malloc(sizeof(double)*S.dim);
        printf("valor de wmax: ");
        scanf("%lf",&wmax);
        printf("valor de wmin: ");
        scanf("%lf",&wmin);
        printf("valor de numPred: ");
        scanf("%lf",&numPred);
        printf("valor de lambda: ");
        scanf("%lf",&lambda);
        printf("valor de alpha: ");
        scanf("%lf",&alpha);


        double aux;
        for(int i = 0; i<S.dim; i++){
            printf("valor de A em %d:", i);
            scanf("%lf", &aux);
            A[i] = aux;
        }
        for(int t=0;t < tmax;t++){
            updateSwarmWeightPreyPredator(&S,rastrigin,wmax,wmin,t,tmax,numPred,A,lambda,alpha);
            out[t] = rastrigin(S.x_opt);
        }
    }
    else if(choice == 2){
        double wmax;
        double wmin;
        double numPred;
        double lambda;
        double alpha;
        double* A = malloc(sizeof(double)*S.dim);
        printf("valor de wmax: ");
        scanf("%lf",&wmax);
        printf("valor de wmin: ");
        scanf("%lf",&wmin);
        printf("valor de numPred: ");
        scanf("%lf",&numPred);
        printf("valor de lambda: ");
        scanf("%lf",&lambda);
        printf("valor de alpha: ");
        scanf("%lf",&alpha);


        double aux;
        for(int i = 0; i<S.dim; i++){
            printf("valor de A em %d:", i);
            scanf("%lf", &aux);
            A[i] = aux;
        }
        for(int t=0;t < tmax;t++){
            updateSwarmWeightPreyPredator(&S,rosenbrock,wmax,wmin,t,tmax,numPred,A,lambda,alpha);
            out[t] = rosenbrock(S.x_opt);
        }
    }
    else if(choice == 3){
        double wmax;
        double wmin;
        double Dnuc;
        double Drep;
        printf("valor de wmax: ");
        scanf("%lf",&wmax);
        printf("valor de wmin: ");
        scanf("%lf",&wmin);
        printf("valor de Dnuc: ");
        scanf("%lf",&Dnuc);
        printf("valor de Drep: ");
        scanf("%lf",&Drep);
        for(int t=0;t < tmax;t++){
            updateSwarmWeightCharged(&S,rastrigin,wmax,wmin,t,tmax,Dnuc,Drep);
            out[t] = rastrigin(S.x_opt);
        }
    }
    else if(choice == 4){
        double wmax;
        double wmin;
        double Dnuc;
        double Drep;
        printf("valor de wmax: ");
        scanf("%lf",&wmax);
        printf("valor de wmin: ");
        scanf("%lf",&wmin);
        printf("valor de Dnuc: ");
        scanf("%lf",&Dnuc);
        printf("valor de Drep: ");
        scanf("%lf",&Drep);
        for(int t=0;t < tmax;t++){
            updateSwarmWeightCharged(&S,rosenbrock,wmax,wmin,t,tmax,Dnuc,Drep);
            out[t] = rosenbrock(S.x_opt);
        }
    }
    else if(choice == 5){
        double vmax;
        double numPred;
        double lambda;
        double alpha;
        double* A = malloc(sizeof(double)*S.dim);
        printf("valor de vmax: ");
        scanf("%lf",&vmax);
        printf("valor de numPred: ");
        scanf("%lf",&numPred);
        printf("valor de lambda: ");
        scanf("%lf",&lambda);
        printf("valor de alpha: ");
        scanf("%lf",&alpha);


        double aux;
        for(int i = 0; i<S.dim; i++){
            printf("valor de A em %d:", i);
            scanf("%lf", &aux);
            A[i] = aux;
        }
        
        for(int t=0;t < tmax;t++){
            updateSwarmCutPreyPredator(&S,rastrigin, vmax, numPred, A, lambda, alpha);
            out[t] = rastrigin(S.x_opt);
        }
    }
    else if(choice == 6){
        double vmax;
        double numPred;
        double lambda;
        double alpha;
        double* A = malloc(sizeof(double)*S.dim);
        printf("valor de vmax: ");
        scanf("%lf",&vmax);
        printf("valor de numPred: ");
        scanf("%lf",&numPred);
        printf("valor de lambda: ");
        scanf("%lf",&lambda);
        printf("valor de alpha: ");
        scanf("%lf",&alpha);


        double aux;
        for(int i = 0; i<S.dim; i++){
            printf("valor de A em %d:", i);
            scanf("%lf", &aux);
            A[i] = aux;
        }
        
        for(int t=0;t < tmax;t++){
            updateSwarmCutPreyPredator(&S,rosenbrock, vmax, numPred, A, lambda, alpha);
            out[t] = rosenbrock(S.x_opt);
        }
    }
    else if(choice == 7){
        double vmax;
        double Dnuc;
        double Drep;
        printf("valor de vmax: ");
        scanf("%lf",&vmax);
        printf("valor de Dnuc: ");
        scanf("%lf",&Dnuc);
        printf("valor de Drep: ");
        scanf("%lf",&Drep);
        for(int t=0;t < tmax;t++){
            updateSwarmCutCharged(&S,rastrigin, vmax, Dnuc, Drep);
            out[t] = rastrigin(S.x_opt);
        }
    }
    else if(choice == 8){
        double vmax;
        double Dnuc;
        double Drep;
        printf("valor de vmax: ");
        scanf("%lf",&vmax);
        printf("valor de Dnuc: ");
        scanf("%lf",&Dnuc);
        printf("valor de Drep: ");
        scanf("%lf",&Drep);
        for(int t=0;t < tmax;t++){
            updateSwarmCutCharged(&S,rosenbrock, vmax, Dnuc, Drep);
            out[t] = rosenbrock(S.x_opt);
        }
    }

    writeArray("resultados_N=5.dat",out,tmax);
    return 0;
}