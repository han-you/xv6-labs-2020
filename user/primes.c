#include "kernel/types.h"
#include "user/user.h"
#define RD 0
#define WR 1
int lpipe_first_data(int fd[2], int *ft)
{
  if (read(fd[RD], ft, sizeof(int)) == sizeof(int))
  {
    printf("prime %d\n", *ft);
    return 0;
  }
  return -1;
}

void transmit_data(int lpipe[2], int rpipe[2], int first)
{
  int data;
  while (read(lpipe[RD], &data, sizeof(int)) == sizeof(int))
  {
    if (data % first != 0)
      write(rpipe[WR], &data, sizeof(int));
  }
  close(lpipe[RD]);
  close(rpipe[WR]);
}
void primes(int fd[2])
{
  close(fd[WR]);
  int num;
  if (lpipe_first_data(fd, &num) == 0)
  {
    int p[2];
    pipe(p);
    transmit_data(fd, p, num);
    if (fork() == 0)
    {
      primes(p);
    }
    else
    {
      close(p[RD]);
      wait(0);
    }
  }
  exit(0);
}
int main()
{
  int fd[2];
  pipe(fd);
  for (int i = 2; i <= 35; i++)
    write(fd[WR], &i, sizeof(int));

  if (fork() == 0)
  {
    primes(fd);
  }
  else
  {
    close(fd[RD]);
    close(fd[WR]);
    wait(0);
  }
  exit(0);
}