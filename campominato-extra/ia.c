#include "lib/gioco.h"
#include "lib/listcoord.h"
#include "lib/gioco.h"
#include "lib/ia.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define KBLU  "\x1B[34m"
#define KGRN  "\x1B[32m"
#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"
#define YEL   "\x1B[33m"
#define CYN   "\x1B[36m"

void ia_aggiorna_prob_dato_n_bombe(matrice* campo, ia_info* ia_data, int righe, int colonne, int rig, int col, int *celle_scoperte, float new_prob);
/*stampa campo: stampo il campo per vedere cosa sta combinando il computer*/
void ia_stampa_campo(matrice campo, int righe, int colonne){
	int i, j;
	printf("\n\t");
	for(i=0; i<righe; i++){
		for (j = 0; j < colonne; j++) {
			if (campo[i][j].scoperta > 0 && campo[i][j].value != -1){ /*è scoperta*/
				if(campo[i][j].value>0)
					printf("%s%d%s ", YEL,campo[i][j].value, KNRM);
				else
					printf("%d ", campo[i][j].value);
			}
			else if(campo[i][j].marcata == 1) /*è marcata*/
				printf("%s$%s ", CYN, KNRM);
			else if(campo[i][j].scoperta == 1 && campo[i][j].value == -1)
				printf("%sB%s ", KRED, KNRM);
			else
				printf("%s#%s ", KBLU, KNRM);
		}
			printf("\n\t");
	}
}
/*stampa bombe limitrofe */
void stampa_bombe_limitrofe(ia_info ia_data){
    int i, j;
    printf("\n\tMATRICE BOMBE LIMITROFE:\n\t ");
    for(i=0; i<ia_data.righe; i++){
        for(j=0; j<ia_data.colonne; j++){
            if(ia_data.mat_prob[i][j].scoperta==1)
                printf("%d ", ia_data.mat_prob[i][j].bombe_limitrofe_non_scoperte);
            else
                printf("0 ");
        }
            printf("\n\t");
    }
}
/*stampa celle coperte limmitrofe*/
void stampa_celle_coperte(ia_info ia_data){
    int i, j;
    printf("\n\tMATRICE CELLE LIMITROFE COPERTE:\n\t ");
    for(i=0; i<ia_data.righe; i++){
        for(j=0; j<ia_data.colonne; j++){
            printf("%d ", ia_data.mat_prob[i][j].celle_vicine_coperte);
        }
            printf("\n\t");
    }
}

/*aggiorna probabilita: aggiorno la probabilita del campo*/
void aggiorna_probabilita(ia_info* ia_data, int celle_scoperte){
	int i, j;
	ia_data->prob_casuale_bomba=(float)ia_data->bombe_da_scoprire/((float)(ia_data->righe*ia_data->colonne)-ia_data->celle_scoperte);
	for(i=0; i<ia_data->righe; i++){
		for(j=0; j<ia_data->colonne; j++){
			if(ia_data->mat_prob[i][j].prob==0) /*se prob==0 allora prob_c==0 (NON é UNA BOMBA)*/
                    		ia_data->mat_prob[i][j].prob_c=0;
			else if(ia_data->mat_prob[i][j].prob==1) /*se è = a 1 allora È UNA BOMBA*/
                ia_data->mat_prob[i][j].prob_c=1;
			else if(ia_data->mat_prob[i][j].prima_volta==0) /*se non è mai stata toccata, viene modificata prob_c*/
            	ia_data->mat_prob[i][j].prob_c=ia_data->prob_casuale_bomba;
        	}
	}
}
/*inizializza ia: inizializza la struttura di gioco e alloca la matrice*/
int inizializza_ia(gioco game, ia_info *ia_data){
	int i, j;
	printf("\n\tINIZIALIZZO");
	
 	ia_data->righe=game.righe;
    ia_data->colonne=game.colonne;
	ia_data->prob_casuale_bomba=(float)game.bombe/(float)(game.righe*game.colonne); /* P = bombe/cellecoperte*/
    ia_data->mat_prob=(ia_cella**)malloc(sizeof(ia_cella*)*ia_data->righe);
	
    if(!ia_data)
        	return 1; /*errore malloc*/
	for(i=0; i<ia_data->righe; i++){
		printf("\n\tALLOCCO COLONNA %d\n\t", i);
        	(ia_data->mat_prob)[i]=(ia_cella*)malloc(sizeof(ia_cella)*ia_data->colonne);
        	if(!(ia_data->mat_prob)[i]){
				printf("\n\tERRORE\n\t");
        		for(j=0; j<i; j++){
        	            free((ia_data->mat_prob)[j]);
        	        }
       	            return 1; /*errore malloc*/
        	}
    }
	for(i=0; i<ia_data->righe; i++){
			for(j=0; j<ia_data->colonne; j++){
				printf("\n\t[%d %d] \n\t", i, j);
            	ia_data->mat_prob[i][j].prob=ia_data->prob_casuale_bomba;
            	ia_data->mat_prob[i][j].prob_c=ia_data->prob_casuale_bomba;
            	if(i==0 || j==0 || i==ia_data->righe-1 || j==ia_data->colonne-1){
                	if((i==0 && j==0) || (i==ia_data->righe-1 && j==ia_data->colonne-1) || (i==0 && j==ia_data->colonne-1) || (i==ia_data->righe-1 && j==0))
                    		ia_data->mat_prob[i][j].celle_vicine_coperte=3;
                	else
                    		ia_data->mat_prob[i][j].celle_vicine_coperte=5;
            	}
            	else
                	ia_data->mat_prob[i][j].celle_vicine_coperte=8;
				ia_data->mat_prob[i][j].scoperta=0;
            	ia_data->mat_prob[i][j].bombe_limitrofe_non_scoperte=0;
            	ia_data->mat_prob[i][j].prima_volta=0;
	
		}
	}
	ia_data->bombe_da_scoprire=game.bombe;
	return 0;
}
/*free ia: libero la memoria dinamica*/
void free_ia(ia_info* ia_data){
	int i;
	printf("\n\tFREE IA...");

    for(i=0; i<ia_data->righe; i++)
        free(ia_data->mat_prob[i]);
    free(ia_data->mat_prob);
	ia_data->mat_prob=NULL;
	ia_data->righe=0;
	ia_data->colonne=0;
	ia_data->bombe_da_scoprire=0;
	ia_data->celle_scoperte=0;
	sleep(1);
}
/*ia_annulla_mossa: annulla la mossa.*/
void ia_annulla_mossa(matrice* campo, ia_info* ia_data, int righe, int colonne, int rig, int col, int* celle_scoperte){
	int i, j;
	if((*campo)[rig][col].marcata == 0){ /*se non è marcata...*/
		if((*campo)[rig][col].scoperta == 1){
			(*campo)[rig][col].scoperta--;
			/*se non è stata scoperta*/
			(*celle_scoperte)--;
            /**PARTE IA: scoperta=0 se è stata scoperta 1 volta solo*/
            ia_data->mat_prob[rig][col].scoperta=0;

			if((*campo)[rig][col].value==0){ /*se è nulla, allora copri le mosse limitrofe*/
				for(i = rig-1; i<=rig+1; i++)
					for(j = col-1; j<=col+1; j++){
						/*controllo se sono posizioni corrette*/
						if(i>=0 && i<righe && j>=0 && j<colonne)
							annulla_mossa(campo, righe, colonne, i, j, celle_scoperte);
				}
			}
		}
		else if((*campo)[rig][col].scoperta>0){
			(*campo)[rig][col].scoperta--;
			if((*campo)[rig][col].scoperta==0){
				(*celle_scoperte)--;
                ia_data->mat_prob[rig][col].scoperta=0;
            }
		}
			
	}
}
/*ia azzero prob: azzero la probabilita delle celle limitrofe alla cella rig, col*/
void ia_azzero_prob(ia_info* ia_data, int righe, int colonne, int rig, int col){
    int i, j;
    for(i = rig-1; i<=rig+1; i++)
		for(j = col-1; j<=col+1; j++){
			/*controllo se sono posizioni corrette*/
			if(i>=0 && i<righe && j>=0 && j<colonne && (i!=rig || j!=col)){
                printf("\n\tAZZERO PROBABILITA DI %d-%d\n\t", i, j);
                if(ia_data->mat_prob[i][j].prob!=1)
                    ia_data->mat_prob[i][j].prob=0;
            }
		}
}
/*ia decrmenta bombe limitrofe: decrementa le bombe limitrofe ad una cella*/
void ia_decrementa_bombe_limitrofe(matrice* campo, ia_info* ia_data, int righe, int colonne, int rig, int col, int* celle_scoperte){
    int i, j;
    float new_prob;
    for(i = rig-1; i<=rig+1; i++)
		for(j = col-1; j<=col+1; j++){
			/*controllo se sono posizioni corrette*/
			if(i>=0 && i<righe && j>=0 && j<colonne && (i!=rig || j!=col)){
                printf("\n\tDECREMENTO BOMBE CELLE LIMITROFE DI %d-%d", i, j);
                ia_data->mat_prob[i][j].bombe_limitrofe_non_scoperte--;
                ia_data->mat_prob[i][j].celle_vicine_coperte--;
                if(ia_data->mat_prob[i][j].bombe_limitrofe_non_scoperte==0 && ia_data->mat_prob[i][j].prima_volta==1){
                    /*se è pari a 0, allora setto P=0 alle celle limitrofe (non sono sicuramente bombe)*/
                    printf("\n\tAZZERO P LIMITROFE DI %d-%d\n\t", i, j);
                    ia_azzero_prob(ia_data, righe, colonne, i, j);
                }
                else{
                    if(ia_data->mat_prob[i][j].scoperta==1){
                        new_prob=(float)ia_data->mat_prob[i][j].bombe_limitrofe_non_scoperte/(float)ia_data->mat_prob[i][j].celle_vicine_coperte;
                        ia_aggiorna_prob_dato_n_bombe(campo, ia_data, righe, colonne, i, j, celle_scoperte, new_prob);
                    }
                }
            }
        }
}
/*ia segnala scoperta cella: segnalo alle celle limitrofe che è stata scoperta una cella*/
void ia_segnala_scoperta_cella(matrice* campo,ia_info* ia_data, int rig, int col, int *celle_scoperte){
 	int i, j;
	float new_prob;
	printf("\n\tSEGNALO CHE È STATA SCOPERTA UNA CELLA %d-%d\n\t", rig, col);
	for(i = rig-1; i<=rig+1; i++)
		for(j = col-1; j<=col+1; j++){
			if(i>=0 && i<ia_data->righe && j>=0 && j<ia_data->colonne && (i!=rig || j!=col)){
				ia_data->mat_prob[i][j].celle_vicine_coperte--;
				new_prob = (float)ia_data->mat_prob[i][j].bombe_limitrofe_non_scoperte/(float)ia_data->mat_prob[i][j].celle_vicine_coperte;
				if(ia_data->mat_prob[i][j].bombe_limitrofe_non_scoperte!=0)
					ia_aggiorna_prob_dato_n_bombe(campo, ia_data, ia_data->righe, ia_data->colonne, i, j, celle_scoperte, new_prob); /*aggiorno la P*/
			}
        }
}
/*ia aggiorna P dato n bombe: aggiorna la P delle celle limitrofe di una cella. se la nuova P è pari a 1, vuol dire che quella cella sarà una bomba e lo devo segnalare.*/
void ia_aggiorna_prob_dato_n_bombe(matrice* campo, ia_info* ia_data, int righe, int colonne, int rig, int col, int *celle_scoperte, float new_prob){
	int i, j;
	if(new_prob==1){
		for(i = rig-1; i<=rig+1; i++)
			for(j = col-1; j<=col+1; j++){
				if(i>=0 && i<ia_data->righe && j>=0 && j<ia_data->colonne && (i!=rig || j!=col)){
					if(ia_data->mat_prob[i][j].scoperta==0 && ia_data->mat_prob[i][j].prob<1){
						if(ia_data->mat_prob[i][j].prima_volta==0){
							ia_data->mat_prob[i][j].prima_volta=1;
							ia_data->mat_prob[i][j].prob=1;
							marca_cella(campo, righe, colonne, i, j);
							(*celle_scoperte)++;
							ia_data->celle_scoperte++;
							ia_data->bombe_da_scoprire--;
							ia_data->mat_prob[i][j].prima_volta=1;
							ia_decrementa_bombe_limitrofe(campo, ia_data, righe, colonne, i, j, celle_scoperte);
						}
					}
				}
			}
	}else{
	for(i = rig-1; i<=rig+1; i++)
		for(j = col-1; j<=col+1; j++){
			/*controllo se sono posizioni corrette*/
			if(i>=0 && i<righe && j>=0 && j<colonne && (i!=rig || j!=col)){
				if(ia_data->mat_prob[i][j].prob>0 && ia_data->mat_prob[i][j].prob<new_prob){
					ia_data->mat_prob[i][j].prob=new_prob;
				}
			}
		}
	}
}
/*ia annulla mosse: annulla le mosse nel campo*/
void ia_annulla_mosse(mosse* moves, ia_info* ia_data, matrice* campo, int righe, int colonne, int* celle_coperte){
    if(!moves->elementi)
         printf("\n\tNON CI SONO MOSSE DA ANNULLARE!");
    else if(moves->mosse_da_annullare>moves->max_mosse)
        printf("\n\tGAME OVER!");
    else{
        int annullamenti = moves->mosse_da_annullare;
        int riga=0, colonna=0;
        while(moves->elementi && annullamenti){
            if(pop(moves->buff_mosse, moves->dim_mosse, &(moves->idx_coda), &(moves->elementi), &riga, &colonna)==0)
                ia_annulla_mossa(campo, ia_data, righe, colonne, riga, colonna, celle_coperte);
            annullamenti--;
        }
        moves->mosse_da_annullare++;
        if(moves->mosse_da_annullare>moves->max_mosse)
                printf("\n\tATTENZIONE! Nessun annullamento rimasto!");
            else
                printf("\n\tATTENZIONE: la prossima volta verranno annullate %d mosse. %d annullamenti rimanenti!", moves->mosse_da_annullare, (moves->max_mosse-moves->mosse_da_annullare)+1);
    }  
}
/*stampo la mat di P*/
void stampa_mat_prob(ia_info ia_data){
    int i, j;
    printf("\n\t");
    for(i=0; i<ia_data.righe; i++){
        for(j=0; j<ia_data.colonne; j++){
            if(ia_data.mat_prob[i][j].prob>ia_data.mat_prob[i][j].prob_c)
                printf("%.2f ", ia_data.mat_prob[i][j].prob);
            else
                printf("%.2f ", ia_data.mat_prob[i][j].prob_c);
        }
        printf("\n\t");
    }
}
/*scopri cella aux: funzione ausiliaria di scopri cella, permette di gestire lo scoprimento delle celle con valore pari a 0.*/
void ia_scopri_cella_aux(matrice* campo, int righe, int colonne, int rig, int col, int* celle_scoperte, ia_info* ia_data){
	int i, j;
	printf("\n\tSCOPRO CELLA %d %d\n", rig, col);
	if((*campo)[rig][col].marcata == 0){
		if((*campo)[rig][col].scoperta == 0){
			(*campo)[rig][col].scoperta++;

			if(ia_data->mat_prob[rig][col].prima_volta == 0){
				ia_data->mat_prob[rig][col].scoperta=1;
				ia_data->mat_prob[rig][col].prob=0;
				ia_data->mat_prob[rig][col].prima_volta=1;
				ia_data->mat_prob[rig][col].bombe_limitrofe_non_scoperte+=(*campo)[rig][col].value;
                ia_data->celle_scoperte++;
				ia_segnala_scoperta_cella(campo, ia_data, rig, col, celle_scoperte); /*segnalo che è stata scoperta una cella*/
				if(ia_data->mat_prob[rig][col].bombe_limitrofe_non_scoperte==0 && (*campo)[rig][col].value!=0)
					ia_azzero_prob(ia_data, righe, colonne, rig, col); /*azzero la P se non ci sono bombe limitrofe*/
			}
			if(ia_data->mat_prob[rig][col].celle_vicine_coperte!=0){
				float new_prob = (float)ia_data->mat_prob[rig][col].bombe_limitrofe_non_scoperte/(float)ia_data->mat_prob[rig][col].celle_vicine_coperte;
				ia_aggiorna_prob_dato_n_bombe(campo, ia_data,righe, colonne, rig, col, celle_scoperte, new_prob); /*aggiorno la P*/
			}

			(*celle_scoperte)++;
			if((*campo)[rig][col].value==0){ /*se è nulla, allora */
				for(i = rig-1; i<=rig+1; i++)
					for(j = col-1; j<=col+1; j++){
						/*controllo se sono posizioni corrette*/
						if(i>=0 && i<righe && j>=0 && j<colonne)
							ia_scopri_cella_aux(campo, righe, colonne, i, j, celle_scoperte, ia_data);
					}
			}
		}
	}
	else if((*campo)[rig][col].value>0){
        (*campo)[rig][col].scoperta++;
    }
}
/*ia scopri cella: scopri una cella nel campo. Se il valore è 0, allora vengono scoperte le sue celle limitrofe.*/
int ia_scopri_cella(matrice* campo, int righe, int colonne, int rig, int col, int* celle_scoperte, ia_info* ia_data){
    printf("SCOPRI CELLA\n\t ");
	if((*campo)[rig][col].value == -1){
		(*campo)[rig][col].scoperta++;
		(*celle_scoperte)++;
		ia_data->celle_scoperte++;

		ia_data->bombe_da_scoprire--;
		ia_data->mat_prob[rig][col].prob=1.0;
		ia_data->mat_prob[rig][col].scoperta=1;
		ia_data->mat_prob[rig][col].prima_volta=1;
		ia_decrementa_bombe_limitrofe(campo, ia_data, righe, colonne, rig, col, celle_scoperte);

		return 1;
	}
	else
		ia_scopri_cella_aux(campo, righe, colonne, rig, col, celle_scoperte, ia_data);
	return 0;
}
/*ia fai mossa: fai una mossa. viene cercata la P minore, e fatta una mossa con quella P*/
int ia_fai_mossa(gioco *game, ia_info *ia_data){
	int i, j;
    int rig=0, col=0;
    float min=1;
    int trovata_mossa=0;
	aggiorna_probabilita(ia_data, game->celle_scoperte);
    /*STEP 1: trovo coordinate possibili*/
    for(i=0; i<ia_data->righe; i++){
        for(j=0; j<ia_data->colonne; j++){
			printf("\n\tPmin: %f, Pcella: %f, PcellaCasuale: %f", min, ia_data->mat_prob[i][j].prob,ia_data->mat_prob[i][j].prob_c);
            if(game->campo[i][j].scoperta==0 && game->campo[i][j].marcata==0 &&  (ia_data->mat_prob[i][j].prob<=min)){
                rig=i;
                col=j;
                trovata_mossa=1;
                if(ia_data->mat_prob[i][j].prob_c>=0 && ia_data->mat_prob[i][j].prob_c<=ia_data->mat_prob[i][j].prob){
                    min=ia_data->mat_prob[i][j].prob_c; /*aggiorno minimo*/
                }
                else
                    min=ia_data->mat_prob[i][j].prob;
            }
        }
    }
    printf("FACCIO MOSSA [%d %d] con P=%f\n", rig, col, min);
    if(!trovata_mossa){
       	if(ia_data->celle_scoperte==ia_data->righe*ia_data->colonne)
			return 1;
		else
			return 3;
    }
	if(ia_scopri_cella(&(game->campo), game->righe, game->colonne, rig, col, &(game->celle_scoperte), ia_data)==1){
		inserisci_nuova_mossa(&(game->moves), rig, col);
        printf("\n\t[FAI MOSSA]BECCATA BOMBA!");
        if(posso_annullare(game->moves)){ /*se posso annullare, annullo */
			ia_annulla_mosse(&(game->moves), ia_data, &(game->campo),  game->righe, game->colonne, &(game->celle_scoperte));
			/*è una bomba. me la segno.*/
			marca_cella(&(game->campo), game->righe, game->colonne, rig, col);
        }
        else{
            printf("\n\t[FAI MOSSA]HO PERSO :(");
            sleep(1);
            return 2;
        }
    }
	else
		inserisci_nuova_mossa(&(game->moves), rig, col);
	ia_data->prob_casuale_bomba=(float)(ia_data->bombe_da_scoprire)/(float)((game->righe*game->colonne)-game->celle_scoperte);
    return 0;
}


/*simula partita: simula una partita*/
int simula_partita(gioco* game, ia_info* ia_data){
    int ret_value=0;
    do{
        system("clear");
        ret_value=ia_fai_mossa(game, ia_data);
        aggiorna_probabilita(ia_data, game->celle_scoperte);
        stampa_bombe_limitrofe(*ia_data);
        stampa_mat_prob(*ia_data);
        stampa_celle_coperte(*ia_data);
        ia_stampa_campo(game->campo, game->righe, game->colonne);
        printf("\n\tBOMBE RIMANENTI: %d", ia_data->bombe_da_scoprire);
        printf("\n\tPROBABILITÀ MOSSA CASUALE: %f", ia_data->prob_casuale_bomba);
        printf("\n\tCELLE SCOPERTE: %d", ia_data->celle_scoperte);
		printf("\n\tANNULLAMENTI RIMASTI: %d", (game->moves.max_mosse)-(game->moves.mosse_da_annullare)+1);
        printf("\n\t-----------------------------------------\n\t");
		if(ia_data->celle_scoperte==ia_data->righe*ia_data->colonne){
			sleep(1);
			printf("HO VINTO ;)\n\t");
			ret_value=1;
		}
        sleep(1);
    }while(ret_value==0);
	if(ret_value==2){
		printf("HO PERSO ;(");
	}
	else if(ret_value==3){
		printf("ERRORE INASPETTATO -.-");
	}
	sleep(2);
    return ret_value;
}