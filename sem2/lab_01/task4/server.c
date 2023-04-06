#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define port 9877 
#define MAX_CONN 16
#define MAX_EVENTS 32
#define BUF_SIZE 22

void epoll_ctl_add(int epfd, int fd, uint32_t events) {
  struct epoll_event ev;
  ev.events = events;
  ev.data.fd = fd;
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
    perror("epoll_ctl() error\n");
    exit(1);
  }
}

int set_non_blocking(int sockfd) {
  if (fcntl(sockfd, F_SETFD, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK) == -1) {
    return -1;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);
  int epollfd, nfds;
  int listen_sock, conn_sock;
  int socklen;
  struct sockaddr_in srv_addr, cli_addr;
  struct epoll_event events[MAX_EVENTS];
  char buf[BUF_SIZE];

  if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket() failed\n");
    exit(1);
  }
  memset(&srv_addr, 0, sizeof(srv_addr));
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_addr.s_addr = INADDR_ANY;
  srv_addr.sin_port = htons(port);
  if ((bind(listen_sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr))) ==
      -1) {
    perror("bind() failed\n");
    exit(1);
  }
  set_non_blocking(listen_sock);
  if (listen(listen_sock, MAX_CONN) == -1) {
    perror("listen() error");
    exit(1);
  }
  if ((epollfd = epoll_create(1)) == -1) {
    perror("epoll_create() failed");
    exit(1);
  }

  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = listen_sock;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
    perror("epoll_ctl() failed");
    exit(1);
  }

  socklen = sizeof(cli_addr);
  for (;;) {
    if ((nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1)) == -1) {
      perror("epoll_wait() failed\n");
      exit(1);
    }
    for (int i = 0; i < nfds; i++) {
      if (events[i].data.fd == listen_sock) {
        if ((conn_sock = accept(listen_sock, (struct sockaddr *)&cli_addr,
                                (unsigned int *)&socklen)) == -1) {
          perror("accept() fail");
          exit(1);
        }
        sleep(2);
        set_non_blocking(conn_sock);
        epoll_ctl_add(epollfd, conn_sock,
                      EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
      } else if (events[i].events & EPOLLIN) {
        memset(buf, 0, sizeof(buf));
        int len;
        if ((len = read(events[i].data.fd, buf, sizeof(buf))) > 0) {
          printf("data: %s\n", buf);
          char msg[BUF_SIZE];
          snprintf(msg, sizeof(buf) + 3, "SERVER PID = %d", getpid());
          write(conn_sock, msg, sizeof(msg));
          printf("send: %s\n", msg);
        }
      } else {
        printf("unexpected\n");
      }

      if (events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
        printf("connection closed\n");
        epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
        close(events[i].data.fd);
      }
    }
  }
  return 0;
}
