#include "lib/gioco.h"
#include <stdlib.h>
#include <time.h>
int main(){
	srand(time(NULL)); /*setto l'srand per i numeri casuali*/
	gioca(); /*gioco*/
	return 0;
}