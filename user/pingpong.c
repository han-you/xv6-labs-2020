#include "kernel/types.h"
#include "user/user.h"
#define RD 0
#define WR 1
int main(int argc, char const *argv[])
{
  char buf = 'p';
  int fd_c2p[2];
  int fd_p2c[2];
  pipe(fd_c2p);
  pipe(fd_p2c);
  int exit_state = 0;
  int pid = fork();

  if (pid < 0)
  {
    fprintf(2, "fork() error!\n");
    close(fd_c2p[RD]);
    close(fd_c2p[WR]);
    close(fd_p2c[RD]);
    close(fd_p2c[WR]);
    exit(1);
  }
  else if (pid == 0)
  { // 子进程
    close(fd_p2c[WR]);
    close(fd_c2p[RD]);

    if (read(fd_p2c[RD], &buf, sizeof(char)) != sizeof(char))
    {
      printf("child read() error!\n");
      exit_state = 1;
    }
    else
    {
      printf("%d: received ping\n", getpid());
    }

    if (write(fd_c2p[WR], &buf, sizeof(char)) != sizeof(char))
    {
      printf("child write() error!\n");
      exit_state = 1;
    }
    close(fd_c2p[WR]);
    close(fd_p2c[RD]);
    exit(exit_state);
  }
  else // 父进程
  {
    close(fd_c2p[WR]);
    close(fd_p2c[RD]);

    if (write(fd_p2c[WR], &buf, sizeof(char)) != sizeof(char))
    {
      printf("parent write() error!\n");
      exit_state = 1;
    }

    if (read(fd_c2p[RD], &buf, sizeof(char)) != sizeof(char))
    {
      printf("parent read() error!\n");
      exit_state = 1;
    }
    else
    {
      printf("%d: received pong\n", getpid());
    }
    close(fd_c2p[RD]);
    close(fd_p2c[WR]);
    exit(exit_state);
  }
}
