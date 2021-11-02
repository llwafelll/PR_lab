#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include<pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 3333

struct struktura_t { 
  int l_wkf;
  pthread_mutex_t *tab_kuf; 
  int l_kr;
  pthread_mutex_t *tab_kran;  
};

struct struktura_t pub_wsk;
int liczba_kufli;
int liczba_kranow;

pthread_mutex_t mutex_kran;
pthread_mutex_t mutex_kufel;

void * watek_klient (void * arg);

int main( void ){
  pthread_t *tab_klient;
  int *tab_klient_id;

  int l_kl, l_kf, l_kr, i;

  printf("\nLiczba klientow: "); scanf("%d", &l_kl);

  printf("\nLiczba kufli: "); scanf("%d", &l_kf);
  liczba_kufli = l_kf;

  printf("\nLiczba kranow: "); scanf("%d", &l_kr);
  liczba_kranow = l_kr;

  tab_klient = (pthread_t *) malloc(l_kl*sizeof(pthread_t));
  tab_klient_id = (int *) malloc(l_kl*sizeof(int));
  for(i=0;i<l_kl;i++) tab_klient_id[i]=i;

  pub_wsk.tab_kuf = (pthread_mutex_t *) malloc(l_kf*sizeof(pthread_mutex_t));
  for(i=0;i<l_kf;i++) pthread_mutex_init(&pub_wsk.tab_kuf[i], NULL);

  pub_wsk.tab_kran = (pthread_mutex_t *) malloc(l_kr*sizeof(pthread_mutex_t));
  for(i=0;i<l_kr;i++) pthread_mutex_init(&pub_wsk.tab_kran[i], NULL);

  printf("\nOtwieramy pub (simple)!\n");
  printf("\nLiczba wolnych kufli %d\n", l_kf); 

  for(i=0;i<l_kl;i++){
    pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]); 
  }
  for(i=0;i<l_kl;i++){
    pthread_join( tab_klient[i], NULL);
  }
  printf("\nZamykamy pub!\n");
  printf("\nLiczba kufli w uzyciu: %d\n", l_kf - liczba_kufli);
}


void * watek_klient (void * arg_wsk){

  int moj_id = * ((int *)arg_wsk);

  int i, i_wypite, kran, kufel, result;

  int ile_musze_wypic = ILE_MUSZE_WYPIC;

  printf("\nKlient %d, wchodzę do pubu\n", moj_id); 
    
  for(i_wypite=0; i_wypite<ile_musze_wypic; i_wypite++){

    printf("\nKlient %d, wybieram kufel\n", moj_id); 

    // Pobranie kufla
    // pthread_mutex_lock(&mutex_kufel);
    printf("\nKlient %d, wybrałem kufel %d\n", moj_id, kufel = liczba_kufli--);
    // pthread_mutex_unlock(&mutex_kufel);

    printf("\nPozostalo %d wolnych kufli\n", kufel - 1);
    if (kufel - 1 < 0) printf("Blad kufel zostal pobrany pomino ich braku.\n");

    printf("\nKlient %d, wybieram kran\n", moj_id); 

    // Zajecie kranu
    // pthread_mutex_lock(&mutex_kran);
    printf("\nKlient %d, nalewam z kranu %d\n", moj_id, kran = liczba_kranow--); 
    // usleep(100);
    // pthread_mutex_unlock(&mutex_kran);

    if(kran==0)  printf("\nKlient %d, pije piwo Guinness\n", moj_id); 
    else if(kran==1)  printf("\nKlient %d, pije piwo Żywiec\n", moj_id); 
    else if(kran==2)  printf("\nKlient %d, pije piwo Heineken\n", moj_id); 
    else if(kran==3)  printf("\nKlient %d, pije piwo Okocim\n", moj_id); 
    else if(kran==4)  printf("\nKlient %d, pije piwo Karlsberg\n", moj_id); 

    // usleep(100);

    // Zwolnienie kranu
    // pthread_mutex_lock(&mutex_kran);
    liczba_kranow++;
    // pthread_mutex_unlock(&mutex_kran);

    // Oddanie kufla
    // pthread_mutex_lock(&mutex_kufel);
    printf("\nKlient %d, odkladam kufel %d\n", moj_id, liczba_kufli++); 
    // pthread_mutex_unlock(&mutex_kufel);
  }

  return(NULL);
} 
