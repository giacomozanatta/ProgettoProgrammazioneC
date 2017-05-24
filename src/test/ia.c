
#include "lib/listcoord.h"
/*L'intelligenza artificiale di campo minato possiede una matrice delle probabilità, di dimensione pari al campo di gioco.
    Inizialmente, ogni cella di questa matrice ha valore (numero_bombe)/numero_celle_coperte.
    Ogni volta che verrà scoperta una cella, questa matrice verrà aggiornata.
    Più alto è il valore di una cella, più probabilità ho di trovare una bomba in quella cella.
    */

struct ia_data{
    float** matrice; /*matrice di probabilità*/
    float p;
    int mosse_fatte;
    int colonne, righe;
    int bombe_rimanenti;
    coordpila* mosse_sicure; /*inserisco le coordinate di una cella di cui sono sicuro non ci sia una bomba*/
    coordpila* da_marcare; /*se sono sicuro che in una cella ci sia una bomba, la metto in questa lista*/

};
typedef struct ia_data ia;

void nuova_ia(ia* dati, int colonne, int righe, int bombe_rimanenti){
    int i=0, j=0;
    float p;
    dati->colonne=colonne;
    dati->righe=righe;
    dati->bombe_rimanenti=bombe_rimanenti;
    dati->mosse_fatte=0;
    dati->matrice=(float**)malloc(sizeof(float*)*righe);
    for(i=0; i<righe; i++)
		(*matrice)[i] = (matrice*) malloc(sizeof(matrice)*colonne);
    /*riempio la matrice con la probabilità iniziale.*/
    p = bombe_rimanenti/(colonne*righe);
    for(i=0; i<righe; i++){
        for(j=0; j<colonne; j++){
            matrice[i][j]=p; /*inserisco p nella matrice*/
        }
    }
}

void aggiorna_probabilita(ia* dati, gioco game){
    int i=0, j=0;
    float p;
    p=bombe_rimanenti/((colonne*righe)-game.celle_scoperte);
    for(i=0; i<dati->righe; i++){
        for(j=0; j<dati->colonne; j++){
            if(game[i][j].scoperta!=0) /*se è stata scoperta... aggiorno probabilità a 0 (non c'è bomba!)*/
                dati->matrice[i][j]=0;
            else{
                dati->matrice[i][j]=p;
            }
        }   
    }
}