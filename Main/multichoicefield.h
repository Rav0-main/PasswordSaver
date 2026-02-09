#pragma once

#define PROMPT_MAX_LENGTH 128

typedef int ChoiceCode;

typedef struct _ChoiceField {
	const char prompt[PROMPT_MAX_LENGTH + 1];
	const ChoiceCode code;
} ChoiceField;

#define ERROR_CODE (ChoiceCode) 0
#define PRESSED_LEFT_ARROW (ChoiceCode) -1

/*
* Output the text:
* fields[0]->prompt(cursor)
* fields[1]->prompt
* fields[2]->prompt
* ...
* fields[fieldsLength-1]->prompt
* Returns the code of choosed field(fields[choosedField]->code)
* Returns 0 if error
* Returns -1 if pressed left arrow button
*/
ChoiceCode displayMultiChoiceFields(
	const ChoiceField** const restrict fields,
	const unsigned int fieldsLength, const int startY
);