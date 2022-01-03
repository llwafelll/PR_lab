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

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  
  if(size>1){
    if((rank > 0) && (size - 1 > rank)){ dest=0; tag=0; 
      current = rank;
      next = current + 1;

      // petla?
      MPI_Recv(&prev, 1, MPI_INT, current - 1, 
          MPI_ANY_TAG, MPI_COMM_WORLD, &status );
      printf("*%d Received message from: %d*\n", current, prev);

      printf("*%d Sending message to: %d*\n", current, next);
      MPI_Send( &current, 1, MPI_INT, next, 0, MPI_COMM_WORLD );

    } else if (rank == 0) {
      current = rank;
      next = current + 1;

      printf("*%d Sending message to: %d*\n", current, next);
      MPI_Send(&current, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    } else {
        current = rank;
        next = 1;
        MPI_Recv(&prev, 1, MPI_INT, current - 1, 
            MPI_ANY_TAG, MPI_COMM_WORLD, &status );
        printf("*%d Received message from: %d*\n", current, prev);

        printf("Communication terminated\n");
    }

  } else {
    printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
  }
  printf("\n");

  MPI_Finalize(); 
  
  return(0);

}

