#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"


/*** Implementacja procedur interfejsu ***/

int my_read_lock_lock(czytelnia_t* czytelnia_p){
    czytelnia_p->liczba_czyt += 1;
}


int my_read_lock_unlock(czytelnia_t* czytelnia_p){
    czytelnia_p->liczba_czyt -= 1;
}


int my_write_lock_lock(czytelnia_t* czytelnia_p){
    czytelnia_p->liczba_pisz += 1;
}


int my_write_lock_unlock(czytelnia_t* czytelnia_p){
    czytelnia_p->liczba_pisz -= 1;
}

void inicjuj(czytelnia_t* czytelnia_p){
    czytelnia_p->liczba_czyt = 0;
    czytelnia_p->liczba_pisz = 0;
}

void czytam(czytelnia_t* czytelnia_p){
    usleep(rand()%300000);
}

void pisze(czytelnia_t* czytelnia_p){
    usleep(rand()%300000);
}


