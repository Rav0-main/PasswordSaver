#include "mainmenuscreen.h"
#include "multichoicefield.h"
#include "screencodes.h"
#include <stdio.h>

int showMainMenuScreen(const char* login);

void runMainMenuScreen(int* currentScreen, const char* login) {
	const int SIGN_IN_SCREEN_CODE = -1;
	const int CREATE_RECORD_SCREEN_CODE = 1;
	const int FIND_RECORD_SCREEN_CODE = 2;
	const int OUTPUT_ALL_RECORDS_SCREEN_CODE = 3;
	const int CHANGE_RECORD_SCREEN_CODE = 4;
	const int DELETE_RECORD_SCREEN_CODE = 5;
	const int DELETE_ACCOUNT_SCREEN_CODE = 6;

	int choice = showMainMenuScreen(login);
	if (choice == SIGN_IN_SCREEN_CODE)
		*currentScreen = SIGN_IN_SCREEN;

	else if (choice == CREATE_RECORD_SCREEN_CODE)
		*currentScreen = CREATE_RECORD_SCREEN;

	else if (choice == FIND_RECORD_SCREEN_CODE)
		*currentScreen = FIND_RECORD_SCREEN;

	else if (choice == OUTPUT_ALL_RECORDS_SCREEN_CODE)
		*currentScreen = OUTPUT_ALL_RECORDS_SCREEN;

	else if (choice == CHANGE_RECORD_SCREEN_CODE)
		*currentScreen = CHANGE_RECORD_SCREEN;

	else if (choice == DELETE_RECORD_SCREEN_CODE)
		*currentScreen = DELETE_RECORD_SCREEN;

	else if (choice == DELETE_ACCOUNT_SCREEN_CODE)
		*currentScreen = DELETE_ACCOUNT_SCREEN;
}

int showMainMenuScreen(const char* login) {
	ChoiceField createRecordField = {
		.outputLine = "Create record",
		.code = 1
	};

	ChoiceField findRecordField = {
		.outputLine = "Find record",
		.code = 2
	};

	ChoiceField outputAllRecordsField = {
		.outputLine = "Output all records",
		.code = 3
	};

	ChoiceField changeRecordField = {
		.outputLine = "Change record",
		.code = 4
	};

	ChoiceField deleteRecordField = {
		.outputLine = "Delete record",
		.code = 5
	};

	ChoiceField deleteAccountField = {
		.outputLine = "Delete account",
		.code = 6
	};

	ChoiceField* fields[] = {
		&createRecordField, &findRecordField, &outputAllRecordsField, &changeRecordField,
		&deleteRecordField, &deleteAccountField
	};

	printf("Account login: \033[4m%s\033[0m\n", login);
	printf("Choose action\n");
	return displayMultiChoiceFields(fields, 6, 2);
}