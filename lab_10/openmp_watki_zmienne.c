#include<stdlib.h>
#include<stdio.h>
#include<omp.h>

static int f_threadprivate = 10;
int main(){
  
#ifdef   _OPENMP
  printf("\nKompilator rozpoznaje dyrektywy OpenMP\n");
#endif

  // export OMP_NUM_THREADS=6
  omp_set_num_threads(5);
  // num_threads(6) <-- klauzula

  int liczba_watkow;
  
  int a_shared = 1;
  int b_private = 2;
  int c_firstprivate = 3;
  int e_atomic=5;
  
  
  printf("przed wejsciem do obszaru rownoleglego -  nr_threads %d, thread ID %d\n",
	 omp_get_num_threads(), omp_get_thread_num());
  printf("\ta_shared \t= %d\n", a_shared);
  printf("\tb_private \t= %d\n", b_private);
  printf("\tc_firstprivate \t= %d\n", c_firstprivate);
  printf("\te_atomic \t= %d\n", e_atomic);
    
  
#pragma omp parallel default(none) shared(a_shared, e_atomic) \
private(b_private) firstprivate(c_firstprivate)
  {
    int i;
    int d_local_private;
    d_local_private = a_shared + c_firstprivate;

    #pragma omp barrier
    #pragma omp critical(a_shared)
    for(i=0;i<10;i++){
      a_shared ++;
    }

    for(i=0;i<10;i++){
      c_firstprivate += omp_get_thread_num();
    }

    for(i=0;i<10;i++){
      #pragma omp atomic update
      e_atomic+=omp_get_thread_num();
    }

    #pragma omp threadprivate(f_threadprivate)
    f_threadprivate += omp_get_thread_num();
    
    #pragma omp barrier
    #pragma omp critical
    {
      printf("\nw obszarze równoległym: aktualna liczba watkow %d, moj ID %d\n",
	     omp_get_num_threads(), omp_get_thread_num());
      
      printf("\ta_shared \t= %d\n", a_shared);
      printf("\tb_private \t= %d\n", b_private);
      printf("\tc_firstprivate \t= %d\n", c_firstprivate);
      printf("\td_local_private = %d\n", d_local_private);
      printf("\te_atomic \t= %d\n", e_atomic);
      printf("\tf_threadprivate = %d\n", f_threadprivate);
    }

    //#pragma omp single
/* #pragma omp master */
/*         { */
    
/*           printf("\ninside single: nr_threads %d, thread ID %d\n", */
/*     	     omp_get_num_threads(), omp_get_thread_num()); */
/*           /\* Get environment information *\/ */
/*           int procs = omp_get_num_procs(); */
/*           int nthreads = omp_get_num_threads(); */
/*           int maxt = omp_get_max_threads(); */
/*           int inpar = omp_in_parallel(); */
/*           int dynamic = omp_get_dynamic(); */
/*           int nested = omp_get_nested(); */
    
/*           /\* Print environment information *\/ */
/*           printf("Number of processors = %d\n", procs); */
/*           printf("Number of threads = %d\n", nthreads); */
/*           printf("Max threads = %d\n", maxt); */
/*           printf("In parallel? = %d\n", inpar); */
/*           printf("Dynamic threads enabled? = %d\n", dynamic); */
/*           printf("Nested parallelism supported? = %d\n", nested); */
    
/*         } */
    
  }
  
  printf("po zakonczeniu obszaru rownoleglego:\n");
  printf("\ta_shared \t= %d\n", a_shared);
  printf("\tb_private \t= %d\n", b_private);
  printf("\tc_firstprivate \t= %d\n", c_firstprivate);
  printf("\te_atomic \t= %d\n", e_atomic);

  printf("\n");
  #pragma omp parallel default(none)
  {
    printf("f_threadprivate = %d\n", f_threadprivate);
  }
}
