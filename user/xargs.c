#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define LINESIZE 1024

int readline(char *line) {
  char *eline = line;
  while (read(0, eline, 1)) {
    if (*eline == '\n') {
      *eline = 0;
      return 1;
    }
    eline++;
  }
  return 0;
}

int main(int argc, char *argv[])
{
  char *exec_argv[MAXARG], line[LINESIZE];

  // argv[0] is xargs
  // argv[1] is cmdpath to exec
  char *path = argv[1];
  int i;
  for (i = 1; i < argc; i++) {
    exec_argv[i - 1] = argv[i];
  }
  int exec_argc = argc - 1;
  while(readline(line)) {
    if(fork() == 0) {
      char *p0 = line, *p1 = line, *el = line + strlen(line) + 1;
      while(p1 != el) {
        if ((*p1 == ' ') | (*p1 == '\0')) {
          *p1 = 0;
          exec_argv[exec_argc++] = p0;
          p0 = p1 + 1;
        }
        p1++;
      }
      exec_argv[exec_argc] = 0;
      exec(path, exec_argv);
    }
    wait(0);
  }

  exit(0);
}