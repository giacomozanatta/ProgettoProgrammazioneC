campominato: main.o gioco.o inout.o campo.o mosse.o listcoord.o
	gcc -std=gnu89 -Wall -pedantic campo.o gioco.o listcoord.o mosse.o main.o inout.o -o campominato

campo.o: campo.c
	gcc -g -std=gnu89 -Wall -pedantic -c campo.c -o campo.o
gioco.o: gioco.c
	gcc -g -std=gnu89 -Wall -pedantic -c gioco.c -o gioco.o
listcoord.o: campo.c
	gcc -g -std=gnu89 -Wall -pedantic -c listcoord.c -o listcoord.o
mosse.o: mosse.c
	gcc -g -std=gnu89 -Wall -pedantic -c mosse.c -o mosse.o
main.o: main.c
	gcc -g -std=gnu89 -Wall -pedantic -c main.c -o main.o
inout.o: inout.c
	gcc -g -std=gnu89 -Wall -pedantic -c inout.c -o inout.o
