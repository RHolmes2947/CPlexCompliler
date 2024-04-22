/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: Rian Olson
* Professors: Paulo Sousa
************************************************************
*/


#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0




/* Language name */
#define STR_LANGNAME	"Plex"

/* Logical constants */
#define True  1
#define False 0



/* typedefs */
typedef char			Plex_char;
typedef char*			str;

typedef void			Plex_none;

typedef unsigned char	Plex_boln;
typedef unsigned char	Plex_byte;

typedef long			Plex_long;
typedef double			Plex_doub;

/*
------------------------------------------------------------
Programs:
1: Reader - invokes MainReader code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_READER	= 'r',
	PGM_SCANNER = 's',
	PGM_PARSER	= 'p'
};

/*
------------------------------------------------------------
Main functions signatures
------------------------------------------------------------
*/
int mainReader(int argc, str* argv);
int mainScanner(int argc, str* argv);
int mainParser(int argc, str* argv);


Plex_none printLogo();

#endif
