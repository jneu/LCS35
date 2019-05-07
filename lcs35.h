#ifndef LCS35_H
#define LCS35_H

#include "challenge.h"

#include <error.h>

/*
 * Basic error reporting
 */

#define CHECK_RV(rv, str) do { if (0 != (rv)) { error_at_line (EXIT_FAILURE, 0, __FILE__, __LINE__, (str)); } } while (0)
#define CHECK_RV0(rv, str) do { if (0 == (rv)) { error_at_line (EXIT_FAILURE, 0, __FILE__, __LINE__, (str)); } } while (0)

#endif
