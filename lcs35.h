#ifndef LCS35_H
#define LCS35_H

#include "challenge.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Basic error reporting
 */

#define CHECK_RV(rv, str) do { if (0 != (rv)) { fprintf (stderr, "%s:%d:%s\n", __FILE__, __LINE__, (str)); exit (EXIT_FAILURE); } } while (0)
#define CHECK_RV0(rv, str) do { if (0 == (rv)) { fprintf (stderr, "%s:%d:%s\n", __FILE__, __LINE__, (str)); exit (EXIT_FAILURE); } } while (0)

#endif
