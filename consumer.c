#include<stdlib.h>
#include<stdio.h>
#include <pthread.h>
#include<unistd.h>
#include<time.h>



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

  return rand() % 100 + 1;
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

void insertBuffer(int value, int producerNumber){

    buffer[position_insert] = value;
    printf("Thread  %d (value: %d)\n", producerNumber, value);
    position_write++;
    if(position_write == BUFFER_SIZE) {
      position_insert = 0;
    }
    counter++;
}

void* producer(void *arg) {
    while(1){
      int producerNumber = *((int *)arg);  
      pthread_mutex_lock(&block);
      if(counter < BUFFER_SIZE){
        int number = generatePosotiveNumber();
        buffer[counter] = number;
        printf("Thread producer %d (value: %d)\n", producerNumber, number);
        counter++;
      }
      pthread_mutex_unlock(&block);
  }
}


void* consumer(void *arg){

    while(1){
      int producerNumber = *((int *)arg);  
      pthread_mutex_lock(&block);
      if(counter == 0){
        int number = buffer[counter - 1];
        printf("Thread consumer %d (value: %d)\n", producerNumber, number);
        counter--;
      }
    pthread_mutex_unlock(&block);
    }
}

int main(int argc,char *argv[]){

    pthread_t thread_prod, thread_cons;
    int id;

    if (argc != 3) {
        printf("Modo de utilizar: ./consumer <no produtores> <no produtores>");
    }else{
      numProd = atoi(argv[1]);
      numCons = atoi(argv[2]);
      
      if(numCons >= 1 && numProd >= 1){
        
        for(id = 0; id < numProd; id++){
          pthread_create(&thread_prod, NULL, producer, (void *)&id);
        }
        
        for(id = 0; id < numCons; id++){
          pthread_create(&thread_cons, NULL, consumer, (void *)&id);
        }

      }else{
        printf("ONúmero de produtores e consumidores deve ser maior ou igual a 1");
        exit(0);
      }
    }
}
