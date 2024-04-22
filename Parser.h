/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: Rian Olson
* Professors: Paulo Sousa
************************************************************
*/


#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token			lookahead;
extern BufferPointer	stringLiteralTable;
extern int		line;
extern Token			tokenizer(Plex_none);
extern str		keywordTable[KWT_SIZE];
static int		syntaxErrorNumber = 0;

#define LANG_WRTE		"print"
#define LANG_READ		"read"
#define LANG_MAIN		"main"

/* : Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	KW_data,
	KW_code,
	KW_int,
	KW_real,
	KW_string,
	KW_if,
	KW_then,
	KW_else,
	KW_while,
	KW_do,
	KW_print,
	KW_await,
	KW_nonlocal,
	KW_break,
	KW_import,
	KW_in,
	KW_del,
	KW_true,
	KW_class,
	KW_finally,
	KW_is,
	KW_return,
	KW_and,
	KW_as,
	KW_none,
	KW_def,
	KW_for,
	KW_elif,
	KW_read


};

/* : Define the number of BNF rules */
#define NUM_BNF_RULES 11

/* Parser */
typedef struct parserData {
	int parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
int numParserErrors;

/* Scanner data */
ParserData psData;

/* Function definitions */
Plex_none startParser();
Plex_none matchToken(int, int);
Plex_none syncErrorHandler(int);
Plex_none printError();
Plex_none printBNFData(ParserData psData);

/* List of BNF statements */
enum BNF_RULES {
	BNF_error,										/*  0: Error token */
	BNF_function,									/*  1 */
	BNF_comment,									/*  2 */
	BNF_optVarListDeclarations,						/*  4 */
	BNF_optionalStatements,							/*  5 */
	BNF_outputStatement,							/*  6 */
	BNF_outputVariableList,							/*  7 */
	BNF_program,									/*  8 */
	BNF_statement,									/*  9 */
	BNF_statements,									/* 10 */
	BNF_statementsPrime,								/* 11 */
};


/* : Define the list of keywords */
static str BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error",
	"BNF_functionDefinition",
	"BNF_comment",
	"BNF_optVarListDeclarations",
	"BNF_optionalStatements",
	"BNF_outputStatement",
	"BNF_outputVariableList",
	"BNF_program",
	"BNF_statement",
	"BNF_statements",
	"BNF_statementsPrime"
};



Plex_none functionDefinition();
Plex_none comment();
Plex_none optVarListDeclarations();
Plex_none optionalStatements();
Plex_none outputStatement();
Plex_none outputVariableList();
Plex_none program();
Plex_none statement();
Plex_none statements();
Plex_none statementsPrime();

#endif
