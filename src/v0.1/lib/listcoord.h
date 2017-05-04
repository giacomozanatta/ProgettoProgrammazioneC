struct elem{
    int x;
    int y;
};
typedef struct elem elem;

struct coordpila{
    elem coord;
    struct coordpila* next;
};
typedef struct coordpila* coordpila;

void inserisci_in_testa(coordpila* pila, int x, int y);

int preleva_in_testa(coordpila* pila, elem* el);

