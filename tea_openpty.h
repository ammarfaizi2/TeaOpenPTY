
/**
 * @author Ammar Faizi <ammarfaizi2@gmail.com> https://www.facebook.com/ammarfaizi2
 * @license MIT
 */
#ifndef OPENPTY_H
#define OPENPTY_H


typedef struct _tea_openpty_t {
  int         poll_timeout;
  char        error[255];
  char        **argv;
  char        *app;
} tea_openpty_t;

/**
 * @param tea_openpty_t *pty_data
 * @return int
 */
int
tea_openpty(tea_openpty_t *pty_data);

#endif
