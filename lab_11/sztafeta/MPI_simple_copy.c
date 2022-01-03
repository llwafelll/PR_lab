#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<unistd.h>

#include "mpi.h"

static const int HOSTNAME_BUF_SIZE = 64;
int main( int argc, char** argv ){ 
  
  int rank, ranksent, size, source, dest, tag; 
  int prev, next, current;
  MPI_Status status;

  // printf("hostname = %s", hostname);
  
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  
  if(size>1){
    if( rank != 0 ){ dest=0; tag=0; 
      current = rank;
      // prev = current -1;
      next = current + 1;

      // petla?
      // printf("*Waiting for process rank: %d*\n", current);
      MPI_Recv(&prev, 1, MPI_INT, current - 1, 
          MPI_ANY_TAG, MPI_COMM_WORLD, &status );


      printf("*Receiving message from: %d*\n", prev);
      // printf(">>>prev: %d | current: %d | next: %d\n", prev, current, next);

      if (next <= size-1) {
        printf("*Sending message to: %d*\n", next);
        MPI_Send( &current, 1, MPI_INT, next, 0, MPI_COMM_WORLD );
      }
      // else next % size - 1
      
    } else {
      current = rank;
      next = current + 1;

      // printf(">>>prev: INIT | current: %d | next: %d\n", current, next);

      printf("*Sending message to: %d*\n", next);
      MPI_Send(&current, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
      // MPI_Recv( &ranksent, 1, MPI_INT, MPI_ANY_SOURCE, 
      //     MPI_ANY_TAG, MPI_COMM_WORLD, &status );

      // printf("Dane od procesu o randze: %d (status.MPI_SOURCE -> %d) \n", 
      //       ranksent, status.MPI_SOURCE);
    }
  } else{
    printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
  }
  printf("\n");

  
  MPI_Finalize(); 
  
  return(0);

}

