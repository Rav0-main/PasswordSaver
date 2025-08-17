#pragma once

#include "multiinputfield.h"
#include "multichoicefield.h"

#define DROP_COLOR_SET "\033[0m"
#define SET_WHITE_COLOR "\033[97m"
#define SET_GREY_COLOR "\033[90m"
#define SECURITY_SYMBOL '*'

void _cursorMoveTo(const int x, const int y);
bool _isArrowButton(const char symbol);
bool _isDownArrowButton(const char symbol);
bool _isUpArrowButton(const char symbol);
bool _isLeftArrowButton(const char symbol);
void _outputInputedValueAfterMoving(const InputField* field, const char* textColor, const bool showSecurityFields);
void _outputTextOf(const ChoiceField* field, const char* textColor);
bool _isEnterButton(const char symbol);
bool _isBackspaceButton(const char symbol);