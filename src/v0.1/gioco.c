#include "lib/gioco.h"
#include "lib/campo.h"
#include <stdio.h>
int new_gioco(gioco *game, int righe, int colonne, int bombe){
    if(bombe>=righe*colonne){
        return 1;
    }
    /*inizializzo game*/
    game->righe=righe;
    game->colonne=colonne;
    game->bombe=bombe;
    game->celle_scoperte=0;
    return genera_campo(&(game->campo), righe, colonne, bombe);
}
void stampa_gioco(gioco game){
    stampa_campo(game.campo, game.righe, game.colonne);
    printf("\n\n\tCELLE SCOPERTE: %d\n", game.celle_scoperte);
}
void stampa_gioco_scoperto(gioco game){
    stampa_campo_scoperto(game.campo, game.righe, game.colonne);
    printf("\n\n\tCELLE SCOPERTE: %d\n", game.celle_scoperte);
}
