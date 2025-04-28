#include "swarm.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "particles.h"
#include "utils.h"
#define Q 1

void initParticle(particle *p, int dim, double *x_0)
{
    /*
        Inicializa uma partícula.
        p: partícular a ser inicializada
        dim: dimensão do espaço de busca
        x_0: posição inicial
    */
    // Posição inicial
    p->x = malloc(dim * sizeof(double));
    assert(p->x);
    copyArray(p->x, x_0, dim);
    // Posição ótima
    p->x_opt = malloc(dim * sizeof(double));
    assert(p->x_opt);
    copyArray(p->x_opt, x_0, dim);
    // Velocidade inicial é zero
    p->v = malloc(dim * sizeof(double));
    assert(p->v);
    for (int i = 0; i < dim; i++)
        p->v[i] = 0;
    // Dimensão
    p->dim = dim;
}

void freeParticle(particle *p)
{
    if (p != NULL)
    {
        if (p->x != NULL)
            free(p->x);
        if (p->x_opt != NULL)
            free(p->x_opt);
        if (p->v != NULL)
            free(p->v);
    }
}

void updateVelocityCut(particle *p, double *c1, double *c2, double *swarm_opt, double v_max)
{
    // Termo individual da velocidade
    double *aux_sub1 = arraySub(p->x_opt, p->x, p->dim);
    double *aux_mul1 = arrayMul(c1, aux_sub1, p->dim);
    // Termo coletivo da velocidade
    double *aux_sub2 = arraySub(swarm_opt, p->x, p->dim);
    double *aux_mul2 = arrayMul(c2, aux_sub2, p->dim);
    // Ponteiro para velocidade anterior
    double *aux_v = p->v;
    // Atualiza velocidade
    double *aux_sum = arraySum(aux_mul1, aux_mul2, p->dim);
    // aqui começa a diferença com o update velocity:
    double *aux_v2 = arraySum(aux_v, aux_sum, p->dim);

    if (norm(aux_v2, p->dim) > v_max)
    {
        p->v = constMul(aux_v2, v_max / norm(aux_v2, p->dim), p->dim);
        free(aux_v2);
    }
    else
    {
        p->v = aux_v2;
    }
    // Libera memória
    free(aux_sub1);
    free(aux_sub2);
    free(aux_mul1);
    free(aux_mul2);
    free(aux_v);
    free(aux_sum);
}

void updateVelocityWeight(particle *p, double *c1, double *c2, double *swarm_opt, double W)
{
    /*
    Muitos dos termos do controle de velocidade por peso
    já são calculados no updateVelocity, reproduziremos
    aqui e adicionaremos W
    */
    // Termo individual da velocidade
    double *aux_sub1 = arraySub(p->x_opt, p->x, p->dim);
    double *aux_mul1 = arrayMul(c1, aux_sub1, p->dim);
    // Termo coletivo da velocidade
    double *aux_sub2 = arraySub(swarm_opt, p->x, p->dim);
    double *aux_mul2 = arrayMul(c2, aux_sub2, p->dim);
    // Ponteiro para velocidade anterior
    double *aux_v = p->v;
    // Atualiza velocidade
    double *aux_sum = arraySum(aux_mul1, aux_mul2, p->dim);
    // aqui ocorre a mudança:
    double *aux_v2 = constMul(aux_v, W, p->dim);
    p->v = arraySum(aux_v2, aux_sum, p->dim);
    // Libera memória
    free(aux_sub1);
    free(aux_sub2);
    free(aux_mul1);
    free(aux_mul2);
    free(aux_v);
    free(aux_v2);
    free(aux_sum);
}

void updateVelocity(particle *p, double *c1, double *c2, double *swarm_opt)
{
    /*
        Função para atualizar a velocidade de uma partícula.
        p: partícula a ser atualizada
        c1 e c2: vetores de coeficientes
        swarm_opt: vetor ótimo do enxame
    */
    // Termo individual da velocidade
    double *aux_sub1 = arraySub(p->x_opt, p->x, p->dim);
    double *aux_mul1 = arrayMul(c1, aux_sub1, p->dim);
    // Termo coletivo da velocidade
    double *aux_sub2 = arraySub(swarm_opt, p->x, p->dim);
    double *aux_mul2 = arrayMul(c2, aux_sub2, p->dim);
    // Ponteiro para velocidade anterior
    double *aux_v = p->v;
    // Atualiza velocidade
    double *aux_sum = arraySum(aux_mul1, aux_mul2, p->dim);
    // AQUI
    p->v = arraySum(aux_v, aux_sum, p->dim);
    // Libera memória
    free(aux_sub1);
    free(aux_sub2);
    free(aux_mul1);
    free(aux_mul2);
    free(aux_v);
    free(aux_sum);
}

void updatePosition(particle *p, double (*cost)(double *))
{
    /*
        Função para atualiar a posição de uma parícula.
        p: partícula a ser atualizada
        cost: função custo
    */
    // Ponteiro para posição anterior
    double *aux_x = p->x;
    // Atualiza posição atual
    p->x = arraySum(aux_x, p->v, p->dim);
    // Checa se x_opt deve ser atualizada
    double cost_prev = cost(p->x_opt);
    double cost_new = cost(p->x);
    if (cost_prev > cost_new)
    {
        copyArray(p->x_opt, p->x, p->dim);
    }
    // Libera memória
    free(aux_x);
}

// Testar diferentes valores de A, lambda, c1, c2, W/Vmax, alpha (predador), quantidade de predadores e presas?
void updateVelocityPrey(particle *p, double *c1, double *c2, double *swarm_opt, double WorVmax, double *Xpred,
                        void (*updateVelocityControl)(particle *, double *, double *, double *, double), double *A, double lambda)
{
    // calculo de vi(t+1), passado direto à particula
    updateVelocityControl(p, c1, c2, swarm_opt, WorVmax);
    // Xprey(t) - Xpred(t+1)
    double *aux_sub = arraySub(p->x, Xpred, p->dim);

    // Termo repulsivo: A*(exp(-λ*||aux_sub||))
    double *aux_mul = constMul(A, exp(-lambda * (norm(aux_sub, p->dim))), p->dim);
    double *aux = p->v;
    // Termo final: vi(t+1)+aux_mul, lembrando que aux_mul é escalar e vi, um vetor
    p->v = arraySum(aux, aux_mul, p->dim);

    free(aux);
    free(aux_sub);
}

void updateVelocityPredator(particle *p, double *prey_opt, double alpha)
{
    //[xoptPrey - xpred]
    double *aux_sub = arraySub(prey_opt, p->x, p->dim);
    double *aux_v = p->v;
    // a*aux_sub (a*[xopt - xpred])
    p->v = constMul(aux_sub, alpha, p->dim);
    free(aux_sub);
    free(aux_v);
}

void updateVelocityChargedParticles(particle *p, double *c1, double *c2, double WorVmax, int swarmSize, double *swamXOpt, particle *particles,
                                    void (*updateVelocityControl)(particle *, double *, double *, double *, double), double Dnuc, double Drep, int arrayPos)
{
    // calculo de vi(t+1), passado direto à particula
    updateVelocityControl(p, c1, c2, swamXOpt, WorVmax);

    double *t_sum = malloc(sizeof(double) * p->dim);
    for (int i = 0; i < p->dim; i++)
    {
        t_sum[i] = 0;
    }

    for (int i = 0; i < swarmSize; i++)
    {
        // o nosso j != i

        if (i != arrayPos)
        {
            particle aux_part = particles[i];

            double *aux_sub = arraySub(p->x, aux_part.x, p->dim);
            double aux_norm = norm(aux_sub, p->dim);

            // Dnuc < ||xi - xj|| < Drep
            if (Dnuc < aux_norm && aux_norm < Drep)
            {

                // double* aux_up = constMul(aux_sub,Q*Q,p->dim);
                // double aux_pow = pow(aux_norm,3);
                double *t = constMul(aux_sub, Q * Q / pow(aux_norm, 3), p->dim);
                double *aux_tsum = t_sum;
                t_sum = arraySum(t_sum, t, p->dim);
                free(aux_tsum);
                free(t);
            }
            free(aux_sub);
        }
    }

    double *aux_vant = p->v;
    p->v = arraySum(aux_vant, t_sum, p->dim);

    free(aux_vant);
    free(t_sum);
}