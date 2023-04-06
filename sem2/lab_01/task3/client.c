#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
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

  // задаем адрес клиента
  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sun_family = AF_UNIX;
  sprintf(client_addr.sun_path, "/tmp/client%d", getpid());

  // привязываем сокет к адресу клиента
  unlink(client_addr.sun_path);
  if (bind(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  srand(time(NULL) + getpid());
  // задаем адрес сервера
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_UNIX;
  strncpy(server_addr.sun_path, SOCKET_NAME, sizeof(server_addr.sun_path) - 1);

  // отправляем сообщения серверу и читаем ответы
    int sleep_time = rand() % 4 + 1;
    sleep(sleep_time);
    snprintf(buffer, sizeof(buffer), "Message from client: pid=%d", getpid());

    // отправляем сообщение серверу
    if (sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
      perror("sendto failed");
      exit(EXIT_FAILURE);
    }

    // читаем ответ от сервера
    n = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
    if (n < 0) {
      perror("recvfrom failed");
      exit(EXIT_FAILURE);
    }

    printf("Received message: %s\n", buffer);
  close(client_addr);
  // unlink(client_addr);
  return 0;
}
