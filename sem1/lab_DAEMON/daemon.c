#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <time.h>
#include <syslog.h>
#include <unistd.h>

#define lockfile_dir "/var/run/daemon.pid"
#define perms (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define sleep_t 5

extern int lockfile(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return fcntl(fd, F_SETLK, &fl);
}

sigset_t mask;

void daemonize(const char* cmd)
{
    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    umask(0);

    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
    {
        printf("%s: невозможно получить максимальный номер дескриптора", cmd);
        exit(1);
    }

    if ((pid = fork()) < 0)
    {
        printf("%s: ошибка вызова fork", cmd);
        exit(1);
    }
    else if (pid != 0)
    {
        exit(0);
    }

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask); 
    sa.sa_flags = 0; 
    if (sigaction(SIGHUP, &sa, NULL) < 0)
    {
        printf("%s: невозможно игнорировать сигнал SIGHUP", cmd);
        exit(1);
    }

    if (setsid() == -1)
    {
        printf("%s: невозможно создать новую сессию", cmd);
        exit(1);
    }

    if ((pid = fork()) < 0)
    {
        printf("%s: ошибка вызова fork", cmd);
        exit(1);
    }
    else if (pid != 0)
    {
        exit(0);
    }


    if (chdir("/") < 0)
    {
        printf("%s: невозможно изменить директорию файла на /", cmd);
        exit(1);
    }

    if (rl.rlim_max == RLIM_INFINITY)
    {
        rl.rlim_max = 1024;
    }

    for (int i = 0; i < rl.rlim_max; i++)
    {
        close(i);
    }

    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    openlog(cmd, LOG_CONS, LOG_DAEMON);

    if (fd0 != 0 || fd1 != 1 || fd2 != 2)
    {
        syslog(LOG_ERR, "ошибочные файловые дескрипторы %d %d %d", fd0, fd1, fd2);
        exit(1);
    }
}

int already_running(void)
{
    char buf[16];

    int fd = open(lockfile_dir, O_RDWR | O_CREAT, perms);
    if (fd < 0)
    {
        syslog(LOG_ERR, "невозможно открыть %s: %s", lockfile_dir, strerror(errno));
        exit(1);
    }

    if (lockfile(fd) < 0)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            close(fd);
            syslog(LOG_INFO, "невозможно установить блокировку %s: %s (already locked)", lockfile_dir, strerror(errno));
            return 1;
        }

        syslog(LOG_ERR, "невозможно установить блокировку %s: %s", lockfile_dir, strerror(errno));
        exit(1);
    }

    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf) + 1);

    return 0;
}

void reread(void)
{
    syslog(LOG_INFO, "getlogin: %s", getlogin());
}

void *thr_fn(void *arg)
{
    int err, signo;

    for (;;)
    {
        err = sigwait(&mask, &signo);
        if (err != 0) {
            syslog(LOG_ERR, "ошибка вызова sigwait");
            exit(1);
        }

        switch (signo)
        {
            case SIGHUP:
                syslog(LOG_INFO, "Чтение конфигурационного файла");
                reread();
                break;

            case SIGTERM:
                syslog(LOG_INFO, "получен сигнал SIGTERM; выход");
                exit(0);

            default:
                syslog(LOG_INFO, "получен непредвиденный сигнал %d\n", signo);
        }
    }

    return (void*)0;
    
}


int main(int argc, char* argv[])
{
    daemonize(argv[0]);

    if (already_running())
    {
        syslog(LOG_ERR, "Демон уже запущен");
        exit(1);
    }

    int err;
	pthread_t tid;
    struct sigaction sa;

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)
    {
        syslog(LOG_ERR, "невозможно восстановить действие SIG_DFL для SIGHUP");
        exit(1);
    }
	sigfillset(&mask);
	if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0)
    {
        syslog(LOG_ERR, "ошибка выполнения операции SIG_BLOCK");
        exit(1);
    }
    
    err = pthread_create(&tid, NULL, thr_fn, 0);
	if (err != 0)
    {
        syslog(LOG_ERR, "невозможно создать поток");
        exit(1);
    }
    while (1)
    {
        sleep(sleep_t);
        syslog(LOG_INFO, "Демон запущен. Время: %d", time(NULL));
    }
}

