#include "deleterecordscreen.h"
#include "autocompletioninputfield.h"
#include "accountdata.h"
#include "generalscreenfuncts.h"

#include <stdlib.h>

void runDeleteRecordScreen(
	Database* const restrict accountDatabase, Screen* const restrict currentScreen
) {
	AutoCompletionInputField inputField = {
		.prompt = "Input record name: ",
		.bufferLength = RECORD_NAME_MAX_LENGTH,
		.startX = 0,
		.startY = 0
	};

	const char* const restrict inputedRecordName = displayAutoCompletionInputField(inputField, accountDatabase);

	//user pressed left arrow button
	if (inputedRecordName[0] == -1) {
		free(inputedRecordName);

		*currentScreen = MainMenuScreen;
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
	
	*currentScreen = MainMenuScreen;
	
	clearScreen();
	showGreenSuccessWithMessage("Record: %s success deleted!\n", inputedRecordName);

	free(inputedRecordName);
	showToPressEnter();
}