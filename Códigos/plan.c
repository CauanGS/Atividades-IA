
#include "plan.h"
#include "stdio.h"
#include <unistd.h>

void cleanEnviroment(cleaner* C, enviroment E){
/*
    Plano de limpeza para um agente que conhece o ambiente, mas não sabe onde 
    está a sujeira a ser limpada.
*/
    //Contador de lugares
    int k = 0;
    //Para checar direção de movimento
    bool goUp = true;
    bool goDown = false;
    //Passa por todos os lugares do ambiente
    while(k<E.h*E.w){
        //Anda pela altura
        for (int i=0; i<E.h; i++){
            printSimulation(*C,E);
            clean(C);
            //Checa se bateria atingiu limite crítico
            if (C->battery<E.h+E.w){
                int p = C->whereCleaner->row;
                int t = C->whereCleaner->col;
                //Retorna para carregar bateria
                charge(C,E);
                //Retorna para posição anterior
                goTarget(C,E,&E.grid[p][t]);
            }
            //Se subindo, move para cima
            if (goUp)
                if (C->whereCleaner->row<E.h-1)
                    move(
                        C,E,&E.grid[C->whereCleaner->row+1][C->whereCleaner->col]
                    );
            //Se descendo, move para baixo
            if (goDown)
                if (C->whereCleaner->row>0)
                    move(
                        C,E,&E.grid[C->whereCleaner->row-1][C->whereCleaner->col]
                    );
            k++;
        }
        if (C->battery<E.h+E.w){
            int p = C->whereCleaner->row;
            int t = C->whereCleaner->col;
            //Retorna para carregar bateria
            charge(C,E);
            //Retorna para posição anterior
            goTarget(C,E,&E.grid[p][t]);
        }
        move(C,E,&E.grid[C->whereCleaner->row][C->whereCleaner->col+1]);
        //Atualiza sentido de movimento
        goUp = !goUp;
        goDown = !goDown;
    }
}