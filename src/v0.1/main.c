#include <stdio.h>
#include <stdlib.h>
#include "lib/gioco.h"
#include "lib/campo.h"
int main(){
    gioco game;
    printf("%d",new_gioco(&game, 10,10,5));
    stampa_campo(game.campo, game.righe, game.colonne);
    stampa_campo_scoperto(game.campo, game.righe, game.colonne);

}