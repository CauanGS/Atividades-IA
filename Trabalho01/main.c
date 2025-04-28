/*
MEMBROS
Alexandre Jun Hayasaka
Cauan Gabriel Souza
Enzo Picelli Stevanato
Felipe Pereira de Souza
Gabriel Sanches Pinto
Hannah Caroline Cavalcanti Santos Andrade
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "swarm.h"
#include <time.h>

#define PI 3.141592684

double rastrigin(double *x)
{
    double out = 2;
    out += x[0] * x[0] - cos(2 * PI * x[0]);
    out += x[1] * x[1] - cos(2 * PI * x[1]);
    return out;
}

double rosenbrock(double *x)
{
    double out = 0;
    // 100*(y-x^2)^2 + (1-x)^2
    out = 100 * pow(x[1] - pow(x[0], 2), 2) + pow(1 - x[0], 2);
    return out;
}

void writeArray(char *fname, double *out, int n)
{
    FILE *fp = fopen(fname, "w");
    for (int i = 0; i < n; i++)
    {
        fprintf(fp, "%.17g \n", out[i]);
    }
    fclose(fp);
}

int main()
{
    srand(1);
    // Definindo parametros:
    char versaoParametros[] = "./PlotsN=25/";

    double wmax, wmin, numPred, lambda, alpha, vmax, Dnuc, Drep, A0, A1;
    int dim;
    int n_particles = 25;
    wmax = 0.95;
    wmin = 0.65;
    vmax = 0.008;
    numPred = 2;
    lambda = 0.001000;
    alpha = 0.001000;
    A0 = 0.0000005;
    A1 = 0.0000005;
    Dnuc = 0.000000;
    Drep = 0.000100;
    dim = 2;
    double c1[2] = {0.5, 0.5};
    double c2[2] = {0.2, 0.2};

    int tmax = 1000;

    double out[tmax];

    int choice = -1;

    /*
        printf("Escolha qual controle de velocidade, swarm e funcao deseja:\n");
        printf(" 1 - Velocity Weight, Swarm Prey/Predator, Rastrigin\n");
        printf(" 2 - Velocity Weight, Swarm Prey/Predator, Rosenbrock\n");
        printf(" 3 - Velocity Weight, Swarm Charged Particles, Rastrigin\n");
        printf(" 4 - Velocity Weight, Swarm Charged Particles, Rosenbrock\n");
        printf(" 5 - Velocity Cut, Swarm Prey/Predator, Rastrigin\n");
        printf(" 6 - Velocity Cut, Swarm Prey/Predator, Rosenbrock\n");
        printf(" 7 - Velocity Cut, Swarm Charged Particles, Rastrigin\n");
        printf(" 8 - Velocity Cut, Swarm Charged Particles, Rosenbrock\n");
    */

    double yOpts[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 1; i <= 10; i++)
    {
        // Loop de teste
        for (int j = 1; j <= 8; j++)
        {
            choice = j;
            switch (choice)
            {
            case 1:
            {
                swarm S1;
                initSwarm(&S1, n_particles, dim, c1, c2);

                double *A = malloc(sizeof(double) * S1.dim);

                A[0] = A0;
                A[1] = A1;

                for (int t = 0; t < tmax; t++)
                {
                    updateSwarmWeightPreyPredator(&S1, rastrigin, wmax, wmin, t, tmax, numPred, A, lambda, alpha);
                    out[t] = rastrigin(S1.x_opt);
                }
                free(A);

                char fname[100];
                snprintf(fname, sizeof(fname), "%sCondInic%dCaso%dResultN=%d.dat", versaoParametros, i, choice, n_particles);
                writeArray(fname, out, tmax);

                yOpts[choice - 1] += out[tmax - 1];

                freeSwarm(&S1);

                break;
            }
            case 2:
            {
                swarm S2;
                initSwarm(&S2, n_particles, dim, c1, c2);

                double *A = malloc(sizeof(double) * S2.dim);

                A[0] = A0;
                A[1] = A1;
                for (int t = 0; t < tmax; t++)
                {
                    updateSwarmWeightPreyPredator(&S2, rosenbrock, wmax, wmin, t, tmax, numPred, A, lambda, alpha);
                    out[t] = rosenbrock(S2.x_opt);
                }
                free(A);

                char fname[100];
                snprintf(fname, sizeof(fname), "%sCondInic%dCaso%dResultN=%d.dat", versaoParametros, i, choice, n_particles);
                writeArray(fname, out, tmax);

                yOpts[choice - 1] += out[tmax - 1];

                freeSwarm(&S2);
                break;
            }
            case 3:
            {
                swarm S3;
                initSwarm(&S3, n_particles, dim, c1, c2);

                for (int t = 0; t < tmax; t++)
                {
                    updateSwarmWeightCharged(&S3, rastrigin, wmax, wmin, t, tmax, Dnuc, Drep);
                    out[t] = rastrigin(S3.x_opt);
                }

                char fname[100];
                snprintf(fname, sizeof(fname), "%sCondInic%dCaso%dResultN=%d.dat", versaoParametros, i, choice, n_particles);
                writeArray(fname, out, tmax);

                yOpts[choice - 1] += out[tmax - 1];

                freeSwarm(&S3);
                break;
            }
            case 4:
            {

                swarm S4;
                initSwarm(&S4, n_particles, dim, c1, c2);

                for (int t = 0; t < tmax; t++)
                {
                    updateSwarmWeightCharged(&S4, rosenbrock, wmax, wmin, t, tmax, Dnuc, Drep);
                    out[t] = rosenbrock(S4.x_opt);
                }

                char fname[100];
                snprintf(fname, sizeof(fname), "%sCondInic%dCaso%dResultN=%d.dat", versaoParametros, i, choice, n_particles);
                writeArray(fname, out, tmax);

                yOpts[choice - 1] += out[tmax - 1];

                freeSwarm(&S4);
                break;
            }
            case 5:
            {

                swarm S5;
                initSwarm(&S5, n_particles, dim, c1, c2);

                double *A = malloc(sizeof(double) * S5.dim);

                A[0] = A0;
                A[1] = A1;

                for (int t = 0; t < tmax; t++)
                {
                    updateSwarmCutPreyPredator(&S5, rastrigin, vmax, numPred, A, lambda, alpha);
                    out[t] = rastrigin(S5.x_opt);
                }
                free(A);

                char fname[100];
                snprintf(fname, sizeof(fname), "%sCondInic%dCaso%dResultN=%d.dat", versaoParametros, i, choice, n_particles);
                writeArray(fname, out, tmax);

                yOpts[choice - 1] += out[tmax - 1];

                freeSwarm(&S5);
                break;
            }
            case 6:
            {

                swarm S6;
                initSwarm(&S6, n_particles, dim, c1, c2);

                double *A = malloc(sizeof(double) * S6.dim);

                A[0] = A0;
                A[1] = A1;

                for (int t = 0; t < tmax; t++)
                {
                    updateSwarmCutPreyPredator(&S6, rosenbrock, vmax, numPred, A, lambda, alpha);
                    out[t] = rosenbrock(S6.x_opt);
                }
                free(A);

                char fname[100];
                snprintf(fname, sizeof(fname), "%sCondInic%dCaso%dResultN=%d.dat", versaoParametros, i, choice, n_particles);
                writeArray(fname, out, tmax);

                yOpts[choice - 1] += out[tmax - 1];

                freeSwarm(&S6);
                break;
            }
            case 7:
            {
                swarm S7;
                initSwarm(&S7, n_particles, dim, c1, c2);

                for (int t = 0; t < tmax; t++)
                {
                    updateSwarmCutCharged(&S7, rastrigin, vmax, Dnuc, Drep);
                    out[t] = rastrigin(S7.x_opt);
                }

                char fname[100];
                snprintf(fname, sizeof(fname), "%sCondInic%dCaso%dResultN=%d.dat", versaoParametros, i, choice, n_particles);
                writeArray(fname, out, tmax);

                yOpts[choice - 1] += out[tmax - 1];

                freeSwarm(&S7);
                break;
            }
            case 8:
            {
                swarm S8;
                initSwarm(&S8, n_particles, dim, c1, c2);

                for (int t = 0; t < tmax; t++)
                {
                    updateSwarmCutCharged(&S8, rosenbrock, vmax, Dnuc, Drep);
                    out[t] = rosenbrock(S8.x_opt);
                }

                char fname[100];
                snprintf(fname, sizeof(fname), "%sCondInic%dCaso%dResultN=%d.dat", versaoParametros, i, choice, n_particles);
                writeArray(fname, out, tmax);

                yOpts[choice - 1] += out[tmax - 1];

                freeSwarm(&S8);
                break;
            }
            default:
                printf("Opção inválida!\n");
                break;
            }
        }
    }
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%sParametrosUsados.dat", versaoParametros);
    FILE *fp = fopen(buffer, "w");
    if (fp == NULL)
    {
        perror("Erro ao abrir/criar o arquivo");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "Número de partículas: %d\n", n_particles);
    fprintf(fp, "Dimensão: %d\n", dim);
    fprintf(fp, "Número máximo de iterações: %d\n", tmax);
    fprintf(fp, "Parâmetros de controle de velocidade:\n");
    fprintf(fp, "c1: %.10f %.10f\n", c1[0], c1[1]);
    fprintf(fp, "c2: %.10f %.10f\n", c2[0], c2[1]);
    fprintf(fp, "wmax: %.10f\n", wmax);
    fprintf(fp, "wmin: %.10f\n", wmin);
    fprintf(fp, "vmax: %.10f\n", vmax);
    fprintf(fp, "Número de predadores: %.10f\n", numPred);
    fprintf(fp, "lambda: %.10f\n", lambda);
    fprintf(fp, "alpha: %.10f\n", alpha);
    fprintf(fp, "A0: %.10f\n", A0);
    fprintf(fp, "A1: %.10f\n", A1);
    fprintf(fp, "Dnuc: %.10f\n", Dnuc);
    fprintf(fp, "Drep: %.10f\n", Drep);

    fclose(fp);

    // Gerando arquivo com as médias dos yOpts
    char buffer2[100];
    snprintf(buffer2, sizeof(buffer2), "%sMediasYOpts.dat", versaoParametros);
    FILE *fp2 = fopen(buffer2, "w");
    if (fp2 == NULL)
    {
        perror("Erro ao abrir/criar o arquivo");
        exit(EXIT_FAILURE);
    }
    fprintf(fp2, "Caso 1: %.17g\n", yOpts[0] / 4);
    fprintf(fp2, "Caso 2: %.17g\n", yOpts[1] / 4);
    fprintf(fp2, "Caso 3: %.17g\n", yOpts[2] / 4);
    fprintf(fp2, "Caso 4: %.17g\n", yOpts[3] / 4);
    fprintf(fp2, "Caso 5: %.17g\n", yOpts[4] / 4);
    fprintf(fp2, "Caso 6: %.17g\n", yOpts[5] / 4);
    fprintf(fp2, "Caso 7: %.17g\n", yOpts[6] / 4);
    fprintf(fp2, "Caso 8: %.17g\n", yOpts[7] / 4);
    fclose(fp2);
    printf("Arquivos gerados com sucesso!\n");

    return 0;
}