#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/demo_socket.sock"
#define BUFFER_SIZE 1024

int main() {
  // создаем сокет
  int client_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (client_fd == -1) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  srand(time(NULL) + getpid());
  // задаем имя сокету
  struct sockaddr_un client_addr;
  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sun_family = AF_UNIX;
  strncpy(client_addr.sun_path, SOCKET_NAME, sizeof(client_addr.sun_path) - 1);

  // отправляем сообщение на сервер
  char buffer[BUFFER_SIZE];
  for (int i = 0; i < 10; i++) {
    int sleep_time = rand() % 4 + 1;
    sleep(sleep_time);

    snprintf(buffer, BUFFER_SIZE, "message %d from client %d", i, getpid());
    int len = strlen(buffer);
    if (sendto(client_fd, buffer, len, 0, (struct sockaddr *)&client_addr,
               sizeof(client_addr)) == -1) {
      perror("sendto failed");
      exit(EXIT_FAILURE);
    }
    printf("Sent message: %s\n", buffer);
  }

  // закрываем сокет
  close(client_fd);

  return 0;
}
