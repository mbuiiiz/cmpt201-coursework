#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

pthread_mutex_t mutex0 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

static void *thread0(void *arg) {
  pthread_mutex_lock(&mutex0);
  printf("lock0\n");
  pthread_mutex_lock(&mutex1);
  printf("lock1\n");
  pthread_mutex_unlock(&mutex1);
  pthread_mutex_unlock(&mutex0);
  printf("unlock both\n");
  pthread_exit(0);
}

static void *thread1(void *arg) {
  pthread_mutex_lock(&mutex1);
  printf("lock1\n");
  pthread_mutex_lock(&mutex0);
  printf("lock0\n");
  pthread_mutex_unlock(&mutex1);
  pthread_mutex_unlock(&mutex0);
  printf("unlock both\n");
  pthread_exit(0);
}

int main(int argc, char *argv[]) {
  pthread_t t0;

  pthread_t t1;

  pthread_create(&t0, NULL, thread0, NULL);

  pthread_create(&t1, NULL, thread1, NULL);

  pthread_join(t0, NULL);
  pthread_join(t1, NULL);
}
