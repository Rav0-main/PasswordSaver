#pragma once
#include <stdbool.h>

#define OUTPUT_LINE_MAX_LENGTH 128
#define INPUTED_VALUE_MAX_LENGTH 64

typedef struct _InputField {
	const char outputLine[OUTPUT_LINE_MAX_LENGTH+1];
	char inputedValue[INPUTED_VALUE_MAX_LENGTH + 1];
	const bool isSecurity;
} InputField;

/*
* Output the text:
* fields[0]->outputLine(cursor) 
* fields[1]->outputLine
* fields[2]->outputLine
* ...
* fields[fieldsLength-1]->outputLine.
* move between lines with arrow buttons(up, down)
* press enter on last line is end of input
* In fields[i]->inputedValue is inputed text
* If fields[0]->inputedValue[0] == -1 pressed left arrow button
*/
void displayMultiInputFields(InputField** fields, const unsigned int fieldsLength, const int startY);