#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include<pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 3333

struct struktura_t { 
  int l_wkf; // wkf stands for wolne kufle?
  pthread_mutex_t *tab_kuf; 
  int l_kr;
  pthread_mutex_t *tab_kran;  
};
struct struktura_t pub_wsk;
int kufle_w_uzyciu = 0;
pthread_mutex_t mutex;


void * watek_klient (void * arg);

// ABY ZEPSUC PROGRAM
// 1. ILE_MUSZE_WYPIC 3 -> 3333
// 2. comment all usleep functions
// 3. 100 clients and 1000 kufli

int main( void ){
  pthread_t *tab_klient;
  int *tab_klient_id;

  int l_kl, l_kf, l_kr, i;

  printf("\nLiczba klientow: "); scanf("%d", &l_kl);

  printf("\nLiczba kufli: "); scanf("%d", &l_kf);
  pub_wsk.l_wkf = l_kf;


  printf("\nLiczba kranow: "); scanf("%d", &l_kr);
  pub_wsk.l_kr = l_kr;

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
  printf("\nLiczba kufli w uzyciu: %d\n", kufle_w_uzyciu);


}


void * watek_klient (void * arg_wsk){

  int moj_id = * ((int *)arg_wsk);

  int i, i_wypite, kran, kufel, result;

  int ile_musze_wypic = ILE_MUSZE_WYPIC;


  kufel = moj_id % pub_wsk.l_wkf;
  kran = moj_id % pub_wsk.l_kr;

  printf("\nKlient %d, wchodzę do pubu\n", moj_id); 
    
  for(i_wypite=0; i_wypite<ile_musze_wypic; i_wypite++){

    printf("\nKlient %d, wybieram kufel\n", moj_id); 

    // ???????????
    pthread_mutex_lock(&pub_wsk.tab_kuf[kufel]);

    printf("\nKlient %d, wybrałem kufel %d\n", moj_id, kufel);
    //pthread_mutex_lock(&mutex);
    kufle_w_uzyciu++;
    //pthread_mutex_unlock(&mutex);
    printf("\nAktualnie w uzyciu jest %d kufli\n", kufle_w_uzyciu);

    printf("\nKlient %d, wybieram kran\n", moj_id); 

    // ????????????
    pthread_mutex_lock(&pub_wsk.tab_kran[kran]);
    
    printf("\nKlient %d, nalewam z kranu %d\n", moj_id, kran); 
    //usleep(100);

    // ????????????
    pthread_mutex_unlock(&pub_wsk.tab_kran[kran]);

    if(kran==0)  printf("\nKlient %d, pije piwo Guinness\n", moj_id); 
    else if(kran==1)  printf("\nKlient %d, pije piwo Żywiec\n", moj_id); 
    else if(kran==2)  printf("\nKlient %d, pije piwo Heineken\n", moj_id); 
    else if(kran==3)  printf("\nKlient %d, pije piwo Okocim\n", moj_id); 
    else if(kran==4)  printf("\nKlient %d, pije piwo Karlsberg\n", moj_id); 
    // itd., itp. 

    //usleep(100);

    // ????????????
    pthread_mutex_unlock(&pub_wsk.tab_kuf[kufel]);

    printf("\nKlient %d, odkladam kufel %d\n", moj_id, kufel); 
    //pthread_mutex_lock(&mutex);
    kufle_w_uzyciu--;
    //pthread_mutex_unlock(&mutex);
  }

  return(NULL);
} 
