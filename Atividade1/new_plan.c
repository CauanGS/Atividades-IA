#include <stdio.h>
#include "new_plan.h"
#include <unistd.h>

/*
Nome dos integrantes:
    Alexandre Jun Hayasaka 
    Cauan Gabriel de Souza
    Enzo Picelli Stevanato
    Felipe Pereira de Souza
    Gabriel Sanches Pinto
    Hannah Caroline Cavalcanti Santos Andrade
*/

void clean_environment(cleaner *C, enviroment E)
/*
    Plano de limpeza do agente que conhece o ambiente e
    busca de forma autônoma por sujeiras
*/
{
    // Define se o ambiente foi limpo
    int room_clean = 0;
    printSimulation(*C, E);
    // Executa ate que todo o ambiente esteja limpo
    while (!room_clean)
    {
        place target = {-1, -1, -1};

        // Verifica se achou sujeira
        if (searchDirt(C, E, &target))
        {

            // Verifica se tem bateria suficiente
            if (C->battery < E.h + E.w)
            {
                int p = C->whereCleaner->row;
                int t = C->whereCleaner->col;
                // Retorna para carregar bateria
                charge(C, E);
                // Retorna para posição anterior
                goTarget(C, E, &E.grid[p][t]);
            }

            goTarget(C, E, &target);
            clean(C);
            printSimulation(*C, E);
        }
        else
        {
            // Se não achou, a sala está limpa
            room_clean = 1;
        }
    }
}
