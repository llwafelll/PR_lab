#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 10

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


  // podwójna pętla - docelowo równolegle
  double suma_parallel;
  omp_set_nested(1);

  // Dekompzycja wierszowa
  suma_parallel = 0.0;
  #pragma omp parallel for default(none) shared(a, j) private(i) \
  reduction(+ : suma_parallel) schedule(static) ordered
  for(i = 0; i < WYMIAR; ++i) {
    int id_w = omp_get_thread_num();
    #pragma omp ordered
    // printf("a[%2d]->W_%1d  \n",i,id_w); 

    #pragma omp parallel for default(none) shared(a) private(j) firstprivate(i, id_w) \
    reduction(+ : suma_parallel) schedule(static) ordered
    for(j = 0; j < WYMIAR; ++j) {
      int id_wIN = omp_get_thread_num();
      suma_parallel += a[i][j];
      // dla dekompozycji 1D
      #pragma omp ordered
      // printf("\033[1m\033[%dm\033[%dm(%2d,%2d)-W_%1d \033[0m",
      //        id_wIN + 91, id_w + 41, i, j, id_w); 
      printf("\033[1m\033[%dm\033[%dm(%2d,%2d) \033[0m",
             id_wIN + 91, id_w + 41, i, j); 
      // printf("\033[%dm %06.3f \033[m", omp_get_thread_num() + 33, a[i][j]);

      // dla dekompozycji 2D
      //printf("(%1d,%1d)-W_%1d,%1d ",i,j,id_w,omp_get_thread_num()); 
    }
    printf("\n");
  }

  printf("Suma wyrazów tablicy równolegle: %lf\n", suma_parallel);
}
