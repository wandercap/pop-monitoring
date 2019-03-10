#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdlib.h>

inline static void err_exit(char *msg) {
  printf("[Fatal Error]: %s \nExiting... \n", msg);
  exit(1);
}

#endif