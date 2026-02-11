#include "createrecordscreen.h"
#include "accountdata.h"
#include "multiinputfield.h"
#include "fillstrbuffer.h"
#include "generalscreenfuncts.h"
#include "recorddatamanipulations.h"

#include <stdio.h>
#include <string.h>

RecordData showCreateRecordScreen(void);

void runCreateRecordScreen(
	Database* const restrict accountDatabase, Screen* const restrict currentScreen,
	const char* const restrict password
) {
	RecordData recordData = showCreateRecordScreen();

	if (recordData.recordName[0] == -1) {
		*currentScreen = MainMenuScreen;
		return;
	}

	else if (recordData.recordName[0] == '\0') {
		clearScreen();
		showRedErrorWithMessage("Need input record name!\n");
		showToPressEnter();

		return;
	}

	else if (recordData.login[0] == '\0') {
		clearScreen();
		showRedErrorWithMessage("Need input login!\n");
		showToPressEnter();

		return;
	}

	else if (recordData.password[0] == '\0') {
		clearScreen();
		showRedErrorWithMessage("Need input password!\n");
		showToPressEnter();

		return;
	}

	else if (recordData.description[0] == '\0') {
		recordData.description[0] = 'n';
		recordData.description[1] = 'u';
		recordData.description[2] = 'l';
		recordData.description[3] = 'l';
		recordData.description[4] = '\0';
	}

	char notEncryptedRecordName[RECORD_NAME_MAX_LENGTH + 1];
	fillString(notEncryptedRecordName, RECORD_NAME_MAX_LENGTH, '\0');

	strncpy(notEncryptedRecordName, recordData.recordName, strlen(recordData.recordName)+1);
	notEncryptedRecordName[strlen(recordData.recordName)] = '\0';

	encryptRecord(&recordData, password);

	if (databaseExistsKey(accountDatabase, notEncryptedRecordName)) {
		clearScreen();

		showRedErrorWithMessage("Record with name: %s exists in database!\n", notEncryptedRecordName);
		showToPressEnter();
		return;
	}

	databaseAppendByKey(accountDatabase, notEncryptedRecordName, &recordData);
	
	clearScreen();
	showGreenSuccessWithMessage("Record success appended!\n");
	showToPressEnter();
}

RecordData showCreateRecordScreen(void) {
	InputField recordNameField = {
		.prompt = "Record name: ",
		.isSecurity = false
	};

	InputField loginField = {
		.prompt = "Login: ",
		.isSecurity = false,
	};

	InputField passwordField = {
		.prompt = "Password: ",
		.isSecurity = true
	};

	InputField descriptionField = {
		.prompt = "Description: ",
		.isSecurity = false,
	};

	InputField* fields[] = {
		&recordNameField, &loginField, &passwordField, &descriptionField
	};

	printf("Input data:\n");
	displayMultiInputFields(fields, 4, 1);

	if (recordNameField.value[0] == -1) {
		RecordData recordData = {
			.recordName[0] = -1,
			.login[0] = -1,
			.password[0] = -1,
			.description[0] = -1
		};

		return recordData;
	}

	RecordData recordData;

	fillString(recordData.recordName, RECORD_NAME_MAX_LENGTH, '\0');
	fillString(recordData.login, LOGIN_MAX_LENGTH, '\0');
	fillString(recordData.password, PASSWORD_MAX_LENGTH, '\0');
	fillString(recordData.description, DESCRIPTION_MAX_LENGTH, '\0');

	strncpy(recordData.recordName, recordNameField.value, strlen(recordNameField.value) + 1);
	strncpy(recordData.login, loginField.value, strlen(loginField.value) + 1);
	strncpy(recordData.password, passwordField.value, strlen(passwordField.value) + 1);
	strncpy(recordData.description, descriptionField.value, strlen(descriptionField.value) + 1);

	return recordData;
}