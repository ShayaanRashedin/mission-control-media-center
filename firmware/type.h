/* Define shared data types. */

/* Includes */
#include <stdint.h>

#ifndef __TYPE_H__
/* Definitions */
#define __TYPE_H__

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (1)
#endif

/* Data types */
typedef enum { RESET = 0, SET = !RESET } FlagStatus, ITStatus;
typedef enum { DISABLE = 0, ENABLE = !DISABLE } FunctionalState;

#endif
