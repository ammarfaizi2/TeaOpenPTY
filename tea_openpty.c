
/**
 * @author Ammar Faizi <ammarfaizi2@gmail.com> https://www.facebook.com/ammarfaizi2
 * @license MIT
 */

#include <pty.h>
#include <poll.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#include "tea_openpty.h"


/**
 * @param int fd
 * @return int
 */
inline static int
set_nonblocking(int fd)
{
  int flags;
  /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
  /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
  if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
      flags = 0;
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
  /* Otherwise, use the old way of doing it */
  flags = 1;
  return ioctl(fd, FIOBIO, &flags);
#endif
}


/**
 * @param char *const argv[]
 * @param int  pipe_write_fd
 * @return void
 */
inline static void
execute_bin(char *const argv[], int pipe_write_fd)
{
  if (execvp(argv[0], argv) == -1) {
    int ret;
    ssize_t wlen;
    char buf[255] = {0};
    ret  = snprintf(buf, sizeof(buf), "execvp(%s): %s",
                    argv[0], strerror(errno));
    wlen = write(pipe_write_fd, buf, ret + 1);
    (void)wlen;
  }
}


#define CLOSE_FD(FD)            \
do {                            \
  if ((FD) != -1) close((FD));  \
} while (0)


/**
 * @param tea_openpty_t *pty_data
 * @return int
 */
int
tea_openpty(tea_openpty_t *pty_data)
{
  pid_t pid;
  int retval      = 0;
  int aslave      = -1;
  int amaster     = -1;
  int pipe_fd[2]  = {-1, -1};
  int tmp_stdout  = -1;
  const char *errp = NULL;

  /* Poll variables. */
  nfds_t nfds = 2;
  struct pollfd fds[2] = {0};


  /*
   * Init pty.
   */
  if (openpty(&amaster, &aslave, NULL, NULL, NULL) == -1) {
    errp = "openpty";
    goto error;
  }


  /*
   * Open a pipe, this is used in child process to communicate
   * with parent process.
   *
   * pipe_fd[0] is for read operation.
   * pipe_fd[1] is for write operation.
   */
  if (pipe(pipe_fd) == -1) {
    errp = "pipe";
    goto error;
  }


  /*
   * Duplicate parent process STDOUT fd and
   * update current STDOUT fd with aslave.
   */
  tmp_stdout = dup(STDOUT_FILENO);
  if (tmp_stdout == -1) {
    errp = "dup";
    goto error;
  }
  if (dup2(aslave, STDOUT_FILENO) == -1) {
    errp = "dup2";
    goto error;
  }


  /*
   * Spawn child process.
   */
  pid = fork();
  if (pid == -1) {
    errp = "fork";
    goto error;
  }


  /*
   * Handle the child process.
   */
  if (pid == 0) {
    goto child;
  }


  /**
   * Set read fd(s) to be non blocking.
   */
  if (set_nonblocking(amaster) == -1) {
    errp = "set_nonblocking";
    goto error;
  }
  if (set_nonblocking(pipe_fd[0]) == -1) {
    errp = "set_nonblocking";
    goto error;
  }

  fds[0].fd     = amaster;
  fds[0].events = POLLIN;
  fds[1].fd     = pipe_fd[0];
  fds[1].events = POLLIN;

  while (true) {
    int ret = poll(fds, nfds, pty_data->poll_timeout);

    if (waitpid(pid, NULL, WNOHANG) == pid) {
      /* Child has been terminated. */
      break;
    }

    if (ret == 0) {
      /* poll reached timeout. */
      continue;
    }

    if (ret == -1) {
      /* poll returned error. */
      errp = "poll";
      goto error;
    }

    if ((fds[1].revents & POLLIN) != 0) {
      /* Error from child process. */
      ssize_t rlen;

      rlen = read(pipe_fd[0], pty_data->error, 255);
      if (rlen == -1) {
        if (errno == EAGAIN) {
          continue;
        } else {
          goto error;
        }
      }

      retval = -1;
      goto ret;
    }

    char    buf[4096];
    ssize_t rlen;
    ssize_t wlen;

    rlen = read(amaster, buf, 4096);
    if (rlen == -1) {
      if (errno == EAGAIN)
        continue;
      else
        goto error;
    }

    wlen = write(tmp_stdout, buf, (size_t)rlen);
    (void)wlen;
  }

ret:

  if (tmp_stdout != -1) {
    /* Synchronize. */
    fsync(STDOUT_FILENO);

    /* Restore stdout. */
    dup2(tmp_stdout, STDOUT_FILENO);
  }

  /* Clean up fd. */
  CLOSE_FD(aslave);
  CLOSE_FD(amaster);
  CLOSE_FD(pipe_fd[0]);
  CLOSE_FD(pipe_fd[1]);
  return retval;

error:
  retval = -1;
  snprintf(pty_data->error, sizeof(pty_data->error),
           "%s: %s", errp, strerror(errno));
  goto ret;

child:
  {
    execute_bin(pty_data->argv, pipe_fd[1]);
    exit(0);
  }
  __builtin_unreachable();
  /* Shut GCC up. */
  return -1;
}
