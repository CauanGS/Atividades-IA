#include "swarm.h"
#include "particles.h"
#include "utils.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>

void initSwarm(swarm* S, int N, int dim, double* c1, double* c2){
/*
    Inicializa um enxame com posições aleatórias para as partículas.
    N: número de partículas
    dim: dimensão do espaço de busca
*/
    S->P = malloc(N*sizeof(particle));
    assert(S->P);
    for (int i=0; i<N; i++){
        double aux_x[dim];
        for (int s=0; s<dim; s++){
            aux_x[s] = ((double)rand())/((double)RAND_MAX);
            aux_x[s] = 2*aux_x[s]-1;
        }
        initParticle(&S->P[i],dim,aux_x);
    }
    //Posição ótima inicial é também aleatória
    S->x_opt = malloc(dim*sizeof(double));
    assert(S->x_opt);
    for (int s=0; s<dim; s++){
        S->x_opt[s] = ((double)rand())/((double)(RAND_MAX));
        S->x_opt[s] = 2*S->x_opt[s]-1;
    }
    //Dimensão
    S->dim = dim;
    //Vetores coeficientes
    S->c1 = c1;
    S->c2 = c2;
    S->N = N;
}

void freeSwarm(swarm* S){
    if (S!=NULL){
        for (int i=0; i<S->N; i++){
            freeParticle(&S->P[i]);
        }
        free(S->P);
    }
}

void updateSwarm(swarm* S, double (*cost)(double*)){
    //Valor ótimo atual
    double f_opt = cost(S->x_opt);
    //Índice inicial da melhor partícula é -1
    int id_opt = -1;

    //Atualiza posições das partículas e busca novo valor ótimo
    for (int i=0; i<S->N; i++){
        updateVelocity(&S->P[i],S->c1,S->c2,S->x_opt);
        updatePosition(&S->P[i],cost);
        double f_new = cost(S->P[i].x_opt);
        if (f_opt>f_new){
            f_opt = f_new;
            id_opt = i;
        }
    }

    //Se melhorou ótimo atual, atualiza
    if (id_opt>-1){
        copyArray(S->x_opt,S->P[id_opt].x_opt,S->dim);
    }
}

void updateSwarmCutCharged(swarm* S, double (*cost)(double*), double v_max, double Dnuc, double Drep){
    //Valor ótimo atual
    double f_opt = cost(S->x_opt);
    //Índice inicial da melhor partícula é -1
    int id_opt = -1;

    //Atualiza posições das partículas e busca novo valor ótimo
    for (int i=0; i<S->N; i++){
        updateVelocityChargedParticles(&S->P[i],S->c1,S->c2,v_max,S->N,S->x_opt,S->P,updateVelocityCut, Dnuc, Drep, i);
        updatePosition(&S->P[i],cost);
        double f_new = cost(S->P[i].x_opt);
        if (f_opt>f_new){
            f_opt = f_new;
            id_opt = i;
        }
    }

    //Se melhorou ótimo atual, atualiza
    if (id_opt>-1){
        copyArray(S->x_opt,S->P[id_opt].x_opt,S->dim);
    }
}

void updateSwarmWeightCharged(swarm* S, double (*cost)(double*), double w_max, double w_min, int t, int t_max, double Dnuc, double Drep){
    //calculo do w atual
    double W = w_max - ((w_max - w_min)/t_max)*t;
    printf("W(%d): %lf\n",t,W);
    //Valor ótimo atual
    double f_opt = cost(S->x_opt);
    //Índice inicial da melhor partícula é -1
    int id_opt = -1;

    //Atualiza posições das partículas e busca novo valor ótimo
    for (int i=0; i<S->N; i++){
        updateVelocityChargedParticles(&S->P[i],S->c1,S->c2,W,S->N,S->x_opt,S->P,updateVelocityWeight, Dnuc, Drep, i);
        updatePosition(&S->P[i],cost);
        double f_new = cost(S->P[i].x_opt);
        if (f_opt>f_new){
            f_opt = f_new;
            id_opt = i;
        }
    }

    //Se melhorou ótimo atual, atualiza
    if (id_opt>-1){
        copyArray(S->x_opt,S->P[id_opt].x_opt,S->dim);
    }
}

void updateSwarmCutPreyPredator(swarm* S, double (*cost)(double*), double v_max, int numPred, double* A, double lambda, double alpha){
    //verificar se tem mais particulas predador que o max do swarm
    if(S->N <= numPred){
         printf("Muitas particulas de predador, reduza!\n");
         return;
    }
     //Valor ótimo atual
     double f_opt = cost(S->x_opt);
     //Índice inicial da melhor partícula é -1
     int id_opt = -1;
     //média
     double* x_pred = malloc(sizeof(double)*S->dim); 
    //Para obter o melhor das presas ANTES de move-los novamente
    double* x_optPrey = malloc(sizeof(double)*S->dim); 
   
    for(int i = 0;i<S->dim;i++){
        x_pred[i] = 0;
        x_optPrey[i] = 1000;
    }

    for(int i=numPred;i<S->N;i++){
        //pegar o menor x_opt dentre as prezas, segundo a proximidade do x_opt para 0
        double aux1 = cost(S->P[i].x_opt);
        double aux2 = cost(x_optPrey);
        if(aux1 < aux2){
            copyArray(x_optPrey, S->P[i].x_opt, S->dim);
            //x_optPrey = S->P[i].x_opt;
        }
    }
 
     //Atualiza posições das partículas e busca novo valor ótimo
     for (int i=0; i<S->N; i++){
         if(i<numPred){
             //primeiras particulas são predadores    
             updateVelocityPredator(&S->P[i],x_optPrey,alpha);
             updatePosition(&S->P[i],cost);
             
             double* aux = x_pred;
             x_pred = arraySum(x_pred,S->P[i].x,S->dim);
             free(aux);
             
             
         } else {
         //últimas são presas
             if(i==numPred){
                 double* aux = x_pred;
                 x_pred = constMul(x_pred,1/numPred,S->dim);
                 free(aux);
             }
         
             updateVelocityPrey(&S->P[i],S->c1,S->c2,x_optPrey,v_max,x_pred,updateVelocityWeight,A,lambda);
             updatePosition(&S->P[i],cost);

            double f_new = cost(S->P[i].x_opt);
            if (f_opt>f_new){
             f_opt = f_new;
             id_opt = i;
         }
         }
     }

     //Free de x_pred e x_optPrey?
     free(x_pred);
     free(x_optPrey);
     //Se melhorou ótimo atual, atualiza
     if (id_opt>-1){
         copyArray(S->x_opt,S->P[id_opt].x_opt,S->dim);
     }
 }


void updateSwarmWeightPreyPredator(swarm* S, double (*cost)(double*), double w_max, double w_min, int t, int t_max, int numPred, double* A, double lambda, double alpha){
   //verificar se tem mais particulas predador que o max do swarm
   if(S->N <= numPred){
        printf("Muitas particulas de predador, reduza!\n");
        return;
   }
    //calculo do w atual
    double W = w_max - ((w_max - w_min)/t_max)*t;
    printf("W(%d): %lf\n",t,W);
    //Valor ótimo atual
    double f_opt = cost(S->x_opt);
    //Índice inicial da melhor partícula é -1
    int id_opt = -1;
    //média
    double* x_pred = malloc(sizeof(double)*S->dim);
    //Para obter o melhor das presas ANTES de move-los novamente
    double* x_optPrey = malloc(sizeof(double)*S->dim); 
   
    for(int i = 0;i<S->dim;i++){
        x_pred[i] = 0;
        x_optPrey[i] = 1000;
    }

    for(int i=numPred;i<S->N;i++){
        //pegar o menor x_opt dentre as prezas, segundo a proximidade do x_opt para 0
        double aux1 = cost(S->P[i].x_opt);
        double aux2 = cost(x_optPrey);
        if(aux1 < aux2){
            copyArray(x_optPrey, S->P[i].x_opt, S->dim);
            // x_optPrey = S->P[i].x_opt;
        }
    }

    //Atualiza posições das partículas e busca novo valor ótimo
    for (int i=0; i<S->N; i++){
        if(i<numPred){
            //primeiras particulas são predadores    
            updateVelocityPredator(&S->P[i],x_optPrey,alpha);
            updatePosition(&S->P[i],cost);
            
            double* aux = x_pred;
            x_pred = arraySum(x_pred,S->P[i].x,S->dim);
            free(aux);
            
            
        } else {
        //últimas são presas
            if(i==numPred){
                double* aux = x_pred;
                x_pred = constMul(x_pred,1/numPred,S->dim);
                free(aux);
            }
        
            updateVelocityPrey(&S->P[i],S->c1,S->c2,x_optPrey,W,x_pred,updateVelocityWeight,A,lambda);
            updatePosition(&S->P[i],cost);

        double f_new = cost(S->P[i].x_opt);
        if (f_opt>f_new){
            f_opt = f_new;
            id_opt = i;
        }
        }
        
        
    }
    //free x_pred e optPrey
    free(x_pred);
    free(x_optPrey);
    //Se melhorou ótimo atual, atualiza
    if (id_opt>-1){
        copyArray(S->x_opt,S->P[id_opt].x_opt,S->dim);
    }
}

void updateSwarmWeight(swarm* S, double (*cost)(double*), double w_max, double w_min, int t, int t_max){
    //calculo do w atual
    double W = w_max - ((w_max - w_min)/t_max)*t;
    printf("W(%d): %lf\n",t,W);
    //Valor ótimo atual
    double f_opt = cost(S->x_opt);
    //Índice inicial da melhor partícula é -1
    int id_opt = -1;

    //Atualiza posições das partículas e busca novo valor ótimo
    for (int i=0; i<S->N; i++){
        updateVelocityWeight(&S->P[i],S->c1,S->c2,S->x_opt,W);
        updatePosition(&S->P[i],cost);
        double f_new = cost(S->P[i].x_opt);
        if (f_opt>f_new){
            f_opt = f_new;
            id_opt = i;
        }
    }

    //Se melhorou ótimo atual, atualiza
    if (id_opt>-1){
        copyArray(S->x_opt,S->P[id_opt].x_opt,S->dim);
    }
}

void updateSwarmCut(swarm* S, double (*cost)(double*), double v_max){
    //Valor ótimo atual
    double f_opt = cost(S->x_opt);
    //Índice inicial da melhor partícula é -1
    int id_opt = -1;

    //Atualiza posições das partículas e busca novo valor ótimo
    for (int i=0; i<S->N; i++){
        updateVelocityCut(&S->P[i],S->c1,S->c2,S->x_opt,v_max);
        updatePosition(&S->P[i],cost);
        double f_new = cost(S->P[i].x_opt);
        if (f_opt>f_new){
            f_opt = f_new;
            id_opt = i;
        }
    }
    
    //Se melhorou ótimo atual, atualiza
    if (id_opt>-1){
        copyArray(S->x_opt,S->P[id_opt].x_opt,S->dim);
    }
}