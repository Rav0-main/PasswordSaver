#pragma once

#include <stdbool.h>

#define PROMPT_MAX_LENGTH 128
#define INPUTED_VALUE_MAX_LENGTH 64

typedef struct _InputField {
	const char prompt[PROMPT_MAX_LENGTH+1];
	char value[INPUTED_VALUE_MAX_LENGTH+1];
	const bool isSecurity;
} InputField;

/*
* Output the text:
* fields[0]->prompt(cursor) 
* fields[1]->prompt
* fields[2]->prompt
* ...
* fields[fieldsLength-1]->prompt.
* move between lines with arrow buttons(up, down)
* press enter on last line is end of input
* In fields[i]->value is inputed text
* If fields[0]->value[0] == -1 pressed left arrow button
*/
void displayMultiInputFields(
	InputField** const restrict fields,
	const unsigned int fieldsLength, const int startY
);