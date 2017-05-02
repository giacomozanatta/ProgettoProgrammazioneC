/*CAMPO MINATO prova*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define KBLU  "\x1B[34m"
#define KGRN  "\x1B[32m"
#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"
#define YEL   "\x1B[33m"
#define CYN   "\x1B[36m"
struct cella{
	int value;
	int marcata;
	int scoperta;
};
typedef struct cella cella;
typedef cella* riga;
typedef riga* matrice; /*mi capisco meglio matrice == cella**/

/*INIZIALIZZA CAMPO VUOTO -> inizializza una matrice di celle*/
void inizializza_campo_vuoto(matrice* campo, int righe, int colonne){
	matrice campoDe;
	campoDe = *campo;
	printf("\nINIZIALIZZO CAMPO VUOTO!");
	int i, j;
	for(i=0; i<righe; i++){
		for(j=0; j<colonne; j++){
			campoDe[i][j].value = 0;
			campoDe[i][j].marcata = 0;
			campoDe[i][j].scoperta = 0;
		}
	}
}

/*CREA CAMPO VUOTO -> crea un campo con valori a 0 */
int crea_campo_vuoto(cella*** campo, int righe, int colonne){
	int i;
	cella **campoDe;
	printf("CREO CAMPO VUOTO:");
	*campo = (cella**) malloc(sizeof(cella*)*righe);
	campoDe = *campo;
	if(! (*campo))
		return 1;
	for(i=0; i<righe; i++){
		campoDe[i] = (cella*) malloc(sizeof(cella)*colonne);
		if(! (campoDe[i]))
			return 1; /*errore*/
	}
	printf("\nOK.");
	inizializza_campo_vuoto(campo, righe, colonne);
	return 0; /*eseguito correttamente.*/
}

/*AGGIORNA POST INSERIMENTO BOMBA -> dopo aver inserito una bomba in posizione rig col aggiorna le celle limitrofe non bomba*/
void aggiorna_post_inserimento_bomba(matrice* campo, int righe, int colonne,int rig, int col){
	/*controllo adiacenti*/
	matrice campoDe = *campo;
	int i, j;
	for(i = rig-1; i<=rig+1; i++)
		for(j = col-1; j<=col+1; j++){
				/*controllo se sono posizioni corrette [DA OTTIMIZZARE]*/
				if(i>=0 && i<righe && j>=0 && j<colonne)
					if(campoDe[i][j].value!=-1)
						campoDe[i][j].value++;
		}
}
/*INSERISCI BOMBE -> inserisce N BOMBE in modo casuale nel campo*/
void inserisci_bombe(matrice* campo, int righe, int colonne, int bombe){
	int x, y;
	matrice campoDe = *campo;
	if(bombe>0){
		do{
			/*creo due numeri casuali, x e y...*/
			x = rand() % (righe);
			y = rand() % (colonne);
		}while(campoDe[x][y].value==-1);
		/*ho beccato una cella che non contiene una bomba... aggiorno celle limitrofe*/
		campoDe[x][y].value = -1;
		aggiorna_post_inserimento_bomba(campo, righe, colonne, x, y);
		inserisci_bombe(campo, righe, colonne, bombe-1);
	}
}
/*GENERA -> genera un nuovo campo di gioco*/	
int genera( matrice* campo, int righe, int colonne, int n_bombe){
	if(n_bombe>=righe*colonne)
		return 1; /*non posso creare un campo di gioco con così tante bombe!*/
	if(crea_campo_vuoto(campo, righe, colonne)==1)
		return 1;
	//printa_campo(*campo, righe, colonne);
	printf("\n\tInserisco bombe...");
	printf("\n\tAggiorno valore celle...");
	inserisci_bombe(campo, righe, colonne, n_bombe);
	return 0;
}

int menu_1(){
	int scelta = 0;
	do{
		printf("\n\t1. GENERA CAMPO!");
		printf("\n\t2. CARICA DA FILE!");
		scanf("%d", &scelta);
	}while(scelta > 2 && scelta < 1);
	return scelta;
}
void printa_campo(matrice campo, int righe, int colonne){
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

void scopri_cella_aux(matrice* campo, int righe, int colonne, int rig, int col){
	int i, j;
	matrice campoDe = *campo;
	printf("%d, %d", campoDe[rig][col].scoperta, campoDe[rig][col].marcata);
	if(campoDe[rig][col].scoperta==0 && campoDe[rig][col].marcata == 0){ /*se non l'ho già scoperta...*/
		campoDe[rig][col].scoperta=1; /*scopro.*/
		if(campoDe[rig][col].value==0){ /*se è nulla, allora */
			for(i = rig-1; i<=rig+1; i++)
				for(j = col-1; j<=col+1; j++){
					printf("\n%d, %d\n", i,j);
						/*controllo se sono posizioni corrette [DA OTTIMIZZARE]*/
						if(i>=0 && i<righe && j>=0 && j<colonne)
							scopri_cella_aux(campo, righe, colonne, i, j);
				}
		}
	}
}


int scopri_cella(matrice* campo, int righe, int colonne, int rig, int col){
	matrice campoDe = *campo;
	if(campoDe[rig][col].scoperta==1){
		printf("\n\t[ERR]Già scoperta.\n");
		return 0;
	}
		/*se non è scoperta*/
	else if(campoDe[rig][col].marcata == 1){
		printf("\n\t[ERR]Cella marcata.\n");
		return 0;
	}
	else if(campoDe[rig][col].value == -1){
		printf("\n\t[GAME OVER] bomba!\n");
		campoDe[rig][col].scoperta=1;
		return 1;
	}
	else
		scopri_cella_aux(campo, righe, colonne, rig, col);
		return 0;
}
/*void scopri_cella(matrice* campo, int righe, int colonne, int rig, int col){
	matrice campoDe = *campo;
	if(campoDe[rig][col].scoperta==1)
		printf("\n\t[ERR]Già scoperta.\n");
	else if(campoDe[rig][col].marcata == 1)
		printf("\n\t[ERR]Cella marcata.\n");
	else
		campoDe[rig][col].scoperta=1;
}*/

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


/*MAIN!*/
int main(){
	srand(time(NULL));
	int scelta;
	int righe, colonne, bombe;
	int game_over=0;
	int scelta_1;
	int x,y;
	int returned;
	matrice campo = NULL; /*rappresenta il mio campo di gioco*/
	scelta = menu_1();
	switch(scelta){
		case 1:
			printf("\n\t GENERA CAMPO!");
			printf("\n\t INSERISCI LARGHEZZA: ");
			scanf("%d", &righe);
			printf("\n\tINSERISCI ALTEZZA: ");
			scanf("%d", &colonne);
			printf("\n\tINSERISCI BOMBE: ");
			scanf("%d", &bombe);
			printf("\n\tGENERO CAMPO....");
			genera(&campo, righe, colonne, bombe);
			printf("\n\n");
			stampa_campo(campo, righe, colonne);
			do{
					printf("\n\t1.SCOPRI CELLA!");
					printf("\n\t2. MARCA/SMARCA CELLA!");
					printf("\n\t3. ESCI!");
					scanf("%d", &scelta_1);
					switch(scelta_1){
						case 1:
							printf("\n\tX ->");
							scanf("%d", &x);
							printf("\n\tY ->");
							scanf("%d", &y);
							game_over = scopri_cella(&campo, righe, colonne, x,y);
						break;
						case 2:
							printf("\n\tX ->");
							scanf("%d", &x);
							printf("\n\tY ->");
							scanf("%d", &y);
							returned = marca_cella(&campo, righe, colonne, x,y);
						break;
						case 3:
							game_over=2;
						break;
						default:
						break;
					}
					stampa_campo(campo, righe, colonne);
			}while(game_over==0);
			printf("\n\tGAME OVER!!!");
		break;
		case 2:
		
		break;
		default:
		break;
	}
	return 0;
}
