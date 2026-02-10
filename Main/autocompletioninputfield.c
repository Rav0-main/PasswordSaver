#include "autocompletioninputfield.h"
#include "multifieldinginteractive.h"
#include "fillstrbuffer.h"

#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

static char runCharOfAutoCompletionInputField(
	const int startX, const int startY, const char* const restrict buffer,
	const Database* const restrict wordDatabase, int* const restrict indexOnPressedEnter
);

static unsigned int changeAutoCompletionWord(
	const int startX, const int startY, const char* const restrict buffer,
	const unsigned int pastLineLength, const Database* const restrict wordDatabase,
	const RecordCount index
);

char* displayAutoCompletionInputField(
	const AutoCompletionInputField inputField, const Database* wordDatabase
) {
	char* restrict buffer = (char*)calloc(inputField.bufferLength + 1, sizeof(char));

	if (buffer == NULL)
		return NULL;

	fillString(buffer, inputField.bufferLength, '\0');

	const unsigned int outputLineLength = strlen(inputField.prompt);
	bool run = true;
	unsigned int currentBufferLength = 0;
	char symbol;
	int indexOnPressedEnter;

	printf(SET_WHITE_COLOR "%s" DROP_COLOR_SET, inputField.prompt);

	while (run) {
		symbol = runCharOfAutoCompletionInputField(
			inputField.startX+outputLineLength, inputField.startY,
			buffer, wordDatabase, &indexOnPressedEnter
		);

		if (symbol == -1) {
			buffer[0] = -1;
			run = false;
		}

		else if (!isBackspaceButton(symbol) && !isEnterButton(symbol) && symbol >= 32 && symbol <= 126) {
			putchar(symbol);

			buffer[currentBufferLength] = symbol;
			++currentBufferLength;
			buffer[currentBufferLength] = '\0';
		}

		else if (isBackspaceButton(symbol) && currentBufferLength > 0) {
			cursorMoveTo(inputField.startX + currentBufferLength + outputLineLength - 1, inputField.startY);
			putchar(' ');

			--currentBufferLength;
			buffer[currentBufferLength] = '\0';
		}

		else if (isEnterButton(symbol) && indexOnPressedEnter != -1) {
			free(buffer);

			buffer = databaseGetKeyByIndex(wordDatabase, indexOnPressedEnter);
			run = false;
		}
		else if (isEnterButton(symbol))
			run = false;
	}

	return buffer;
}

static char runCharOfAutoCompletionInputField(
	const int startInputX, const int startInputY, const char* const restrict buffer,
	const Database* const restrict wordDatabase, int* const restrict indexOnPressedEnter
) {
	cursorMoveTo(startInputX, startInputY);

	const unsigned int bufferLength = strlen(buffer);

	RecordCount* indexes = databaseGetIndexesOfKeysWhichStartWith(
		wordDatabase, buffer
	);
	unsigned int autoCompletionsStrLength = 0;

	if (indexes != NULL)
		autoCompletionsStrLength = indexes[1] - indexes[0] + 1;

	else {
		printf(SET_WHITE_COLOR "%s" DROP_COLOR_SET, buffer);
		
		char symbol;
		cursorMoveTo(startInputX + bufferLength, startInputY);
		*indexOnPressedEnter = -1;

		while (true) {
			if (_kbhit()) {
				symbol = _getch();

				if (isArrowButton(symbol)) {
					symbol = _getch();

					if (isLeftArrowButton(symbol))
						return -1;
				}

				else if (isEnterButton(symbol))
					return symbol;

				else if (isBackspaceButton(symbol))
					return symbol;

				else if (symbol >= 32 && symbol <= 126)
					return symbol;
			}
			Sleep(SLEEP_TIME_MS);
		}
	}

	unsigned int currentLineLength = changeAutoCompletionWord(startInputX, startInputY, buffer, 0, wordDatabase, indexes[0]);
	cursorMoveTo(startInputX + bufferLength, startInputY);

	bool moveBetweenCompletions = true;
	int currentAutoCompletionWordIndex = 0;
	char symbol;

	while (moveBetweenCompletions) {
		if (_kbhit()) {
			symbol = _getch();

			if (isArrowButton(symbol)) {
				symbol = _getch();

				if (isUpArrowButton(symbol)) {
					--currentAutoCompletionWordIndex;
					if (currentAutoCompletionWordIndex < 0)
						currentAutoCompletionWordIndex = autoCompletionsStrLength - 1;
				}
				else if (isDownArrowButton(symbol))
					currentAutoCompletionWordIndex = (currentAutoCompletionWordIndex + 1) % autoCompletionsStrLength;

				else if (isLeftArrowButton(symbol)) {
					moveBetweenCompletions = false;
					symbol = -1;
					continue;
				}
				currentLineLength = changeAutoCompletionWord(
					startInputX, startInputY, buffer,
					currentLineLength, wordDatabase, 
					indexes[0] + currentAutoCompletionWordIndex
				);
			}
			else if (isEnterButton(symbol)) {
				*indexOnPressedEnter = indexes[0] + currentAutoCompletionWordIndex;
				moveBetweenCompletions = false;
			}
			else {
				*indexOnPressedEnter = -1;
				moveBetweenCompletions = false;
			}
		}
		Sleep(SLEEP_TIME_MS);
	}

	cursorMoveTo(startInputX, startInputY);

	if (currentLineLength != 0)
		for (int i = 0; i <= currentLineLength - 1; i++)
			putchar(' ');

	cursorMoveTo(startInputX, startInputY);
	printf("%s", buffer);

	free(indexes);
	return symbol;
}

static unsigned int changeAutoCompletionWord(
	const int startX, const int startY, const char* const restrict buffer,
	const unsigned int pastLineLength, const Database* const restrict wordDatabase,
	const RecordCount index
) {
	cursorMoveTo(startX, startY);

	if(pastLineLength != 0)
		for (int i = 0; i <= pastLineLength - 1; ++i)
			putchar(' ');

	cursorMoveTo(startX, startY);
	printf(SET_WHITE_COLOR "%s" DROP_COLOR_SET, buffer);
	const unsigned int bufferLength = strlen(buffer);

	const char* const restrict newAutoCompletionWord = databaseGetKeyByIndex(wordDatabase, index);

	printf(SET_GREY_COLOR "%s" DROP_COLOR_SET, (newAutoCompletionWord + bufferLength));
	
	const unsigned int length = strlen(newAutoCompletionWord);
	free(newAutoCompletionWord);

	return length;
}