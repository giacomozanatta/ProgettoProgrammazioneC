struct matrice_ia{
	int value;
	int prob_bomba;
	int bombe_trovate;
};
typedef struct matrice_ia** matrice_ia;

struct data{
    int bombe_trovate;
    matrice_ia matrice_bombe;
    coordlist* mie_mosse;
    int righe;
    int colonne;
	coordlist* mosse_sicure;
	coordlist* marca_celle;
}

int inizializza_ia_data(data* d, int righe, int colonne){
    int i, j;
    d->bombe_trovate=0;
    d->righe=righe;
    d->colonne=colonne;
    d->matrice_bombe = (int**) malloc(sizeof(int*)*righe);
	if(! d->matrice_bombe))
		return 1;
	for(i=0; i<righe; i++){
		d->matrice_bombe[i] = (int*) malloc(sizeof(int)*colonne);
		if(! (d->matrice_bombe[i]))
			return 1; /*errore*/
	}
    /*inizializzazione*/
    for(i=0; i<righe; i++)
        for(j=0; j<colonne; j++){
            d->matrice_bombe[i][j].value=-2; /*-2 cella non scoperta*/
			d->matrice_bombe[i][j].prob_bomba=0;
			d->matrice_bombe[i][j].bombe_trovate=0;
        }
    return 0;
}
int fai_mossa_casuale(data *d, gioco *game){
    do{
        int rig=rand()%(d->righe);
        int col=rand()%(d->colonne);
    }while(d->matrice_bombe[rig][col].value!=-2);
    /*sblocco cella*/
    fai_mossa_ia(d, rig, col, game);
}
int start_ia(data* iadata, gioco* game){
	inizializza_ia_data(iadata, game->righe, game->colonne);
	while(fai_mossa_ia(iadata, game)==0); /*finchè posso fare mosse...*/

}
int fai_mossa_ia(data* iadata, gioco* game){
	int rig, col;
	int status;
	if(!(iadata->mie_mosse)){
		/*mossa casuale*/
		rig=rand()%iadata->righe;
		col=rand()%iadata->col;
	}
	else{
		el e;
		preleva_in_testa(iadata->mie_mosse, &el);
		rig=e.riga;
		col=e.colonna;
	}
    system("clear");
    stampa_gioco(*game);
    status=scopri_cella_ia(&(game->campo), game->righe, game->colonne, rig, col, &(game->celle_scoperte), ia_data);
    /*inserisci_nuova_mossa(&(game->moves), rig,col);*/
    return status;
}

int bomba_trovata_aggiorno_limitrofi(data* iadata, int rig, int col){ /*chiamata se sono sicuro che in rig col c'è una bomba*/
	for(i = rig-1; i<=rig+1; i++)
		for(j = col-1; j<=col+1; j++){
			/*controllo se sono posizioni corrette [DA OTTIMIZZARE]*/
			if(i>=0 && i<righe && j>=0 && j<colonne)
				if(iadata[i][j].value>0){
					iadata[i][j].value--;
					if(iadata[i][j].value==0){
						/*so per certo che non c'è una bomba.*/
						inserisci_in_testa(iadata->mosse_sicure, i, j); /*inserisco su mosse sicure*/
					}
				}
		}
}

int scopri_cella_ia(matrice* campo, int righe, int colonne, int rig, int col, int celle_scoperte, data* ia_data){
	if((*campo)[rig][col].scoperta!=0){
		return 3; /*3 -> codice già scoperta*/ 
	}
	/*se non è scoperta*/
	else if((*campo)[rig][col].marcata == 1){
		return 1; /*1 -> codice marcata*/
	}
	else if((*campo)[rig][col].value == -1){
		(*campo)[rig][col].scoperta++;
        d->matrice_bombe[rig][col].prob_bomba=8; /*qui c'è sicuramente una bomba*/
		d->matrice_bomba[rig][col].value=8;
		bomba_trovata_aggiorno_limitrofi(iadata, rig, col);
        d->bombe_trovate++;
        (*celle_scoperte)++;
		return 2; /*2 -> codice bomba*/
	}
	else
		scopri_cella_aux_ia(campo, righe, colonne, rig, col, celle_scoperte);
	return 0; /*0 -> codice OK */
}
}


void scopri_cella_aux_ia(matrice* campo, int righe, int colonne, int rig, int col, int* celle_scoperte, data* dataia){
	int i, j;
	if((*campo)[rig][col].marcata == 0){ /*se non è marcata...*/
		if((*campo)[rig][col].scoperta == 0 ){ /*e se non è stata scoperta...*/
            dataia->matrice_bombe[rig][col]=0; /*qui non c'è una bomba*/
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
			else{
				/*se non è nulla, aggiorno limitrofi*/
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