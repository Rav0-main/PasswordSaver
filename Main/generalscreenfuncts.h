#pragma once

#define TITLE_MAX_LENGTH 64

/*
* Delete all text from screen
*/
void clearScreen(void);

/*
* Prints "Press <Enter> to continue" and wait when pressed enter
*/
void showToPressEnter(void);

/*
* Set name on console
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