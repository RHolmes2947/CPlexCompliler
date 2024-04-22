/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Spring, 2024
* Author: Rian Olson
* Professors: Paulo Sousa
************************************************************
=---------------------------------------------------------------------------=
|                   COMPILERS - ALGONQUIN COLLEGE (W24)                     |
=---------------------------------------------------------------------------=
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>	/* standard input / output */
#include <ctype.h>	/* conversion functions */
#include <stdlib.h> /* standard library functions and constants */
#include <string.h> /* string functions */
#include <limits.h> /* integer types constants */
#include <float.h>	/* floating-point types constants */

 /* #define NDEBUG to suppress assert() call */
#include <assert.h> /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable; /* String literal table */
int line;							 /* Current line number of the source code */
extern int errorNumber;			 /* Defined in platy_st.c - run-time error number */

extern int stateType[NUM_STATES];
extern str keywordTable[KWT_SIZE];
extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern int transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer; /* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer; /* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */


int startScanner(BufferPointer psc_buf)
{
	/*  Start histogram */
	for (int i = 0; i < NUM_TOKENS; i++)
		scData.scanHistogram[i] = 0;
	/* Basic scanner initialization */
	/* in case the buffer has been read previously  */
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(joe_void)
{


	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	Plex_char c;				  /* input symbol */
	int state = 0;		  /* initial state of the FSM */
	int lexStart;		  /* start offset of a lexeme in the input char buffer (array) */
	int lexEnd;		  /* end offset of a lexeme in the input char buffer (array)*/

	int lexLength; /* token length */
	int i;			/* counter */
	/*
	Plex_char newc;			// new char
	*/

	while (1)
	{ /* endless loop broken by token returns it will generate a warning */
		c = readerGetChar(sourceBuffer);

	
		switch (c)
		{

			/* Cases for spaces */
		case ' ':
		case '\t':
		case '\f':
			break;
		case '\n':
			line++;
			break;

			/* Cases for symbols */
		case ';':
			currentToken.code = EOS_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '(':
			currentToken.code = LPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case ')':
			currentToken.code = RPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '{':
			currentToken.code = LBR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '#':
			currentToken.code = CMT_T;
			scData.scanHistogram[currentToken.code]++;
			while (readerGetChar(sourceBuffer) != '\n' && c != EOF)
			{
			}

			return currentToken;
		case '}':
			currentToken.code = RBR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '[':
			currentToken.code = LSB_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case ']':
			currentToken.code = RSB_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case ',':
			currentToken.code = COM_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
			/*Cases for arithmatic operators*/
		case '+':
			currentToken.code = ADD_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '-':
			currentToken.code = SUB_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '*':
			currentToken.code = MUL_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '/':
			currentToken.code = DIV_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
			/*Cases for logical operators +++*/
		case '!':
			currentToken.code = NOT_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '>':
			currentToken.code = GRT_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '<':
			currentToken.code = LST_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;

			
			case '=':
				currentToken.code = EQU_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			

		case '%':
			currentToken.code = PRC_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '^':
			currentToken.code = POW_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '|':
			currentToken.code = OR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '&':
			currentToken.code = AND_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case ':':
			currentToken.code = COL_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
			/* Cases for END OF FILE */
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;

		default: // general case
			state = nextState(state, c);
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOFS)
			{
				c = readerGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[state] == FSWR)
				readerRetract(sourceBuffer);
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate((int)lexLength + 2, 0, MODE_FIXED);
			if (!lexemeBuffer)
			{
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			currentToken = (*finalStateTable[state])(readerGetContent(lexemeBuffer, 0));
			readerRestore(lexemeBuffer); // xxx
			return currentToken;
		} // switch

	} // while

} // tokenizer

/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to False (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */

int nextState(int state, Plex_char c)
{
	int col;
	int next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %ld Column: %ld Next: %ld \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS)
		{
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %ld Column: %ld\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
 * Create a function to return the column number in the transition table:
 * Considering an input char c, you can identify the "class".
 * For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */


int nextClass(Plex_char c)
{
	int val = -1;
	switch (c)
	{
	case CHRCOL2:
		val = 2;
		break;
	case CHRCOL3:
		val = 3;
		break;
	case CHRCOL4:
		val = 4;
		break;
	case CHRCOL6:
		val = 6;
		break;
	case CHRCOL8:
		val = 8;
		break;
	case CHRCOL9:
		val = 9;
		break;
	case CHRCOL10:
		val = 10;
		break;
	case CHARSEOF0:
	case CHARSEOF255:
		val = 5;
		break;
	default:
		if (isalpha(c))
			val = 0;
		else if (isdigit(c))
			val = 1;
		else
			val = 7;
	}
	return val;
}

/*
 ************************************************************
 * Acceptance State Function COM
 *		Function responsible to identify COM (comments).
 ***********************************************************
 */


Token funcCMT(str lexeme)
{
	Token currentToken = { 0 };
	int i = 0, len = (int)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++)
	{
		if (lexeme[i] == '\n')
			break;
	}
	currentToken.code = CMT_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

/*
 ************************************************************
 * Acceptance State Function IL
 *		Function responsible to identify IL (integer literals).
 * - It is necessary respect the limit (ex: 2-byte integer in C).
 * - In the case of larger lexemes, error shoul be returned.
 * - Only first ERR_LEN characters are accepted and eventually,
 *   additional three dots (...) should be put in the output.
 ***********************************************************
 */


Token funcIL(str lexeme)
{
	Token currentToken = { 0 }; 
	long tlong;
	int len = strlen(lexeme);
	for (int i = 0; i < len; i++)
	{
		if (lexeme[i] == '.')
		{									
			currentToken = funcFLT(lexeme);
		}
	}

	if (lexeme[0] != '\0' && strlen(lexeme) > NUM_LEN)
	{													 
		currentToken = (*finalStateTable[ESNR])(lexeme); 
	}
	else
	{
		tlong = atol(lexeme); 
		if (tlong >= 0 && tlong <= SHRT_MAX)
		{											
			currentToken.code = INL_T;					  
			scData.scanHistogram[currentToken.code]++;	 
			currentToken.attribute.intValue = (int)tlong; 
		}
		else
		{
			currentToken = (*finalStateTable[ESNR])(lexeme); 
		}
	}
	return currentToken; // Return the resulting token
}

/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */


Token funcID(str lexeme) {
	Token currentToken = { 0 }; 
	size_t length = strlen(lexeme); 
	char lastch = lexeme[length - 1]; 
	Plex_boln isID = False; 

	switch (lastch) {
	case MNID_SUF: 
		currentToken.code = MNID_T; 
		scData.scanHistogram[currentToken.code]++;
		isID = True; 
		break;
	default:
		currentToken = funcKEY(lexeme); 
		if (currentToken.code != KW_T) {
			isID = True; 
		}
		break;
	}

	if (isID) { // If isID flag is true
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN - 1);
		currentToken.attribute.idLexeme[VID_LEN - 1] = '='; 
		currentToken.attribute.idLexeme[VID_LEN] = '\0';
	}

	return currentToken; 
}


/*
************************************************************
* Acceptance State Function SL
*		Function responsible to identify SL (string literals).
* - The lexeme must be stored in the String Literal Table
*   (stringLiteralTable). You need to include the literals in
*   this structure, using offsets. Remember to include \0 to
*   separate the lexemes. Remember also to incremente the line.
***********************************************************
*/


Token funcSL(str lexeme) {
	Token currentToken = { 0 }; 
	int i = 0, len = strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);

	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == '\n')
			line++; 
		if (!readerAddChar(stringLiteralTable, lexeme[i])) { 
			
			currentToken.code = RTE_T;
			scData.scanHistogram[currentToken.code]++;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}

	if (!readerAddChar(stringLiteralTable, ' ') || !readerAddChar(stringLiteralTable, '\0')) {
	
		currentToken.code = RTE_T;
		scData.scanHistogram[currentToken.code]++;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}

	currentToken.code = STR_T; 
	scData.scanHistogram[currentToken.code]++; 
	return currentToken; 
}


/*
************************************************************
* This function checks if one specific lexeme is a keyword.
* - Tip: Remember to use the keywordTable to check the keywords.
***********************************************************
*/


Token funcKEY(str lexeme) {
	Token currentToken = { 0 }; 
	int keyIndex = -1; 
	for (int j = 0; j < KWT_SIZE; j++) { 
		if (!strcmp(lexeme, keywordTable[j])) {
			keyIndex = j; 
			break;
		}
	}
	if (keyIndex != -1) { 
		currentToken.code = KW_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.codeType = keyIndex; 
	}
	else {
		int isValidVariable = 1; 
		if (isValidVariable) {
			currentToken.code = VAR_T; 
			strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN - 1);
			currentToken.attribute.idLexeme[VID_LEN - 1] = '\0'; 
		}
		else {
			
			currentToken = funcErr(lexeme);
		}
	}
	return currentToken; 
}


/*
************************************************************
* Acceptance State Function Error
*		Function responsible to deal with ERR token.
* - This function uses the errLexeme, respecting the limit given
*   by ERR_LEN. If necessary, use three dots (...) to use the
*   limit defined. The error lexeme contains line terminators,
*   so remember to increment line.
***********************************************************
*/


Token funcErr(str lexeme)
{
	Token currentToken = { 0 };
	int i = 0, len = (int)strlen(lexeme);
	if (len > ERR_LEN)
	{
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else
	{
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == '\n')
			line++;
	currentToken.code = ERR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

Plex_none printToken(Token t)
{
	extern str keywordTable[]; /* link to keyword table in */
	switch (t.code)
	{
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber)
		{
			printf("%ld", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T\t\t%ld\t ", (int)t.attribute.codeType);
		printf("%s\n", readerGetContent(stringLiteralTable, (int)t.attribute.codeType));
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	case ADD_T:
		printf("ADD_T\t\t%d\n", t.attribute.arithmeticOperator);
		break;
	case SUB_T:
		printf("MINUS_T\t\t%d\n", t.attribute.arithmeticOperator);
		break;
	case MUL_T:
		printf("MULT_T\t\t%d\n", t.attribute.arithmeticOperator);
		break;
	case DIV_T:
		printf("DIV_T\t\t%d\n", t.attribute.arithmeticOperator);
		break;
	case NOT_T:
		printf("NOT_T\n");
		break;
	case GRT_T:
		printf("GT_T\t\t%d\n", t.attribute.relationalOperator);
		break;
	case LST_T:
		printf("LT_T\t\t%d\n", t.attribute.relationalOperator);
		break;
	case EQU_T:
		printf("EQU_T\t\t%d\n", t.attribute.relationalOperator);
		break;
	case FLT_T:
		printf("FLT_T\t\t%f\n", t.attribute.floatValue);
		break;
	case COM_T:
		printf("COM_T\n");
		break;
	case POW_T:
		printf("POW_T\n");
		break;
	case LSB_T:
		printf("LSB_T\n");
		break;
	case RSB_T:
		printf("RSB_T\n");
		break;
	case QUO_T:
		printf("QUO_T\n");
		break;
	case VAR_T:
		printf("VAR_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case COL_T:
		printf("COL_T\n");
		break;
	case AND_T:
		printf("AND_T\n");
		break;
	case OR_T:
		printf("OR_T\n");
		break;
	case INL_T:
		printf("INL_T\t\t%d\n", t.attribute.intValue);
		break;
	case PRC_T:
		printf("PRC_T\n");
		break;
	default:
		printf("Scanner error: invalid token code: %ld\n", t.code);
	}
}

/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
Plex_none printScannerData(ScannerData scData)
{
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++)
	{
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%ld%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}


Token funcMLC(str lexeme)
{
	Token currentToken = { 0 };
	int i = 0, len = (int)strlen(lexeme);
	Plex_boln in_comment = 1; 

	while (in_comment)
	{
		for (i = 1; i < len - 2; i++)
		{ 
			if (lexeme[i] == '\n')
			{
				line++;
			}
			else if (lexeme[i] == '\'' && lexeme[i + 1] == '\'' && lexeme[i + 2] == '\'')
			{
				i += 2;		
				in_comment = 0; 
				break;
			}
		}
		if (i < len - 2)
		{
			break;
		}
		
		in_comment = 0;
	}

	if (!in_comment)
	{
		currentToken.code = MLC_T;
		scData.scanHistogram[currentToken.code]++;
	}
	else
	{
		currentToken.code = ERR_T;
		return currentToken;
	}
	return currentToken;
}
Token funcFLT(str lexeme) {
	Token currentToken = { 0 }; 
	float fvalue;
	int len = strlen(lexeme);

	if (sscanf(lexeme, "%f", &fvalue) == 1) { 
		
		currentToken.code = FLT_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.floatValue = fvalue; 
	}
	else {
		
		currentToken.code = ERR_T;
		
	}

	return currentToken; 
}
