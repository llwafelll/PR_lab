#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

void * foo(void* id) {
  pthread_t system_tid = pthread_self();
  int custom_id = *((int*)id);

  printf("\ncustom thread id: %015d\nsystem thread id: %lu\n",
      custom_id, system_tid);
}

static const int N_THREADS = 10;

int main() {
  pthread_t tids[N_THREADS];
  int ids[N_THREADS];

  int i;

  for (i = 0; i < N_THREADS; ++i)
    ids[i] = i + 1;

  for (i = 0; i < N_THREADS; ++i)
    pthread_create(&tids[i], NULL, foo, (void*) &ids[i]);

  for (i = 0; i < N_THREADS; ++i)
    pthread_join(tids[i], NULL);

  return 0;
}

