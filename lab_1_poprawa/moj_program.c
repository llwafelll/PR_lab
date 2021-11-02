#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<pomiar_czasu.h>
 
const int liczba = 100000*100;

int main(){

	// Deklaracja zmiennych do zapisu czasu
	double t1, t2;

	double a, b, c;
	int i,j,k,l;


	// PETLA Z OPERACJAMI WEJSCIA/WYJSCIA
	k = -50000;

	// Rozpoczecie pomiaru czasu dla petli z operacjami we/wy
	inicjuj_czas();

	for(i=0; i < liczba; i++) printf("%d ",k+i);

	printf("\n\n");
	printf("Czas wykonania %d operacji wejscia/wyjscia: \n",liczba);

	// Wypisanie czasu na ekran
	drukuj_czas();


	// PETLA Z OPERACJAMI ARYTMETYCZNYMI
	a = 1.000001;

	// Rozpoczecie pomiaru czasu dla petli z operacjami arytmetycznymi
	t1 = czas_zegara();
	t2 = czas_CPU();

	for(i=0;i<liczba;i++) a = 1.000001 * a + 0.000001; 

	printf("\nWynik operacji arytmetycznych: %lf\n", a);
	printf("Czas wykonania %d operacji arytmetycznych: \n",liczba);

	// Zakonczenie pomiaru czasu dla petli z operacjami arytmetycznymi
	t1 = czas_zegara() - t1;
	t2 = czas_CPU() - t2;

	// Wypisanie czasu na ekran
	printf("czas CPU         = %lf\n", t2);
	printf("czas zegarowy    = %lf\n", t1);

	printf("\n");
}
