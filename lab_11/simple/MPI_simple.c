#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<unistd.h>

#include "mpi.h"

static const int HOSTNAME_BUF_SIZE = 64;
int main( int argc, char** argv ){ 
  
  int rank, ranksent, size, source, dest, tag, i; 
  char hostname[HOSTNAME_BUF_SIZE], hostname_received[HOSTNAME_BUF_SIZE];
  MPI_Status status;

  // printf("hostname = %s", hostname);
  
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  
  if(size>1){
    if( rank != 0 ){ dest=0; tag=0; 
      gethostname(hostname, HOSTNAME_BUF_SIZE);

      MPI_Send( &rank, 1, MPI_INT, dest, tag, MPI_COMM_WORLD );
      MPI_Send( hostname, HOSTNAME_BUF_SIZE, MPI_CHAR, dest, tag, MPI_COMM_WORLD );
      
    } else {
      for(i = 1; i < size; ++i) { 
        MPI_Recv( &ranksent, 1, MPI_INT, MPI_ANY_SOURCE, 
            MPI_ANY_TAG, MPI_COMM_WORLD, &status );

        MPI_Recv( hostname_received, HOSTNAME_BUF_SIZE, MPI_CHAR, MPI_ANY_SOURCE, 
            MPI_ANY_TAG, MPI_COMM_WORLD, &status );
        printf("Dane od procesu o randze: %d (status.MPI_SOURCE -> %d) (i=%d), hostname=%s\n", 
              ranksent, status.MPI_SOURCE, i, hostname_received);
        // printf("\tReceived hostname: %s\n", hostname_received);
      }
    }
  } else{
    printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
  }
  printf("\n");
  
  MPI_Finalize(); 
  
  return(0);

}

