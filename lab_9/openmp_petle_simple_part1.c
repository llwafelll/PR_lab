#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <omp.h>
#include <savetofile.h>

// #define WYMIAR 18
// #define WYMIAR 200000000

static unsigned long int WYMIAR;
int main (int argc, char *argv []) {
  WYMIAR = strtoul(argv[1], NULL, 10);
  double t_seq, t_par;

  // double a[WYMIAR];
  double *a = (double*)malloc(WYMIAR * sizeof(double));
  unsigned long int n,i;

  for(i=0;i<WYMIAR;i++) a[i]=1.02*i;

  n=WYMIAR;

  // pętla sekwencyjna
  double suma=0.0;

  t_seq = omp_get_wtime();
  for(i=0;i<WYMIAR;i++) {
      suma += a[i];
  }
  t_seq = omp_get_wtime() - t_seq;

  
  // pętla równoległa
  printf("=== Obliczenia sekwencyjne ===\n");
  printf("\t*Suma wyrazów tablicy: %lf*\n", suma);
  printf("\t*Czas: %.10lf*\n\n", t_seq);

  // pętla równoległa
  printf("=== Obliczenia równoległe ===\n");
  printf("\t*Liczba elementów w tablicy %d*\n", n);
  printf("\t*Liczba wątków widzianych przez system: %d*\n",
         omp_get_max_threads());
  int TperE = ceil((double)n/(double)omp_get_max_threads()) ;
  printf("\t*ceil(N/T) = %d*\n\n", TperE);

  // algorytm
  double suma_parallel = 0.0;

  t_par = omp_get_wtime();
  #pragma omp parallel for default(none) \
  shared(a, n) reduction(+:suma_parallel) ordered
    for (i = 0; i < n; ++i) {
      int id_w = omp_get_thread_num();
      suma_parallel += a[i];

      #pragma omp ordered
      printf("\033[%dma[%2d]->W_%1d\033[m\n", id_w + 33, i, id_w);
    }
  t_par = omp_get_wtime() - t_par;

  printf("\n\t*Suma wyrazów tablicy równolegle: %lf*\n",
	 suma_parallel);
  printf("\t*Czas: %.10lf*\n", t_par);
  save_to_file("data.txt", t_seq, t_par);

  free(a);

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
