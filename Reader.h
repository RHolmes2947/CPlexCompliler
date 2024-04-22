/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: Rian Olson
* Professors: Paulo Sousa
************************************************************
*/



#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_


/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */



/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'f', /* Fixed mode (constant size) */
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'm'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls */
#define READER_ERROR		(-1)						/* General error message */
#define READER_TERMINATOR	'\0'							/* General EOF */



#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */


/* BITS                                (7654.3210) */
#define READER_DEFAULT_FLAG 0x00 	/* (0000.0000)_2 = (000)_10 */
/* : BIT 3: END = End of buffer flag */
#define FLAG_END 0x08
/* : BIT 2: REL = Rellocation memory flag */
#define FLAG_REL 0x04
/* : BIT 1: EMP = Buffer empty flag */
#define FLAG_EMP 0x02
/* : BIT 0: FUL = Buffer full flag */
#define FLAG_FUL 0x01

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */





/* Offset declaration */
typedef struct position {
	int mark;			/* the offset to the mark position (in chars) */
	int read;			/* the offset to the get a char position (in chars) */
	int wrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	str	content;			/* pointer to the beginning of character array (character buffer) */
	int		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	int		increment;			/* character array increment factor */
	int		mode;				/* operational mode indicator */
	Plex_byte		flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;				/* Offset / position field */
	int		histogram[NCHAR];	/* Statistics of chars */
	int		numReaderErrors;	/* Number of errors from Reader */
} Buffer, *BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	readerCreate		(int, int, int);
BufferPointer	readerAddChar		(BufferPointer const, Plex_char);
Plex_boln		readerClear		    (BufferPointer const);
Plex_boln		readerFree		    (BufferPointer const);
Plex_boln		readerIsFull		(BufferPointer const);
Plex_boln		readerIsEmpty		(BufferPointer const);
Plex_boln		readerSetMark		(BufferPointer const, int);
int		readerPrint		    (BufferPointer const);
int		readerLoad			(BufferPointer const, FILE* const);
Plex_boln		readerRecover		(BufferPointer const);
Plex_boln		readerRetract		(BufferPointer const);
Plex_boln		readerRestore		(BufferPointer const);
/* Getters */
Plex_char		readerGetChar		(BufferPointer const);
str	readerGetContent	(BufferPointer const, int);
int		readerGetPosRead	(BufferPointer const);
int		readerGetPosWrte	(BufferPointer const);
int		readerGetPosMark	(BufferPointer const);
int		readerGetSize		(BufferPointer const);
int		readerGetInc		(BufferPointer const);
int		readerGetMode		(BufferPointer const);
Plex_byte		readerGetFlags		(BufferPointer const);
Plex_none		readerPrintStat		(BufferPointer const);
int		readerNumErrors		(BufferPointer const);

#endif
