#include "findrecordscreen.h"
#include "autocompletioninputfield.h"
#include "fillstrbuffer.h"
#include "../Log2Database/main.h"
#include "accountdata.h"
#include "generalscreenfuncts.h"
#include "screencodes.h"
#include "recorddatamanipulations.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

void runFindRecordScreen(const Database* accountDatabase,  int* currentScreen, const char* password) {
	AutoCompletionInputField inputField = {
		.bufferLength = RECORD_NAME_MAX_LENGTH,
		.outputLine = "Input record name: ",
		.startX = 0,
		.startY = 0,
	};

	const char* inputedRecordName = displayAutoCompletionInputField(inputField, accountDatabase);

	//user pressed left arrow button
	if (inputedRecordName[0] == -1) {
		free(inputedRecordName);
		*currentScreen = MAIN_MENU_SCREEN;
		return;
	}

	if (!databaseExistsKey(accountDatabase, inputedRecordName)) {
		clearScreen();
		showRedErrorWithMessage("Record with name: %s not found!\n", inputedRecordName);
		free(inputedRecordName);
		showToPressEnter();
		return;
	}

	RecordData recordData;
	databaseGetValueByKey(accountDatabase, inputedRecordName, &recordData);
	free(inputedRecordName);

	decryptRecord(&recordData, password);

	*currentScreen = MAIN_MENU_SCREEN;

	clearScreen();
	showGreenSuccessWithMessage("Found record\n------------------------------------------------------------\n");
	outputRecord(&recordData);
	printf("\n");
	showToPressEnter();

	return;
}