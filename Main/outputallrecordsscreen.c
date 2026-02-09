#include "accountdata.h"
#include "generalscreenfuncts.h"
#include "screencodes.h"
#include "recorddatamanipulations.h"
#include "multifieldinginteractive.h"
#include "outputallrecordsscreen.h"

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>

inline static int calculateRecordCountPerPage(const int height);

static void displayScreen(
    const Database* const restrict accountDatabase, const RecordCount currentTopIndex,
    const RecordCount recordCount, ScreenSize screenSize,
    const char* const restrict password
);

static void displayRecord(
    const Database* const restrict accountDatabase,
    const RecordCount index, const char* const restrict password
);

inline static bool isValidButton(const char symbol);

static bool executeCommandOf(
    char symbol, RecordCount* const restrict currentTopIndex,
    RecordCount recordCount, int recordCountPerPage,
    bool* needRedraw
);

inline static void upTopIndex(
    RecordCount* const restrict currentTopIndex, 
    bool* const restrict needRedraw
);

inline static void downTopIndex(
    RecordCount* const restrict currentTopIndex, int recordCountPerPage,
    RecordCount recordCount, bool* const restrict needRedraw
);

void runOutputAllRecordsScreen(
    const Database* const restrict accountDatabase,
    Screen* const restrict currentScreen, const char* restrict const password
) {
    const RecordCount recordCount = databaseGetCountOfRecords(
        accountDatabase
    );

    if (recordCount == 0) {
        printf("You have not records in database.\n");
        showToPressEnter();
        *currentScreen = MainMenuScreen;
        return;
    }

    const ScreenSize screenSize = {
        .width = getConsoleWindowSize().width,
        .height = getConsoleWindowSize().height - 3
    };

    int recordCountPerPage = calculateRecordCountPerPage(screenSize.height);

    RecordCount currentTopIndex = 0;
    bool needRedraw = true;

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    displayScreen(accountDatabase, currentTopIndex, recordCount, screenSize, password);
    needRedraw = false;

    char symbol;
    while (true) {
        if (_kbhit()) {
            symbol = _getch();

            if (isValidButton(symbol)) {
                if (!executeCommandOf(symbol,
                    &currentTopIndex, recordCount,
                    recordCountPerPage, &needRedraw
                )) {
                    break;
                }

                else if (needRedraw) {
                    displayScreen(accountDatabase, currentTopIndex, recordCount, screenSize, password);
                    needRedraw = false;
                }
            }
        }

        Sleep(SLEEP_TIME_MS);
    }

    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    *currentScreen = MainMenuScreen;
}

static int calculateRecordCountPerPage(const int height) {
    int lines = height - (FIELD_COUNT_IN_RECORD_DATA+3);
    if (lines <= 0)
        return 1; 

    return lines / (FIELD_COUNT_IN_RECORD_DATA + 1);
}

static void displayScreen(
    const Database* const restrict accountDatabase, const RecordCount currentTopIndex,
    const RecordCount recordCount, ScreenSize screenSize,
    const char* const restrict password
) {
    clearScreen();

    int recordCountPerPage = calculateRecordCountPerPage(screenSize.height);

    int outputedRecordCount = 0;
    RecordCount index;
    for (int i = 0; i < recordCountPerPage; i++) {
        index = currentTopIndex + i;
        if (index < recordCount) {
            displayRecord(accountDatabase, index, password);
            ++outputedRecordCount;

            if (i < recordCountPerPage - 1 && index + 1 < recordCount)
                printf("%-*s\n", screenSize.width, " ");
        }
    }

    int outputedLineCount = 2+(outputedRecordCount * (FIELD_COUNT_IN_RECORD_DATA + 1)) + \
                                       (outputedRecordCount > 0 ? outputedRecordCount - 1 : 0);

    //fill empty space
    for (int i = outputedLineCount; i < screenSize.height - 1; i++)
        printf("%-*s\n", screenSize.width, " ");

    //print status
    RecordCount endRecord = currentTopIndex + outputedRecordCount - 1;
    if (endRecord < currentTopIndex)
        endRecord = currentTopIndex;
    
    printf("\nRecords " SET_INVERT_COLOR "%lld" \
        DROP_COLOR_SET " - " SET_INVERT_COLOR "%lld" DROP_COLOR_SET \
        " of " SET_INVERT_COLOR "%lld" DROP_COLOR_SET,
        currentTopIndex + 1,
        endRecord + 1,
        recordCount
    );
}

static void displayRecord(
    const Database* const restrict accountDatabase,
    const RecordCount index, const char* const restrict password
) {
    const char* key;
    RecordData record;

    key = databaseGetKeyByIndex(accountDatabase, index);
    databaseGetValueByKey(accountDatabase, key, &record);

    decryptRecord(&record, password);

    if (index == 0)
        printf("\033[3m%lld-st record\033[0m:\n", index + 1);

    else if (index == 1)
        printf("\033[3m%lld-nd record\033[0m:\n", index + 1);

    else if (index == 2)
        printf("\033[3m%lld-rd record\033[0m:\n", index + 1);

    else
        printf("\033[3m%lld-th record\033[0m:\n", index + 1);

    outputRecord(&record);
    putchar('\n');

    free(key);
}

static bool isValidButton(const char symbol) {
    return isArrowButton(symbol) || isSpecialButton(symbol);
}

static bool executeCommandOf(
    char symbol, RecordCount* const restrict currentTopIndex,
    RecordCount recordCount, const int recordCountPerPage,
    bool* needRedraw
) {
    *needRedraw = false;

    if (isArrowButton(symbol) || isSpecialButton(symbol)) {
        symbol = _getch();

        switch (symbol) {
        case UP_ARROW_BUTTON: 
            upTopIndex(currentTopIndex, needRedraw);
            break;

        case DOWN_ARROW_BUTTON:
            downTopIndex(currentTopIndex, recordCountPerPage, recordCount, needRedraw);
            break;

        case LEFT_ARROW_BUTTON:
            *needRedraw = false;
            return false;

        case PAGE_UP_BUTTON: 
            if (*currentTopIndex > 0) {
                if (*currentTopIndex < recordCountPerPage)
                    *currentTopIndex = 0;
                else
                    *currentTopIndex -= recordCountPerPage;

                *needRedraw = true;
            }
            break;

        case PAGE_DOWN_BUTTON: 
            if (*currentTopIndex + recordCountPerPage < recordCount) {
                *currentTopIndex += recordCountPerPage;
                
                if (*currentTopIndex + recordCountPerPage > recordCount) {
                    if (recordCount < recordCountPerPage)
                        *currentTopIndex = 0;
                    else
                        *currentTopIndex = recordCount - recordCountPerPage;
                }

                *needRedraw = true;
            }
            break;

        case HOME_BUTTON:
            if (*currentTopIndex != 0) {
                *currentTopIndex = 0;
                *needRedraw = true;
            }
            break;

        case END_BUTTON: 
            if (*currentTopIndex + recordCountPerPage < recordCount) {
                if (recordCount < recordCountPerPage)
                    *currentTopIndex = 0;
                else
                    *currentTopIndex = recordCount - recordCountPerPage;
                
                *needRedraw = true;
            }
            break;
        }
    }

    return true;
}

inline static void upTopIndex(
    RecordCount* const restrict currentTopIndex,
    bool* const restrict needRedraw
) {
    --(*currentTopIndex);
    *needRedraw = true;
    
    if (*currentTopIndex < 0) {
        *currentTopIndex = 0;
        *needRedraw = false;
    }
}

inline static void downTopIndex(
    RecordCount* const restrict currentTopIndex, int recordCountPerPage,
    RecordCount recordCount, bool* const restrict needRedraw
) {
    if (*currentTopIndex + recordCountPerPage < recordCount) {
        ++(*currentTopIndex);
        *needRedraw = true;
    }
}