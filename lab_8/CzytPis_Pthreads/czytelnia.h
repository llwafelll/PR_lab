#ifndef _czytelnia_
#define _czytelnia_

/*** Definicje typow zmiennych ***/
typedef struct {
  int liczba_czyt, liczba_pisz;
  int czekajacy_czyt, czekajacy_pis;
  pthread_cond_t czytelnicy, pisarze;
  pthread_mutex_t mutex;
} czytelnia_t;

/*** Deklaracje procedur interfejsu ***/
void inicjuj(czytelnia_t* czytelnia_p);
void czytam(czytelnia_t* czytelnia_p);
void pisze(czytelnia_t* czytelnia_p);

int my_read_lock_lock(czytelnia_t* czytelnia_p);
int my_read_lock_unlock(czytelnia_t* czytelnia_p);
int my_write_lock_lock(czytelnia_t* czytelnia_p);
int my_write_lock_unlock(czytelnia_t* czytelnia_p);

#endif
