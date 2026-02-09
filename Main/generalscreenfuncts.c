#include "generalscreenfuncts.h"
#include <stdio.h>
#include <windows.h>
#include <stdarg.h>

inline static void waitToPressEnter(void);

inline void clearScreen(void) {
	system("cls");
}

inline void showToPressEnter(void) {
    printf("Press <Enter> to continue\n");
    waitToPressEnter();
}

static void waitToPressEnter(void) {
    getchar();
}

inline void titleProcess(const char* title) {
    char command[TITLE_MAX_LENGTH + 6];
    
    sprintf(command, "title %s", title);

    system(command);
}

inline void showRedErrorWithMessage(const char* message, ...) {
    showRedError();

    printf("\033[3mMessage\033[0m: ");
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
}

inline void showRedError(void) {
    printf("\033[1m\033[31mERROR!\033[0m\n");
}

inline void showGreenSuccessWithMessage(const char* message, ...) {
    showGreenSuccess();

    printf("\033[3mMessage\033[0m: ");
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
}

inline void showGreenSuccess(void) {
    printf("\033[1m\033[32m\SUCCESS!\033[0m\n");
}

inline ScreenSize getConsoleWindowSize(void) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width, height;

    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    else {
        //default
        width = 80;
        height = 25;
    }

    return (ScreenSize) { width, height };
}