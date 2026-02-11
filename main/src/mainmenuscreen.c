#include "mainmenuscreen.h"
#include "multichoicefield.h"

#include <stdio.h>

ChoiceCode showMainMenuScreen(const char* const restrict login);

void runMainMenuScreen(
	Screen* const restrict currentScreen, const char* const restrict login
) {
	ChoiceCode choice = showMainMenuScreen(login);
	
	//back to previous screen
	if (choice == PRESSED_LEFT_ARROW)
		*currentScreen = SignInScreen;
	else
		*currentScreen = (Screen) choice;
}

ChoiceCode showMainMenuScreen(const char* const restrict login) {
	ChoiceField createRecordField = {
		.prompt = "Create record",
		.code = CreateRecordScreen
	};

	ChoiceField findRecordField = {
		.prompt = "Find record",
		.code = FindRecordScreen
	};

	ChoiceField outputAllRecordsField = {
		.prompt = "Output all records",
		.code = OutputAllRecordsScreen
	};

	ChoiceField changeRecordField = {
		.prompt = "Change record",
		.code = ChangeRecordScreen
	};

	ChoiceField deleteRecordField = {
		.prompt = "Delete record",
		.code = DeleteRecordScreen
	};

	ChoiceField deleteAccountField = {
		.prompt = "Delete account",
		.code = DeleteAccountScreen
	};

	ChoiceField* fields[] = {
		&createRecordField, &findRecordField,
		&outputAllRecordsField, &changeRecordField,
		&deleteRecordField, &deleteAccountField
	};

	printf("Account login: \033[4m%s\033[0m\n", login);
	printf("Choose action\n");

	return displayMultiChoiceFields(fields, 6, 2);
}