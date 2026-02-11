#pragma once

#include "multiinputfield.h"
#include "multichoicefield.h"

#define DROP_COLOR_SET "\033[0m"
#define SET_WHITE_COLOR "\033[97m"
#define SET_GREY_COLOR "\033[90m"
#define SET_INVERT_COLOR "\033[7m"
#define SECURITY_SYMBOL '*'

#define ARROW_BUTTON -32
#define UP_ARROW_BUTTON 72
#define DOWN_ARROW_BUTTON 80
#define LEFT_ARROW_BUTTON 75
#define PAGE_UP_BUTTON 73
#define PAGE_DOWN_BUTTON 81
#define HOME_BUTTON 71
#define END_BUTTON 79

#define SLEEP_TIME_MS 35

void cursorMoveTo(const int x, const int y);
bool isArrowButton(const char symbol);
bool isDownArrowButton(const char symbol);
bool isUpArrowButton(const char symbol);
bool isLeftArrowButton(const char symbol);

void outputInputedValueAfterMoving(
	const InputField* const restrict field, const char* const restrict textColor,
	const bool showSecurityFields
);

void outputTextOf(
	const ChoiceField* const restrict field,
	const char* const restrict textColor
);

bool isEnterButton(const char symbol);
bool isBackspaceButton(const char symbol);
bool isSpecialButton(const char symbol);