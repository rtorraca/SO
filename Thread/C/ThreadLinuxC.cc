//Um exemplo simples do uso de threads no C.
//Basicamente cria threads passando como argumento: seu id e um tempo de espera que é sorteado por meio de um rand.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <cstdlib>
#include <unistd.h>

struct valor{
		int tempo;
		int id;
		};


void *espera(void *tmp){

	struct valor *v = (struct valor *) tmp;
		sleep(v->tempo);
			printf("Oi eu sou a thread %d esperei %d segundos p executar\n",v->id,v->tempo);
				}

int main(){
 	pthread_t linhas[10];
		int execute,i;
			struct valor *v;
 				srand(time(NULL));

 					for (i=0;i<10;i++){
 						v = (struct valor *)malloc(sizeof(struct valor*));
 							v->tempo = (rand()%10)+2;
 								v->id = i;
 									printf("Criei a thread <%d> com tempo <%d>\n",i,v->tempo);
 										execute = pthread_create(&linhas[i],NULL,espera,(void *)v);
 							}
 		pthread_exit(NULL);
	 }
