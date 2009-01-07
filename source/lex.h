/************************************************************************
 * A Little Smalltalk                                           Version 3
 *
 * This code is gifted to the public domain. Share freely.
 *
 * Values returned by the lexical analyzer.
 ************************************************************************/

typedef enum tokensyms { nothing, nameconst, namecolon,
    intconst, floatconst, charconst, symconst,
    arraybegin, strconst, binary, closing, inputend
} tokentype;

extern tokentype nextToken(NOARGS);

extern tokentype token;		/* token variety */
extern char tokenString[];	/* text of current token */
extern int tokenInteger;	/* integer (or character) value of token */
extern double tokenFloat;	/* floating point value of token */
extern noreturn lexinit();	/* initialization routine */
