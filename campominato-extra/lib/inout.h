#ifndef INOUT_H
#define INOUT_H

#include "listcoord.h"
#include "campo.h"
int scrivi_su_file(matrice campo, int righe, int colonne, char* nome_file);
int leggi_da_file(coordpila *lista_bombe, int* righe, int *colonne, int *bombe, char* nome_file);
void stampa_lista(coordpila lista_bombe);
int file_gia_esistente(char* nome_file);
#endif