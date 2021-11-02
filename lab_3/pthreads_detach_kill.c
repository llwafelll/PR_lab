#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

int zmienna_wspolna=0;

#define WYMIAR 1000
#define ROZMIAR WYMIAR*WYMIAR
double a[ROZMIAR],b[ROZMIAR],c[ROZMIAR];


double czasozajmowacz(){
  int i, j, k;
  int n=WYMIAR;
  for(i=0;i<ROZMIAR;i++) a[i]=1.0*i;
  for(i=0;i<ROZMIAR;i++) b[i]=1.0*(ROZMIAR-i);
  for(i=0;i<n;i++){
    for(j=0;j<n;j++){
      c[i+n*j]=0.0;
      for(k=0;k<n;k++){
	c[i+n*j] += a[i+n*k]*b[k+n*j];
      }
    }
  }
  return(c[ROZMIAR-1]);
}

void * zadanie_watku (void * arg_wsk)
{
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	printf("\twatek potomny: uniemozliwione zabicie\n");

	czasozajmowacz();

	printf("\twatek potomny: umozliwienie zabicia\n");
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

	pthread_testcancel();

	zmienna_wspolna++;
	printf("\twatek potomny: zmiana wartosci zmiennej wspolnej\n");

	return(NULL);
}

int main()
{
	pthread_t tid;
	pthread_attr_t attr;
	void *wynik;
	int i;


	// >>> Wątek przyłączalny
	
	printf("watek glowny: tworzenie watku potomnego nr 1\n");

  // Tworzenie watku z domyslnymi wartosciami [#m01]
  pthread_create(&tid, NULL, zadanie_watku, NULL);

	sleep(2); // czas na uruchomienie watku

	printf("\twatek glowny: wyslanie sygnalu zabicia watku\n");
	pthread_cancel(tid);

  // pthread_join powoduje oczekiwanie na zakonczenie watku.
  // W tym przypadku uruchomienie tej funkcji powoduje
  // zwolnienie wszystkich zasobow zakonczonego watku [#m02]
  pthread_join(tid, &wynik);

  // Dodatkowo urzycie funkcji pthread_cancel powoduje przypisanie
  // odpowiedniej wartosci do zmiennej wynik przez pthread_join
	if (wynik == PTHREAD_CANCELED)
		printf("\twatek glowny: watek potomny zostal zabity\n");
	else
		printf("\twatek glowny: watek potomny NIE zostal zabity - blad\n");


	// >>> Odłączanie wątku

	zmienna_wspolna = 0;

	printf("watek glowny: tworzenie watku potomnego nr 2\n");

  // Tworzenie watku z domyslnymi wartosciami [#m03]
  pthread_create(&tid, NULL, zadanie_watku, NULL);

	sleep(2); // czas na uruchomienie watku

	printf("\twatek glowny: odlaczenie watku potomnego\n");

  // Odlaczenie watku potomnego [#m04]
  pthread_detach(tid);

	printf("\twatek glowny: wyslanie sygnalu zabicia watku odlaczonego\n");
	pthread_cancel(tid);

  // Sprawdzenie czy watek zostal zabity [#m05]
  for (i = 0; i < 10; ++i) {
    sleep(1);
    if (zmienna_wspolna != 0) break;
  }

  if (zmienna_wspolna == 0)
    printf("\twatek glowny: odlaczony watek potomny PRAWDOPODOBNIE zostal zabity\n");
  else
    printf("\twatek glowny: odlaczony watek potomny PRAWDOPODOBNIE NIE zostal zabity\n");
	

	// >>> Wątek odłączony
	
	// Inicjacja atrybutów [#m06]
  pthread_attr_init(&attr);

	// Ustawianie typu watku na odłaczony [#m07]
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	printf("watek glowny: tworzenie odlaczonego watku potomnego nr 3\n");
	pthread_create(&tid, &attr, zadanie_watku, NULL);

	//Niszczenie atrybutów [#m08]
  pthread_attr_destroy(&attr);

	printf("\twatek glowny: koniec pracy, watek odlaczony pracuje dalej\n");
	pthread_exit(NULL);

  // exit(0) albo return 0 - powoduje zakonczenie procesu = zamkniecie wszystkich watkow
  // pthread_exit(NULL) - zakonczenie tylko watku z ktorego zostaje wywolana ta funkcja
}


