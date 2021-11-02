#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include<pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 3333

void * watek_klient (void * arg);

pthread_mutex_t mutex;
pthread_mutex_t mutex_inc;
pthread_mutex_t mutex_dec;

int kufle_w_uzyciu = 0;

int main(char argc, char *argv[]) {

  pthread_t *tab_klient;
  int *tab_klient_id;

  int l_kl, l_kf, l_kr, i;

  // Przypisanie wartosci do zmiennych na postawie parametrow
  //l_kl = (int)strtol(argv[1], (char**)NULL, 10);
  //l_kf = (int)strtol(argv[2], (char**)NULL, 10);

  printf("\nLiczba klientow: "); scanf("%d", &l_kl);

  printf("\nLiczba kufli: "); scanf("%d", &l_kf);

  //printf("\nLiczba kranow: "); scanf("%d", &l_kr);
  l_kr = 1;

  tab_klient = (pthread_t *) malloc(l_kl*sizeof(pthread_t));
  tab_klient_id = (int *) malloc(l_kl*sizeof(int));
  for(i = 0; i < l_kl; ++i) tab_klient_id[i] = i;


  printf("\nOtwieramy pub (simple)!\n");
  printf("\nLiczba wolnych kufli %d\n", l_kf); 

  for(i = 0; i < l_kl; ++i){
    pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]); 
  }

  for(i = 0; i < l_kl; ++i){
    pthread_join( tab_klient[i], NULL);
  }
  printf("\nZamykamy pub!\n");
  printf("kufle w uzyciu na konice dnia %d\n", kufle_w_uzyciu);
}

void * watek_klient (void * arg_wsk){

  int moj_id = * ((int *)arg_wsk);

  int i, j, kufel, result;
  int ile_musze_wypic = ILE_MUSZE_WYPIC;

  printf("\nKlient %d, wchodzę do pubu\n", moj_id); 
    
  for(i = 0; i < ile_musze_wypic; ++i) {

    printf("\nKlient %d, wybieram kufel\n", moj_id); 
    pthread_mutex_lock(&mutex_inc);
    //pthread_mutex_lock(&mutex);
    kufle_w_uzyciu = kufle_w_uzyciu + 1; // [#m01]
    //pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex_dec);
    
    j=0;
    printf("\nKlient %d, nalewam z kranu %d\n", moj_id, j); 
    // usleep(3);
    
    printf("\nKlient %d, pije\n", moj_id); 
    // nanosleep((struct timespec[]){{0, 50000000L}}, NULL);
    
    printf("\nKlient %d, odkladam kufel\n", moj_id);
    pthread_mutex_lock(&mutex_dec);
    //pthread_mutex_lock(&mutex);
    //kufle_w_uzyciu--; // [#m02]
    kufle_w_uzyciu = kufle_w_uzyciu - 1; // [#m02]
    //pthread_mutex_unlock(&mutex);
    //pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex_dec);
    
  }

  printf("\nKlient %d, wychodzę z pubu\n", moj_id); 
    
  return(NULL);
} 


