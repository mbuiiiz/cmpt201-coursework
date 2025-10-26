#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void *my_thread(void *arg) {
  char *message = arg;
  printf("Inside a thread!\n");
  printf("Message is: %s\n", message);
  return (void *)strlen(message);
}

int main(void) {

  printf("Hello World\n");
  pthread_t thread_id;

  pthread_create(&thread_id, NULL, my_thread, "Hello threads");

  void *ans;
  pthread_join(thread_id, &ans);
  /*sleep(5);*/
  long long len = (long long)ans;
  printf("Done: asnwer is %lld .\n", len);
}
