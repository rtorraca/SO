#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5
#define ESQUERDA (i+N-1)%N //Para Cálculo dos vizinhos
#define DIREITA (i+1)%N  //Para Cálculo dos vizinhos
#define PENSANDO 0
#define FOME 1
#define COMENDO  2

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//------------Global--------------
int estado[N];// vetor de estados. um estado para cada filósofo
int i, r;
char* filosofos[] = {"Karl_Marx","Friedrich_Engels","Charles_Darwin","Adam_Smith","Thomas_Piketty"};	//Todos os filósofos 


//-------Prototipação-------------
void pensar(int i);
void pegar_garfo(int i);
void comer(int i);
void devolver_garfo(int i);
void verifica_vizinhos(int i);

//---------------semaforos---------
sem_t mutex;//definição de um objeto de semaforo binário, que vai ser a região crítica
sem_t sem_filosofo[N];// vetor de semáforo um sem para cada filósofo.
 

//funcao que mostra o estado dos N filosofos
void mostrar(){
   for(i=1; i<=N; i++){
       if(estado[i-1] == PENSANDO){//0
          printf(ANSI_COLOR_RED "O filosofo [%s] está Pensando." ANSI_COLOR_RESET" ",filosofos[i-1]);
	  }
       if(estado[i-1] == FOME){//1
          printf(ANSI_COLOR_YELLOW "O filosofo [%s] está com Fome." ANSI_COLOR_RESET" ",filosofos[i-1]);
       }
       if(estado[i-1] == COMENDO){//2
           printf(ANSI_COLOR_GREEN "O filosofo [%s] está comendo." ANSI_COLOR_RESET" ",filosofos[i-1]);
	 }
       printf(ANSI_COLOR_CYAN "Array de estado [%d] do filosofo [%s]." ANSI_COLOR_RESET"\n",estado[i-1],filosofos[i-1]);

	
   }
   printf("\n");
}
 
//acao do filosofo   
void *acao_filosofo(void *parametro){
   
	int i= *(int*) parametro;//filósofo

   printf("\n Filosofos para atendimento [%s]\n",filosofos[i]);

   for(;;){  /* repete eternamente */
 
    pensar(i);/* o filosofo esta pensando */
    pegar_garfo(i);/* pega dois garfos ou bloqueia passa indice do filósofo*/
    comer(i);/* come espaguete passa indice do filósofo*/
    devolver_garfo(i);/* coloca os dois garfos de volta na mesa, passa indice do filosofo */
   }
    system("clear");
}


//espera um tempo aleatoria pensando ou comendo
void pensar(int i){
 
	r= (rand() % 10 + 1);
   	sleep(r);
		}
 

void pegar_garfo(int i){
   sem_wait(&mutex);//Função para (decrementar/aguardar) down(&mutex) entra na região crítica
   estado[i]=FOME;//Coloca estado do estado[filosofo[i]]=2
   mostrar();
   verifica_vizinhos(i);//tenta pegar dois garfos, seria verificar se os visinhos não estão comendo, na primeira nunca estarão só você ficará com fome
   sem_post(&mutex);//Função para (incrementar/liberar), up(&mutex) sai da regiao cri­tica
   sem_wait(&sem_filosofo[i]);//Função para (decrementar/aguardar), down(&sem_fil[i]) bloqueia se os garfos 
}

 

//testa se os filosofos vizinhos podem comer
void verifica_vizinhos(int i){
   if(estado[i] == FOME && estado[ESQUERDA] != COMENDO && estado[DIREITA] != COMENDO){//Se nenhum visinho estiver comendo
       printf(ANSI_COLOR_BLUE "\n[%s] deve verificar se o vizinho da esquerda [%s] e o vizinho da direita [%s] não estão com FOME/COMENDO." ANSI_COLOR_RESET"\n",filosofos[i],filosofos[ESQUERDA],filosofos[DIREITA]);
       estado[i]=COMENDO;// o filosofo vai comer estado[filosofo[i]]=2 
       mostrar();
       sem_post(&sem_filosofo[i]);//Função para (incrementar/liberar), up(&sem_filsofo[i]), desbloqueia os garfos
   } 
}


void comer(int i){
  r= (rand() % 10 + 1);//Tempo que vai ficar comendo é variável
  sleep(r);
}

void devolver_garfo(int i){
   sem_wait(&mutex);//Função para (decrementar/aguardar)
   estado[i]=PENSANDO;// o filosofo vai pensar estado[filosofo[i]]=0
   mostrar();

   //Se uma destas condições for verdadeira então quem está estiver com fome e não está comendo vai começar à comer		
   verifica_vizinhos(ESQUERDA);//ver se vizinho está comendo, para largar o garfo da esquerda
   verifica_vizinhos(DIREITA);//ver se vizinho está comendo, para largar o garfo da direita

   sem_post(&mutex);//Função para (incrementar/liberar)
}
 

int main(void){
 
    for(i=0;i<N; i++) estado[i]=0;//vai colocar todos inicialmente para pensar

    mostrar();//Chama e mostra para os estados em zero, todos pensando
 
    int codigo_de_erro;
 
    pthread_t thread[N];
 
    void *thread_result;// thread resultado para ser usada em pthread_join
 
    //inicialização dos semáforos...    
    codigo_de_erro = sem_init(&mutex,0,1);//inicia o objeto no semáforo

    if(codigo_de_erro!=0){
       perror("Erro na inicialização do semaforo!");
       exit(EXIT_FAILURE);
    }
 
    for(i=0; i<N; i++){
       codigo_de_erro = sem_init(&sem_filosofo[i],0,0);//Para iniciar os vários objetos com semáforo
       if(codigo_de_erro!=0){
          perror("Erro na inicialização do semaforo!");
          exit(EXIT_FAILURE);
       }
    }
 
//cria as threads(filosofos)
 
    for(i=0; i<N; i++){
       codigo_de_erro = pthread_create(&thread[i],NULL,acao_filosofo,&i);//Uma Thread por filósofo
       if(codigo_de_erro!=0){
          perror("Erro na inicialização da thread!");
          exit(EXIT_FAILURE);
       }
    }
 
//faz um join nas threads
 
        for(i=0; i<N; i++){
       codigo_de_erro = pthread_join(thread[i],&thread_result);
       if(codigo_de_erro!=0){
          perror("Erro ao fazer join nas threads!");
          exit(EXIT_FAILURE);
       }
    }
}
