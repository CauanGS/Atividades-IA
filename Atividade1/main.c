#include "vacuum.h"
#include "plan.h"
#include "new_plan.h"
#include <stdio.h>

/*
Nomes dos integrantes:
    Alexandre Jun Hayasaka 
    Cauan Gabriel de Souza
    Enzo Picelli Stevanato
    Felipe Pereira de Souza
    Gabriel Sanches Pinto
    Hannah Caroline Cavalcanti Santos Andrade
*/

int main(){

    int h, w, numDirt;
    printf("Digite as dimensões do ambiente (linhas e colunas): ");
    scanf("%d %d", &h, &w);
    printf("Digite a quantidade de sujeira: ");
    scanf("%d",&numDirt);

    //Inicialização do ambiente
    enviroment E = newEnviroment(h,w);
    initEnviromentDirt(E,numDirt);
    
    //Inicialização do agente
    cleaner C = newCleaner(E);

    //Limpeza do ambiente
    clean_environment(&C,E); //Seria o novo plano com o sensor de sujeira
    printf("Número total de ações executadas pelo agente: %d \n",C.numActions);

    delEnviroment(&E);
    return 0;
}