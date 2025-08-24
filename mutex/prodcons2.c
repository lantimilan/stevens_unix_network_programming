// prodcons2.c
#include "unpipc.h"

#define MAXNITEMS 1000000
#define MAXNTHREADS 100

int nitems; // read-only by producers and consumers
struct {
  pthread_mutex_t mutex;
  int buf[MAXNITEMS];
  int nput;
  int nval;
} shared = {
  PTHREAD_MUTEX_INITIALIZER
};

void *produce(void*), *consume(void*);

int main(int argc, char* argv[])
{
  int i, nthreads, count[MAXNTHREADS];
  pthread_t tid_produce[MAXNTHREADS], tid_consume;
  
  if (argc != 3)
    err_quit("usage: prodcons2 <#items> <#threads>");
  nitems = min(atoi(argv[1]), MAXNITEMS);
  nthreads = min(atoi(argv[2]), MAXNTHREADS);

  pthread_setconcurrency(nthreads);

  // starts all producer threads
  for (i = 0; i < nthreads; ++i) {
    count[i] = 0;
    pthread_create(&tid_produce[i], NULL, produce, &count[i]);
  }

  int total = 0;
  // wait for all producer threads
  for (i = 0; i < nthreads; ++i) {
    pthread_join(tid_produce[i], NULL);
    printf("count[%d] = %d\n", i, count[i]);
    total += count[i];
  }

  printf("total = %d, must match nitems = %d\n", total, nitems);

  // consumer thread
  pthread_create(&tid_consume, NULL, consume, NULL);
  pthread_join(tid_consume, NULL);

  exit(0);
}

// implementation of produce and consume
void *
produce(void *arg)
{
  for ( ; ; ) {
    pthread_mutex_lock(&shared.mutex);
    if (shared.nput >= nitems) {
      pthread_mutex_unlock(&shared.mutex);
      return NULL; // buffer full, we are done
    }
    shared.buf[shared.nput] = shared.nval;
    shared.nput++;
    shared.nval++;
    pthread_mutex_unlock(&shared.mutex);
    // how many items that this thread put into buf
    *((int *)arg) += 1;
  }
}

void *
consume(void *arg)
{
  int i;
  for (i = 0; i < nitems; ++i) {
    if (shared.buf[i] != i) {
      // consumer inspects buf and prints bad values if any.
      // a correct implementation should not emit any line from here.
      printf("buf[%d] = %d\n", i, shared.buf[i]);
    }
  }
  return NULL;
}
