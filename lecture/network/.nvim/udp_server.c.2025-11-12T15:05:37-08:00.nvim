// server

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main() {
  printf("SERVER:\n");

  // socket
  int socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if(socket_fd == -1){
      perror("Socket failed");
      exit(EXIT_FAILURE);
  }

  // remove the existing socket if any
  if(remove("socket_fun") == -1 && errno != ENOENT){
    perror("remove");
    exit(EXIT_FAILURE);
  }

  // bind 
  struct sockaddr_un sockstruct;
  sockstruct.sun_family = AF_UNIX;
  snprintf(sockstruct.sun_path, 100, "da udp socket");
  char sun_path[100];
  
  if(bind(socket_fd, &sockstruct, sizeof(struct sockaddr_un)) == -1){
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // listen
  if(listen(socket_fd, 10) == -1){
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // accept
  while(1){

    // read
    const int SIZE = 1024;
    char buff[SIZE];

    struct sockaddr_un return_address;
    socklen_t retaddr_size = sizeof(struct sockaddr_un);
    memset(&return_address, 0, sizeof(struct socketaddr_un));

    ssize_t ans = recvfrom(socket_fd, buff, SIZE, 0, (struct socketddr*) &return_address,
        &retaddr_size);

    if(ans == -1){
     perror("Recvfrom");
     _exit(1);
    }
    int bytes_read;
    while((bytes_read = read(connected_fd, buff, SIZE)) > 0){ 
     // write a message to screen
     write(1, buff, bytes_read);
    }

    if(bytes_read == -1){
      perror("read");
      exit(EXIT_FAILURE);
    }

   int connected_fd = accept(socket_fd, NULL, NULL); 
    if(connected_fd == -1){
        perror("accept");
        exit(EXIT_FAILURE);
    } 
  

    close(connected_fd);
  }
  // close
  close(socket_fd);

  return 0;
}
