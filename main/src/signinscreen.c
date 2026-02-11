#include "signinscreen.h"
#include "multiinputfield.h"
#include "authentication.h"
#include "generalscreenfuncts.h"
#include "fillstrbuffer.h"

#include <stdio.h>
#include <string.h>

/*
* Returns Normal AccountData if login and password inputed
* Returns AccountData {.login[0] = -1, .password[0] = -1} if user pressed left arrow button
*/
static AccountData showSignInScreen(void);

bool runSignInScreen(
	const Database* const restrict users, Screen* const restrict currentScreen,
	AccountData* const restrict account
) {
	AccountData userData = showSignInScreen();

	//user not inputed login
	if (userData.login[0] == '\0') {
		clearScreen();
		showRedErrorWithMessage("Need input login!\n");
		showToPressEnter();

		return false;
	}
	//user not inputed password
	else if (userData.password[0] == '\0') {
		clearScreen();
		showRedErrorWithMessage("Need input password!\n");
		showToPressEnter();

		return false;
	}
	//user pressed left arrow button
	else if (userData.login[0] == -1) {
		*currentScreen = InputScreen;

		return false;
	}

	AuthenticationStatus status = checkAccountDataIsCorrect(
		users, userData.login, userData.password
	);
	
	clearScreen();

	switch (status) {
	case AccountDataOk:
		*currentScreen = MainMenuScreen;

		strncpy(account->login, userData.login, strlen(userData.login) + 1);
		strncpy(account->password, userData.password, strlen(userData.password) + 1);

		return true;
			
	case PasswordIsNotCorrect:
		showRedErrorWithMessage("Password is not correct!\n");
		showToPressEnter();

		return false;

	case LoginDoesNotExists:
		showRedErrorWithMessage("Login: %s does not exist!\n", userData.login);
		showToPressEnter();

		return false;
	}
}

static AccountData showSignInScreen(void) {
	InputField loginField = {
		.prompt = "Login: ",
		.isSecurity = false
	};

	InputField passwordField = {
		.prompt = "Password: ",
		.isSecurity = true
	};

	InputField* fields[] = { &loginField, &passwordField };

	printf("Input login and password of account\n");
	displayMultiInputFields(fields, 2, 1);

	//user pressed left arrow button
	if (loginField.value[0] == -1) {
		AccountData account = {
			.login[0] = -1,
			.password[0] = -1
		};

		return account;
	}

	AccountData account;

	fillString(account.login, LOGIN_MAX_LENGTH, '\0');
	fillString(account.password, PASSWORD_MAX_LENGTH, '\0');

	strncpy(account.login, loginField.value, strlen(loginField.value)+1);
	strncpy(account.password, passwordField.value, strlen(passwordField.value)+1);

	return account;
}