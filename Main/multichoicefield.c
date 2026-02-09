#include "multifieldinginteractive.h"

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <string.h>

ChoiceCode displayMultiChoiceFields(
	const ChoiceField** const restrict fields,
	const unsigned int fieldsLength, const int startY
) {
	unsigned int* cursorXPositions = (unsigned int*)calloc(fieldsLength, sizeof(unsigned int));
	if (cursorXPositions == NULL)
		return ERROR_CODE;

	cursorMoveTo(0, startY);
	printf(SET_WHITE_COLOR "%s" DROP_COLOR_SET, fields[0]->prompt);
	cursorXPositions[0] = strlen(fields[0]->prompt);
	cursorMoveTo(0, startY + 1);

	for (unsigned int i = 1; i <= fieldsLength - 1; i++) {
		printf(SET_GREY_COLOR "%s" DROP_COLOR_SET, fields[i]->prompt);
		cursorXPositions[i] = strlen(fields[0]->prompt);
		cursorMoveTo(0, startY + i + 1);
	}

	int currentLine = 0; //0 - first line, 1 - second line, 2 - third line etc.
	cursorMoveTo(cursorXPositions[currentLine], startY);

	bool run = true;
	char symbol;

	while (run) {
		if (_kbhit()) {
			symbol = _getch();

			if (isArrowButton(symbol)) {
				symbol = _getch();

				if (isDownArrowButton(symbol)) {
					cursorMoveTo(0, startY + currentLine);
					outputTextOf(fields[currentLine], SET_GREY_COLOR);
					currentLine = (currentLine + 1) % fieldsLength;
				}

				else if (isUpArrowButton(symbol)) {
					cursorMoveTo(0, startY + currentLine);
					outputTextOf(fields[currentLine], SET_GREY_COLOR);
					if (--currentLine < 0)
						currentLine = fieldsLength - 1;
				}

				else if (isLeftArrowButton(symbol)) {
					run = false;

					free(cursorXPositions);
					return PRESSED_LEFT_ARROW;
				}

				cursorMoveTo(0, startY + currentLine);
				outputTextOf(fields[currentLine], SET_WHITE_COLOR);
			}

			else if (isEnterButton(symbol))
				run = false;
		}
		Sleep(SLEEP_TIME_MS);
	}

	cursorMoveTo(0, fieldsLength);
	free(cursorXPositions);

	return fields[currentLine]->code;
}