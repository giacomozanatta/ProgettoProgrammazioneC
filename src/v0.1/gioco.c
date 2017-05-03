#include "lib/gioco.h"
#include "lib/campo.h"
int new_gioco(gioco *game, int righe, int colonne, int bombe){
    if(bombe>=righe*colonne){
        return 1;
    }
    /*inizializzo game*/
    game->righe=righe;
    game->colonne=colonne;
    game->bombe=bombe;
    return genera_campo(&(game->campo), righe, colonne, bombe);
}