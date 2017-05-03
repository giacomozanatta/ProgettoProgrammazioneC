#include "lib/listcoord.h"
#include <stdlib.h>
void inserisci_in_testa(coordpila* pila, int x, int y){
    coordpila nuova_testa=(coordpila)malloc(sizeof(struct coordpila));
    coordpila->elem.x=x;
    coordpila->elem.y=y;
    coordpila->next=(*pila);
    (*pila)=nuova_testa;
}

elem preleva_in_testa(coordpila* pila){
    elem estratto = null;
    if(*pila){
        estratto = pila->elem;
        coordpila* del = pila;
        *pila = (*pila)->next;
        free(del);
    }
    return estratto;
}
