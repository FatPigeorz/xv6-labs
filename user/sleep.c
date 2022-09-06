#include "kernel/types.h"
#include "user/user.h"

const char *err_msg = "Argument Error:no argument";

int
main(int argc, char* argv[]) {
  if (argc < 2) {
    write(1, err_msg, strlen(err_msg));
    exit(0);
  }
  int arg = atoi(argv[1]);
  sleep(arg);
  exit(0);
}
