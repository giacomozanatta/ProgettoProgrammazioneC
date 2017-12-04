#include <stdio.h>
#include <stdlib.h>
#include "lib/inout.h"
#include "lib/campo.h"
int file_gia_esistente(char* nome_file){
    FILE *f=fopen(nome_file, "r");
    if(f!=NULL){
        fclose(f);
        return 1;
    }
    return 0;
}

int scrivi_su_file(matrice campo, int righe, int colonne, char* nome_file){
    int i, j;
    FILE* f;
    f=fopen(nome_file, "w");
    if(f==NULL)
        return 1;
    printf("\n\tScrivo su file... ");
    fprintf(f, "%d, %d\n", righe, colonne);
    for(i=0; i<righe; i++){
        for(j=0; j<colonne; j++){
            if(campo[i][j].value==-1){
                /*è una bomba, la salvo.*/
                fprintf(f, "%d, %d\n", i, j);
            }
        }
    }
    fclose(f);
    printf("OK!\n\t");
    return 0;
}

/*leggi da file: leggo il campo da un file. vengono salvate le bombe su una lista di coordinate*/
int leggi_da_file(coordpila *lista_bombe, int* righe, int *colonne, int *bombe, char* nome_file){
    FILE *f;
    int x=0, y=0;
    (*bombe) = 0;
    f=fopen(nome_file, "r");
    if(!f)
        return 1;
    fscanf(f, "%d , %d\n", righe, colonne); /*leggo le righe e le colonne*/
    while(!feof(f)){
        fscanf(f, "%d , %d\n" , &x, &y);/*leggo la riga e la colonna*/
        inserisci_in_testa(lista_bombe, x, y); /*inserisco in testa alla lista*/
        (*bombe)++; /*incremento bombe*/
    }
    fclose(f);
    return 0;
}