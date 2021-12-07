#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <omp.h>

#define WYMIAR 18

int main () {
  double a[WYMIAR];
  int n,i;

  for(i=0;i<WYMIAR;i++) a[i]=1.02*i;

  n=WYMIAR;

  // pętla sekwencyjna
  double suma=0.0;
  for(i=0;i<WYMIAR;i++) {
      suma += a[i];
  }
  
  // pętla równoległa
  printf("=== Obliczenia sekwencyjne ===\n");
  printf("Suma wyrazów tablicy: %lf\n\n", suma);

  // pętla równoległa
  printf("=== Obliczenia równoległe ===\n");
  printf("\t*Liczba elementów w tablicy %d*\n", n);
  printf("\t*Liczba wątków widzianych przez system: %d*\n",
         omp_get_max_threads());
  int TperE = ceil((double)n/(double)omp_get_max_threads()) ;
  printf("\t*ceil(N/T) = %d*\n", TperE);

  // algorytm
  double suma_parallel = 0.0;
  #pragma omp parallel for default(none) \
  shared(a) reduction(+:suma_parallel) ordered
    for (i = 0; i < WYMIAR; ++i) {
      int id_w = omp_get_thread_num();
      suma_parallel += a[i];

      #pragma omp ordered
      printf("a[%2d]->W_%1d  \n", i, id_w);
    }

  printf("\nSuma wyrazów tablicy równolegle (z klauzulą - ....: %lf\n",
	 suma_parallel);

  // old code
  // #pragma omp parallel for default(none) shared(suma_parallel, a) ordered
  // for(int i=0;i<WYMIAR;i++) {
  //   int id_w = omp_get_thread_num();
  //   // ...
  //     suma_parallel += a[i];
  //     // ...
  // #pragma omp ordered
  //       printf("a[%2d]->W_%1d  \n",i,id_w); 
  //   }
}
