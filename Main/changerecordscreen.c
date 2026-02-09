#include "changerecordscreen.h"
#include "autocompletioninputfield.h"
#include "multiinputfield.h"
#include "accountdata.h"
#include "generalscreenfuncts.h"
#include "recorddatamanipulations.h"

#include <stdlib.h>
#include <string.h>

void runChangeRecordScreen(
	Database* const restrict accountDatabase, Screen* const restrict currentScreen, 
	const char* const restrict password
) {
	AutoCompletionInputField inputField = {
		.prompt = "Input record name: ",
		.bufferLength = RECORD_NAME_MAX_LENGTH,
		.startX = 0,
		.startY = 0
	};

	const char* const restrict inputedRecordName = displayAutoCompletionInputField(
		inputField, accountDatabase
	);

	if (inputedRecordName[0] == -1) {
		free(inputedRecordName);
		*currentScreen = MainMenuScreen;
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
		.prompt = "Login: ",
		.isSecurity = false
	};

	InputField passwordField = {
		.prompt = "Password: ",
		.isSecurity = true
	};

	InputField descriptionField = {
		.prompt = "Description: ",
		.isSecurity = false
	};

	InputField* fields[] = {
		&loginField, &passwordField, &descriptionField
	};

	displayMultiInputFields(fields, 3, 1);

	if (loginField.value[0] == -1) {
		free(inputedRecordName);

		*currentScreen = MainMenuScreen;
		return;
	}
	else if (loginField.value[0] == '\0') {
		free(inputedRecordName);

		clearScreen();
		showRedErrorWithMessage("Need input login!\n");
		showToPressEnter();

		return;
	}
	else if (passwordField.value[0] == '\0') {
		free(inputedRecordName);

		clearScreen();
		showRedErrorWithMessage("Need input password!\n");
		showToPressEnter();

		return;
	}
	else if (descriptionField.value[0] == '\0') {
		descriptionField.value[0] = 'n';
		descriptionField.value[1] = 'u';
		descriptionField.value[2] = 'l';
		descriptionField.value[3] = 'l';
		descriptionField.value[4] = '\0';
	}

	RecordData newRecordData;

	strncpy(newRecordData.recordName, inputedRecordName, strlen(inputedRecordName) + 1);
	strncpy(newRecordData.login, loginField.value, strlen(loginField.value) + 1);
	strncpy(newRecordData.password, passwordField.value, strlen(passwordField.value) + 1);
	strncpy(newRecordData.description, descriptionField.value, strlen(descriptionField.value) + 1);

	encryptRecord(&newRecordData, password);

	databaseChangeValueByKey(accountDatabase, inputedRecordName, &newRecordData);

	*currentScreen = MainMenuScreen;

	clearScreen();
	showGreenSuccessWithMessage("Record: %s success changed!\n", inputedRecordName);

	free(inputedRecordName);
	
	showToPressEnter(); 
}