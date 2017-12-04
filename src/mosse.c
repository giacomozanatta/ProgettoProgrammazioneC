#include "lib/mosse.h"
#include <stdlib.h>
#include "lib/campo.h"
#include <stdio.h>
int inizializza_mosse(mosse* moves, int max_mosse){
    int i;
    moves->index=0;
    moves->max_mosse=max_mosse;
    moves->mosse_da_annullare=1; /*all'inizio, c'è solo 1 mossa da annullare*/
    moves->dim_mosse = max_mosse*(max_mosse+1)/2;
    moves->mosse_correnti=0;
    moves->buff_mosse = (elem*)malloc(sizeof(elem)*(moves->dim_mosse)); /*allocco vettore: questa è la dimensione massima, in quanto se annullo una mossa, e poi ne annullo un'altra, vado indietro di 2*/
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
    moves->buff_mosse[moves->index].riga=riga;
    moves->buff_mosse[moves->index].colonna=colonna;
    moves->index=(moves->index+1)%moves->dim_mosse; /*incremento indice ultima mossa*/
    if(moves->mosse_correnti!=moves->dim_mosse)
        moves->mosse_correnti++;
}
/*annulla delle mosse nel campo*/
void annulla_mosse(mosse* moves, matrice* campo, int righe, int colonne, int* celle_coperte){
    if(moves->mosse_correnti){
        int i=0;
        int index_last;
        while(moves->mosse_correnti && i<moves->mosse_da_annullare){
            index_last = (((moves->index-1)%moves->dim_mosse)+moves->dim_mosse)%moves->dim_mosse;
            printf("\n\tANNULLO MOSSA: %d-%d IN POSIZIONE: %d", moves->buff_mosse[index_last].riga, moves->buff_mosse[index_last].colonna, index_last);
            annulla_mossa(campo, righe, colonne, moves->buff_mosse[index_last].riga, moves->buff_mosse[index_last].colonna, celle_coperte); /*annulla la mossa*/
            moves->index=index_last; /*decremento ultima mossa, ora punta alla mossa successiva*/
            moves->mosse_correnti--;
            i++;
        }
        moves->mosse_da_annullare++;
        printf("\n\tANNULLATE %d MOSSE!", i);
        if(moves->mosse_da_annullare>moves->max_mosse)
            printf("\n\tATTENZIONE! Nessun annullamento rimasto!");
        else
            printf("\n\tATTENZIONE: la prossima volta verranno annullate %d mosse. %d annullamenti rimanenti!", moves->mosse_da_annullare, (moves->max_mosse-moves->mosse_da_annullare)+1);
    }else
        printf("\n\tNON CI SONO MOSSE DA ANNULLARE!");
}