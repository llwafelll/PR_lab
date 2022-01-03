#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<unistd.h>

#include "mpi.h"

static const int HOSTNAME_BUF_SIZE = 64;
int main( int argc, char** argv ){ 
  
  int rank, ranksent, size, source, dest, tag; 
  int data = 0;
  MPI_Status status;
  char initial = 1;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  
  if(size>1){
    if (rank == 0){
          printf("*[INIT] %d Sending message to: %d*\n", rank, 1);
          MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }

    while (1) {
        // Receive
        MPI_Recv(&data, 1, MPI_INT, (rank + size - 1) % size, 
            MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("*[LOOP] %d Receives message from %d: %d*\n",
               rank, (rank + size - 1) % size, data);
        sleep(1);

        // Send
        printf("*[LOOP] %d Sending massage to: %d*\n",
               rank, (rank + 1) % size, data);
        ++data;
        MPI_Send(&data, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
    }

  } else {
    printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
  }
  printf("\n");

  MPI_Finalize(); 
  
  return(0);

}

