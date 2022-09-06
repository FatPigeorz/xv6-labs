#include "kernel/types.h"
#include "user/user.h"

#define PRIME(X) printf("prime %d\n", (X))
#define PWR 1
#define PRD 0

void pipe_num(int* p0) {
  int start;
  if (! read(p0[PRD], &start, sizeof(int) / sizeof(char))) {
    close(p0[PRD]);
    exit(0);
  };
  PRIME(start);

  int p1[2];        // p0: in, p1: out
  if (pipe(p1) < 0) {
    fprintf(2, "pipe error\n");
  }

  int pid = fork();
  if (pid < 0) {
    fprintf(2, "fork error\n");
  }
  else if (0 == pid) {
    // child process
    close(p0[PRD]);
    close(p1[PWR]);
    pipe_num(p1);
  }
  else {
    // parent process
    int rd;
    close(p1[PRD]);
    while(read(p0[PRD], &rd, sizeof(int) / sizeof(char))) {
      if (rd % start != 0) {
        write(p1[PWR], &rd, sizeof(int) / sizeof(char));
      }
    }
    close(p1[PWR]);
    close(p0[PRD]);
    wait((void *)0);
  }
  exit(0);
}

void prime(int start, int end) {
  int p1[2];        // p0: in, p1: out
  if (pipe(p1) < 0) {
    fprintf(2, "pipe error\n");
  }
  PRIME(start);
  
  int pid = fork();
  if (pid < 0) {
    fprintf(2, "fork error\n");
  }
  else if (pid == 0) {
    close(p1[PWR]);
    pipe_num(p1);
  }
  else {
    close(p1[PRD]);
    for (int i = start + 1; i <= end; i++) {
      if (i % start != 0) {
        write(p1[PWR], &i, sizeof(int) / sizeof(char));
      }
    }
    close(p1[PWR]);
    wait((void *)0);
  }
  exit(0);
}

int
main() {
  prime(2, 35);
  exit(0);
}