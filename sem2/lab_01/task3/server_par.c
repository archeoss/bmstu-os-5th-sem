
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>
#define SOCKET_NAME "/tmp/socket.sock"

int main(int argc, char *argv[]) {
  int sockfd;
  struct sockaddr_un server_addr, client_addr;
  char buffer[256];
  int n;

  // создаем сокет
  sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  // задаем адрес сервера
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_UNIX;
  strncpy(server_addr.sun_path, SOCKET_NAME, sizeof(server_addr.sun_path) - 1);

  // привязываем сокет к адресу сервера
  // unlink(SOCKET_NAME);
  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  srand(time(NULL) + getpid());
  // читаем сообщения от клиентов и отправляем им ответы
    int sleep_time = rand() % 4 + 1;
    sleep(sleep_time);
    socklen_t client_len = sizeof(client_addr);
    n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                 (struct sockaddr *)&client_addr, &client_len);
    if (n < 0) {
      perror("recvfrom failed");
      exit(EXIT_FAILURE);
    }

    printf("Received message: %s\n", buffer);

    // отправляем ответ клиенту
    snprintf(buffer, sizeof(buffer), "Message from server, pid=%d", getpid());
    if (sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&client_addr, client_len) < 0) {
      perror("sendto failed");
      exit(EXIT_FAILURE);
  }

  return 0;
}
