/*
Questions to answer at top of client.c:
(You should not need to change the code in client.c)

1. What is the address of the server it is trying to connect to (IP address and
port number). IP address is 127.0.0.1 and port number is 8000

2. Is it UDP or TCP? How do you know?
It is TCP because socket is created with socket(AF_INET, SOCK_STREAM, 0);

3. The client is going to send some data to the server. Where does it get this
data from? How can you tell in the code? Standard input because the program used
read(STDIN_FILENO, buf, BUF_SIZE)

4. How does the client program end? How can you tell that in the code?
Normal termination happens after the read loop ends (return -1) then the code
calls close(sfd) and exit(EXIT_SUCCESS);


*/

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8000
#define BUF_SIZE 64
#define ADDR "127.0.0.1"

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

int main() {
  struct sockaddr_in addr;
  int sfd;
  ssize_t num_read;
  char buf[BUF_SIZE];

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, ADDR, &addr.sin_addr) <= 0) {
    handle_error("inet_pton");
  }

  int res = connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
  if (res == -1) {
    handle_error("connect");
  }

  while ((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 1) {
    if (write(sfd, buf, num_read) != num_read) {
      handle_error("write");
    }
    printf("Just sent %zd bytes.\n", num_read);
  }

  if (num_read == -1) {
    handle_error("read");
  }

  close(sfd);
  exit(EXIT_SUCCESS);
}

#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 64
#define PORT 8000
#define LISTEN_BACKLOG 32

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

// Shared counters for: total # messages, and counter of clients (used for
// assigning client IDs)
int total_message_count = 0;
int client_id_counter = 1;

// Mutexs to protect above global state.
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_id_mutex = PTHREAD_MUTEX_INITIALIZER;

struct client_info {
  int cfd;
  int client_id;
};

void *handle_client(void *arg) {
  struct client_info *client = (struct client_info *)arg;
  char buf[BUF_SIZE];
  ssize_t nread;

  // TODO: print the message received from client
  // TODO: increase total_message_count per message

  while (1) {
    nread = recv(client->cfd, buf, BUF_SIZE - 1, 0);

    if (nread == -1) {
      perror("recv");
      break;
    } else if (nread == 0) {
      // connection closed by client;
      break;
    }

    if (nread >= 0 && nread < BUF_SIZE) {
      buf[nread] = '\0';
    } else {
      buf[BUF_SIZE - 1] = '\0';
    }

    // icrease total message count
    if (pthread_mutex_lock(&count_mutex) != 0) {
      perror("pthread_mutex_lock");
      break;
    } else {
      total_message_count++;
      int current_total = total_message_count;
      if (pthread_mutex_unlock(&count_mutex) != 0) {
        perror("pthread_mutex_lock");
      }

      printf("Msg #%4d; Client ID %d: %s", current_total, client->client_id,
             buf);
      fflush(stdout);
    }
  }

  // disconnect
  printf("Ending thread for client %d\n", client->client_id);
  fflush(stdout);

  close(client->cfd);
  free(client);

  return NULL;
}

int main() {
  struct sockaddr_in addr;
  int sfd;

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    handle_error("bind");
  }

  if (listen(sfd, LISTEN_BACKLOG) == -1) {
    handle_error("listen");
  }

  for (;;) {
    // TODO: create a new thread when a new connection is encountered
    // TODO: call handle_client() when launching a new thread, and provide
    struct client_info *ci = malloc(sizeof(*ci));
    if (!ci) {
      fprintf(stderr, "malloc failed\n");
      continue;
    }

    socklen_t addrlen = sizeof(struct sockaddr_in);
    ci->cfd = accept(sfd, (struct sockaddr *)&addr, &addrlen);
    if (ci->cfd == -1) {
      perror("accept");
      free(ci);
      continue;
    }

    if (pthread_mutex_lock(&client_id_mutex) != 0) {
      perror("pthread_mutex_lock");
      ci->client_id = client_id_counter++;
    } else {
      ci->client_id = client_id_counter++;
      if (pthread_mutex_unlock(&client_id_mutex) != 0) {
        perror("pthread_mutex_unlock");
      }
    }

    printf("New client created! ID %d on socket FD %d\n", ci->client_id,
           ci->cfd);
    fflush(stdout);

    pthread_t tid;
    if (pthread_create(&tid, NULL, handle_client, ci) != 0) {
      perror("pthread_create");
      close(ci->cfd);
      free(ci);
      continue;
    }

    if (pthread_detach(tid) != 0) {
      perror("pthread_detach");
    }
  }

  if (close(sfd) == -1) {
    handle_error("close");
  }

  return 0;
}
