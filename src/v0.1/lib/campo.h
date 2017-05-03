#ifndef CAMPO_H
#define CAMPO_H

struct cella{
	int value;
	int marcata;
	int scoperta;
};
typedef struct cella cella;
typedef cella** matrice;

int genera_campo(matrice* campo, int righe, int colonne, int bombe);
void stampa_campo(matrice campo, int righe, int colonne);
void stampa_campo_scoperto(matrice campo, int righe, int colonne);
int scopri_cella(matrice* campo, int righe, int colonne, int x, int y, int* celle_scoperte);
int marca_cella(matrice* campo, int righe, int colonne, int rig, int col);
#endif