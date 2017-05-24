#ifndef LISTCOORD_H
#define LISTCOORD_H

struct elem{
    int riga;
    int colonna;
};
typedef struct elem elem;

struct coordpila{
    elem coord;
    struct coordpila* next;
};
typedef struct coordpila* coordpila;

int inserisci_in_testa(coordpila* pila, int riga, int colonna);
int preleva_in_testa(coordpila* pila, elem* el);
void rimuovi_lista(coordpila* pila);
void stampa_lista(coordpila pila);

#endif