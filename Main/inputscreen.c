#include "inputscreen.h"
#include "multichoicefield.h"
#include "screencodes.h"
#include <stdio.h>

static ChoiceCode showInputScreen(void);

void runInputScreen(Screen* const restrict currentScreen) {
	*currentScreen = (Screen) showInputScreen();
}

static ChoiceCode showInputScreen(void) {
	ChoiceField signInScreen = {
		.prompt = "Sign In",
		.code = SignInScreen
	};

	ChoiceField registerScreen = {
		.prompt = "Register",
		.code = RegisterScreen
	};

	ChoiceField helpScreen = {
		.prompt = "Help",
		.code = HelpScreen
	};

	ChoiceField* fields[] = {
		&signInScreen, &registerScreen, &helpScreen
	};

	printf("Choose next screen: \n");
	return displayMultiChoiceFields(fields, 3, 1);
}