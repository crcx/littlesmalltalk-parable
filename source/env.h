/************************************************************************
 * A Little Smalltalk                                           Version 3
 *
 * This code is gifted to the public domain. Share freely.
 *
 * This include file gathers together environmental factors that are
 * likely to change from one C compiler to another, or from one system
 * to another.
 ************************************************************************/

/* define custom data types */
typedef int      boolean;
typedef unsigned char byte;


#define true  1
#define false 0

#define byteToInt(b) (b)
#define longCanBeInt(l) ((l >= -16383) && (l <= 16383))
#define streq(a,b) (strcmp(a,b) == 0)
#define ignore (void)
#define noreturn void


/* Prototypes */
#define X ,
#define OBJ object
#define OBJP object *
#define INT int
#define BOOL boolean
#define STR char *
#define FLOAT double
#define NOARGS void
#define FILEP FILE *
#define FUNC ()
