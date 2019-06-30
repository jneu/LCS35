#ifndef LCS35_H
#define LCS35_H

#include <assert.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gmp.h>

#include "challenge.h"

/*
 * Basic error reporting
 */

#define ASSERT_FATAL(b, str) \
  do \
    { \
      if (!(b)) \
        { \
          fprintf (stderr, "%s:%d:%s\n", __FILE__, __LINE__, (str)); \
          exit (EXIT_FAILURE); \
        } \
    } \
  while (0)

#endif
