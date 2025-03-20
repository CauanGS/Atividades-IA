#include <stdio.h>
#include "new_plan.h"
#include <unistd.h>

void clean_environment(cleaner *C, enviroment E)
{
    int room_clean = 0;
    printSimulation(*C, E);
    while (!room_clean)
    {   



        // Checa se bateria atingiu limite crítico

        place target = {-1, -1, -1};
        int found_dirt = searchDirt(C, E, &target);
        if (found_dirt)
        {
            
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
        else{
            room_clean = 1;
        }
    }
}
