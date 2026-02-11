#include "multiinputfield.h"
#include "multifieldinginteractive.h"

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>

void displayMultiInputFields(
	InputField** const restrict fields,
	const unsigned int fieldsLength, const int startY
) {
	cursorMoveTo(0, startY);

	unsigned int* const cursorXPositions = (unsigned int*)calloc(fieldsLength, sizeof(unsigned int));
	unsigned int* const lengthsOfInputedValues = (unsigned int*)calloc(fieldsLength, sizeof(unsigned int));

	if (cursorXPositions == NULL || lengthsOfInputedValues == NULL)
		return;

	cursorMoveTo(0, startY);
	printf(SET_WHITE_COLOR "%s" DROP_COLOR_SET, fields[0]->prompt);
	cursorXPositions[0] = strlen(fields[0]->prompt);
	lengthsOfInputedValues[0] = 0;
	cursorMoveTo(0, startY + 1);

	for (unsigned int i = 1; i <= fieldsLength - 1; ++i) {
		cursorXPositions[i] = strlen(fields[i]->prompt);
		lengthsOfInputedValues[i] = 0;
		printf(SET_GREY_COLOR "%s" DROP_COLOR_SET, fields[i]->prompt);
		cursorMoveTo(0, startY + i + 1);
	}

	int currentLine = 0; //0 - first line, 1 - second line, 2 - third line etc.
	cursorMoveTo(cursorXPositions[currentLine], startY);

	bool run = true;
	bool showSecurityFields = false;
	char symbol;

	while (run) {
		Sleep(SLEEP_TIME_MS);

		if (!_kbhit())
			continue;

		symbol = _getch();
		if (isArrowButton(symbol)) {
			symbol = _getch();

			if (isDownArrowButton(symbol)) {
				cursorMoveTo(0, startY + currentLine);
				outputInputedValueAfterMoving(fields[currentLine], SET_GREY_COLOR, showSecurityFields);
				currentLine = (currentLine + 1) % fieldsLength;
			}
			
			else if (isUpArrowButton(symbol)) {
				cursorMoveTo(0, startY + currentLine);
				outputInputedValueAfterMoving(fields[currentLine], SET_GREY_COLOR, showSecurityFields);
				if (--currentLine < 0)
					currentLine = fieldsLength - 1;
			}

			else if (isLeftArrowButton(symbol)) {
				run = false;
				fields[0]->value[0] = -1;
				++lengthsOfInputedValues[0];
			}

			cursorMoveTo(0, startY + currentLine);
			outputInputedValueAfterMoving(fields[currentLine], SET_WHITE_COLOR, showSecurityFields);
		}

		else if (isEnterButton(symbol) && currentLine == fieldsLength - 1)
			run = false;

		else if (isEnterButton(symbol)) {
			cursorMoveTo(0, startY + currentLine);
			outputInputedValueAfterMoving(fields[currentLine], SET_GREY_COLOR, showSecurityFields);
			currentLine = (currentLine + 1) % fieldsLength;
			
			cursorMoveTo(0, startY + currentLine);
			outputInputedValueAfterMoving(fields[currentLine], SET_WHITE_COLOR, showSecurityFields);
		}

		else if (isBackspaceButton(symbol)) {
			if (lengthsOfInputedValues[currentLine] > 0) {
				--lengthsOfInputedValues[currentLine];

				fields[currentLine]->value[lengthsOfInputedValues[currentLine]] = '\0';
				--cursorXPositions[currentLine];
				
				cursorMoveTo(cursorXPositions[currentLine], startY + currentLine);
				putchar(' ');
				cursorMoveTo(cursorXPositions[currentLine], startY + currentLine);
			}
		}

		else if (symbol == 126) {
			showSecurityFields = !showSecurityFields;
			for (int i = 0; i <= fieldsLength - 1; ++i) {
				cursorMoveTo(0, startY + i);

				if (i == currentLine)
					outputInputedValueAfterMoving(fields[i], SET_WHITE_COLOR, showSecurityFields);
				else
					outputInputedValueAfterMoving(fields[i], SET_GREY_COLOR, showSecurityFields);
			}
			cursorMoveTo(cursorXPositions[currentLine], startY + currentLine);
		}

		else if (symbol >= 32 && symbol <= 125) {
			if (lengthsOfInputedValues[currentLine] <= INPUTED_VALUE_MAX_LENGTH - 1) {
				fields[currentLine]->value[lengthsOfInputedValues[currentLine]] = symbol;
				++lengthsOfInputedValues[currentLine];
				++cursorXPositions[currentLine];

				if (fields[currentLine]->isSecurity && !showSecurityFields)
					putchar(SECURITY_SYMBOL);
				else
					putchar(symbol);
			}
		}
	}

	for (currentLine = 0; currentLine <= fieldsLength - 1; ++currentLine) 
		fields[currentLine]->value[lengthsOfInputedValues[currentLine]] = '\0';

	putchar('\n');
	free(cursorXPositions);
	free(lengthsOfInputedValues);
}