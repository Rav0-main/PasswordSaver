#include "generalscreenfuncts.h"
#include <stdio.h>
#include <windows.h>
#include <stdarg.h>

void _waitToPressEnter(void);

void clearScreen(void) {
	system("cls");
}

void showToPressEnter(void) {
    printf("Press <Enter> to continue\n");
    _waitToPressEnter();
}

void _waitToPressEnter(void) {
    getchar();
}

void titleProcess(const char* title) {
    const char command[TITLE_MAX_LENGTH + 6];
    
    sprintf(command, "title %s", title);

    system(command);
}

void showRedErrorWithMessage(const char* message, ...) {
    showRedError();

    printf("\033[3mMessage\033[0m: ");
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
}

void showRedError(void) {
    printf("\033[1m\033[31mERROR!\033[0m\n");
}

void showGreenSuccessWithMessage(const char* message, ...) {
    showGreenSuccess();

    printf("\033[3mMessage\033[0m: ");
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
}

void showGreenSuccess(void) {
    printf("\033[1m\033[32m\SUCCESS!\033[0m\n");
}