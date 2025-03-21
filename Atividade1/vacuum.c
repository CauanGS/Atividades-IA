#include "vacuum.h"
#include <time.h>
#include <stdio.h>
#include <unistd.h>
//#include <windows.h> função sleep no windows
#include <math.h>
#include "queue.h"

/*------------------------------------------------------------------------------
    Funções básicas do ambiente
------------------------------------------------------------------------------*/
 
enviroment newEnviroment(int h, int w){
    enviroment E;
    E.h = h; E.w = w;
    E.grid = malloc(E.h*sizeof(place*));
    if (E.grid!=NULL){
        for (int i=0; i<h; i++){
            E.grid[i] = malloc(E.w*sizeof(place));
            if (E.grid[i]!=NULL)
                for (int j=0; j<w; j++){
                    E.grid[i][j].dirt = false;
                    E.grid[i][j].row = i;
                    E.grid[i][j].col = j;
                }
        }
    }
    return E;
}

void delEnviroment(enviroment* E){
    if (E!=NULL){
        for (int i=0; i<E->h; i++){
            free(E->grid[i]);
        }
        free(E->grid);
    }
}

void initEnviromentDirt(enviroment E, int numDirt){
/*
    Inicializa sujeiras no ambiente.
*/
    srand(time(NULL));
    while (numDirt>0){
        int i, j;
        i = rand()%E.h;
        j = rand()%E.w;
        if (!E.grid[i][j].dirt){
            E.grid[i][j].dirt = true;
            numDirt--;
        }
    }
}

bool isNeighbor(place p, place q){
/*
    Considerando vizinhança-4 (em cruz). No exemplo abaixo, as posições marcadas
    como v são vizinhas de o, enquanto aquelas marcadas como u não são.
    u v u
    v o v
    u v u
*/
    if (p.row==q.row || p.col==q.col){
        if (p.row==q.row+1 || p.row==q.row-1)
            return true;
        if (p.col==q.col+1 || p.col==q.col-1)
            return true;
    }
    return false;
}

place* getPlace(enviroment E, int i, int j){
/*
    Retorna ponteiro para place com indices i e j no grid.
*/
    return &E.grid[i][j];
}

/*------------------------------------------------------------------------------
    Funções básicas do agente (cleaner)
------------------------------------------------------------------------------*/

cleaner newCleaner(enviroment E){
    cleaner C;
    C.battery = MAX_BATTERY;
    C.whereCharger = &E.grid[0][0];
    C.whereCleaner = &E.grid[0][0];
    C.numActions = 0;
    return C;
}

bool move(cleaner* C, enviroment E, place* target){
/*
    Movimenta o agente para a posição place, desde que esta seja uma vizinha de
    cleaner.whereCleaner e que a bateria de C não esteja vazia.
*/
    if (isNeighbor(*C->whereCleaner,*target) && C->battery>0){
        C->whereCleaner = target;
        C->battery--;
        C->numActions++;
        return true;
    }
    return false;
}

bool goTarget(cleaner* C, enviroment E, place* target){
    if (C->whereCleaner!=target){
        //Distâncias verticais e horizontais (distância de Hamming)
        int dist_i = abs(C->whereCleaner->row-target->row);
        int dist_j = abs(C->whereCleaner->col-target->col);
        //Checa se bateria permite deslocamento
        if (dist_i+dist_j<C->battery){
            //Movimento pela altura
            while (C->whereCleaner->row!=target->row){
                //Checa se deve mover para cima ou para baixo
                if (C->whereCleaner->row>target->row)
                    move(
                        C,E,&E.grid[C->whereCleaner->row-1][C->whereCleaner->col]
                    );
                else
                    move(
                        C,E,&E.grid[C->whereCleaner->row+1][C->whereCleaner->col]
                    );
            }
            //Movimento pela largura
            while (C->whereCleaner->col!=target->col){
                //Checa se deve mover para esquerda ou para direita
                if (C->whereCleaner->col>target->col)
                    move(
                        C,E,&E.grid[C->whereCleaner->row][C->whereCleaner->col-1]
                    );
                else
                    move(
                        C,E,&E.grid[C->whereCleaner->row][C->whereCleaner->col+1]
                    );
            }
        }
        else{
            printf("Bateria insuficiente! \n");
            return false;
        }
    }
    return true;
}

bool charge(cleaner* C, enviroment E){
    //Se está na posição do carregador, recarrega
    if (goTarget(C,E,C->whereCharger)){
        printf("Recarregando bateria... \n");
        sleep(1);
        C->battery = MAX_BATTERY;
        return true;
    }
    return false;
    
}

void clean(cleaner* C){
    if (C->whereCleaner->dirt){
        C->whereCleaner->dirt = false;
        C->battery--;
        C->numActions++;
    }
}

//Imprime o ambiente e o agente na tela
void printSimulation(cleaner C, enviroment E){
    //Posição do carregador é sempre a mesma
    //Itera sobre posições do grid
    for (int i=0; i<E.h; i++){
        for (int j=0; j<E.w; j++){
            //Posição do agente
            if (C.whereCleaner->row==i && C.whereCleaner->col==j){
                printf("o");
            }
            else{
                if (C.whereCharger->row==i && C.whereCharger->col==j){
                    printf("C");
                }
                else{
                    if (E.grid[i][j].dirt)
                        printf("x");
                    else
                        printf("-");
                }
            }
        }
        printf("\n");
    }
    printf("\n");
    sleep(1);
}

//Função que procura pela sujeira utilizando BFS
int searchDirt(cleaner* C, enviroment E, place* pl){
    
    //Inicializa fila
    Queue q = newQueue();
    //Vetor se o nó foi visitado ou não
    int visited[E.h][E.w];
    
    //int when[E.h][E.w];

    for(int i = 0;i < E.h;i++){
        for(int j = 0;j < E.w;j++){
            visited[i][j] = 0;
        }
    }

    /*for(int i = 0;i < E.h;i++){
        for(int j = 0;j < E.w;j++){
            when[i][j] = 0;
        }
    }*/
        
    queue_insert(&q,*C->whereCleaner);
    place p = {-1,-1,-1};

    /*
    int distance[E.h][E.w];
    for(int i = 0;i < E.h;i++){
        for(int j = 0;j < E.w;j++){
            distance[i][j] = abs(C->whereCleaner->row - i) + abs(C->whereCleaner->col - j);
        }
    }
    */
    

    
    // int k  = 1;
    while(!queue_empty(&q)){
        queue_front(&q,&p);
        queue_pop(&q);
        
        visited[p.row][p.col] = 1;
        // when[p.row][p.col] = k++;

        /*for(int i = 0;i < E.h;i++){
            for(int j = 0;j < E.w;j++){
                printf("%d ",when[i][j]);
            }
            printf("\t");
            for(int j = 0;j < E.w;j++){
                printf("%d ",distance[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        */
        
        //Se achou sujeira
        if(p.dirt){
            *pl = p; //ponteiro para buscar
            delQueue(&q); //Deleta a fila
            return 1;
        }
                
        //a partir da nova posição atual, tenta percorrer posições adjacentes
        if(p.row + 1 < E.h && !visited[p.row+1][p.col]){
            queue_insert(&q,E.grid[p.row+1][p.col]);
            visited[p.row+1][p.col] = 1;
        } // baixo

        if(p.row - 1 >= 0 && !visited[(p.row-1)][p.col]){
            queue_insert(&q,E.grid[p.row-1][p.col]);
            visited[p.row-1][p.col] = 1;
        } //cima

        if(p.col + 1 < E.w && !visited[p.row][p.col+1]){
            queue_insert(&q,E.grid[p.row][p.col+1]);
            visited[p.row][p.col+1] = 1;
        } //direita

        if(p.col - 1 >= 0 && !visited[p.row][p.col-1]){
            queue_insert(&q,E.grid[p.row][p.col-1]);
            visited[p.row][p.col-1] = 1;
        } //esquerda
    }
    delQueue(&q);
    return 0;
}
