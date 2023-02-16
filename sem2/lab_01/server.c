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
  int server_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (server_fd == -1) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  // задаем имя сокету
  struct sockaddr_un server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_UNIX;
  strncpy(server_addr.sun_path, SOCKET_NAME, sizeof(server_addr.sun_path) - 1);

  // привязываем сокет к имени
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // прослушиваем сокет
  char buffer[BUFFER_SIZE];
  while (1) {
    int len = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, NULL, NULL);
    if (len == -1) {
      break;
    }
    buffer[len] = '\0';
    printf("Received message: %s\n", buffer);
  }

  // закрываем сокет
  unlink(SOCKET_NAME);
  close(server_fd);

  return 0;
}
