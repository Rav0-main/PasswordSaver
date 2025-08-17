#include "multiinputfield.h"
#include "multifieldinginteractive.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>

void displayMultiInputFields(InputField** fields, const unsigned int fieldsLength, const int startY) {
	_cursorMoveTo(0, startY);

	unsigned int* const cursorXPositions = (unsigned int*)calloc(fieldsLength, sizeof(unsigned int));
	unsigned int* const lengthsOfInputedValues = (unsigned int*)calloc(fieldsLength, sizeof(unsigned int));

	if (cursorXPositions == NULL || lengthsOfInputedValues == NULL)
		return;

	_cursorMoveTo(0, startY);
	printf(SET_WHITE_COLOR "%s" DROP_COLOR_SET, fields[0]->outputLine);
	cursorXPositions[0] = strlen(fields[0]->outputLine);
	lengthsOfInputedValues[0] = 0;
	_cursorMoveTo(0, startY + 1);

	for (unsigned int i = 1; i <= fieldsLength - 1; i++) {
		cursorXPositions[i] = strlen(fields[i]->outputLine);
		lengthsOfInputedValues[i] = 0;
		printf(SET_GREY_COLOR "%s" DROP_COLOR_SET, fields[i]->outputLine);
		_cursorMoveTo(0, startY + i + 1);
	}

	int currentLine = 0; //0 - first line, 1 - second line, 2 - third line etc.
	_cursorMoveTo(cursorXPositions[currentLine], startY);

	bool run = true;
	bool showSecurityFields = false;
	char symbol;

	while (run) {
		if (_kbhit() == 0)
			continue;

		symbol = _getch();
		if (_isArrowButton(symbol)) {
			symbol = _getch();

			if (_isDownArrowButton(symbol)) {
				_cursorMoveTo(0, startY + currentLine);
				_outputInputedValueAfterMoving(fields[currentLine], SET_GREY_COLOR, showSecurityFields);
				currentLine = (currentLine + 1) % fieldsLength;
			}
			else if (_isUpArrowButton(symbol)) {
				_cursorMoveTo(0, startY + currentLine);
				_outputInputedValueAfterMoving(fields[currentLine], SET_GREY_COLOR, showSecurityFields);
				if (--currentLine < 0)
					currentLine = fieldsLength - 1;
			}

			else if (_isLeftArrowButton(symbol)) {
				run = false;
				fields[0]->inputedValue[0] = -1;
				lengthsOfInputedValues[0]++;
			}

			_cursorMoveTo(0, startY + currentLine);
			_outputInputedValueAfterMoving(fields[currentLine], SET_WHITE_COLOR, showSecurityFields);
		}

		else if (_isEnterButton(symbol) && currentLine == fieldsLength - 1)
			run = false;

		else if (_isEnterButton(symbol)) {
			_cursorMoveTo(0, startY + currentLine);
			_outputInputedValueAfterMoving(fields[currentLine], SET_GREY_COLOR, showSecurityFields);
			currentLine = (currentLine + 1) % fieldsLength;
			
			_cursorMoveTo(0, startY + currentLine);
			_outputInputedValueAfterMoving(fields[currentLine], SET_WHITE_COLOR, showSecurityFields);
		}

		else if (_isBackspaceButton(symbol)) {
			if (lengthsOfInputedValues[currentLine] > 0) {
				lengthsOfInputedValues[currentLine]--;
				fields[currentLine]->inputedValue[lengthsOfInputedValues[currentLine]] = '\0';
				cursorXPositions[currentLine]--;
				_cursorMoveTo(cursorXPositions[currentLine], startY + currentLine);
				printf(" ");
				_cursorMoveTo(cursorXPositions[currentLine], startY + currentLine);
			}
		}

		else if (symbol == 126) {
			showSecurityFields = !showSecurityFields;
			for (int i = 0; i <= fieldsLength - 1; i++) {
				_cursorMoveTo(0, startY + i);

				if (i == currentLine)
					_outputInputedValueAfterMoving(fields[i], SET_WHITE_COLOR, showSecurityFields);
				else
					_outputInputedValueAfterMoving(fields[i], SET_GREY_COLOR, showSecurityFields);
			}
			_cursorMoveTo(cursorXPositions[currentLine], startY + currentLine);
		}

		else if (symbol >= 32 && symbol <= 125) {
			if (lengthsOfInputedValues[currentLine] <= INPUTED_VALUE_MAX_LENGTH - 1) {
				fields[currentLine]->inputedValue[lengthsOfInputedValues[currentLine]] = symbol;
				lengthsOfInputedValues[currentLine]++;
				cursorXPositions[currentLine]++;
				if (fields[currentLine]->isSecurity && !showSecurityFields)
					printf("%c", SECURITY_SYMBOL);
				else
					printf("%c", symbol);
			}
		}
	}

	for (currentLine = 0; currentLine <= fieldsLength - 1; currentLine++) 
		fields[currentLine]->inputedValue[lengthsOfInputedValues[currentLine]] = '\0';

	printf("\n");
	free(cursorXPositions);
	free(lengthsOfInputedValues);
}