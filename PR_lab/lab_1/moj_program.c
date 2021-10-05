#include<stdlib.h>
#include<stdio.h>
#include<time.h>
 
const int liczba = 100000;

main(){

  double a, b, c;
  int i,j,k,l;

  k = -50000;
  for(i=0;i<liczba;i++){

    printf("%d ",k+i);

  }
  printf("\n");

  printf("Czas wykonania %d operacji wejscia/wyjscia: \n",liczba);


  a = 1.000001;
  for(i=0;i<liczba;i++){

    a = 1.000001*a+0.000001; 

  }

  printf("Wynik operacji arytmetycznych: %lf\n", a);
  printf("Czas wykonania %d operacji arytmetycznych: \n",liczba);


}
