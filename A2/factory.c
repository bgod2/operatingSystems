#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "assemblers.h"
#include "packers.h"


int STRLEN = sizeof("orange");
int DONE = 0;
int BUFF_COUNT = 0;

//To understand pthread locks in a producer consumer setting
//i read the code from:
//https://gist.github.com/Alexis-D/1801206
//so there will be some similarities 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t can_consume = PTHREAD_COND_INITIALIZER;
pthread_cond_t can_produce = PTHREAD_COND_INITIALIZER;


typedef struct c_buff{
  // main idea for the circular buffer and some code was taken from
  //http://stackoverflow.com/questions/827691/how-do-you-implement-a-circular-buffer-in-c
  //refer here on all my code on circular buffers
  void *ptr_s; //pointer to start of buffer
  void *ptr_e; //pointer to start of buffer
  void *head;  
  void *tail;
  
  int item_size;
  int length;  // amount of items that can be placed
  int count;   // amount of item in the buffer

  

}c_buff;

typedef struct a_pack{
  c_buff buffer;
  char *colour;
  int amount;

}a_pack;

typedef struct p_pack{
  c_buff buffer;
  int box_size;
  
}p_pack;


extern c_buff *buff;


int main( int argc, char *argv[]){
  
  if( argc != 6){
    printf("Command line input must be 6:\n./factory (#assembler threads) (#packer threads) (size af assembly line) (#products per box) (#products a packer will make)");
    return 1;
  }
  
  
  int assemblers = atoi(argv[1]);
  int packers    = atoi(argv[2]);
  int buff_size  = atoi(argv[3]);
  int box_size   = atoi(argv[4]);
  int produce    = atoi(argv[5]); // amount an assembler will produce
  
  //colours

  /*
  char *colours[5];
  colours[0] = "orange";
  colours[1] = "blue";
  colours[2] = "red";
  colours[3] = "yellow";
  colours[4] = "green";
  int coloursize = 5;
  */
  
  //create buffer----------------
  

  c_buff *buff = malloc( 1* sizeof(c_buff));
  buff->length = buff_size;
  buff->count = 0;
  buff->item_size = STRLEN + 1;

  buff->ptr_s = malloc( buff_size * STRLEN );
  if(buff->ptr_s == NULL){
    printf("allocation error");
    return -1;
  }
  
  buff->ptr_e = buff->ptr_s + (buff_size * STRLEN); // **
  buff->head = buff->ptr_s;
  buff->tail = buff->ptr_s;

  


 
  //start threads------------------
  
  //ptrs to save threads
  pthread_t *pack_threads;
  pack_threads = malloc( sizeof(pthread_t) * packers);
  int pack_i = 0;
  pthread_t *assemb_threads;
  assemb_threads = malloc( sizeof(pthread_t) * assemblers);
  int assemb_i = 0;

  //temp counters
  int pack = packers;     
  int assemb = assemblers;

  
 
  //  
  
  
  while( ( pack != 0 ) || ( assemb != 0) ){
    
    
    if( assemb != 0 ){
      assemb--;
      a_pack *AP =  malloc(1* sizeof(a_pack));
      AP->buffer = *buff;
      // select colour
      AP->colour = "blue";
      AP->amount = produce;

      // create assembler thread
      pthread_create(&assemb_threads[assemb_i], NULL, assemble_func, AP);
      assemb_i++;
     
    }
    
    if( pack != 0){
      pack--;
      p_pack *PP = malloc(1* sizeof(p_pack));
      PP->buffer = *buff;
      PP->box_size = box_size;
      // create packer thread
      pthread_create(&pack_threads[pack_i], NULL, pack_func , PP);
      pack_i++;
    } 
  }

  for(int i = 0; i < assemblers; i++){
    pthread_join(assemb_threads[i], NULL);
  }
  DONE = 1;
  
  printf("\n\n");
  for(int j = 0; j < packers; j++){
    pthread_join(pack_threads[j], NULL);
  }
  

  exit(0);

} 
