#pragma once

#define OUTPUT_LINE_MAX_LENGTH 128

typedef struct _ChoiceField {
	const char outputLine[OUTPUT_LINE_MAX_LENGTH + 1];
	const int code;
} ChoiceField;

/*
* Output the text:
* fields[0]->outputLine(cursor)
* fields[1]->outputLine
* fields[2]->outputLine
* ...
* fields[fieldsLength-1]->outputLine
* Returns the code of choosed field(fields[choosedField]->code)
* Returns 0 if error
* Returns -1 if pressed left arrow button
*/
int displayMultiChoiceFields(ChoiceField** fields, const unsigned int fieldsLength, const int startY);