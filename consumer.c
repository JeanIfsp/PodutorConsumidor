#include<stdlib.h>
#include<stdio.h>
#include <pthread.h>
#include<unistd.h>
#include<time.h>
#define BUFFER_SIZE 5

pthread_cond_t free_espace, full_space;
pthread_mutex_t block;

int buffer[BUFFER_SIZE];
int counter = 0;

int numCons;
int numProd;

void* producer(void *arg);
void* consumer(void *arg);

int generatePosotiveNumber(){

  return rand() % 100 + 1;
}

void* producer(void *arg) {
    while(1){
      int producerNumber = *((int *)arg);  
      pthread_mutex_lock(&block);
      if(counter < BUFFER_SIZE){
        int number = generatePosotiveNumber();
        buffer[counter] = number;
        printf("Thread %d produziu valor %d\n", producerNumber, number);
        counter++;
      }
      pthread_mutex_unlock(&block);
  }
}

void* consumer(void *arg){

    while(1){
      int producerNumber = *((int *)arg);  
      pthread_mutex_lock(&block);
      if(counter == BUFFER_SIZE){
        int number = buffer[counter - 1];
        printf("Thread %d consumiu valor %d\n", producerNumber, number);
        counter--;
      }
    pthread_mutex_unlock(&block);
    }
}

int main(int argc,char *argv[]){

    pthread_t thread[2];
    int id, success;

    if (argc != 3) {
        printf("Modo de utilizar: ./consumer <no produtores> <no produtores>");
    }
    else{
      
      numProd = atoi(argv[1]);
      numCons = atoi(argv[2]);
      
      if(numCons >= 1 && numProd >= 1){
        
        for(id = 0; id < numProd; id++){
           success = pthread_create(&thread[id], NULL, producer, (void *)&id);
        }
        
        for(id = 0; id < numCons; id++){
           success = pthread_create(&thread[id], NULL, consumer, (void *)&id);
        }

      }else{
        printf("ONúmero de produtores e consumidores deve ser maior ou igual a 1");
        exit(0);
      }
    }
    pthread_exit(NULL);
}
