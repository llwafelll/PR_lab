#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <savetofile.h>

// #define WYMIAR 10

static unsigned long int WYMIAR;
int main (int argc, char *argv [])
{
  omp_set_num_threads(3);
  WYMIAR = strtoul(argv[1], NULL, 10);
  double t_seq, t_par;
  // omp_sched_t kind = omp_sched_dynamic;
  // int chunk_size = 2; // if 0 then default is used

  // double a[WYMIAR][WYMIAR];
  double **a = (double**)malloc(sizeof(double) * WYMIAR);
  for (int k = 0; k < WYMIAR; ++k)
    a[k] = (double*)malloc(sizeof(double) * WYMIAR);

  unsigned long int n,i,j;

  for(i=0;i<WYMIAR;i++) for(j=0;j<WYMIAR;j++) a[i][j]=1.02*i+1.01*j;

  n=WYMIAR;

  // podwójna pętla - sekwencyjnie
  t_seq = omp_get_wtime();
  double suma=0.0;
  for(i=0;i<WYMIAR;i++) {
    for(j=0;j<WYMIAR;j++) {
      suma += a[i][j];
    }
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
  omp_set_nested(1);

  // podwójna pętla - docelowo równolegle
  double suma_parallel;

  // Dekompzycja wierszowa
  // suma_parallel = 0.0;
  // t_par = omp_get_wtime();
  // #pragma omp parallel for default(none) shared(a, n) private(i, j) \
  // reduction(+ : suma_parallel) schedule(static, 2) ordered
  // for(i = 0; i < n; ++i) {
  //   int id_w = omp_get_thread_num();

  //   // Print row and assigned thread (id_w) to it
  //   // #pragma omp ordered
  //   // printf("a[%2d]->W_%1d  \n",i,id_w); 

  //   for(j = 0; j < n; ++j) {
  //     // sum all columns in this row
  //     suma_parallel += a[i][j];

  //     #pragma omp ordered
  //     // Original print
  //     // printf("(%1d,%1d)-W_%1d ",i,j,omp_get_thread_num()); 

  //     // Print value of an element in the matrix that is being processed
  //     // printf("\033[%dm %06.3f \033[m", omp_get_thread_num() + 33, a[i][j]);

  //     // Print pairs (i, j)
  //     printf("\033[%dm (%d, %d) \033[m", omp_get_thread_num() + 33, i, j);

  //     // dla dekompozycji 2D
  //     //printf("(%1d,%1d)-W_%1d,%1d ",i,j,id_w,omp_get_thread_num()); 
  //   }

  //   printf("\n");
  // }
  // t_par = omp_get_wtime() - t_par;

  // free(a);

  // printf("\n\t*Suma wyrazów tablicy równolegle: %lf*\n",
	//  suma_parallel);
  // printf("\t*Czas: %.10lf*\n", t_par);
  // save_to_file("openmp_petle1.txt", t_seq, t_par);


  // Dekompzycja kolumnowa v1
  // suma_parallel = 0.0;
  // t_par = omp_get_wtime();
  // #pragma omp parallel default(none) shared(a, n) private(i, j) \
  // reduction(+ : suma_parallel)
  // for(i = 0; i < n; ++i) {
  //   int id_w = omp_get_thread_num();
  //   // #pragma omp ordered
  //   // printf("a[%2d]->W_%1d  \n",i,id_w); 

  //   #pragma omp for schedule(dynamic) ordered
  //   for(j = 0; j < n; ++j) {
  //     suma_parallel += a[i][j];
  //     // dla dekompozycji 1D
  //     #pragma omp ordered
  //     // Original print
  //     // printf("(%1d,%1d)-W_%1d ",i,j,omp_get_thread_num()); 

  //     // Print value of an element in the matrix that is being processed
  //     // printf("\033[%dm %06.3f \033[m", omp_get_thread_num() + 33, a[i][j]);

  //     // Print pairs (i, j)
  //     printf("\033[%dm (%d, %d) \033[m", omp_get_thread_num() + 33, i, j);

  //     // dla dekompozycji 2D
  //     //printf("(%1d,%1d)-W_%1d,%1d ",i,j,id_w,omp_get_thread_num()); 
  //   }
  //   #pragma omp single
  //   printf("\n");
  // }
  // t_par = omp_get_wtime() - t_par;

  // free(a);

  // printf("\n\t*Suma wyrazów tablicy równolegle: %lf*\n",
	//  suma_parallel);
  // printf("\t*Czas: %.10lf*\n", t_par);
  // save_to_file("openmp_petle.txt", t_seq, t_par);

  // Dekompzycja kolumnowa v2 (zamiana pętli)
  suma_parallel = 0.0;
  t_par = omp_get_wtime();
  #pragma omp parallel default(none) shared(a, n, suma_parallel) \
  private(i, j)
  {
    double suma_part = 0; // dales int'a a nie doubla palo
    int id_w = omp_get_thread_num();

    // #pragma omp ordered
    // printf("a[%2d]->W_%1d  \n",i,id_w); 

    #pragma omp for schedule(static) ordered
    for(j = 0; j < n; ++j) {
      for(i = 0; i < n; ++i) {
        suma_part += a[i][j];
        #pragma omp ordered
        // Original print
        // printf("(%1d,%1d)-W_%1d ",i,j,omp_get_thread_num()); 

        // Print value of an element in the matrix that is being processed
        // printf("\033[%dm %06.3f \033[m", omp_get_thread_num() + 33, a[i][j]);

        // Print pairs (i, j)
        printf("\033[%dm (%d, %d) \033[m",
               omp_get_thread_num() + 33, i, j);

        // dla dekompozycji 2D
        //printf("(%1d,%1d)-W_%1d,%1d ",i,j,id_w,omp_get_thread_num()); 
      }
      // #pragma omp single
      printf("\n");
    }

    #pragma omp atomic update
    suma_parallel += suma_part;
  }
  t_par = omp_get_wtime() - t_par;

  free(a);

  printf("\n\t*Suma wyrazów tablicy równolegle: %lf*\n",
	 suma_parallel);
  printf("\t*Czas: %.10lf*\n", t_par);
  save_to_file("openmp_petle.txt", t_seq, t_par);

  // suma_parallel = 0.0;
  // #pragma omp parallel for default(none) shared(a, suma_parallel) private(i, j) \
  // schedule(static, 2) ordered
  // for(j = 0; j < WYMIAR; ++j) {
  //   int id_w = omp_get_thread_num();
  //   #pragma omp ordered
  //   printf("a[%2d]->W_%1d  \n",i,id_w); 

  //   for(i = 0; i < WYMIAR; ++i) {

  //     // #pragma omp critical(suma_parallel)
  //     #pragma omp atomic update
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
