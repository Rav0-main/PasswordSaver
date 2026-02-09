#pragma once

#define TITLE_MAX_LENGTH 64

/*
* Deletes all text from screen
*/
void clearScreen(void);

/*
* Prints "Press <Enter> to continue" and waits when pressed enter
*/
void showToPressEnter(void);

/*
* Set name of console
*/
void titleProcess(const char* title);

void showRedErrorWithMessage(const char* message, ...);

/*
* printf("ERROR!\n") in red color
*/
void showRedError(void);

void showGreenSuccessWithMessage(const char* message, ...);

/*
* printf("SUCCESS!\n") in green color
*/
void showGreenSuccess(void);

typedef struct _ScreenSize {
	const int width;
	const int height;
} ScreenSize;

/*
* Set 'width' and 'height' of console screen in symbols
*/
ScreenSize getConsoleWindowSize(void);