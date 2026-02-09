#include "findrecordscreen.h"
#include "autocompletioninputfield.h"
#include "fillstrbuffer.h"
#include "accountdata.h"
#include "generalscreenfuncts.h"
#include "recorddatamanipulations.h"
#include "multifieldinginteractive.h"

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

#define SYMBOL_TO_COPY_LOGIN 113 //q
#define SYMBOL_TO_COPY_PASSWORD 101 //e
#define WINDOWS_COPY_COMMAND_PART "echo| clip"

static void copyStrToWindowsBuffer(
	const char* const restrict buffer, const unsigned int maxLength
);

void runFindRecordScreen(
	const Database* const restrict accountDatabase,  Screen* const restrict currentScreen,
	const char* const restrict password
) {
	AutoCompletionInputField inputField = {
		.bufferLength = RECORD_NAME_MAX_LENGTH,
		.prompt = "Input record name: ",
		.startX = 0,
		.startY = 0,
	};

	const char* const restrict inputedRecordName = displayAutoCompletionInputField(inputField, accountDatabase);

	//user pressed left arrow button
	if (inputedRecordName[0] == -1) {
		free(inputedRecordName);

		*currentScreen = MainMenuScreen;
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

	*currentScreen = MainMenuScreen;

	clearScreen();
	
	showGreenSuccessWithMessage("Found record\n------------------------------------------------------------\n");
	outputRecord(&recordData);
	putchar('\n');
	printf("Press <Enter> to continue\n");

	char symbol;
	while (true) {
		if (_kbhit()) {
			symbol = tolower(_getch());

			if (isEnterButton(symbol))
				break;

			switch (symbol) {
			case SYMBOL_TO_COPY_LOGIN:
				copyStrToWindowsBuffer(recordData.login, LOGIN_MAX_LENGTH);
				break;

			case SYMBOL_TO_COPY_PASSWORD:
				copyStrToWindowsBuffer(recordData.password, PASSWORD_MAX_LENGTH);
				break;
			}
		}
		Sleep(SLEEP_TIME_MS);
	}

	return;
}

static void copyStrToWindowsBuffer(
	const char* const restrict buffer, const unsigned int maxLength
) {
	char* command = (char*)calloc(maxLength + strlen(WINDOWS_COPY_COMMAND_PART), sizeof(char));
	char* const commandBegin = command;

	if (command == NULL)
		return;

	strncpy(command, "echo ", strlen("echo "));

	command = command + strlen("echo ");
	strncpy(command, buffer, strlen(buffer));

	command = command + strlen(buffer);

	strncpy(command, "| clip", strlen("| clip"));
	
	command = command + strlen("| clip");
	*command = '\0';

	system(commandBegin);

	free(commandBegin);
}