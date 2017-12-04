#include "gioco.h"
struct ia_elem{
    float prob;
    int celle_vicine_coperte;
    int scoperta;
    int bombe_limitrofe_non_scoperte;
    int prima_volta;
    float prob_c;
};
typedef struct ia_elem ia_cella;
struct info{
    float prob_casuale_bomba;
    ia_cella** mat_prob;
    int righe;
    int colonne;
    int bombe_da_scoprire;
    int celle_scoperte;
};
typedef struct info ia_info;

int inizializza_ia(gioco game, ia_info *ia_data);
int simula_partita(gioco* game, ia_info* ia_data);
void free_ia(ia_info* ia_data);