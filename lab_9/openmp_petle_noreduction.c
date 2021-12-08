#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 20

int main ()
{
  double a[WYMIAR][WYMIAR];
  int n,i,j;

  for(i=0;i<WYMIAR;i++) for(j=0;j<WYMIAR;j++) a[i][j]=1.02*i+1.01*j;

  n=WYMIAR;

  // podwójna pętla - sekwencyjnie
  double suma=0.0;
  for(i=0;i<WYMIAR;i++) {
    for(j=0;j<WYMIAR;j++) {
      suma += a[i][j];
    }
  }
  
  printf("Suma wyrazów tablicy: %lf\n", suma);

  omp_set_nested(1);

  // podwójna pętla - docelowo równolegle
  // 
  unsigned const int THREADS = omp_get_max_threads();
  double suma_parallel;
  double sums[THREADS]; for (int i = 0; i < THREADS; ++i) sums[i] = 0;

  printf("Liczba watkow: %d\n", omp_get_max_threads());

  // Dekompzycja wierszowa
  #if (1)
  suma_parallel = 0.0;
  #pragma omp parallel for default(none) shared(a, sums) private(i, j) \
  schedule(static, 2) ordered
  for(i = 0; i < WYMIAR; ++i) {
    int id_w = omp_get_thread_num();
    // #pragma omp ordered
    // printf("a[%2d]->W_%1d  \n",i,id_w); 

    for(j = 0; j < WYMIAR; ++j) {
      // #pragma omp atomic read
      sums[id_w] += a[i][j];

      // dla dekompozycji 1D
      #pragma omp ordered
      printf("\033[%dm %08.3f \033[m", 31 + id_w, sums[id_w]);

      // dla dekompozycji 2D
      //printf("(%1d,%1d)-W_%1d,%1d ",i,j,id_w,omp_get_thread_num()); 
    }

    printf("\n");
  }

  for (int i = 0; i < THREADS; ++i) suma_parallel += sums[i];
  printf("Suma wyrazów tablicy równolegle: %lf\n", suma_parallel);
  #endif
}
