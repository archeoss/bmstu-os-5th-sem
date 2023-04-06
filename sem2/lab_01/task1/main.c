#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char **argv) {
  int sockets[2];
  char buf[BUF_SIZE];
  char *msg[3] = {"Message1", "Message2", "Message3"};
  int pid;
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) {
    perror("socketpair() failed");
    exit(1);
  }
  for (int i = 0; i < 3; ++i) {
    if ((pid = fork()) == -1) {
      perror("fork() failed");
      exit(1);
    } else if (pid == 0) {
      // close(sockets[0]);
      snprintf(buf, BUF_SIZE, "message from child %d", getpid());
      printf("child %d write: %s\n", getpid(), buf);
      sleep(2 * i);
      write(sockets[1], buf, sizeof(buf));
      read(sockets[1], buf, sizeof(buf));
      printf("child %d recieve: %s\n", getpid(), buf);
      // close(sockets[1]);
      break;
    }
  }
  if (pid) {
    // close(sockets[1]);
    for (int i = 0; i < 3; ++i) {
      // sleep(2 * i);
      read(sockets[0], buf, sizeof(buf));
      printf("parent recieve: %s \n", buf);
      snprintf(buf, BUF_SIZE, "message from parent %d", getpid());
      write(sockets[0], buf, sizeof(buf));
    printf("parent write: %s\n", buf);
    }
    // close(sockets[0]);
  }
}
