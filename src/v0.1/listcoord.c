#include "lib/listcoord.h"
#include <stdlib.h>
void inserisci_in_testa(coordpila* pila, int x, int y){
    coordpila nuova_testa=(coordpila)malloc(sizeof(struct coordpila));
    nuova_testa->coord.x=x;
    nuova_testa->coord.y=y;
    nuova_testa->next=(*pila);
    (*pila)=nuova_testa;
}

int preleva_in_testa(coordpila* pila, elem* el){
    if(!(*pila))
        return 1;
    *el=(*pila)->coord;
    coordpila del = *pila;
    *pila = (*pila)->next;
    free(del);
    return 0;
}
