#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define WR 1
#define RD 0

int 
main() {
  char buf[512] = {'W'};
  int pc[2], cp[2]; // p[0]: writing, p[1]: reading
  pipe(pc);
  pipe(cp);
  int pid = fork();
  if (pid < 0) {
    // error
    fprintf(2, "fork error\n");
    close(pc[RD]);
    close(pc[WR]);
    close(cp[RD]);
    close(cp[WR]);
    exit(1);
  }
  else if (pid == 0) {
    // child process
    close(cp[RD]);
    close(pc[WR]);

    read(pc[RD], buf, 1);  // it will wait until parent process write to p[0]
    close(pc[RD]);
    fprintf(1, "%d: received ping\n", getpid());
    write(cp[WR], buf, 1);
    close(cp[WR]);
  }
  else {
    // parent process
    close(pc[RD]);
    close(cp[WR]);

    write(pc[WR], buf, 1);
    close(pc[WR]);
    read(cp[RD], buf, 1);
    close(cp[RD]);
    fprintf(1, "%d: received pong\n", getpid());
  }

  exit(0);
}