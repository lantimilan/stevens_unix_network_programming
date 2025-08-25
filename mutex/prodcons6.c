// prodcond6.c
#include "unpipc.h"

#define MAXNITEMS 1000000
#define MAXNTHREADS 100

// globals shared by threads
int nitems; // readonly by producers and consumers
int buf[MAXNITEMS]; // protected by put.mutex

struct {
  pthread_mutex_t mutex;
  int nput; // next index to store
  int nval; // next value to store
} put = {
  PTHREAD_MUTEX_INITIALIZER
};

struct {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int nready; // number ready for consumer
} nready = {
  PTHREAD_MUTEX_INITIALIZER,
  PTHREAD_COND_INITIALIZER
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

  // create all producers and one consumer
  pthread_setconcurrency(nthreads + 1);

  // starts all producer threads
  for (i = 0; i < nthreads; ++i) {
    count[i] = 0;
    pthread_create(&tid_produce[i], NULL, produce, &count[i]);
  }
  // also start consumer thread
  pthread_create(&tid_consume, NULL, consume, NULL);

  int total = 0;
  // wait for all producer threads
  for (i = 0; i < nthreads; ++i) {
    pthread_join(tid_produce[i], NULL);
    printf("count[%d] = %d\n", i, count[i]);
    total += count[i];
  }

  printf("total = %d, must match nitems = %d\n", total, nitems);
  // and wait for consumer thread
  pthread_join(tid_consume, NULL);

  exit(0);
}

// implementation of produce and consume
void *
produce(void *arg)
{
  for ( ; ; ) {
    pthread_mutex_lock(&put.mutex);
    if (put.nput >= nitems) {
      pthread_mutex_unlock(&put.mutex);
      return NULL; // buffer full, we are done
    }
    buf[put.nput] = put.nval;
    put.nput++;
    put.nval++;
    pthread_mutex_unlock(&put.mutex);

    pthread_mutex_lock(&nready.mutex);
    if (nready.nready == 0) { // we are the first one add to empty buf
      // must hold lock when signal if predictive scheduling is required, per posix
      pthread_cond_signal(&nready.cond); // signal consumer
    }
    nready.nready++;
    pthread_mutex_unlock(&nready.mutex);

    // how many items that this thread put into buf
    // this arg is per thread so do not need to be
    // in the locked region
    *((int *)arg) += 1;
  }
}

void *
consume(void *arg)
{
  int i;
  // now we need to lock before access buf
  for (i = 0; i < nitems; ++i) {
    pthread_mutex_lock(&nready.mutex);
    while (nready.nready == 0) { // need while to handle spurious wakeup
      // wait means unlock and back to condvar queue
      pthread_cond_wait(&nready.cond, &nready.mutex);
    }
    // we must have mutex at this line, so we can access nready
    --nready.nready;
    pthread_mutex_unlock(&nready.mutex);

    // no lock needed because buf[i] has been written by some producer
    // and will never be written by anyone again
    if (buf[i] != i) {
      // consumer inspects buf and prints bad values if any.
      // a correct implementation should not emit any line from here.
      printf("buf[%d] = %d\n", i, buf[i]);
    }
  }
  return NULL;
}

// this is similar to signal
// pthread_cond_broadcast(pthread_cond_t* cptr); 
//
// timed wait, returns ETIMEOUT if wait timeout
// pthread_cond_timedwait(pthread_cond_t* cptr, pthread_mutex_t *mptr,
//                          const struct timespec *abstime);
// struct timespec {
//   time_t tv_sec; // seconds
//   long tv_nsec; // nanoseconds
// }

// mutex and condvar attributes
//int pthread_mutex_init(pthread_mutex_t *mptr, const pthread_mutexattr_t *attr);
//int pthread_mutex_destroy(pthread_mutex_t *mptr);
//int pthread_cond_init(pthread_cond_t *cptr, const pthread_condattr_t *attr);
//int pthread_cond_destroy(pthread_cond_t *cptr);
