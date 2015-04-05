//Autor: Robson Torraca
//compilando 
//g++ -Wall -o Fibonacci Fibonacci.cc -lpthread


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define TAMANHO 5 //valor para sequência

int fibo[TAMANHO];

void *thread_nova(void* parametro){

	int n = *((int*)parametro);

		//* Corpo da thread que fibo
		
		if (n < 2) fibo[n] = n;

			else fibo[n] = fibo[n - 1] + fibo[n - 2]; 

					}



int main (int argc, char *argv[]){

	printf("Início da thread main \n Para cálculo da sequência do número: %d definida na variável TAMANHO.\n",TAMANHO);

		pthread_t thread_fibo[TAMANHO];

	/*
	* Variáveis declaradas com pthread_t representam a identificação única de uma thread que será criada.
	*/
	

	for(int i = 0 ; i <= TAMANHO ; i++)  
	{
		int codigo_de_erro;

		codigo_de_erro = pthread_create(&thread_fibo[TAMANHO], NULL,thread_nova,(void*)&i);

		if (codigo_de_erro != 0){
		
			printf("ERRO na criação de uma nova thread. Código do erro: %d\n", codigo_de_erro);
			exit(1);
	
					}

				pthread_join(thread_fibo[TAMANHO], NULL);

	}

	
		printf("Fibonacci: ");

		for(int i = 0 ; i <= TAMANHO ; i++)	printf("%d ", fibo[i]);
	
			printf("\n Fim da thread main\n");
		
				pthread_exit(NULL);
			
					return 0;



}
