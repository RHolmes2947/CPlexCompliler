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
#include "Reader.h"
#endif




BufferPointer readerCreate(int size, int increment, int mode) {
	BufferPointer readerPointer;
	/* Defensive programming  */
	if ((size < 0) || (size == 0))
		size = READER_DEFAULT_SIZE;
	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
	if (!readerPointer)
		return NULL;
	readerPointer->content = (str)malloc(size);
	if (!readerPointer->content)
		return NULL;
	/* Initialize the histogram + */
	for (int i = 0; i < NCHAR; i++)
		readerPointer->histogram[i] = 0;
	readerPointer->size = size;
	readerPointer->increment = increment;
	readerPointer->mode = mode;
	/* Initialize flags + */
	readerPointer->flags = READER_DEFAULT_FLAG;
	readerPointer->flags = readerPointer->flags | FLAG_EMP;
	if (readerPointer->content)
		readerPointer->content[0] = READER_TERMINATOR;
	readerPointer->position.wrte = 0;
	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;
	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* 
*************************************************************
*/

BufferPointer readerAddChar(BufferPointer const readerPointer, Plex_char ch) {
	str tempReader = NULL;
	int newSize = 0;
	/* Defensive programming  */
	if (!readerPointer)
		return NULL;
	if (ch < 0 || ch > NCHAR)
		return NULL;
	/* Reset Realocation  */
	readerPointer->flags = readerPointer->flags & FLAG_REL;

	/*Test the inclusion of chars */
	if (readerPointer->position.wrte * (int)sizeof(Plex_char) < readerPointer->size) {
		/* : This buffer is NOT full */
	} else {
		/* Reset Full flag */
		readerPointer->flags = readerPointer->flags & FLAG_FUL;
		switch (readerPointer->mode) {
		case MODE_FIXED:
			return NULL;
		case MODE_ADDIT:
			/* Adjust new size */
			newSize = readerPointer->size + readerPointer->increment;
			/* Defensive programming */
			if (newSize < 0 || newSize > READER_MAX_SIZE)
				return NULL;
			break;
		case MODE_MULTI:
			/* Adjust new size */
			newSize = readerPointer->size * readerPointer->increment;
			/* Defensive programming */
			if (newSize < 0 || newSize > READER_MAX_SIZE)
				return NULL;
			break;
		default:
			return NULL;
		}
		/* New reader allocation */
		str tempReader = realloc(readerPointer->content, newSize);
		/* Defensive programming */
		if (!tempReader)
			return NULL;
		/* Check Relocation */
		if (tempReader != readerPointer->content) {
			readerPointer->flags |= FLAG_REL;
		}
		
	}
	/* Add the char */
	readerPointer->content[readerPointer->position.wrte++] = ch;
	/* Updates histogram + */
	readerPointer->histogram[ch]++;
	readerPointer->flags = readerPointer->flags & FLAG_EMP;
	
	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* 
*************************************************************
*/
Plex_boln readerClear(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return False;
	/* Adjust flags original */
	readerPointer->flags = READER_DEFAULT_FLAG;
	readerPointer->position.wrte = readerPointer->position.mark = readerPointer->position.read = 0;
	return True;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* 
*************************************************************
*/
Plex_boln readerFree(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return False;
	/* Free pointers */
	free(readerPointer->content);
	free(readerPointer);
	return True;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* :
*************************************************************
*/
Plex_boln readerIsFull(BufferPointer const readerPointer) {
	/* : Defensive programming */
	if (!readerPointer)
		return False;
	if (readerPointer->flags & FLAG_FUL)
		return True;
	else
		return False;

}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* 
*************************************************************
*/
Plex_boln readerIsEmpty(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return False;
	if (readerPointer->flags & FLAG_EMP)
		return True;
	else 
		return False;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* 
*************************************************************
*/
Plex_boln readerSetMark(BufferPointer const readerPointer, int mark) {
	/* Defensive programming */
	if (!readerPointer || mark < 0 || mark > readerPointer->position.wrte)
		return False;
	/* Adjust mark */
	readerPointer->position.mark = mark;
	return True;
	
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
*************************************************************
*/
int readerPrint(BufferPointer const readerPointer) {
	int cont = 0;
	Plex_char c;
	/* Defensive programming (including invalid chars) */
	if (!readerPointer)
		return 0;
	c = readerGetChar(readerPointer);
	while (cont < readerPointer->position.wrte) {
		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
	}
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
*************************************************************
*/
int readerLoad(BufferPointer const readerPointer, FILE* const fileDescriptor) {
	int size = 0;
	Plex_char c;
	/* Defensive programming */
	if (!readerPointer || !fileDescriptor)
		return READER_ERROR;
	c = (Plex_char)fgetc(fileDescriptor);
	while (!feof(fileDescriptor)) {
		if (!readerAddChar(readerPointer, c)) {
			ungetc(c, fileDescriptor);
			return READER_ERROR;
		}
		c = (char)fgetc(fileDescriptor);
		size++;
	}
	/* : Defensive programming */
	if (!fileDescriptor)
		return READER_ERROR;
	return size;
}


/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* 
*************************************************************
*/
Plex_boln readerRecover(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return False;
	/* todo Recover positions */
	readerPointer->position.read = 0;
	return True;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* 
*************************************************************
*/
Plex_boln readerRetract(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer || readerPointer->position.read <= 0)
		return False;
	/* Retract (return 1 pos read) */
	readerPointer->position.read--;
	return True;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* 
*************************************************************
*/
Plex_boln readerRestore(BufferPointer const readerPointer) {
	/* : Defensive programming */
	if (!readerPointer)
		return False;
	/* : Restore positions (read/mark) */
	readerPointer->position.read = readerPointer->position.mark;
	return True;
}


/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
*************************************************************
*/
Plex_char readerGetChar(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return READER_TERMINATOR;
	/* : Check condition to read/wrte */
	if (readerPointer->position.read < readerPointer->position.wrte) {
		Plex_char ch = readerPointer->content[readerPointer->position.read++];

		/* : Set EOB flag */
		if (readerPointer->position.read == readerPointer->position.wrte)
			readerPointer->flags |= FLAG_END; 
		else
			/* : Reset EOB flag */
			readerPointer->flags &= FLAG_END;  

		return ch;
	}
	else {
		if (readerPointer->position.wrte > 0)
			return readerPointer->content[readerPointer->position.read++];
		return READER_TERMINATOR;
	}
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* 
*************************************************************
*/
str readerGetContent(BufferPointer const readerPointer, int pos) {
	/* : Defensive programming */
	if (!readerPointer || pos < 0 || pos >= readerPointer->position.wrte)
		return NULL;
	/* : Return content (string) */
	return readerPointer->content + pos;
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
*************************************************************
*/
int readerGetPosRead(BufferPointer const readerPointer) {
	/* : Defensive programming */
	if (!readerPointer)
		return READER_ERROR;
	/* : Return read */
	return readerPointer->position.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
*************************************************************
*/
int readerGetPosWrte(BufferPointer const readerPointer) {
	/* : Defensive programming */
	if (!readerPointer)
		return READER_ERROR;  
	/* : Return wrte */
	return readerPointer->position.wrte;
	return 0;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
*************************************************************
*/
int readerGetPosMark(BufferPointer const readerPointer) {
	/* : Defensive programming */
	if (!readerPointer)
		return READER_ERROR;  

	/* : Return mark */
	return readerPointer->position.mark;
	
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
*************************************************************
*/
int readerGetSize(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return READER_ERROR;
	/* Return size */
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
*************************************************************
*/
int readerGetInc(BufferPointer const readerPointer) {
	/* : Defensive programming */
	if (!readerPointer)
		return READER_ERROR;
	/*  Return increment */
	return readerPointer->increment;
	
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
*************************************************************
*/
int readerGetMode(BufferPointer const readerPointer) {
	/*  Defensive programming */
	if (!readerPointer)
		return READER_ERROR; 
	/*  Return mode */
	return readerPointer->mode;
	
}


/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
*************************************************************
*/
Plex_byte readerGetFlags(BufferPointer const readerPointer) {
	/*  Defensive programming */
	if (!readerPointer)
		return 0;
		/*  Return flags */
		return readerPointer->flags;
}



/*
***********************************************************
* Function name: readerPrintStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
*************************************************************
*/
Plex_none readerPrintStat(BufferPointer const readerPointer) {
	if (!readerPointer)
		return;
	/* Print the histogram + */
	for (int i = 0; i < NCHAR; i++)
		if (readerPointer->histogram[i] > 0)
			printf("Statistics[%c]=%d\n", i, readerPointer->histogram[i]);
	
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* 
*************************************************************
*/
int readerNumErrors(BufferPointer const readerPointer) {
	/*  Defensive programming */
	if (!readerPointer)
		return -1; 

	int numErrors = 0;
	/* Check for errors */
	for (int i = 0; i < NCHAR; i++) {
		if (readerPointer->histogram[i] < 0 || readerPointer->histogram[i] > INT_MAX) {
			numErrors++;
		}
	}
	return numErrors;
}
