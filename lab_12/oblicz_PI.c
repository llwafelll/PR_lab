#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpi.h"
//#include <omp.h>

#define SCALAR double
//#define SCALAR float

#ifndef M_PI // standardy C99 i C11 nie wymagają definiowania stałej M_PI
#define M_PI (3.14159265358979323846)
#endif
#define PARALLEL 1
static SCALAR suma;

int main(int argc, char** argv){
  // program obliczania przybliżenia PI za pomocą wzoru Leibniza
  // PI = 4 * ( 1 - 1/3 + 1/5 - 1/7 + 1/9 - 1/11 + itd. )
  
  #if PARALLEL == 1

  // PARALLEL
  int max_liczba_wyrazow = 0;
  int rank, size, root, n_loc, i;
  SCALAR suma_plus = 0, suma_minus = 0, suma_part = 0;
  MPI_Status status;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  if (rank == 0) {
    printf("Podaj maksymalną liczbę wyrazów do obliczenia przybliżenia PI\n");
    scanf("%d", &max_liczba_wyrazow);

    // wzór: PI/4 = 1 - 1/3 + 1/5 - 1/7 + 1/9 itd. itp.  
    SCALAR suma_plus=0.0;
    SCALAR suma_minus=0.0;
    
    //  double t = omp_get_wtime();
    
    int i=0;
    //#pragma omp parallel for default(none) shared(max_liczba_wyrazow) reduction(+:suma_plus) reduction(+:suma_minus)
    for(i=0; i<max_liczba_wyrazow; i++){
      
      int j = 1 + 4*i;
      
      suma_plus += 1.0/j;
      suma_minus += 1.0/(j+2.0);
      
      // printf("PI obliczone: %20.15lf, aktualna poprawka: %20.15lf\n",
      //  	 4*(suma_plus-suma_minus), 4.0/j-4.0/(j+2.0));
      
    }
    
    //  t = omp_get_wtime() - t;
    
    printf("PI obliczone: \t\t\t%20.15lf\n", 4*(suma_plus-suma_minus));
    printf("PI z biblioteki matematycznej: \t%20.15lf\n", M_PI);
  }

  root = 0;
  MPI_Bcast(&max_liczba_wyrazow, 1, MPI_INT, root, MPI_COMM_WORLD);
  n_loc = ceil(max_liczba_wyrazow/size);
  if (rank == size - 1) n_loc = max_liczba_wyrazow - n_loc*(size - 1);

  //#pragma omp parallel for default(none) shared(max_liczba_wyrazow) reduction(+:suma_plus) reduction(+:suma_minus)
  for(i=rank * n_loc; i<(rank+1) * n_loc; i++) {
    
    int j = 1 + 4*i;
    
    suma_plus += 1.0/j;
    suma_minus += 1.0/(j+2.0);
    
    //printf("PI obliczone: %20.15lf, aktualna poprawka: %20.15lf\n",
    //  	 4*(suma_plus-suma_minus), 4.0/j-4.0/(j+2.0));
    
  }
  suma_part = suma_plus - suma_minus;
  MPI_Reduce(&suma_part, &suma, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);

  MPI_Finalize(); 
  
  if (rank == root)
    printf("PI obliczone rownolegle: \t%20.15lf\n", 4*(suma));

  return(0);

  #endif
}



  // całka
  /* h   = 1.0 / (SCALAR) n; */
  /* sum = 0.0; */
  /* for (i = 1; i <= n; i++) { */
  /*   x = h * ((SCALAR)i - 0.5); */
  /*   sum += 4.0 / (1.0 + x*x); */
  /* } */
  /* mypi = h * sum; */
