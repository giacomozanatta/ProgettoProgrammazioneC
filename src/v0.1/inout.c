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

int scrivi_su_file(matrice campo, int righe, int colonne){
    FILE* f;
    char* nome_file;
    size_t len=0;
    int i, j;
    int read=0;
    do{
        do{
            printf("\n\tINSERISCI NOME FILE: ");
            read = getline(&nome_file, &len, stdin);
        }while(len==0);
        if(file_gia_esistente(nome_file)){
            do{
                printf("\n\tEsiste già un file con questo nome. Vuoi sostituirlo?\n\t0 -> SI\n\t1 -> NO");
                scanf("%d", &read);
            }while(read!=0 && read!=1);
        }
    }while(read==1);

    f=fopen(nome_file, "w");
    if(f==NULL)
        return 1;
    printf("\n\tscrivo su file...");
    //scrivo inzialmente righe, colonne
    fprintf(f, "%d, %d\n", righe, colonne);
    for(i=0; i<righe; i++){
        for(j=0; j<colonne; j++){
            if(campo[i][j].value==-1){
                //è una bomba, la salvo.
                fprintf(f, "%d, %d\n", i, j);
            }
        }
    }
    fclose(f);
    return 0;
}

int leggi_da_file(coordpila *lista_bombe, int* righe, int *colonne, int *bombe){
    /*TO DO*/
    FILE *f;
    char* nome_file;
    size_t len=0;
    int x=0, y=0;
    char test[100];
    (*bombe) = 0;
    do{
        do{
            printf("\n\tINSERISCI NOME FILE:");
            getline(&nome_file, &len, stdin);
        }while(len==0);
    }while(!file_gia_esistente(nome_file));
    /*se sono qui, il file esiste. Allora lo leggo*/
    /*si assume che il file sia corretto.*/
    f=fopen(nome_file, "r");
    fscanf(f, "%d,", righe);
    fscanf(f, "%d", colonne);
    while(!feof(f)){
        fscanf(f, "%d,", &x);
        fscanf(f, "%d\n", &y);
        inserisci_in_testa(lista_bombe, x, y);
        (*bombe)++;
    }
    fclose(f);
    return 0;
}