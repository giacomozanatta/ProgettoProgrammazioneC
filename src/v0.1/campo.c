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
void salva_e_inserisci_bombe(matrice* campo, int righe, int colonne, int bombe, coordpila* coord_bombe){
	printf("\n\tINSERISCO BOMBA...");
	if(bombe>0){
        int x, y;
		do{
			/*creo due numeri casuali, x e y...*/
			x = rand() % (righe);
			y = rand() % (colonne);
		}while((*campo)[x][y].value==-1);
		/*ho beccato una cella che non contiene una bomba... aggiorno celle limitrofe*/
		(*campo)[x][y].value = -1;
		inserisci_in_testa(coord_bombe, x, y);
		aggiorna_post_inserimento_bomba(campo, righe, colonne, x, y);
		printf("\tDONE.");
		salva_e_inserisci_bombe(campo, righe, colonne, bombe-1, coord_bombe);
	}
}
void inserisci_bombe(matrice* campo, int righe, int colonne, int bombe){
	printf("\n\tINSERISCO BOMBA...");
	if(bombe>0){
        int x, y;
		do{
			/*creo due numeri casuali, x e y...*/
			x = rand() % (righe);
			y = rand() % (colonne);
		}while((*campo)[x][y].value==-1);
		/*ho beccato una cella che non contiene una bomba... aggiorno celle limitrofe*/
		(*campo)[x][y].value = -1;
		aggiorna_post_inserimento_bomba(campo, righe, colonne, x, y);
		printf("\tDONE.");
		inserisci_bombe(campo, righe, colonne, bombe-1);
	}
}

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
    printf("\tDONE.");
}

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
	printf("\tDONE.");
	inizializza_campo(campo, righe, colonne);
	return 0; /*eseguito correttamente.*/
}

int genera_campo(matrice* campo, int righe, int colonne, int bombe){
    if(crea_campo_vuoto(campo, righe, colonne)==1)
       return 2; /*errore malloc*/
    if(bombe>=righe*colonne)
            return 1; /*troppe bombe*/
    inserisci_bombe(campo, righe, colonne, bombe);
    return 0;
}

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

void stampa_campo(matrice campo, int righe, int colonne){
	int i, j;
	printf("\n\t");
	for(i=0; i<righe; i++){
		for (j = 0; j < colonne; j++) {
			if (campo[i][j].scoperta == 1 && campo[i][j].value != -1){ /*è scoperta*/
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


void scopri_cella_aux(matrice* campo, int righe, int colonne, int x, int y, int* celle_scoperte){
	int i, j;
	printf("%d, %d", (*campo)[x][y].scoperta, (*campo)[x][y].marcata);
	if((*campo)[x][y].scoperta==0 && (*campo)[x][y].marcata == 0){ /*se non l'ho già scoperta...*/
		(*campo)[x][y].scoperta=1; /*scopro.*/
        celle_scoperte++;
		if((*campo)[x][y].value==0){ /*se è nulla, allora */
			for(i = x-1; i<=x+1; x++)
				for(j = y-1; j<=y+1; y++){
					printf("\n%d, %d\n", i,j);
						/*controllo se sono posizioni corrette [DA OTTIMIZZARE]*/
						if(i>=0 && i<righe && j>=0 && j<colonne)
							scopri_cella_aux(campo, righe, colonne, i, j, celle_scoperte);
				}
		}
	}
}

int marca_cella(matrice* campo, int righe, int colonne, int rig, int col){
	matrice campoDe = *campo;
	if(campoDe[rig][col].scoperta==0){
		/*se non è scoperta*/
		campoDe[rig][col].marcata= !campoDe[rig][col].marcata;
		return 0;
	}
	else
		return 1; /*cella già scoperta*/
}
int scopri_cella(matrice* campo, int righe, int colonne, int x, int y, int* celle_scoperte){
	if((*campo)[x][y].scoperta==1){
		return 0;
	}
		/*se non è scoperta*/
	else if((*campo)[x][y].marcata == 1){
		return 0;
	}
	else if((*campo)[x][y].value == -1){
		printf("\n\t[GAME OVER] bomba!\n");
		(*campo)[x][y].scoperta=1;
        (*celle_scoperte)++;
        /*DA FARE: GESTIONE LISTA ULTIME MOSSE*/
		return 1;
	}
	else
		scopri_cella_aux(campo, righe, colonne, x, y, celle_scoperte);
		return 0;
}