#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdbool.h>
#include <stdio.h>

#define CHECK_OOM(p) do {                               \
                        if (!p) {                       \
                          printf("Out of memory!\n");   \
                          exit(1);                      \
                        }                               \
                      } while (false)
#endif