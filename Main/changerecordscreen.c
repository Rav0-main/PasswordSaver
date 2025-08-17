#include "changerecordscreen.h"
#include "autocompletioninputfield.h"
#include "multiinputfield.h"
#include "accountdata.h"
#include "generalscreenfuncts.h"
#include "recorddatamanipulations.h"
#include "screencodes.h"
#include <stdlib.h>
#include <string.h>

void runChangeRecordScreen(const Database* accountDatabase, int* currentScreen, const char* password) {
	AutoCompletionInputField inputField = {
		.outputLine = "Input record name: ",
		.bufferLength = RECORD_NAME_MAX_LENGTH,
		.startX = 0,
		.startY = 0
	};

	const char* inputedRecordName = displayAutoCompletionInputField(inputField, accountDatabase);

	if (inputedRecordName[0] == -1) {
		free(inputedRecordName);
		*currentScreen = MAIN_MENU_SCREEN;
		return;
	}

	else if (!databaseExistsKey(accountDatabase, inputedRecordName)) {
		clearScreen();
		showRedErrorWithMessage("Record: %s not found!\n", inputedRecordName);
		free(inputedRecordName);
		showToPressEnter();

		return;
	}

	clearScreen();

	printf("Input data:\n");

	InputField loginField = {
		.outputLine = "Login: ",
		.isSecurity = false
	};

	InputField passwordField = {
		.outputLine = "Password: ",
		.isSecurity = true
	};

	InputField descriptionField = {
		.outputLine = "Description: ",
		.isSecurity = false
	};

	InputField* fields[] = {
		&loginField, &passwordField, &descriptionField
	};

	displayMultiInputFields(fields, 3, 1);

	if (loginField.inputedValue[0] == -1) {
		free(inputedRecordName);

		*currentScreen = MAIN_MENU_SCREEN;
		return;
	}
	else if (loginField.inputedValue[0] == '\0') {
		free(inputedRecordName);

		clearScreen();
		showRedErrorWithMessage("Need input login!\n");
		showToPressEnter();

		return;
	}
	else if (passwordField.inputedValue[0] == '\0') {
		free(inputedRecordName);

		clearScreen();
		showRedErrorWithMessage("Need input password!\n");
		showToPressEnter();

		return;
	}
	else if (descriptionField.inputedValue[0] == '\0') {
		descriptionField.inputedValue[0] = 'n';
		descriptionField.inputedValue[1] = 'u';
		descriptionField.inputedValue[2] = 'l';
		descriptionField.inputedValue[3] = 'l';
		descriptionField.inputedValue[4] = '\0';
	}

	RecordData newRecordData;

	strncpy(newRecordData.recordName, inputedRecordName, strlen(inputedRecordName) + 1);
	strncpy(newRecordData.login, loginField.inputedValue, strlen(loginField.inputedValue) + 1);
	strncpy(newRecordData.password, passwordField.inputedValue, strlen(passwordField.inputedValue) + 1);
	strncpy(newRecordData.description, descriptionField.inputedValue, strlen(descriptionField.inputedValue) + 1);

	encryptRecord(&newRecordData, password);

	databaseChangeValueByKey(accountDatabase, inputedRecordName, &newRecordData);

	*currentScreen = MAIN_MENU_SCREEN;

	clearScreen();
	showGreenSuccessWithMessage("Record: %s success changed!\n", inputedRecordName);

	free(inputedRecordName);
	
	showToPressEnter(); 
}