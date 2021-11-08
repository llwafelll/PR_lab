#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include"pomiar_czasu.h"

double funkcja ( double x );
double funkcja ( double x ){ return( sin(x) ); }
double calka_sekw(double a, double b, double dx);
double calka_zrownoleglenie_petli(double a, double b, double dx);
double calka_dekompozycja_obszaru(double a, double b, double dx);
int save_result_to_file(FILE *fd, const char filename[],
                        const char label[], double calka, double time);

#define LICZBA_W_MAX 1000
int l_w_global=0;
pthread_mutex_t muteks;
char typ_dekompozycji = 'b'; // b - blokowa, c - cykliczna

FILE *fd;
char filename[] = "data";

static double calka_global=0.0;
static double a_global;
static double b_global;
static double dx_global;
static int N_global;

int main( int argc, char *argv[] ){

  int i; 
  double t1,t2,t3;
  double a, b, dx, calka;

  printf("\nProgram obliczania całki z funkcji (sinus) metodą trapezów.\n");
  
  // Inicjalizacja przedzialow calkowania (jesli nie zostaly dostarczone jako 
  // parametr zainicjalizowane zostana wartosciami domyslnymi)
  if ((argc >= 4) && (argc <= 5)) {
    a = strtod(argv[3], NULL);
    b = strtod(argv[4], NULL);
  } else {
    a = 0.0;
    b = M_PI;
  }

  // Inicjalizacja dx (wyskokosc trapezu)
  dx = strtod(argv[1], NULL);

  // Inicjalizacja l_w_global przewowujacej liczbe uzytych watkow
  sscanf(argv[2], "%d", &l_w_global);

  // Wypisanie naglowka informacyjnego do pliku z pobranymi danymi
  fd = fopen(filename, "a");

  if (fd == NULL) {
    printf("Blad podczas zapisywania do pliku\n");
  } else {
    fprintf(fd, "\n=== a=%f, b=%f, h_trapez=%f, l_watki=%d ===\n",
        a, b, dx, l_w_global);
    printf("Wynik zapisoano do pliku '%s'.\n", filename);
  }

  fclose(fd);


  // SCENARIO 1
  printf("Poczatek obliczeń sekwencyjnych\n");
  t1 = czas_zegara();
  calka = calka_sekw(a, b, dx);
  t1 = czas_zegara() - t1;
  printf("Koniec obliczen sekwencyjnych\n");
  printf("\tCzas wykonania %lf. \tObliczona całka = %.15lf\n", t1, calka);
  save_result_to_file(fd, filename, "sekwencyjnie", calka, t1);

  // SCENARIO 2
  // Dekompozycja blokowa
  printf("Poczatek obliczeń równoległych (zrównoleglenie pętli)\n");
  t1 = czas_zegara();
  calka = calka_zrownoleglenie_petli(a, b, dx);
  t1 = czas_zegara() - t1;
  printf("Koniec obliczen  równoległych (zrównoleglenie pętli) \n");
  printf("\tCzas wykonania %lf. \tObliczona całka = %.15lf\n", t1, calka);
  save_result_to_file(fd, filename, "zrownolegl_blok", calka, t1);


  // Dekompozycja cykliczna
  typ_dekompozycji = 'c';
  calka_global = 0.0;
  printf("Poczatek obliczeń równoległych (zrównoleglenie pętli)\n");
  t1 = czas_zegara();
  calka = calka_zrownoleglenie_petli(a, b, dx);
  t1 = czas_zegara() - t1;
  printf("Koniec obliczen  równoległych (zrównoleglenie pętli) \n");
  printf("\tCzas wykonania %lf. \tObliczona całka = %.15lf\n", t1, calka);
  save_result_to_file(fd, filename, "zrownolegl_cykl", calka, t1);


  // SCENARIO 3
  printf("Poczatek obliczeń równoległych (dekompozycja obszaru)\n");
  t1 = czas_zegara();
  calka = calka_dekompozycja_obszaru(a, b, dx);
  t1 = czas_zegara() - t1;
  printf("Koniec obliczen  równoległych (dekompozycja obszaru) \n");
  printf("\tCzas wykonania %lf. \tObliczona całka = %.15lf\n", t1, calka);
  save_result_to_file(fd, filename, "dekompozycja", calka, t1);

}

int save_result_to_file(FILE *fd, const char filename[],
                         const char label[], double calka, double time) {

  fd = fopen(filename, "a");

  if (fd == NULL) {
    printf("Blad podczas zapisywania do pliku\n");
    return 0;
  }
  fprintf(fd, "%-15s %s %10.15lf %2.10f\n", label, ":", calka, time);

  fclose(fd);

  printf("Wynik zapisoano do pliku '%s'.\n", filename);
  return 1;
}

double calka_sekw(double a, double b, double dx){
  int i;
  double calka = 0.0;
  int N = ceil((b - a) / dx); // Obliczenie liczby trapezow
  double dx_adjust = (b - a) / N; // dopasowanie dx

  //printf("Obliczona liczba trapezów: N = %d\n", N);
  //printf("a %lf, b %lf, n %d, dx %.12lf (dx_adjust %.12lf)\n", a, b, N, dx, dx_adjust);
  for(i = 0; i < N; i++){

    double x1 = a + i * dx_adjust;
    calka += 0.5*dx_adjust*(funkcja(x1)+funkcja(x1+dx_adjust));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n", 
    //	   i, x1, funkcja(x1), calka);
  }
  return(calka);
}



void* calka_fragment_petli_w(void* arg_wsk);

double calka_zrownoleglenie_petli(double a, double b, double dx){

  int i;
  int l_w = l_w_global;
  int indices[l_w];
  pthread_t threads[l_w];

  int N = ceil((b-a)/dx);
  double dx_adjust = (b-a)/N;
  a_global = a;
  b_global = b;
  N_global = N;
  dx_global = dx_adjust;

  pthread_mutex_init(&muteks, NULL);

  // printf("Obliczona liczba trapezów: N = %d\n", N);
  // printf("dx_global = dx_adjust = %.6lf\n", dx_adjust);
  // printf("a %lf, b %lf, n %d, dx %.12lf (dx_adjust %.12lf)\n", a, b, N, dx, dx_adjust);

  // Inicjalizacja tablicy intekow do identyfikacji watkow
  for (i = 0; i < l_w; ++i) indices[i] = i;

  // Tworzenie wątkow
  for (i = 0; i < l_w; ++i)
    pthread_create(&threads[i], NULL, calka_fragment_petli_w, (void *) &indices[i]);

  // Oczekiwanie na zakończenie pracy wątków
  for (i = 0; i < l_w; ++i)
    pthread_join(threads[i], NULL);

  return(calka_global);
}

void* calka_fragment_petli_w(void* arg_wsk){

  int my_id = *((int *)arg_wsk);
  int my_start, my_end, my_stride;
  double dx = dx_global;

  // dekompozycja cykliczna
  if (typ_dekompozycji == 'c') {
    my_start = my_id;
    my_end = N_global;
    my_stride = l_w_global;
  }

  // dekompozycja blokowa
  if (typ_dekompozycji == 'b') {
    my_start = my_id * (int)ceil((double)N_global/l_w_global);
    my_end = (my_id + 1) * (int)ceil((double)N_global/l_w_global);
    my_stride = 1;

    if (my_end > N_global) my_end = N_global;
  }

  // something else ? (dekompozycja blokowo-cykliczna)
  // printf("\nWątek %d\n", my_id);
  // printf("my_start %d, my_end %d, my_stride %d\n", 
	//  my_start, my_end, my_stride);

  // Algorytm calkowania
  int i;
  double calka = 0.0;
  for(i=my_start; i<my_end; i+=my_stride){
    double x1 = a_global + i*dx;
    calka += 0.5*dx*(funkcja(x1)+funkcja(x1+dx));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n", 
    //	   i, x1, funkcja(x1), calka);

  }

  // Dodanie obliczonej czesci do globalnej sumy
  pthread_mutex_lock(&muteks);
  calka_global += calka;
  pthread_mutex_unlock(&muteks);

}


void* calka_podobszar_w(void* arg_wsk);
struct int_data {
  int my_id;
  double a;
  double b;
  double dx;
  double result;
};

double calka_dekompozycja_obszaru(double a, double b, double dx){

  //printf("a %lf, b %lf, dx %lf\n", a, b, dx);

  int l_w = l_w_global;
  pthread_t threads[l_w];
  //printf("\nPodaj liczbę wątków:  "); scanf("%d", &l_w);

  double calka_suma_local = 0.0;

  // tworzenie struktur danych do obsługi wielowątkowości
  struct int_data arr_data[l_w];

  double delta = (b - a)/l_w_global;

  // tworzenie wątków
  int i;
  
  for (i = 0; i < l_w_global; ++i) {
    arr_data[i].my_id = i;
    arr_data[i].a = i * delta;
    arr_data[i].b = (i + 1) * delta;
    arr_data[i].dx = dx;
  }

  for (i = 0; i < l_w_global; ++i) {
    pthread_create(&threads[i], NULL, calka_podobszar_w, (void*) &arr_data[i]);
  }

  // oczekiwanie na zakończenie pracy wątków
  for (i = 0; i < l_w_global; ++i) {
    pthread_join(threads[i], NULL);
  }

  for (i = 0; i < l_w_global; ++i) {
    calka_suma_local += arr_data[i].result;
  }

  return(calka_suma_local);
}



void* calka_podobszar_w(void* arg_wsk){

  double a_local, b_local, dx;
  // rozpakowanie danych przesłanych do wątku
  struct int_data* arg = (struct int_data*)arg_wsk;
  a_local = arg->a;
  b_local = arg->b;
  dx = arg->dx;

  int N_local = ceil((b_local-a_local)/dx);
  double dx_adjust_local = (b_local-a_local)/N_local;

  int my_id = arg->my_id;
  // printf("\nWątek %d\n", my_id);
  // printf("a_local %lf, b_local %lf, dx_adjust_local %lf, n_local %d\n", 
	//  a_local, b_local, dx_adjust_local, N_local);

  int i;
  double calka = 0.0;
  for(i=0; i<N_local; i++){
    double x1 = a_local + i*dx_adjust_local;
    calka += 0.5*dx_adjust_local*(funkcja(x1)+funkcja(x1+dx_adjust_local));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n", 
    //	   i, x1, funkcja(x1), calka);
  }
  arg->result = calka;

}

