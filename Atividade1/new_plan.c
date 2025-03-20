#include <stdio.h>
#include "new_plan.h"
#include <unistd.h>



//Esse arquivo a gnt n discutiu btw
//N DISCUTIU
void clean_environment(cleaner *C, enviroment E)
{
    int room_clean = 0;
    printSimulation(*C, E);
    while (!room_clean)
    {   




        place target = {-1, -1, -1};

        //Verifica se achou sujeira
        int found_dirt = searchDirt(C, E, &target);
        if (found_dirt) //Caso achou
        {
            
            //Verifica se tem bateria suficiente
            if (C->battery < E.h + E.w)
            {
                int p = C->whereCleaner->row;
                int t = C->whereCleaner->col;
                // Retorna para carregar bateria
                charge(C, E);
                // Retorna para posição anterior
                goTarget(C, E, &E.grid[p][t]);
            }
            
            //Vai para o alvo
            goTarget(C, E, &target);
            clean(C); //limpa
            printSimulation(*C, E);
        }
        else{
            //Se não achou, a sala está limpa
            room_clean = 1;
        }
    }
}
