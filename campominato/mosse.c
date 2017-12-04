#include "lib/mosse.h"
#include <stdlib.h>
#include "lib/campo.h"
#include <stdio.h>
/*push: inserisco una mossa nello stack*/
int push(elem* stack, int riga, int colonna, int dim, int* elementi, int* idx_coda){
    (*idx_coda) = ((*idx_coda)+1)%dim; /*incremento coda*/
    stack[*idx_coda].riga = riga;
    stack[*idx_coda].colonna = colonna;
    if(*elementi <dim)
        (*elementi)++;
    return 0;
}
/*pop: prelevo una mossa dallo stack*/
int pop(elem* stack, int dim, int* idx_coda, int* elementi, int* riga, int* colonna){
    if(!(*elementi))
        return 1;
    *riga = stack[*idx_coda].riga;
    *colonna = stack[*idx_coda].colonna;
    (*elementi)--;
    *idx_coda=(((*idx_coda-1)%dim)+dim)%dim;
    return 0;
    

}
/*inizializza mosse: inizializzo le struttura mosse*/
int inizializza_mosse(mosse* moves, int max_mosse){
    int i;
    moves->idx_coda=0;
    moves->max_mosse=max_mosse;
    moves->mosse_da_annullare=1; /*all'inizio, c'è solo 1 mossa da annullare*/
    moves->dim_mosse = (max_mosse*(max_mosse-1)/2)+1; /*numero max di mosse annullabili*/
    moves->mosse_correnti=0;
    moves->buff_mosse = (elem*)malloc(sizeof(elem)*(moves->dim_mosse)); /*allocco vettore: questa è la dimensione massima, in quanto se annullo una mossa, e poi ne annullo un'altra, vado indietro di 2*/
    moves->elementi = 0;
    if(!(moves->buff_mosse))
        return 1;
    for(i=0; i<moves->dim_mosse; i++){
        moves->buff_mosse[i].riga=0;
        moves->buff_mosse[i].colonna=0;
    }
    return 0;
}
/*inserisco una nuova mossa nello stack*/
void inserisci_nuova_mossa(mosse* moves, int riga, int colonna){
    push(moves->buff_mosse, riga, colonna, moves->dim_mosse, &(moves->elementi), &(moves->idx_coda));

}
/*annulla delle mosse nel campo*/
void annulla_mosse(mosse* moves, matrice* campo, int righe, int colonne, int* celle_coperte){
    if(!moves->elementi)
         printf("\n\tNON CI SONO MOSSE DA ANNULLARE!");
    else if(moves->mosse_da_annullare>moves->max_mosse)
        printf("\n\tGAME OVER!");
    else{
        int annullamenti = moves->mosse_da_annullare;
        int riga=0, colonna=0;
        while(moves->elementi && annullamenti){
            if(pop(moves->buff_mosse, moves->dim_mosse, &(moves->idx_coda), &(moves->elementi), &riga, &colonna)==0) /*prelevo la mossa*/
                annulla_mossa(campo, righe, colonne, riga, colonna, celle_coperte);/*annullo la mossa*/
            annullamenti--;
        }

        moves->mosse_da_annullare++;
        if(moves->mosse_da_annullare>moves->max_mosse)
                printf("\n\tATTENZIONE! Nessun annullamento rimasto!");
            else
                printf("\n\tATTENZIONE: la prossima volta verranno annullate %d mosse. %d annullamenti rimanenti!", moves->mosse_da_annullare, (moves->max_mosse-moves->mosse_da_annullare)+1);
    }  
}

void stampa_mosse(mosse moves){
    int last=moves.idx_coda;
    int elem = moves.elementi;
    int i = last;
    printf("{ ");
    while(elem){
        printf("[%d,%d], ", moves.buff_mosse[i].riga, moves.buff_mosse[i].colonna);
        i=(((i-1)%moves.dim_mosse)+moves.dim_mosse)%moves.dim_mosse;
        elem--;
    }
    printf(" }");
}
