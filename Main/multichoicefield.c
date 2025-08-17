#include "multichoicefield.h"
#include "multifieldinginteractive.h"
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <string.h>

int displayMultiChoiceFields(ChoiceField** fields, const unsigned int fieldsLength, const int startY) {
	unsigned int* cursorXPositions = (unsigned int*)calloc(fieldsLength, sizeof(unsigned int));
	if (cursorXPositions == NULL)
		return 0;

	_cursorMoveTo(0, startY);
	printf(SET_WHITE_COLOR "%s" DROP_COLOR_SET, fields[0]->outputLine);
	cursorXPositions[0] = strlen(fields[0]->outputLine);
	_cursorMoveTo(0, startY + 1);

	for (unsigned int i = 1; i <= fieldsLength - 1; i++) {
		printf(SET_GREY_COLOR "%s" DROP_COLOR_SET, fields[i]->outputLine);
		cursorXPositions[i] = strlen(fields[0]->outputLine);
		_cursorMoveTo(0, startY + i + 1);
	}

	int currentLine = 0; //0 - first line, 1 - second line, 2 - third line etc.
	_cursorMoveTo(cursorXPositions[currentLine], startY);

	bool run = true;
	char symbol;

	while (run) {
		if (_kbhit() == 0)
			continue;

		symbol = _getch();

		if (_isArrowButton(symbol)) {
			symbol = _getch();

			if (_isDownArrowButton(symbol)) {
				_cursorMoveTo(0, startY + currentLine);
				_outputTextOf(fields[currentLine], SET_GREY_COLOR);
				currentLine = (currentLine + 1) % fieldsLength;
			}
			else if (_isUpArrowButton(symbol)) {
				_cursorMoveTo(0, startY + currentLine);
				_outputTextOf(fields[currentLine], SET_GREY_COLOR);
				if (--currentLine < 0)
					currentLine = fieldsLength - 1;
			}

			else if (_isLeftArrowButton(symbol)) {
				run = false;
				free(cursorXPositions);
				return -1;
			}

			_cursorMoveTo(0, startY + currentLine);
			_outputTextOf(fields[currentLine], SET_WHITE_COLOR);
		}

		else if (_isEnterButton(symbol))
			run = false;
	}

	_cursorMoveTo(0, fieldsLength);
	free(cursorXPositions);

	return fields[currentLine]->code;
}