#include <stdio.h>
#include <stdlib.h>
#include "lib/gioco.h"
#include "lib/campo.h"
int menu_1(){
	int scelta = 0;
	do{
		printf("\n\t1. GENERA CAMPO!");
		printf("\n\t2. CARICA DA FILE!");
		scanf("%d", &scelta);
	}while(scelta > 2 && scelta < 1);
	return scelta;
}

int main(){
    gioco game;
    int scelta, scelta1, scelta2;
    int righe, colonne, bombe;
	int x=0;
	//int scrivi_su_file;
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
			new_gioco(&game, righe, colonne, bombe);
			printf("\n\n");
            do{
                printf("\n\tCOSA VUOI FARE ADESSO?");
                printf("\n\t1. SCRIVI CAMPO GENERATO SU FILE");
                printf("\n\t2. GIOCA!");
                printf("\n\t3. ESCI");
				printf("\n");
                scanf("%d", &scelta1);
            }while(scelta1<1 && scelta1>3);
            switch(scelta1){
                case 1:
                    //scrivi_su_file(game->
                break;
				case 2:
					do{
						do{
							stampa_gioco(game);
							printf("\n\t1. FAI MOSSA!");
							printf("\n\t2. ANNULLA MOSSA!");
							printf("\n\t3. MARCA/SMARCA CELLA!");
							printf("\n\t4. ESCI :(");
							printf("\n");
							scanf("%d", &scelta2);
						}while(scelta2<1 && scelta2>3);
						switch(scelta2){
							case 1:
								x=fai_mossa(&game);
							break;
							case 2:
								printf("\n\t[ANNULLA MOSSA] da fare!");
							break;
							case 3:
								marca(&game);
							break;
							default:
								x=-1;
							break;
						}
					}while(x==0);
				break;
				case 3:
				break;
				}
			break;
		case 2:
		
		break;
		default:
		break;
	}
	return 0;

}