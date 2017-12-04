#include "lib/listcoord.h"
#include <stdlib.h>
#include <stdio.h>
/*inserisci in testa: inserisce in testa alla lista le coordinate*/
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
/*preleva in testa: preleva dalla testa le coordinate*/
int preleva_in_testa(coordpila* pila, elem* el){
    coordpila del;
    if(!(*pila))
        return 1;

    (*el).riga=(*pila)->coord.riga;
    (*el).colonna=(*pila)->coord.colonna;
    del = (*pila);
    *pila = (*pila)->next; /*setto la testa*/
    free(del); /*libero la memoria dinamica*/
    return 0;
}

void stampa_lista(coordpila pila){
    if(pila){
        printf("%d, %d\n", pila->coord.riga, pila->coord.colonna);
        stampa_lista(pila->next);
    }
}
/*rimuovi lista: rimuovo la lista*/
void rimuovi_lista(coordpila* pila){
    if(*pila){
        rimuovi_lista(&((*pila)->next));
        free(pila);
    }
}