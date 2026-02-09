#include "deleteaccountscreen.h"
#include "multichoicefield.h"
#include "authentication.h"
#include "generalscreenfuncts.h"

#include <stdio.h>

static enum {
	Back = -1,
	DeleteAccount,
	SaveAccount
};

static ChoiceCode showToConfirmADelete(void);

inline static void deleteAccount(
	Database* const restrict accountDatabase,
	AccountData* const restrict account
);

void runDeleteAccountScreen(
	Database* const restrict accountDatabase, AccountData* const restrict account,
	Database* const restrict userDatabase, Screen* const restrict currentScreen
) {
	ChoiceCode choice = showToConfirmADelete();
	
	switch (choice) {
	case DeleteAccount:
		databaseDeleteValueByKey(userDatabase, account->login);

		deleteAccount(accountDatabase, account);

		*currentScreen = InputScreen;

		clearScreen();
		showGreenSuccessWithMessage("Success deleted!\n");
		showToPressEnter();
		
		break;

	case SaveAccount:
	case Back:
		*currentScreen = MainMenuScreen;
		break;
	}
}

static ChoiceCode showToConfirmADelete(void) {
	ChoiceField deleteField = {
		.prompt = "Delete",
		.code = DeleteAccount
	};

	ChoiceField cancelField = {
		.prompt = "Cancel",
		.code = SaveAccount
	};

	ChoiceField* fields[] = {
		&deleteField, &cancelField
	};

	printf("Confirm delete account\n");

	return displayMultiChoiceFields(fields, 2, 1);
}

static void deleteAccount(
	Database* const restrict accountDatabase, AccountData* const restrict account
) {
	databaseClose(accountDatabase);
	deleteAccountDatabaseFile(account);
	exitFromAccount(account, accountDatabase);
}