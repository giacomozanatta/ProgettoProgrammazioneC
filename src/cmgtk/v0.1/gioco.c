#include "lib/gioco.h"
#include "lib/campo.h"
#include "lib/inout.h"
#include "lib/listcoord.h"
#include <stdio.h>
#include <stdlib.h>
/*new gioco: permette di creare un campo di gioco, di dimensione righe*colonne con bombe bombe
                ritorna:    1 se c'è stato un errore (più bombe del previsto)
                            2 se c'è stato un problema di alloccamento
                            0 se è andato tutto bene                
*/
int new_gioco_da_lista(gioco *game, int righe, int colonne, coordpila* lista_bombe, int annullamenti){
    elem coord_bomb;
    /*inizializzo game*/
    game->righe=righe;
    game->colonne=colonne;
    game->bombe=0;
    game->celle_scoperte=0;
    inizializza_mosse(&(game->moves), annullamenti);
    if(crea_campo_vuoto(&(game->campo), righe, colonne)==1)
        return 1;
    while((*lista_bombe)){
        /*finchè ho bombe*/
        preleva_in_testa(lista_bombe, &coord_bomb);
        inserisci_una_bomba(&(game->campo), game->righe, game->colonne, coord_bomb.riga, coord_bomb.colonna);
        (game->bombe) = game->bombe+1;
    }
    return 0;
}
int new_gioco(gioco *game, int righe, int colonne, int bombe, int max_mosse){
    /*inizializzo game*/
    if(inizializza_mosse(&(game->moves), max_mosse)==1)
        return 1;
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
    int rig, col, status;
    system("clear");
    stampa_gioco(*game);
    printf("\n\t\tFAI MOSSA!");
    do{
        printf("\n\tINSERISCI RIGA");
	    scanf("%d", &rig);
    }while(rig<0 || rig>=game->righe);
    do{
	    printf("\n\tINSERISCI COLONNA");
        scanf("%d", &col);
    }while(col<0 || col>=game->colonne);
    status=scopri_cella(&(game->campo), game->righe, game->colonne, rig, col, &(game->celle_scoperte));
    inserisci_nuova_mossa(&(game->moves), rig,col);
    return status;

}
/*marca : prende due posizioni X e Y, se la cella in posizione x y è marcata la smarca.
            Se non è marcata, la marca.
            MANCANO CONTROLLI SU X e Y
*/
void marca(gioco* game){
    int rig, col;
    system("clear");
    stampa_gioco(*game);
    printf("\n\t\tMARCA/SMARCA!");
    printf("\n\tINSERISCI RIGA -> ");
	scanf("%d", &rig);
	printf("\n\tINSERISCI COLONNA -> ");
    scanf("%d", &col);
    marca_cella(&(game->campo), game->righe, game->colonne, rig, col);
}

/**salva_su_file: salva il gioco su un file
*/
int salva_schema_su_file(gioco game){
	return scrivi_su_file(game.campo, game.righe, game.colonne);
}

int carica_gioco_da_file(gioco* game){
    int righe, colonne, bombe, annullamenti;
    coordpila lista_bombe=NULL;
    if(leggi_da_file(&lista_bombe, &righe, &colonne, &bombe)!=0)
        return 1; /*problemi in lettura...*/
    /*ok, ho letto da file. su lista_bombe dovrei avere la lista delle bombe lette.*/
    printf("%d %d %d", righe, colonne, bombe);
    printf("\n\tSPECIFICARE MASSIMO NUMERO DI ANNULLAMENTI -> ");
    scanf("%d", &annullamenti);
    new_gioco_da_lista(game, righe, colonne, &lista_bombe, annullamenti);
    printf("%d", game->bombe);
    return 0;
}

void annulla(gioco *game){
    printf("\n\t\tANNULLO MOSSE!");
    annulla_mosse(&(game->moves), &(game->campo), game->righe, game->colonne, &(game->celle_scoperte));
}
void esci_dal_gioco(gioco *game){
    int i;
    for(i=0; i<game->righe; i++)
        free(game->campo[i]);
    free(game->campo);
    free(game->moves.buff_mosse);
}
int inizia_partita(gioco* game){
    int input, ret_value=0, input_sub_menu, esci=0;
    do{
        do{
            system("clear");
            stampa_gioco(*game);
            printf("\n\t1. FAI MOSSA!");
            printf("\n\t2. ANNULLA MOSSA!");
            printf("\n\t3. MARCA/SMARCA CELLA!");
            printf("\n\t4. ESCI :(");
            printf("\n\t-> ");
            scanf("%d", &input);
        }while(input<0 || input>4);
        switch(input){
            case 1:
                ret_value=fai_mossa(game);
                if(ret_value==2){
                    printf("\n\t\tBECCATA BOMBA!");
                    if(game->moves.mosse_da_annullare > game->moves.max_mosse){
                        printf("\n\tGAME OVER... :(");
                        esci=1;
                    }
                    else{
                        do{
                            printf("\n\t1. ANNULLA MOSSE!");
                            printf("\n\t2. ESCI");
                            printf("\n\t -> ");
                            scanf("%d", &input_sub_menu);
                        }while(input_sub_menu<0 || input_sub_menu>2);
                        switch(input_sub_menu){
                            case 1:
                                annulla(game);
                            break; 
                            case 2:
                                esci=2;
                            break;

                        }
                    }
                }
                else if(ret_value==3)
                    printf("\n\t\tCELLA GIA SCOPERTA!");
                else if(ret_value==1)
                    printf("\n\tCELLA MARCATA!");
                if((game->righe * game->colonne) - game->bombe==game->celle_scoperte){
                    system("clear");
                    stampa_gioco(*game);
                    printf("\n\tCOMPLIMENTI! HAI VINTO!");
                    esci=1;
                }
            break;
            case 2:
                annulla(game);
            break;
            case 3:
                marca(game);
            break;
            case 4:
                esci=1;
            break;
        }
    }while(!esci);
    return esci;
}
int nuovo_gioco(){
    int colonne=0, righe=0, bombe=0, annullamenti=0;
    gioco game;
    int input, input_sub_menu, ret_value;
    do{
        do{
            system("clear");
            printf("\n\t\t NUOVO GIOCO! ");
            printf("\n\t1. GENERA CAMPO");
            printf("\n\t2. CARICA DA FILE");
            printf("\n\t -> ");
            scanf("%d", &input);
        }while(input<1 && input>2);
        switch(input){
            case 1:
                system("clear");
                printf("\n\t\t GENERA CAMPO! ");
                do{
                    printf("\n\tINSERISCI COLONNE -> ");
                    scanf("%d", &colonne);
                }while(colonne<2);
                do{
                    printf("\n\tISNERISCI RIGHE -> ");
                    scanf("%d", &righe);
                }while(righe<2);
                do{
                    printf("\n\tINSERISCI BOMBE -> ");
                    scanf("%d", &bombe);
                    if(bombe>=righe*colonne)
                        printf("\n\t[ERR] TROPPE BOMBE :(");
                }while(bombe>=righe*colonne || bombe<0);
                do{
                    printf("\n\tINSERISCI NUMERO MASSIMO DI ANNULLAMENTI -> ");
                    scanf("%d", &annullamenti);
                }while(annullamenti<0);
                if(new_gioco(&game, righe, colonne, bombe, annullamenti)==1)
                    printf("\n\tERRORE CREAZIONE NUOVO CAMPO :(");
                else{
                    /*GIOCO.*/
                    do{
                        system("clear");
                        printf("\n\t SCHEMA GENERATO!");
                        printf("\n\t1. GIOCA");
                        printf("\n\t2. SALVA SCHEMA SU FILE");
                        printf("\n\t3. ESCI");
                        scanf("%d", &input_sub_menu);
                    }while(input_sub_menu<0 || input_sub_menu>3);
                    switch(input_sub_menu){
                        case 1:
                            ret_value=inizia_partita(&game);
                            esci_dal_gioco(&game);
                        break;
                        case 2:
                            salva_schema_su_file(game);
                            do{
                                system("clear");
                                printf("\n\t\tGIOCO SALVATO!");
                                printf("\n\t1. GIOCA");
                                printf("\n\t\t2.ESCI");
                                printf("\n\t-> ");
                                scanf("%d", &input_sub_menu);
                            }while(input_sub_menu<0 || input_sub_menu>2);
                            switch(input_sub_menu){
                                case 1:
                                    ret_value=inizia_partita(&game);
                                    esci_dal_gioco(&game);
                                break;
                                case 2:
                                    ret_value=3;
                                break;
                            }
                        break;
                        case 3:
                            ret_value=3;
                        break;
                    }
                }
            break; 
            case 2:
                carica_gioco_da_file(&game);
                ret_value=inizia_partita(&game);
                esci_dal_gioco(&game);
            break;
            case 3:
                ret_value=3;
            break;
        }
    }while(ret_value!=3);
    return 0;
}
int gioca(){
    int input=0;
    do{
        printf("\n\t\tCAMPO MINATO");
        printf("\n\t1. NUOVO GIOCO!");
        printf("\n\t2. ESCI!");
        printf("\n\t -> ");
        scanf("%d", &input);
    }while(input<1 && input>2);
    switch(input){
        case 1:
            nuovo_gioco();
        break;
        case 2:
        break;
    }
    return input;
}

