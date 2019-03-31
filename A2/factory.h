#ifndef _factory_h
#define _factory_h

int STRLEN;
int DONE;
int BUFF_COUNT;



pthread_mutex_t mutex;
pthread_cond_t can_consume;
pthread_cond_t can_produce;

typedef struct c_buff{
  void *ptr_s;
  void *ptr_e;
  void *head;  
  void *tail;
  
  int item_size;
  int length; 
  int count;  
}c_buff;


c_buff BUFF;

typedef struct a_pack{
  c_buff buffer;
  char *colour;
  int amount;

}a_pack;

typedef struct p_pack{
  c_buff buffer;
  int box_size;
  
}p_pack;

#endif
