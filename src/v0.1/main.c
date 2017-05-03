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
    int scelta, scelta1;
    int righe, colonne, bombe;
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
                scanf("%d", %scelta1);
            }while(scelta1<1 && scelta1>3);
            switch(scelta1){
                case 1:
                    scrivi_su_file(game->
                break;
            }
			stampa_gioco(game);
            stampa_gioco_scoperto(game);
		break;
		case 2:
		
		break;
		default:
		break;
	}
	return 0;

}