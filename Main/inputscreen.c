#include "inputscreen.h"
#include "multichoicefield.h"
#include "screencodes.h"
#include <stdio.h>

/*
* Returns 1 if choosed sign in
* Returns 2 if choosed register
* Returns 3 if choosed help
*/
int showInputScreen(void);

void runInputScreen(int* currentScreen) {
	const int EXIT_SCREEN_CODE = -1;
	const int SIGN_IN_SCREEN_CODE = 1;
	const int REGISTER_SCREEN_CODE = 2;
	const int HELP_SCREEN_CODE = 3;

	int choice = showInputScreen();
	if(choice == EXIT_SCREEN_CODE)
		*currentScreen = EXIT_SCREEN;
	
	else if(choice == SIGN_IN_SCREEN_CODE)
		*currentScreen = SIGN_IN_SCREEN;
	
	else if(choice == REGISTER_SCREEN_CODE)
		*currentScreen = REGISTER_SCREEN;

	else if (choice == HELP_SCREEN_CODE) {
		*currentScreen = HELP_SCREEN;
	}
}

int showInputScreen(void) {
	ChoiceField signInScreen = {
		.outputLine = "Sign In",
		.code = 1
	};

	ChoiceField registerScreen = {
		.outputLine = "Register",
		.code = 2
	};

	ChoiceField helpScreen = {
		.outputLine = "Help",
		.code = 3
	};

	ChoiceField* fields[] = {
		&signInScreen, &registerScreen, &helpScreen
	};

	printf("Choose next screen: \n");
	return displayMultiChoiceFields(fields, 3, 1);
}