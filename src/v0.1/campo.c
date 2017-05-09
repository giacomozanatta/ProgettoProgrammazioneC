//test
#include "lib/campo.h"
#include <stdio.h>
#include <stdlib.h>
#include "lib/listcoord.h"
#define KBLU  "\x1B[34m"
#define KGRN  "\x1B[32m"
#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"
#define YEL   "\x1B[33m"
#define CYN   "\x1B[36m"
/**aggiorna post inserimento bomba: questa funzione aggiorna le celle limitrofe alla cella in cui c'è una bomba (incrementando di 1 se non sono bombe)
	OK
*/
void aggiorna_post_inserimento_bomba(matrice* campo, int righe, int colonne,int rig, int col){
	/*controllo adiacenti*/
	int i, j;
	for(i = rig-1; i<=rig+1; i++)
		for(j = col-1; j<=col+1; j++){
				/*controllo se sono posizioni corrette [DA OTTIMIZZARE]*/
				if(i>=0 && i<righe && j>=0 && j<colonne)
					if((*campo)[i][j].value!=-1)
						(*campo)[i][j].value++;
		}
}

/**inserisci bombe: inserisce bombe bombe nel campo in modo casuale, e aggiorna la posizione delle celle limitrofe
*/
void inserisci_bombe(matrice* campo, int righe, int colonne, int bombe){
	if(bombe>0){
        int rig, col;
		do{
			/*creo due numeri casuali, x e y...*/
			rig = rand() % (righe);
			col = rand() % (colonne);
		}while((*campo)[rig][col].value==-1);
		/*ho beccato una cella che non contiene una bomba... aggiorno celle limitrofe*/
		inserisci_una_bomba(campo, righe, colonne, rig, col);
		inserisci_bombe(campo, righe, colonne, bombe-1);
	}
}
/**inserisci una bomba : inserisce una bomba su matrice[x][y]
	questa funzione ritorna: 1 se c'è un problema con le coordinate
	2 se c'è già una bomba'
	0 se è stata inserita
*/
int inserisci_una_bomba(matrice* campo, int righe, int colonne, int rig, int col){
	printf("\n\tINSERISCO BOMBA...");
    (*campo)[rig][col].value=-1;
	aggiorna_post_inserimento_bomba(campo, righe, colonne, rig, col);
	printf("OK!");
    return 0; /*inserita correttamente*/
}
/** inizializza campo: inizializza un campo[righe][colonne] vuoto
*/
void inizializza_campo(matrice* campo, int righe, int colonne){
	int i, j;
    printf("\n\tINIZIALIZZO CAMPO...");
	for(i=0; i<righe; i++){
		for(j=0; j<colonne; j++){
			(*campo)[i][j].value = 0;
			(*campo)[i][j].marcata = 0;
			(*campo)[i][j].scoperta = 0;
		}
	}
    printf("\tOK!");
}
/**crea campo vuoto: allocca un campo in memoria dinamica, e lo inizializza a 0
OK
*/
int crea_campo_vuoto(matrice* campo, int righe, int colonne){
	int i;
	printf("\n\tALLOCCO CAMPO...");
	*campo = (matrice) malloc(sizeof(cella*)*righe);
	if(! (*campo))
		return 1;
	for(i=0; i<righe; i++){
		(*campo)[i] = (cella*) malloc(sizeof(cella)*colonne);
		if(! ((*campo)[i]))
			return 1; /*errore*/
	}
	printf("\tOK!. ");
	inizializza_campo(campo, righe, colonne);
	return 0; /*eseguito correttamente.*/
}
/**genera campo: permette di generare un campo: inizialmente crea un campo vuoto, poi inserisce le bombe.
				valore di ritorno:	1 se ci sono troppe bombe
									2 se c'è stato un errore di alloccazione della funzione crea campo vuoto
									0 se è stata eseguita correttamente
*/
int genera_campo(matrice* campo, int righe, int colonne, int bombe){
    if(crea_campo_vuoto(campo, righe, colonne)==1)
       return 1; /*errore malloc*/
    if(bombe>=righe*colonne)
            return 2; /*troppe bombe*/
    inserisci_bombe(campo, righe, colonne, bombe);
    return 0;
}
/**stampa campo scoperto: stampa il campo di gioco scoperto (senza controlli sulle celle coperte)
*/
void stampa_campo_scoperto(matrice campo, int righe, int colonne){
	int i, j;
	printf("\n\t");
	for(i=0; i<righe; i++){
		for (j = 0; j < colonne; j++) {
			if (campo[i][j].value == -1)
				printf("%sB%s ", KRED, KNRM);
			else
				printf("%d ", campo[i][j].value);
		}
			printf("\n\t");
	}
}
/**stampa campo: stampo il campo di gioco*/
void stampa_campo(matrice campo, int righe, int colonne){
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

/**scopri cella aux:
	CASO BASE: cella con valore non nullo
	PASSO RICORSIVO: scopro la cella limitrofa alla cella corrente
*/
void scopri_cella_aux(matrice* campo, int righe, int colonne, int rig, int col, int* celle_scoperte){
	int i, j;
	if((*campo)[rig][col].marcata == 0){ /*se non è marcata...*/
		if((*campo)[rig][col].scoperta == 0 ){ /*e se non è stata scoperta...*/
			(*campo)[rig][col].scoperta++;
			/*se non è stata scoperta*/
			(*celle_scoperte)++;
			if((*campo)[rig][col].value==0){ /*se è nulla, allora */
				for(i = rig-1; i<=rig+1; i++)
					for(j = col-1; j<=col+1; j++){
						/*controllo se sono posizioni corrette [DA OTTIMIZZARE]*/
						if(i>=0 && i<righe && j>=0 && j<colonne)
							scopri_cella_aux(campo, righe, colonne, i, j, celle_scoperte);
				}
			}
		}
		else if((*campo)[rig][col].value>0)
			(*campo)[rig][col].scoperta++;
		/*se è già stata scoperta, la 'riscopro'*/
			
	}
}


/**scopri cella: permette di scoprire la cella in posizione x y
	valore di ritorno: 	1 se la cella è marcata
						2 se c'è un gameover
						0 se la cella è stata scoperta correttamente
	Se la cella ha valore 0, vengono scoperte le sue celle adiacenti 'a cascata'
*/
int scopri_cella(matrice* campo, int righe, int colonne, int rig, int col, int* celle_scoperte){
	if((*campo)[rig][col].scoperta!=0){
		return 3; /*3 -> codice già scoperta*/ 
	}
	/*se non è scoperta*/
	else if((*campo)[rig][col].marcata == 1){
		return 1; /*1 -> codice marcata*/
	}
	else if((*campo)[rig][col].value == -1){
		printf("\n\t[GAME OVER] bomba!\n");
		(*campo)[rig][col].scoperta++;
        (*celle_scoperte)++;
		return 2; /*2 -> codice bomba*/
	}
	else
		scopri_cella_aux(campo, righe, colonne, rig, col, celle_scoperte);
	return 0; /*0 -> codice OK */
}

void annulla_mossa(matrice* campo, int righe, int colonne, int rig, int col, int* celle_scoperte){
	int i, j;
	if((*campo)[rig][col].marcata == 0){ /*se non è marcata...*/
		if((*campo)[rig][col].scoperta == 1){
			(*campo)[rig][col].scoperta--;
			/*se non è stata scoperta*/
			(*celle_scoperte)--;
			if((*campo)[rig][col].value==0){ /*se è nulla, allora */
				for(i = rig-1; i<=rig+1; i++)
					for(j = col-1; j<=col+1; j++){
						/*controllo se sono posizioni corrette [DA OTTIMIZZARE]*/
						if(i>=0 && i<righe && j>=0 && j<colonne)
							annulla_mossa(campo, righe, colonne, i, j, celle_scoperte);
				}
			}
		}
		else if((*campo)[rig][col].scoperta>0){
			(*campo)[rig][col].scoperta--;
			if((*campo)[rig][col].scoperta==0)
				(*celle_scoperte)--;
		}
		/*se è già stata scoperta, la 'ricopro'*/
			
	}
}

/**marca cella: marca o smarca una cella
	valore di ritorno: 0 se posso marcarla/smarcarla
						1 se la cella è già stata scoperta
*/
int marca_cella(matrice* campo, int righe, int colonne, int rig, int col){
	if((*campo)[rig][col].scoperta==0){
		/*se non è scoperta*/
		(*campo)[rig][col].marcata= !(*campo)[rig][col].marcata;
		return 0;
	}
	else
		return 1; /*cella già scoperta*/
}