// client

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

int main() {
  printf("CLIENT:\n");

  // socket
  int socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if(socket_fd == -1){
      perror("Socket failed");
      exit(EXIT_FAILURE);
  }

  // create destination addr
  struct sockaddr_un addr; 
  memset(&addr, 0, sizeof(struct sockaddr_un)); // empty it 
  addr.sun_family = AF_UNIX;
  snprintf(addr.sun_path, 108, "da udp socket");

  // write a message to screen
  char *msg = "I am sending this";

  int bytes_written = sendto(socket_fd, msg, strlen(msg), 0, (struct sockaddr*)&addr, sizeof(struct sockaddr_un));

  if(bytes_written == -1){
    perror("sendto");
    _exit(1);
  } 
  // close
  close(socket_fd);

  return 0;
}
