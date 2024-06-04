#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc, char *argv[])
{
  if (argc <= 1)
  {
    fprintf(2, "usage: sleep pattern [file ...]\n");
    exit(1);
  }
  for (int i = 1; i < argc; i++)
  {
    int count = atoi(argv[i]);
    sleep(count);
    // printf("%d\n", count);
  }
  exit(0);
}