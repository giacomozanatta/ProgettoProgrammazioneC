#ifndef INOUT_H
#define INOUT_H

#include "listcoord.h"
#include "campo.h"
int scrivi_su_file(matrice campo, int righe, int colonne);
int leggi_da_file(coordpila *lista_bombe, int* righe, int *colonne, int *bombe);
void stampa_lista(coordpila lista_bombe);
#endif