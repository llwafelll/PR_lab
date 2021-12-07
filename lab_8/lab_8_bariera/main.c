#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

extern void bariera_init(int);
extern void bariera(void);

#define LICZBA_W 4
//#define LICZBA_W 44

pthread_t watki[LICZBA_W];

// my vairables
static int bariera_licznik = 0; int *p_bariera_licznik = &bariera_licznik;
static int bariera_stop; int *p_bariera_stop = &bariera_stop;
static pthread_mutex_t bariera_mutex; pthread_mutex_t *p_bariera_mutex = &bariera_mutex;
static pthread_cond_t bariera_cond; pthread_cond_t *p_bariera_cond = &bariera_cond;

void *cokolwiek( void *arg);

int main( int argc, char *argv[] ){

  int i, indeksy[LICZBA_W]; for(i=0;i<LICZBA_W;i++) indeksy[i]=i; 

  // use of pthread_once (once_control, init_routine) ?
  // int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));
  // but init_routine has no arguments...
  bariera_init(LICZBA_W);

  for(i=0; i<LICZBA_W; i++ ) {
    pthread_create( &watki[i], NULL, cokolwiek, (void *) &indeksy[i] );
  }

  for(i=0; i<LICZBA_W; i++ ) pthread_join( watki[i], NULL );

  pthread_exit( NULL);  
}


void *cokolwiek( void *arg){

  int i, moj_id;

  moj_id = *( (int *) arg ); 

  printf("przed bariera 1 - watek %d\n",moj_id);
  // bariera();
  printf("przed bariera 2 - watek %d\n",moj_id);
  // bariera();
  printf("przed bariera 3 - watek %d\n",moj_id);
  // bariera();
  printf("przed bariera 4 - watek %d\n",moj_id);
  // bariera();
  printf("po ostatniej barierze - watek %d\n",moj_id);  
  pthread_exit( (void *)0);
}


void bariera_init(int arg_LICZBA_W) {
  pthread_mutex_init(&bariera_mutex, NULL);
  pthread_cond_init(&bariera_cond, NULL);
  bariera_stop = arg_LICZBA_W;
}

