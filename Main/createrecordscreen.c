#include "createrecordscreen.h"
#include "accountdata.h"
#include "multiinputfield.h"
#include "fillstrbuffer.h"
#include "generalscreenfuncts.h"
#include "../Log2Database/main.h"
#include "screencodes.h"
#include "../SimpleEncryptionOfString/main.h"
#include "recorddatamanipulations.h"
#include <stdio.h>
#include <string.h>

RecordData showCreateRecordScreen(void);

void runCreateRecordScreen(const Database* accountDatabase, int* currentScreen, const char* password) {
	RecordData recordData = showCreateRecordScreen();

	if (recordData.recordName[0] == -1) {
		*currentScreen = MAIN_MENU_SCREEN;
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
		decrypt(recordData.recordName, password);
		
		clearScreen();
		showRedErrorWithMessage("Record with name: %s exists in database!\n", recordData.recordName);
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
		.outputLine = "Record name: ",
		.isSecurity = false
	};

	InputField loginField = {
		.outputLine = "Login: ",
		.isSecurity = false,
	};

	InputField passwordField = {
		.outputLine = "Password: ",
		.isSecurity = true
	};

	InputField descriptionField = {
		.outputLine = "Description: ",
		.isSecurity = false,
	};

	InputField* fields[] = {
		&recordNameField, &loginField, &passwordField, &descriptionField
	};

	printf("Input data:\n");
	displayMultiInputFields(fields, 4, 1);

	if (recordNameField.inputedValue[0] == -1) {
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

	strncpy(recordData.recordName, recordNameField.inputedValue, strlen(recordNameField.inputedValue) + 1);
	strncpy(recordData.login, loginField.inputedValue, strlen(loginField.inputedValue) + 1);
	strncpy(recordData.password, passwordField.inputedValue, strlen(passwordField.inputedValue) + 1);
	strncpy(recordData.description, descriptionField.inputedValue, strlen(descriptionField.inputedValue) + 1);

	return recordData;
}