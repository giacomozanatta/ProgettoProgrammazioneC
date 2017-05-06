#include "lib/gioco.h"
#include "lib/campo.h"
#include "lib/inout.h"
#include "lib/listcoord.h"
#include <stdio.h>
/*new gioco: permette di creare un campo di gioco, di dimensione righe*colonne con bombe bombe
                ritorna:    1 se c'è stato un errore (più bombe del previsto)
                            2 se c'è stato un problema di alloccamento
                            0 se è andato tutto bene                
*/
int new_gioco_da_lista(gioco *game, int righe, int colonne, coordpila* lista_bombe){
    elem coord_bomb;
    /*inizializzo game*/
    game->righe=righe;
    game->colonne=colonne;
    game->bombe=0;
    game->celle_scoperte=0;
    crea_campo_vuoto(&(game->campo), righe, colonne);
    while((*lista_bombe)){
        /*finchè ho bombe*/
        preleva_in_testa(lista_bombe, &coord_bomb);
        inserisci_una_bomba(&(game->campo), game->righe, game->colonne, coord_bomb.x, coord_bomb.y);
        (game->bombe) = game->bombe+1;
    }
}
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
/*fai mossa: permette di fare una mossa nel campo di gioco, ossia scoprire una cella
                Questa funzione ritorna: 0 se è andata a buon fine o la cella è già stata scoperta o è marcata
                                            1 se il giocatore ha perso
                                            2 se il giocatore ha vinto
                MANCANO CONTROLLI SU X e Y
*/
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
/*marca : prende due posizioni X e Y, se la cella in posizione x y è marcata la smarca.
            Se non è marcata, la marca.
            MANCANO CONTROLLI SU X e Y
*/
void marca(gioco* game){
    int x, y;
    printf("\n\tINSERISCI X");
	scanf("%d", &x);
	printf("\n\tINSERISCI Y");
    scanf("%d", &y);
    marca_cella(&(game->campo), game->righe, game->colonne, x, y);
}

/**salva_su_file: salva il gioco su un file
*/
int salva_schema_su_file(gioco game){
    printf("SCRIVI SU FILE");
	return scrivi_su_file(game.campo, game.righe, game.colonne);
}

int carica_gioco_da_file(gioco* game){
    int i=0;
    elem coord;
    int righe, colonne, bombe;
    coordpila lista_bombe=NULL;
    if(leggi_da_file(&lista_bombe, &righe, &colonne, &bombe)!=0)
        return 1; /*problemi in lettura...*/
    /*ok, ho letto da file. su lista_bombe dovrei avere la lista delle bombe lette.*/
    printf("%d %d %d", righe, colonne, bombe);
    new_gioco_da_lista(game, righe, colonne, &lista_bombe);
    printf("%d", game->bombe);
    return 0;
}
