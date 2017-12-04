#ifndef MOSSE_H
#define MOSSE_H

#include "listcoord.h"
#include "campo.h"
struct mosse{
    int idx_coda; /*qui ci sarà l'ultima mossa nella coda -> la n-esima mossa dall'utltima mossa*/
    int max_mosse; /*numero massimo di mosse annullabili*/
    int mosse_da_annullare; /*numero che identifica il numero di mosse da annullare quando si perde*/
    int mosse_correnti; /*variabile che identifica il numero di mosse fatte dall'ultimo azzeramento. 
    mosse_correnti è sempre <= mosse_da_annullare*/
    int dim_mosse;
    int elementi;
    
    elem* buff_mosse;
};
typedef struct mosse mosse;

int inizializza_mosse(mosse* moves, int max_mosse);
void inserisci_nuova_mossa(mosse* moves, int x, int y);
void annulla_mosse(mosse* moves, matrice* campo, int righe, int colonne, int* celle_coperte);

void stampa_mosse(mosse moves);

#endif