#include "multifieldinginteractive.h"
#include <windows.h>
#include <stdio.h>

void _cursorMoveTo(const int x, const int y) {
	COORD coord = { .X = x, .Y = y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool _isArrowButton(const char symbol) {
	return symbol == -32;
}

bool _isDownArrowButton(const char symbol) {
	return symbol == 80;
}

bool _isUpArrowButton(const char symbol) {
	return symbol == 72;
}

bool _isLeftArrowButton(const char symbol) {
	return symbol == 75;
}

void _outputInputedValueAfterMoving(const InputField* field, const char* textColor, const bool showSecurityFields) {
	if (field->isSecurity && !showSecurityFields) {
		printf("%s%s", textColor, field->outputLine);
		for (int i = 0; i <= (int)strlen(field->inputedValue) - 1; i++)
			printf("%c", SECURITY_SYMBOL);
		printf(DROP_COLOR_SET);
	}
	else
		printf("%s%s%s" DROP_COLOR_SET, textColor, field->outputLine, field->inputedValue);
}

bool _isEnterButton(const char symbol) {
	return symbol == '\r';
}

bool _isBackspaceButton(const char symbol) {
	return symbol == '\b';
}

void _outputTextOf(const ChoiceField* field, const char* textColor) {
	printf("%s%s" DROP_COLOR_SET, textColor, field->outputLine);
}