#include "lib/gioco.h"
#include "lib/campo.h"
#include "lib/inout.h"
#include "lib/listcoord.h"
#include "lib/ia.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
    printf("\n\tANNULLAMENTI RIMASTI: %d", (game.moves.max_mosse)-(game.moves.mosse_da_annullare)+1);
    printf("\n\tDIM BUFFER MOSSE: %d", game.moves.dim_mosse);
    printf("\n\tMOSSE NELLO STACK: %d", game.moves.elementi);
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
    if(status==0 || status==2)
        inserisci_nuova_mossa(&(game->moves), rig,col);
    return status;

}
/*marca : prende due posizioni X e Y, se la cella in posizione x y è marcata la smarca.
            Se non è marcata, la marca.
            
*/
void marca(gioco* game){
    int rig, col;
    system("clear");
    stampa_gioco(*game);
    printf("\n\t\tMARCA/SMARCA!");
    printf("\n\tINSERISCI RIGA\n\t -> ");
    do{
	    scanf("%d", &rig);
    }while(rig<0 || rig>= game->righe);
    do{
	    printf("\n\tINSERISCI COLONNA\n\t -> ");
        scanf("%d", &col);
    }while(col<0 || col>=game->colonne);
    marca_cella(&(game->campo), game->righe, game->colonne, rig, col);
}

/**salva_su_file: salva il gioco su un file
*/
int salva_schema_su_file(gioco game){
    char* nome_file=NULL;
    size_t len = 0;
    int read=0;
    char c;
    do 
        c = getchar(); 
    while ((c != EOF) && (c != '\n'));
    printf("\n\tINSERISCI NOME FILE: ");
    do{
        read=getline(&nome_file, &len, stdin);
        if(strlen(nome_file)>0)
            nome_file[strlen(nome_file)-1]='\0';
        if(file_gia_esistente(nome_file)){
            do{
                printf("\n\tEsiste già un file con questo nome. Vuoi sostituirlo?\n\t0 -> SI\n\t1 -> NO\n\t -> ");
                scanf("%d", &read);
            }while(read!=0 && read!=1);
        }
    }while(read==1);
	return scrivi_su_file(game.campo, game.righe, game.colonne, nome_file);
}

int carica_gioco_da_file(gioco* game){
    int righe, colonne, bombe, annullamenti;
    coordpila lista_bombe=NULL;
    char* nome_file = NULL;
    char c;
    int ret_value;
    size_t len=0;
    do 
        c = getchar(); 
    while ((c != EOF) && (c != '\n'));
    printf("\n\tINSERISCI NOME FILE: (0 per ANNULLARE)\n\t ->");
    do{
        
        do{
            getline(&nome_file, &len, stdin);
            if(strlen(nome_file)>0)
                nome_file[strlen(nome_file)-1]='\0';
            if(strlen(nome_file)==1 && nome_file[0]=='0'){
                printf("ANNULLO");
                return 2; /*codice 2: annulla*/
            }
        }while(len==0);
        if(!file_gia_esistente(nome_file))
            printf("\n\tFILE NON ESISTENTE! INSERISCI NOME FILE (0 per ANNULLARE)\n\t ->");
    }while(!file_gia_esistente(nome_file));
    if(leggi_da_file(&lista_bombe, &righe, &colonne, &bombe,nome_file)!=0){
        free(nome_file);
        rimuovi_lista(&lista_bombe);
        return 1; /*problemi in lettura...*/
    }
    /*ok, ho letto da file. su lista_bombe dovrei avere la lista delle bombe lette.*/
    printf("%d %d %d", righe, colonne, bombe);
    printf("\n\tSPECIFICARE MASSIMO NUMERO DI ANNULLAMENTI -> ");
    scanf("%d", &annullamenti);
    free(nome_file);
    ret_value=new_gioco_da_lista(game, righe, colonne, &lista_bombe, annullamenti);
    rimuovi_lista(&lista_bombe);
    return ret_value;
}

void annulla(gioco *game){
    printf("\n\t\tANNULLO MOSSE!");
    annulla_mosse(&(game->moves), &(game->campo), game->righe, game->colonne, &(game->celle_scoperte));
}
void esci_dal_gioco(gioco *game){
    int i;
    printf("\n\tFREE CAMPO...\n\t");
    for(i=0; i<game->righe; i++)
        free(game->campo[i]);
    free(game->campo);
    printf("\n\tFREE MOSSE...\n\t");
    free(game->moves.buff_mosse);
    game->celle_scoperte=0;
    game->righe=0;
    game->colonne=0;
    game->bombe=0;
    game->campo=NULL;
    game->moves.idx_coda=0;
    game->moves.max_mosse=0;
    game->moves.mosse_da_annullare=0;
    game->moves.mosse_correnti=0;
    game->moves.dim_mosse=0;
    game->moves.elementi=0;
    game->moves.buff_mosse=NULL;
    sleep(1);
}
int inizia_partita(gioco* game){
    int input, ret_value=0, input_sub_menu, esci=0;
    do{
        do{
            system("clear");
            stampa_gioco(*game);
            printf("\n\t1. FAI MOSSA!");
            printf("\n\t2. MARCA/SMARCA CELLA!");
            printf("\n\t3. ESCI :(");
            printf("\n\t -> ");
            scanf("%d", &input);
        }while(input<0 || input>4);
        switch(input){
            case 1:
                ret_value=fai_mossa(game);
                if(ret_value==2){
                    system("clear");
                    printf("\n\t\tBECCATA BOMBA!");
                    if(game->moves.mosse_da_annullare > game->moves.max_mosse){
                        printf("\n\tGAME OVER... :(");
                        stampa_gioco_scoperto(*game);
                        sleep(3);
                        esci=1;
                    }
                    else{
                        do{
                            stampa_gioco(*game);
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
                    printf("\n\tCOMPLIMENTI! HAI VINTO!\n\n");
                    esci=1;
                }
            break;
            case 2:
                marca(game);
            break;
            case 3:
                esci=1;
            break;
        }
    }while(!esci);
    return esci;
}
int nuovo_gioco(){
    int colonne=0, righe=0, bombe=0, annullamenti=0;
    gioco game;
    ia_info ia_data;
    int input, input_sub_menu, ret_value;

    do{
        do{
            system("clear");
            printf("\n\t\t NUOVO GIOCO! ");
            printf("\n\t1. GENERA CAMPO");
            printf("\n\t2. CARICA DA FILE");
            printf("\n\t3. ESCI :( ");
            printf("\n\t -> ");
            scanf("%d", &input);
        }while(input<1 && input>2);
        switch(input){
            case 1:
                system("clear");
                printf("\n\t\t GENERA CAMPO! ");
                do{
                    printf("\n\tINSERISCI COLONNE\n\t-> ");
                    scanf("%d", &colonne);
                }while(colonne<2);
                do{
                    printf("\n\tINSERISCI RIGHE\n\t-> ");
                    scanf("%d", &righe);
                }while(righe<2);
                do{
                    printf("\n\tINSERISCI BOMBE\n\t-> ");
                    scanf("%d", &bombe);
                    if(bombe>=righe*colonne)
                        printf("\n\t[ERR] TROPPE BOMBE :(");
                }while(bombe>=righe*colonne || bombe<0);
                do{
                    printf("\n\tINSERISCI NUMERO MASSIMO DI ANNULLAMENTI\n\t-> ");
                    scanf("%d", &annullamenti);
                }while(annullamenti<0);
                if(new_gioco(&game, righe, colonne, bombe, annullamenti)==1)
                    printf("\n\tERRORE CREAZIONE NUOVO CAMPO :(");
                else{
                    /*GIOCO.*/
                    do{ 
                        system("clear");
                        printf("\n\t SCHEMA GENERATO! (DIFFICOLTÀ: %f)", (float)(game.bombe)/(float)(game.colonne*game.righe));
                        printf("\n\t1. GIOCA");
                        printf("\n\t2. SALVA SCHEMA SU FILE");
                        printf("\n\t3. SIMULA (BETA)");
                        printf("\n\t4. ESCI");
                        scanf("%d", &input_sub_menu);
                    }while(input_sub_menu<0 || input_sub_menu>4);
                    switch(input_sub_menu){
                        case 1:
                            ret_value=inizia_partita(&game);
                            esci_dal_gioco(&game);
                        break;
                        case 2:
                            if(salva_schema_su_file(game)!=0)
                                ret_value=2;

                            do{
                                system("clear");
                                printf("\n\t\tGIOCO SALVATO!");
                                printf("\n\t1. GIOCA");
                                printf("\n\t2.ESCI");
                                printf("\n\t-> ");
                                scanf("%d", &input_sub_menu);
                            }while(input_sub_menu<0 || input_sub_menu>2);
                            switch(input_sub_menu){
                                case 1:
                                    ret_value=inizia_partita(&game);
                                    esci_dal_gioco(&game);
                                break;
                                case 2:
                                    esci_dal_gioco(&game);
                                    ret_value=2;
                                break;
                            }
                        break;
                        case 3:
                            printf("\n\tSIMULA\n\t\tIl computer proverà a risolvere lo schema di gioco...");
                            sleep(1);
                            inizializza_ia(game, &ia_data);
                            simula_partita(&game, &ia_data);
                            free_ia(&ia_data);
                            esci_dal_gioco(&game);
                            ret_value=2;
                        break;
                        case 4:
                            esci_dal_gioco(&game);
                    }
                }
            break; 
            case 2:
                if(carica_gioco_da_file(&game)!=0){
                    ret_value=2;
                }
                else{
                    int sub_load_in;
                    do{
                        system("clear");
                        printf("\n\t SCHEMA CARICATO (DIFFICOLTÀ: %f)", (float)(game.bombe)/(float)(game.colonne*game.righe));
                        printf("\n\t1. GIOCA");
                        printf("\n\t2. SIMULA (BETA)");
                        printf("\n\t3. ESCI");
                        scanf("%d", &sub_load_in);
                    }while(sub_load_in<0 || sub_load_in>3);
                    switch(sub_load_in){
                        case 1:
                            ret_value=inizia_partita(&game);
                            sleep(3);
                            esci_dal_gioco(&game);
                        break;
                        case 2:
                            inizializza_ia(game, &ia_data);
                            simula_partita(&game, &ia_data);
                            free_ia(&ia_data);
                            esci_dal_gioco(&game);
                            ret_value=3;
                        break;
                        case 3:
                            esci_dal_gioco(&game);
                        break;
                    }
                }
            break;
            case 3:
                ret_value=3;
            default:
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
            return 2;
        break;
    }
    return input;
}

