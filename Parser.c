
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

#ifndef PARSER_H_
#include "Parser.h"
#endif

/* Parser data */
extern ParserData psData; /* BNF statistics */

/*
************************************************************
 * Process Parser
 ***********************************************************
 */

Plex_none startParser() {
	/*  Initialize Parser data */
	int i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	/* Proceed parser */
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */

Plex_none matchToken(int tokenCode, int tokenAttribute) {
	int matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */

Plex_none syncErrorHandler(int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/*  This is the function to error printing*/
Plex_none printError() {
	extern numParserErrors;			/* link to number of errors (defined in Parser.h) */
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
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
	case EOS_T:
		printf("NA\n");
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
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
		numParserErrors++; // Updated parser error
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> }
 * FIRST(<program>)= {CMT_T, MNID_T (main&), SEOF_T}.
 ***********************************************************
 */
Plex_none program() {
	/* Update program statistics */
	psData.parsHistogram[BNF_program]++;
	/* Program code */
	switch (lookahead.code) {
	case CMT_T:
		comment();
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_MAIN, 5) == 0) {
			matchToken(MNID_T, NO_ATTR);
			matchToken(LBR_T, NO_ATTR);
			functionDefinition();
			matchToken(RBR_T, NO_ATTR);
			break;
		}
		else {
			printError();
		}
	case SEOF_T:
		; 
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
 ************************************************************
 * comment
 * BNF: comment
 * FIRST(<comment>)= {CMT_T}.
 ***********************************************************
 */
Plex_none comment() {
	psData.parsHistogram[BNF_comment]++;
	matchToken(CMT_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}




Plex_none functionDefinition() {
    matchToken(MNID_T, NO_ATTR);
    matchToken(LPR_T, NO_ATTR); 
    matchToken(RPR_T, NO_ATTR);
    matchToken(LBR_T, NO_ATTR); 
	comment();
    while (lookahead.code != RBR_T) {
		optVarListDeclarations();
        optionalStatements();
    }
	
    matchToken(RBR_T, NO_ATTR); 
    printf("%s: Function definition parsed\n", STR_LANGNAME);
}



/*
 ************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
 ***********************************************************
 */
Plex_none optVarListDeclarations() {
	psData.parsHistogram[BNF_optVarListDeclarations]++;

	while (lookahead.code == VAR_T || lookahead.code == MNID_T) {
		if (lookahead.code == VAR_T) {
			matchToken(VAR_T, NO_ATTR);
			if (lookahead.code == EQU_T) {
				matchToken(EQU_T, NO_ATTR); 
				if (lookahead.code == INL_T || lookahead.code == STR_T || lookahead.code == FLT_T) {
					matchToken(lookahead.code, NO_ATTR);
				}
				else {
					printError();
				}
			}
		}
		else if (lookahead.code == MNID_T) {
			lookahead = tokenizer();
		}
	}

	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}



/* : Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *				KW_T(KW_while), MNID_T(print&), MNID_T(input&) }
 ***********************************************************
 */
Plex_none optionalStatements() {
	psData.parsHistogram[BNF_optionalStatements]++;
	while (lookahead.code != RBR_T && lookahead.code != SEOF_T) {
		switch (lookahead.code) {
		case CMT_T:
			comment();
		case MNID_T:
			if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0 ||
				strncmp(lookahead.attribute.idLexeme, LANG_READ, 6) == 0) {
				statements();
			}
			else {
				statement();
			}
			break;
		case KW_T:
			statements();
			break;
		default:
			printError();
			lookahead = tokenizer();
			break;
		}
	}

	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}


/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *		KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
Plex_none statements() {
	psData.parsHistogram[BNF_statements]++;
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T, 
 *		KW_T(KW_if), KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
Plex_none statementsPrime() {
	psData.parsHistogram[BNF_statementsPrime]++;
	switch (lookahead.code) {
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
			statements();
			break;
		}
	default:
		; 
	}
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> -> <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if), KW_T(KW_while),
 *			MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
Plex_none statement() {
	psData.parsHistogram[BNF_statement]++;
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		default:
			printError();
		}
		break;
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
			outputStatement();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> print& (<output statementPrime>);
 * FIRST(<output statement>) = { MNID_T(print&) }
 ***********************************************************
 */
Plex_none outputStatement() {
	psData.parsHistogram[BNF_outputStatement]++;
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
 ***********************************************************
 */
Plex_none outputVariableList() {
	psData.parsHistogram[BNF_outputVariableList]++;

	
	while (lookahead.code != RPR_T && lookahead.code != SEOF_T) {
		if (lookahead.code == STR_T || lookahead.code == INL_T || lookahead.code == FLT_T || lookahead.code == VAR_T) {
			matchToken(lookahead.code, NO_ATTR);
		}
		else {
			printError();
			break; 
		}
		if (lookahead.code == COM_T) {
			matchToken(COM_T, NO_ATTR); 
		}
		else {
			break;
		}
	}

	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}


/*
 ************************************************************
 * The function prints statistics of BNF rules
 * Param:
 *	- Parser data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
/*
Plex_none printBNFData(ParserData psData) {
}
*/
Plex_none printBNFData(ParserData psData) {
	/* Print Parser statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_BNF_RULES; cont++) {
		if (psData.parsHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}
