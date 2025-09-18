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
#include <conio.h>
#include <ctype.h>

#define SYMBOL_TO_COPY_LOGIN 113 //q
#define SYMBOL_TO_COPY_PASSWORD 101 //e
#define WINDOWS_COPY_COMMAND_PART "echo| clip"

static void copyStrToWindowsBuffer(const char* const restrict buffer, const unsigned int maxLengthOfBuffer);

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
	printf("Press <Enter> to continue\n");

	int inputedChar;
	while ((inputedChar = tolower(_getch())) != '\r') {
		if (!_kbhit())
			;
		switch (inputedChar) {
		case SYMBOL_TO_COPY_LOGIN:
			copyStrToWindowsBuffer(recordData.login, LOGIN_MAX_LENGTH);
			break;
		case SYMBOL_TO_COPY_PASSWORD:
			copyStrToWindowsBuffer(recordData.password, PASSWORD_MAX_LENGTH);
			break;
		default:
			break;
		}
	}

	return;
}

static void copyStrToWindowsBuffer(const char* const restrict buffer, const unsigned int maxLengthOfBuffer) {
	char* command = (char*)calloc(maxLengthOfBuffer + strlen(WINDOWS_COPY_COMMAND_PART), sizeof(char));
	char* const commandStart = command;
	if (command == NULL)
		return;

	memset(commandStart, 1, maxLengthOfBuffer + strlen(WINDOWS_COPY_COMMAND_PART));
	strncpy(commandStart, "echo ", strlen("echo "));

	command = command + strlen("echo ");
	strncpy(command, buffer, strlen(buffer));
	command = command + strlen(buffer);

	strncpy(command, "| clip", strlen("| clip"));
	command = command + strlen("| clip");
	*command = '\0';

	system(commandStart);

	free(commandStart);
}