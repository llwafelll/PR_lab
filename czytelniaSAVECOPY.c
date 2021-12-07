#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"


/*** Implementacja procedur interfejsu ***/

// Odpowiednik chce_czytac
int my_read_lock_lock(czytelnia_t* czytelnia_p){
    pthread_mutex_lock(&(czytelnia_p->mutex)); // Blokada mutexa czytelni

    // Czytelnik czeka jesli w czytelni jest jakis pisarz lub pisarz w kolejce
    if ((czytelnia_p->liczba_pisz > 0) || (czytelnia_p->czekajacy_pis != 0)) {
        ++(czytelnia_p->czekajacy_czyt);
        pthread_cond_wait(&(czytelnia_p->czytelnicy),
                          &(czytelnia_p->mutex));
        --(czytelnia_p->czekajacy_czyt);
    }

    ++(czytelnia_p->liczba_czyt);

    // Wpusc kolejnych czytelnikow (budza siebie nawzajem)
    pthread_cond_signal(&(czytelnia_p->czytelnicy));

    pthread_mutex_unlock(&(czytelnia_p->mutex));
}


// Odpowiednik koniec_czytania
int my_read_lock_unlock(czytelnia_t* czytelnia_p){
    pthread_mutex_lock(&(czytelnia_p->mutex));
    --(czytelnia_p->liczba_czyt);

    // Kiedy wychodzacy z czytelni czytelnik jest
    // ostatni - obudz pisarza w kolejce (uczciwosc)
    if (czytelnia_p->liczba_czyt == 0) {
        pthread_cond_signal(&(czytelnia_p->pisarze));
    }
    pthread_mutex_unlock(&(czytelnia_p->mutex));
}


// Odpowiednik chce_pisac
int my_write_lock_lock(czytelnia_t* czytelnia_p){
    pthread_mutex_lock(&(czytelnia_p->mutex));

    // Pisarz nie moze wejsc do czytelni jesli ktokolwiek inny jest w srodku (musi czekac)
    if ((czytelnia_p->liczba_pisz + czytelnia_p->liczba_czyt) > 0) {
        ++(czytelnia_p->czekajacy_pis);
        pthread_cond_wait(&(czytelnia_p->pisarze),
                          &(czytelnia_p->mutex)
                          );
        --(czytelnia_p->czekajacy_pis);
    }

    ++(czytelnia_p->liczba_pisz);
    pthread_mutex_unlock(&(czytelnia_p->mutex));
}


// Odpowiednik koniec_pisania
int my_write_lock_unlock(czytelnia_t* czytelnia_p){
    pthread_mutex_lock(&(czytelnia_p->mutex));
    --(czytelnia_p->liczba_pisz);


    // Jesli pisarz wychodzi z czytelni (a moze byc tylko jeden na raz) obudz czekajacych
    // czytelnikow (uczciwoac, zabezpieczenie przez zaglodzeniem [poprzez kolejnosc])
    if (czytelnia_p->czekajacy_czyt != 0) {
        // --(czytelnia_p->czekajacy_czyt);
        pthread_cond_signal(&(czytelnia_p->czytelnicy));
    // Jesli nie ma czytelnikow to obudz pisarzy
    } else {
        // --(czytelnia_p->czekajacy_pis);
        pthread_cond_signal(&(czytelnia_p->pisarze));
    }

    pthread_mutex_unlock(&(czytelnia_p->mutex));
}

void inicjuj(czytelnia_t* czytelnia_p){
    czytelnia_p->liczba_czyt = 0;
    czytelnia_p->liczba_pisz = 0;
    czytelnia_p->czekajacy_czyt = 0;
    czytelnia_p->czekajacy_pis = 0;
    pthread_cond_init(&(czytelnia_p->czytelnicy), NULL);
    pthread_cond_init(&(czytelnia_p->pisarze), NULL);
    pthread_mutex_init(&(czytelnia_p->mutex), NULL);
}

void czytam(czytelnia_t* czytelnia_p){
    usleep(rand()%300000);
}

void pisze(czytelnia_t* czytelnia_p){
    usleep(rand()%300000);
}


