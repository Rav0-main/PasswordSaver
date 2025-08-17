#include "deleteaccountscreen.h"
#include "multichoicefield.h"
#include "../Log2Database/main.h"
#include "accountdata.h"
#include "authentication.h"
#include "screencodes.h"
#include "generalscreenfuncts.h"
#include <stdio.h>

int showToConfirmADelete(void);
void _deleteAccount(Database* accountDatabase, AccountData* account);

void runDeleteAccountScreen(Database* accountDatabase, AccountData* account, const Database* userDatabase, int* currentScreen) {
	const int DELETE_ACCOUNT_CODE = 1;
	const int NOT_DELETE_ACCOUNT_CODE = 2;
	const int BACK_CODE = -1;

	int choice = showToConfirmADelete();

	if (choice == DELETE_ACCOUNT_CODE) {
		databaseDeleteValueByKey(userDatabase, account->login);

		_deleteAccount(accountDatabase, account);

		*currentScreen = INPUT_SCREEN;

		clearScreen();
		showGreenSuccessWithMessage("Success deleted!\n");
		showToPressEnter();
	}
	else if (choice == NOT_DELETE_ACCOUNT_CODE || choice == BACK_CODE)
		*currentScreen = MAIN_MENU_SCREEN;
}

int showToConfirmADelete(void) {
	ChoiceField deleteField = {
		.outputLine = "Delete",
		.code = 1
	};

	ChoiceField cancelField = {
		.outputLine = "Cancel",
		.code = 2
	};

	ChoiceField* fields[] = {
		&deleteField, &cancelField
	};

	printf("Confirm delete account\n");

	return displayMultiChoiceFields(fields, 2, 1);
}

void _deleteAccount(Database* accountDatabase, AccountData* account) {
	databaseClose(accountDatabase);
	deleteAccountDatabaseFile(account);
	exitFromAccount(account, accountDatabase);
}