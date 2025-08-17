#include "../Log2Database/main.h"
#include "autocompletioninputfield.h"
#include "multifieldinginteractive.h"
#include "fillstrbuffer.h"
#include <stdarg.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char _runOneCharOfAutoCompletionInputField(const int startX, const int startY, char* buffer, const Database* wordDatabase, int* indexOnPressedEnter);
unsigned int _changeAutoCompletionWord(const int startX, const int startY, char* buffer, const unsigned int pastLineLength, const Database* wordDatabase, const RecordCount index);

char* displayAutoCompletionInputField(AutoCompletionInputField inputField, const Database* wordDatabase) {
	char* buffer = (char*)calloc(inputField.bufferLength + 1, sizeof(char));

	if (buffer == NULL)
		return NULL;

	fillString(buffer, inputField.bufferLength, '\0');

	const unsigned int outputLineLength = strlen(inputField.outputLine);
	bool runInput = true;
	unsigned int inputedValueLength = 0;
	char symbol = 0;
	int indexOnPressedEnter;

	printf(SET_WHITE_COLOR "%s" DROP_COLOR_SET, inputField.outputLine);

	while (runInput) {
		symbol = _runOneCharOfAutoCompletionInputField(inputField.startX+outputLineLength, inputField.startY, buffer, wordDatabase, &indexOnPressedEnter);

		if (symbol == -1) {
			buffer[0] = -1;
			runInput = false;
		}

		if (!_isBackspaceButton(symbol) && !_isEnterButton(symbol) && symbol >= 32 && symbol <= 126) {
			printf("%c", symbol);

			buffer[inputedValueLength] = symbol;
			inputedValueLength++;
			buffer[inputedValueLength] = '\0';
		}

		else if (_isBackspaceButton(symbol) && inputedValueLength > 0) {
			_cursorMoveTo(inputField.startX + inputedValueLength + outputLineLength - 1, inputField.startY);
			printf(" ");

			inputedValueLength--;
			buffer[inputedValueLength] = '\0';
		}

		else if (_isEnterButton(symbol) && indexOnPressedEnter != -1) {
			free(buffer);

			buffer = databaseGetKeyByIndex(wordDatabase, indexOnPressedEnter);
			runInput = false;
		}
		else if (_isEnterButton(symbol))
			runInput = false;
	}

	return buffer;
}

char _runOneCharOfAutoCompletionInputField(const int startInputX, const int startInputY, char* buffer, const Database* wordDatabase, int* indexOnPressedEnter) {
	_cursorMoveTo(startInputX, startInputY);

	const unsigned int bufferLength = strlen(buffer);

	RecordCount* indexes = databaseGetIndexesOfKeysWhichStartWith(wordDatabase, buffer);
	unsigned int autoCompletionsStrLength = 0;

	if (indexes != NULL)
		autoCompletionsStrLength = indexes[1] - indexes[0] + 1;

	else {
		printf(SET_WHITE_COLOR "%s" DROP_COLOR_SET, buffer);
		
		char symbol;
		_cursorMoveTo(startInputX + bufferLength, startInputY);

		while (true) {
			if (_kbhit() == 0)
				continue;

			symbol = _getch();
			if (_isArrowButton(symbol)) {
				symbol = _getch();
				if (_isLeftArrowButton(symbol)) {
					*indexOnPressedEnter = -1;
					return -1;
				}
			}

			else if (_isEnterButton(symbol)) {
				*indexOnPressedEnter = -1;
				return symbol;
			}

			else if (_isBackspaceButton(symbol)) {
				*indexOnPressedEnter = -1;
				return symbol;
			}

			else if (symbol >= 32 && symbol <= 126) {
				*indexOnPressedEnter = -1;
				return symbol;
			}
		}
	}

	unsigned int currentLineLength = _changeAutoCompletionWord(startInputX, startInputY, buffer, 0, wordDatabase, indexes[0]);
	_cursorMoveTo(startInputX + bufferLength, startInputY);

	bool canMoveBetweenCompletions = true;
	int currentAutoCompletionWordIndex = 0;
	char symbol;

	while (canMoveBetweenCompletions) {
		if (_kbhit() == 0)
			continue;

		symbol = _getch();

		if (_isArrowButton(symbol)) {
			symbol = _getch();

			if (_isUpArrowButton(symbol)) {
				currentAutoCompletionWordIndex--;
				if (currentAutoCompletionWordIndex < 0)
					currentAutoCompletionWordIndex = autoCompletionsStrLength - 1;
			}
			else if (_isDownArrowButton(symbol))
				currentAutoCompletionWordIndex = (currentAutoCompletionWordIndex + 1) % autoCompletionsStrLength;

			else if (_isLeftArrowButton(symbol)) {
				canMoveBetweenCompletions = false;
				symbol = -1;
				continue;
			}
			currentLineLength = _changeAutoCompletionWord(startInputX, startInputY, buffer, currentLineLength, wordDatabase, indexes[0] + currentAutoCompletionWordIndex);
		}
		else if (symbol == '\r') {
			*indexOnPressedEnter = indexes[0] + currentAutoCompletionWordIndex;
			canMoveBetweenCompletions = false;
		}
		else {
			*indexOnPressedEnter = -1;
			canMoveBetweenCompletions = false;
		}
	}

	_cursorMoveTo(startInputX, startInputY);

	if (currentLineLength != 0)
		for (int i = 0; i <= currentLineLength - 1; i++)
			printf(" ");

	_cursorMoveTo(startInputX, startInputY);
	printf("%s", buffer);

	free(indexes);
	return symbol;
}

unsigned int _changeAutoCompletionWord(const int startX, const int startY, char* buffer, const unsigned int pastLineLength, const Database* wordDatabase, const RecordCount index) {
	_cursorMoveTo(startX, startY);

	if(pastLineLength != 0)
		for (int i = 0; i <= pastLineLength - 1; i++)
			printf(" ");

	_cursorMoveTo(startX, startY);
	printf(SET_WHITE_COLOR "%s" DROP_COLOR_SET, buffer);
	const unsigned int bufferLength = strlen(buffer);

	const char* newAutoCompletionWord = databaseGetKeyByIndex(wordDatabase, index);

	printf(SET_GREY_COLOR "%s" DROP_COLOR_SET, (newAutoCompletionWord + bufferLength));
	
	const unsigned int length = strlen(newAutoCompletionWord);
	free(newAutoCompletionWord);

	return length;
}