#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "factory.h"

#define BUFF pack->buffer

//extern int STRLEN;
//extern int DONE;






void *pack_func( void *pack_ ){
  printf("packer thread started\n");
  
  p_pack *pack = (p_pack *)pack_;
  int box_size = pack->box_size;
  
  
  char box[box_size][STRLEN + 1];
  int box_count = 0;
  //start taking
  while(1){
    
    
    //start CS *************
    
    pthread_mutex_lock(&mutex);
    if(BUFF_COUNT == 0){
      //for 1 packer
      pthread_cond_wait(&can_consume, &mutex);
    }
    
    
    //take item
    printf("               take item %d\n",BUFF_COUNT - 1 );
    strcpy( box[box_count], BUFF.tail  );//copy colour
    
    BUFF.tail = BUFF.tail + BUFF.item_size;    //increment tail
    if( BUFF.tail == BUFF.ptr_e ){   
      BUFF.tail = BUFF.ptr_s;                   
    }
    BUFF_COUNT--;//decrement count
    box_count++;
    //item taken
    
    
    
    
    

    //end CS ************
    

    
    if(box_count == box_size){
      
//print_box( *box, box_size);
      pid_t pid = getpid();   
      printf("[Packer %lu]: I have a box containing:", (unsigned long)pid);
      for( int i = 0; i < box_size-1; i++){
	printf(" %s,",box[i]);
      }
      box_count = 0;
      printf(" %s\n", box[box_size-1]);
      printf("%d\n",DONE);
      if(DONE == 1){
        printf("no more items\n");
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&can_produce);
        pthread_exit(NULL);
      }
          }
    
    //change for multiple threads
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&can_produce);
    
    
    
  }
//free(p_pack);
  pthread_exit(NULL);

}
