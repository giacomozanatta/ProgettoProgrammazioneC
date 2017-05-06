#ifndef GIOCO_H
#define GIOCO_H

#include "campo.h"
#include "inout.h"
struct gioco {
    /*gioco include: il campo, le celle scoperte, */
    matrice campo;
    int celle_scoperte; /*controllo per vincere*/
    int righe; /*numero di righe della matrice*/
    int colonne; /*numero di colonne della matrice*/
    int bombe; /*numero  di bombe all'interno del campo*/
    /*altre variabili da aggiungere: struttura per memorizzare N mosse fatte, stato del gioco (?)*/
};
typedef struct gioco gioco;

int new_gioco(gioco * game, int righe, int colonne, int bombe);
void stampa_gioco(gioco game);
void stampa_gioco_scoperto(gioco game);
int fai_mossa(gioco* game);
void marca(gioco* game);
int salva_schema_su_file(gioco game);
int carica_gioco_da_file(gioco game);
#endif