#include "deleterecordscreen.h"
#include "autocompletioninputfield.h"
#include "accountdata.h"
#include "generalscreenfuncts.h"
#include "screencodes.h"
#include <stdlib.h>

void runDeleteRecordScreen(const Database* accountDatabase, int* currentScreen) {
	AutoCompletionInputField inputField = {
		.outputLine = "Input record name: ",
		.bufferLength = RECORD_NAME_MAX_LENGTH,
		.startX = 0,
		.startY = 0
	};

	const char* inputedRecordName = displayAutoCompletionInputField(inputField, accountDatabase);

	//user pressed left arrow button
	if (inputedRecordName[0] == -1) {
		free(inputedRecordName);
		*currentScreen = MAIN_MENU_SCREEN;
		return;
	}

	else if (!databaseExistsKey(accountDatabase, inputedRecordName)) {
		clearScreen();
		showRedErrorWithMessage("Record with name: %s not found!\n", inputedRecordName);
		free(inputedRecordName);
		showToPressEnter();

		return;
	}

	databaseDeleteValueByKey(accountDatabase, inputedRecordName);
	
	*currentScreen = MAIN_MENU_SCREEN;
	
	clearScreen();
	showGreenSuccessWithMessage("Record: %s success deleted!\n", inputedRecordName);

	free(inputedRecordName);
	showToPressEnter();
}