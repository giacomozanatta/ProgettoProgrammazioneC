#include "lib/gioco.h"
#include "lib/campo.h"
#include "lib/inout.h"
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

int fai_mossa(gioco* game){
    int x, y, status;
    printf("\n\tINSERISCI X");
	scanf("%d", &x);
	printf("\n\tINSERISCI Y");
    scanf("%d", &y);
    status=scopri_cella(&(game->campo), game->righe, game->colonne, x, y, &(game->celle_scoperte));
    switch(status){
        case 0: 
            printf("\n\t[ERR] Cella già scoperta! Riprova");
            return 0;
        break;
        case -1:
            printf("\n\t[ERR] Cella marcata! Riprova");
            return 0;
        break;
        case -2:
            printf("\n\t[GAME_OVER] hai perso!");
            return 1;
        break;
        default:
            if(game->celle_scoperte==(game->righe*game->colonne)-game->bombe){
                printf("\n\t[GAME_OVER] COMPLIMENTI! HAI VINTO!!");
                return 2;
            }
            return 0;
    }
}
void marca(gioco* game){
    int x, y;
    printf("\n\tINSERISCI X");
	scanf("%d", &x);
	printf("\n\tINSERISCI Y");
    scanf("%d", &y);
    marca_cella(&(game->campo), game->righe, game->colonne, x, y);
}
