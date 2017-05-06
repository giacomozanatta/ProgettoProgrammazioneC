#include "lib/listcoord.h"
#include <stdlib.h>
#include <stdio.h>
int inserisci_in_testa(coordpila* pila, int x, int y){
    coordpila nuova_testa=(coordpila)malloc(sizeof(struct coordpila));
    if(nuova_testa){
        nuova_testa->coord.x=x;
        nuova_testa->coord.y=y;
        nuova_testa->next=(*pila);
        (*pila)=nuova_testa;
        return 1;
    }
    else
        return 0;
}
void stampa_lista(coordpila pila){
    if(pila){
        printf("%d, %d\n", pila->coord.x, pila->coord.y);
        stampa_lista(pila->next);
    }
}
int preleva_in_testa(coordpila* pila, elem* el){
    if(!(*pila))
        return 1;
    (*el).x=(*pila)->coord.x;
    (*el).y=(*pila)->coord.y;
    *pila = (*pila)->next;
    return 0;
}
