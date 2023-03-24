#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv)
{
  if(argc < 2){
    printf("Please input the time you want to stop\n");
    exit(1);
  }
  int value = atoi(argv[1]);
  sleep(value);

  exit(0);
}
