#include "lib/listcoord.h"
#include <stdlib.h>
#include <stdio.h>

int inserisci_in_testa(coordpila* pila, int riga, int colonna){
    coordpila nuova_testa=(coordpila)malloc(sizeof(struct coordpila));
    if(nuova_testa){
        nuova_testa->coord.riga=riga;
        nuova_testa->coord.colonna=colonna;
        nuova_testa->next=(*pila);
        (*pila)=nuova_testa;
        return 1;
    }
    else
        return 0;
}

int preleva_in_testa(coordpila* pila, elem* el){
    coordpila del;
    if(!(*pila))
        return 1;

    (*el).riga=(*pila)->coord.riga;
    (*el).colonna=(*pila)->coord.colonna;
    del = (*pila);
    *pila = (*pila)->next;
    free(del); /*libero la memoria dinamica*/
    return 0;
}

void stampa_lista(coordpila pila){
    if(pila){
        printf("%d, %d\n", pila->coord.riga, pila->coord.colonna);
        stampa_lista(pila->next);
    }
}

void rimuovi_lista(coordpila* pila){
    if(*pila){
        rimuovi_lista(&((*pila)->next));
        free(pila);
    }
}