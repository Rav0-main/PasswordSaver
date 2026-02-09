#pragma once

#include "../Log2Database/main.h"

#define PROMPT_MAX_LENGTH 128

typedef struct _AutoCompletionInputField {
	const int startX;
	const int startY;
	const unsigned int bufferLength;
	const char prompt[PROMPT_MAX_LENGTH + 1];

} AutoCompletionInputField;

/*
* Returns inputed word from wordDatabase or any
* Returns buffer[0] = -1 if user pressed left button
* buffer created by 'calloc'
*/
char* displayAutoCompletionInputField(
	const AutoCompletionInputField inputField,
	const Database* wordDatabase
);