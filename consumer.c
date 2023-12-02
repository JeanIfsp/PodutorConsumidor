#include<stdlib.h>
#include<stdio.h>
#include <pthread.h>
#include<unistd.h>
#include<time.h>
#include<semaphore.h>


#define BUFFER_SIZE 5

pthread_cond_t free_espace, full_space;
pthread_mutex_t block;

int buffer[BUFFER_SIZE];
int position_read = 0;
int position_write = 0;
int position_insert = 0;
int counter = 0;

int numCons;
int numProd;

void* producer(void *arg);
void* consumer(void *arg);

int generatePosotiveNumber(){

  srand(time(NULL));
  int number = (rand() % 100) + 1;
  printf("number gerado: %d\n", number); 
  return number;

}

int readBuffer(){

  int item = buffer[position_read];
  position_read++;

  if(position_read == BUFFER_SIZE){
    position_read = 0;
  }
  counter--;
  return item;

}

void insertBuffer(int item){

    buffer[position_insert] = item;
    position_write++;
    if(position_write == BUFFER_SIZE) {
      position_insert = 0;
    }
    counter++;
}

void* producer(void *arg) {

    for(int i = 0; i < numProd; i++){
      pthread_mutex_lock(&block);
      while(counter == BUFFER_SIZE)
        pthread_cond_wait(&free_espace, &block);
      insertBuffer(generatePosotiveNumber());
      pthread_cond_signal(&full_space);
      pthread_mutex_unlock(&block);
    }
    return NULL;
}

void* consumer(void* param){

   for(int i = 0; i < numCons; i++){
      pthread_mutex_lock(&block);
      while(counter == 0)
        pthread_cond_wait(&full_space, &block);
      int item = readBuffer();
      pthread_cond_signal(&free_espace);
      pthread_mutex_unlock(&block);
      printf("Valor lido: %d\n", item);
    }
    return NULL;
}

int main(int argc,char *argv[]){

    pthread_t thread_prod, thread_cons;

    if (argc != 3) {
        printf("Modo de utilizar: ./consumer <no produtores> <no produtores>");
    }else{
      numProd = atoi(argv[1]);
      numCons = atoi(argv[2]);
      
      if(numCons >= 1 && numProd >= 1){
        pthread_create(&thread_prod, NULL, producer, NULL);
        pthread_create(&thread_cons, NULL, consumer, NULL);
      }else{
        printf("ONúmero de produtores e consumidores deve ser maior ou igual a 1");
      exit(0);
      }
    }
}