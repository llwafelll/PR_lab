#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

extern int *p_bariera_licznik;
extern int *p_bariera_stop;
extern pthread_mutex_t *p_bariera_mutex;
extern pthread_cond_t *p_bariera_cond;

void bariera(void) {
  pthread_mutex_lock(p_bariera_mutex);
  ++(*p_bariera_licznik);

  if (*p_bariera_licznik < *p_bariera_stop) {
    pthread_cond_wait(p_bariera_cond, p_bariera_mutex);
  } else {
    (*p_bariera_licznik) = 0;
    pthread_cond_broadcast(p_bariera_cond);
  }

  pthread_mutex_unlock(p_bariera_mutex);
}

