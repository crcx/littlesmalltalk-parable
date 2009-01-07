/************************************************************************
 * A Little Smalltalk                                           Version 3
 *
 * This code is gifted to the public domain. Share freely.
 *
 * This include file gathers together environmental factors that are
 * likely to change from one C compiler to another, or from one system
 * to another.
  ************************************************************************/ 

typedef unsigned char byte;

#define byteToInt(b) (b)

 /* this is a bit sloppy - but it works */ 
#define longCanBeInt(l) ((l >= -16383) && (l <= 16383))

 /* ======== various defines that should work on all systems ==== */ 

#define streq(a,b) (strcmp(a,b) == 0)

#define true 1
#define false 0

 /* define the datatype boolean */ 
typedef int     boolean;


 /* define a bit of lint silencing */ 
/*
 * 
 ignore means ``i know this function returns something,
 but I really,
 * really do mean to ignore it
 */ 
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
