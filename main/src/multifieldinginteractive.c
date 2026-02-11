#include "multifieldinginteractive.h"
#include <windows.h>
#include <stdio.h>

void cursorMoveTo(const int x, const int y) {
	COORD coord = { .X = x, .Y = y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool isArrowButton(const char symbol) {
	return symbol == ARROW_BUTTON;
}

bool isDownArrowButton(const char symbol) {
	return symbol == DOWN_ARROW_BUTTON;
}

bool isUpArrowButton(const char symbol) {
	return symbol == UP_ARROW_BUTTON;
}

bool isLeftArrowButton(const char symbol) {
	return symbol == LEFT_ARROW_BUTTON;
}

void outputInputedValueAfterMoving(
	const InputField* const restrict field,
	const char* const restrict textColor,
	const bool showSecurityFields
) {
	if (field->isSecurity && !showSecurityFields) {
		printf("%s%s", textColor, field->prompt);

		const int inputedValueLength = (int)strlen(field->value);
		for (int i = 0; i <= inputedValueLength - 1; ++i)
			putchar(SECURITY_SYMBOL);

		printf(DROP_COLOR_SET);
	}
	else
		printf("%s%s%s" DROP_COLOR_SET, textColor,
			field->prompt, field->value
		);
}

bool isEnterButton(const char symbol) {
	return symbol == '\r';
}

bool isBackspaceButton(const char symbol) {
	return symbol == '\b';
}

bool isSpecialButton(const char symbol) {
	return symbol == 224 || symbol == 0;
}

void outputTextOf(
	const ChoiceField* const restrict field,
	const char* const restrict textColor
) {
	printf("%s%s" DROP_COLOR_SET, textColor, field->prompt);
}