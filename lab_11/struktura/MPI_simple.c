#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<unistd.h>
#include<string.h>

#include "mpi.h"

static const int HOSTNAME_BUF_SIZE = 64;

struct bogata {
  int age;
  double height;
  char name[10];
};

int main( int argc, char** argv ){ 
  
  int rank, ranksent, size, source, dest, tag, i; 
  MPI_Status status;

  int rozm, rozm_pakietu, pozycja, pozycja_rec; void* bufor;
  int prev, current, next;
  struct bogata user;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  MPI_Pack_size(1, MPI_INT, MPI_COMM_WORLD, &rozm);
  rozm_pakietu += rozm;
  MPI_Pack_size(1, MPI_DOUBLE, MPI_COMM_WORLD, &rozm);
  rozm_pakietu += rozm;
  MPI_Pack_size(10, MPI_CHAR, MPI_COMM_WORLD, &rozm);
  rozm_pakietu += rozm;
  bufor = (void *)malloc(rozm_pakietu); pozycja = 0; pozycja_rec = rozm_pakietu;
  // printf("hostname = %s", hostname);
  
  if(size>1){
    if( rank != 0 ){ dest=0; tag=0; 
      current = rank;
      next = current + 1;

      // Odbieranie danych
      MPI_Recv(bufor, rozm_pakietu, MPI_PACKED, current - 1, 
          MPI_ANY_TAG, MPI_COMM_WORLD, &status );
      
      // Rozpakowanie danych
      MPI_Unpack(bufor, rozm_pakietu, &pozycja, &user.age, 1, MPI_INT, MPI_COMM_WORLD);
      MPI_Unpack(bufor, rozm_pakietu, &pozycja, &user.height, 1, MPI_DOUBLE, MPI_COMM_WORLD);
      MPI_Unpack(bufor, rozm_pakietu, &pozycja, &user.name[0], 10, MPI_CHAR, MPI_COMM_WORLD);

      // Wypisanie danych
      printf("rank: %d -> age=%d | height=%f | name=%s\n", rank, user.age, user.height, user.name);

      // Przeslanie danych dalej
      if (next <= size - 1) {
        printf("Sending message to: %d\n", next);
        pozycja = 0;

        user.age *= 10;
        user.height += 100;
        // strncpy(user.name, "Marcin  \n", 10);
        MPI_Pack(&user.age, 1, MPI_INT, bufor, rozm_pakietu, &pozycja, MPI_COMM_WORLD);
        MPI_Pack(&user.height, 1, MPI_DOUBLE, bufor, rozm_pakietu, &pozycja, MPI_COMM_WORLD);
        MPI_Pack(&user.name[0], 10, MPI_CHAR, bufor, rozm_pakietu, &pozycja, MPI_COMM_WORLD);

        MPI_Send(bufor, pozycja, MPI_PACKED, next, 0, MPI_COMM_WORLD);
      }
      
    } else {
      user.age = 22;
      user.height = 1.80;
      strncpy(user.name, "Marcin  \n", 10);
      MPI_Pack(&user.age, 1, MPI_INT, bufor, rozm_pakietu, &pozycja, MPI_COMM_WORLD);
      MPI_Pack(&user.height, 1, MPI_DOUBLE, bufor, rozm_pakietu, &pozycja, MPI_COMM_WORLD);
      MPI_Pack(&user.name[0], 10, MPI_CHAR, bufor, rozm_pakietu, &pozycja, MPI_COMM_WORLD);

      current = rank;
      next = current + 1;

      printf("*Sending message to: %d\n", next);
      MPI_Send(bufor, pozycja, MPI_PACKED, next, 0, MPI_COMM_WORLD);

      // for (int i = 1; i < 4; ++i)
      //   MPI_Send(bufor, pozycja, MPI_PACKED, i, 0, MPI_COMM_WORLD);

      // MPI_Recv( &ranksent, 1, MPI_INT, MPI_ANY_SOURCE, 
      //     MPI_ANY_TAG, MPI_COMM_WORLD, &status );

    }
  } else{
    printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
  }
  printf("\n");

  
  MPI_Finalize(); 
  
  return(0);

}

