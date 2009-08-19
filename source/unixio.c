/************************************************************************
 * Parable, a little smalltalk
 *
 * This code is gifted to the public domain. Share freely.
 *
 * Unix specific I/O routines.
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"
#include "memory.h"
#include "names.h"

void visit(register object x);
void setFreeLists();
void fileIn(FILE * fd, boolean printit);


struct
{
  int    di;
  object cl;
  short  ds;
} dummyObject;


/*
 * imageRead - read in an object image
 * we toss out the free lists built
 * initially, reconstruct the linkages, then rebuild the free
 * lists around the new objects.
 * The only objects with nonzero reference counts
 * will be those reachable from either symbols
 */
static int fr(FILE * fp, char *p, int s)
{
  int r;
  r = fread(p, s, 1, fp);
  if (r && (r != 1))
    sysError("imageRead count error", "");
  return r;
}

noreturn imageRead(FILE * fp)
{
  short  i, size;
  object *mBlockAlloc();
  ignore fr(fp, (char *) &symbols, sizeof(object));
  i = 0;
  while (fr(fp, (char *) &dummyObject, sizeof(dummyObject)))
  {
    i = dummyObject.di;
    if ((i < 0) || (i > ObjectTableMax))
      sysError("reading index out of range", "");
    objectTable[i].class = dummyObject.cl;
    if ((objectTable[i].class < 0) || ((objectTable[i].class >> 1) > ObjectTableMax))
    {
      fprintf(stderr, "index %d\n", dummyObject.cl);
      sysError("class out of range", "imageRead");
    }
    objectTable[i].size = size = dummyObject.ds;
    if (size < 0)
      size = ((-size) + 1) / 2;
    if (size != 0)
    {
      objectTable[i].memory = mBlockAlloc((int) size);
      ignore fr(fp, (char *) objectTable[i].memory,
      sizeof(object) * (int) size);
    }
    else
      objectTable[i].memory = (object *) 0;
  }
  /* now restore ref counts, getting rid of unneeded junk */
  visit(symbols);
  /* toss out the old free lists, build new ones */
  setFreeLists();
}


/* imageWrite - write out an object image */
static void fw(FILE * fp, char *p, int s)
{
  if (fwrite(p, s, 1, fp) != 1)
  {
    sysError("imageWrite size error", "");
  }
}

noreturn imageWrite(FILE * fp)
{
  short i, size;
  fw(fp, (char *) &symbols, sizeof(object));
  for (i = 0; i < ObjectTableMax; i++)
  {
    if (objectTable[i].referenceCount > 0)
    {
      dummyObject.di = i;
      dummyObject.cl = objectTable[i].class;
      dummyObject.ds = size = objectTable[i].size;
      fw(fp, (char *) &dummyObject, sizeof(dummyObject));
      if (size < 0)
        size = ((-size) + 1) / 2;
      if (size != 0)
        fw(fp, (char *) objectTable[i].memory, sizeof(object) * size);
    }
  }
}


/*
 * i/o primitives - necessarily rather UNIX dependent; basically, files are
 * all kept in a large array. File operations then just give an index into
 * this array
 */
#define MAXFILES 20

/* we assume this is initialized to NULL */
static FILE *fp[MAXFILES];


object ioPrimitive(int number, object * arguments)
{
  int     i, j;
  char   *p, buffer[1024];
  object  returnedObject;
  returnedObject = nilobj;
  i = intValue(arguments[0]);
  switch (number)
  {
    /* file open */
    case 0:
      i = intValue(arguments[0]);
      p = charPtr(arguments[1]);
      if (streq(p, "stdin"))
        fp[i] = stdin;
      else if (streq(p, "stdout"))
        fp[i] = stdout;
      else if (streq(p, "stderr"))
        fp[i] = stderr;
      else
        fp[i] = fopen(p, charPtr(arguments[2]));

      if (fp[i] == NULL)
        returnedObject = nilobj;
      else
        returnedObject = newInteger(i);
      break;
    /* file close - recover slot */
    case 1:
      if (fp[i])
        ignore fclose(fp[i]);
      fp[i] = NULL;
      break;
    /* file size */
    case 2:
    /* file in */
    case 3:
      if (fp[i])
        fileIn(fp[i], true);
      break;
    /* get character */
    case 4:
      sysError("file operation not implemented yet", "");
    /* get string */
    case 5:
      if (!fp[i])
        break;
      j = 0;
      buffer[j] = '\0';
      while (1)
      {
        /* end of file */
        if (fgets(&buffer[j], 512, fp[i]) == NULL)
          return (nilobj);

        /* delete the newline */
        if (fp[i] == stdin)
        {
          j = strlen(buffer);
          if (buffer[j - 1] == '\n')
            buffer[j - 1] = '\0';
        }
        j = strlen(buffer) - 1;
        if (buffer[j] != '\\')
          break;
        /* else we loop again */
      }
      returnedObject = newStString(buffer);
      break;
    /* write an object image */
    case 7:
      if (fp[i])
        imageWrite(fp[i]);
      returnedObject = trueobj;
      break;
    /* print no return */
    case 8:
    /* print string */
    case 9:
      if (!fp[i])
        break;
      ignore fputs(charPtr(arguments[1]), fp[i]);
      if (number == 8)
      {
        ignore fflush(fp[i]);
      }
      else
      {
        ignore fputc('\n', fp[i]);
      }
      break;
    default:
      sysError("unknown primitive", "filePrimitive");
  }
  return (returnedObject);
}
