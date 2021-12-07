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

  omp_set_nested(1);

  // podwójna pętla - docelowo równolegle
  // 
  double suma_parallel;

  // Dekompzycja wierszowa
  // suma_parallel = 0.0;
  // #pragma omp parallel for default(none) shared(a) private(i, j) \
  // reduction(+ : suma_parallel) schedule(static, 2) ordered
  // for(i = 0; i < WYMIAR; ++i) {
  //   int id_w = omp_get_thread_num();
  //   // #pragma omp ordered
  //   // ...
  //   #pragma omp ordered
  //   printf("a[%2d]->W_%1d  \n",i,id_w); 

  //   for(j = 0; j < WYMIAR; ++j) {
  //     suma_parallel += a[i][j];
  //     // dla dekompozycji 1D
  //     #pragma omp ordered
  //     printf("(%1d,%1d)-W_%1d ",i,j,omp_get_thread_num()); 
  //     // printf("\033[%dm %06.3f \033[m", omp_get_thread_num() + 33, a[i][j]);

  //     // dla dekompozycji 2D
  //     //printf("(%1d,%1d)-W_%1d,%1d ",i,j,id_w,omp_get_thread_num()); 
  //   }
  //   // #pragma omp ordered
  //   printf("\n");
  // }

  // printf("Suma wyrazów tablicy równolegle: %lf\n", suma_parallel);


  // Dekompzycja kolumnowa v1
  // suma_parallel = 0.0;
  // #pragma omp parallel default(none) shared(a) private(i, j) \
  // reduction(+ : suma_parallel)
  // for(i = 0; i < WYMIAR; ++i) {
  //   int id_w = omp_get_thread_num();
  //   // #pragma omp ordered
  //   // ...
  //   // #pragma omp ordered
  //   // printf("a[%2d]->W_%1d  \n",i,id_w); 

  //   #pragma omp for schedule(static, 2) ordered
  //   for(j = 0; j < WYMIAR; ++j) {
  //     suma_parallel += a[i][j];
  //     // dla dekompozycji 1D
  //     #pragma omp ordered
  //     // printf("(%1d,%1d)-W_%1d ",i,j,omp_get_thread_num()); 
  //     printf("\033[%dm %06.3f \033[m", omp_get_thread_num() + 33, a[i][j]);

  //     // dla dekompozycji 2D
  //     //printf("(%1d,%1d)-W_%1d,%1d ",i,j,id_w,omp_get_thread_num()); 
  //   }
  //   #pragma omp single
  //   printf("\n");
  // }

  // printf("Suma wyrazów tablicy równolegle: %lf\n", suma_parallel);

  // // Dekompzycja kolumnowa v2 (zamiana pętli)
  suma_parallel = 0.0;
  #pragma omp parallel for default(none) shared(a, suma_parallel) private(i, j) \
  schedule(static, 2) ordered
  for(j = 0; j < WYMIAR; ++j) {
    int id_w = omp_get_thread_num();
    #pragma omp ordered
    printf("a[%2d]->W_%1d  \n",i,id_w); 

    for(i = 0; i < WYMIAR; ++i) {

      // #pragma omp critical(suma_parallel)
      #pragma omp atomic update
      suma_parallel += a[i][j];
      // dla dekompozycji 1D
      #pragma omp ordered
      // printf("(%1d,%1d)-W_%1d ",i,j,omp_get_thread_num()); 
      printf("\033[%dm %06.3f \033[m", omp_get_thread_num() + 33, a[i][j]);

      // dla dekompozycji 2D
      //printf("(%1d,%1d)-W_%1d,%1d ",i,j,id_w,omp_get_thread_num()); 
    }
    // #pragma omp ordered
    printf("\n");
  }

  printf("Suma wyrazów tablicy równolegle: %lf\n", suma_parallel);


  // // Dekompozycja kolumnowa v3 (nie z instrukcji)
  // suma_parallel = 0.0;
  // #pragma omp parallel for default(none) shared(a) private(i, j) \
  // reduction(+ : suma_parallel) schedule(static, 2) ordered
  // for(j = 0; j < WYMIAR; ++j) {
  //   int id_w = omp_get_thread_num();
  //   // #pragma omp ordered
  //   // ...
  //   #pragma omp ordered
  //   printf("a[%2d]->W_%1d  \n",i,id_w); 

  //   for(i = 0; i < WYMIAR; ++i) {
  //     suma_parallel += a[i][j];
  //     // dla dekompozycji 1D
  //     #pragma omp ordered
  //     // printf("(%1d,%1d)-W_%1d ",i,j,omp_get_thread_num()); 
  //     printf("\033[%dm %06.3f \033[m", omp_get_thread_num() + 33, a[i][j]);

  //     // dla dekompozycji 2D
  //     //printf("(%1d,%1d)-W_%1d,%1d ",i,j,id_w,omp_get_thread_num()); 
  //   }
  //   // #pragma omp ordered
  //   printf("\n");
  // }
  // printf("Suma wyrazów tablicy równolegle: %lf\n", suma_parallel);

}
