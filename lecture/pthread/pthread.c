#include <pthread.h>
#include <stdint.h>

int thread_function(char arg) {

  const char ret = strlen(arg);

  pthread_self();
}
int main(void) {

  void *ret_val = 0; // return value
  pthread_t threadId;
  // create thread
  pthread_create(threadId, NULL, *thread_function, NULL);

  // waits for termination
  pthread_join(threadId, &ret_val);

  printf("%ul", (uint64_t)ret_val);
}
