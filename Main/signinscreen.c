#include "signinscreen.h"
#include "multiinputfield.h"
#include "accountdata.h"
#include "screencodes.h"
#include "authentication.h"
#include "generalscreenfuncts.h"
#include "../Log2Database/main.h"
#include "fillstrbuffer.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/*
* Returns Normal AccountData if login and password inputed
* Returns AccountData {.login[0] = -1, .password[0] = -1} if user pressed left arrow button
*/
AccountData showSignInScreen(void);

bool runSignInScreen(const Database* userDatabase, int* currentScreen, AccountData* account) {
	const int OK_SIGNED_CODE = 0;
	const int PASSWORD_IS_NOT_CORRECT_CODE = 1;
	const int LOGIN_DOES_NOT_EXIST_CODE = 2;

	AccountData userData = showSignInScreen();

	//user not input login
	if (userData.login[0] == '\0') {
		clearScreen();
		showRedErrorWithMessage("Need input login!\n");
		showToPressEnter();

		return false;
	}
	//user not input password
	else if (userData.password[0] == '\0') {
		clearScreen();
		showRedErrorWithMessage("Need input password!\n");
		showToPressEnter();

		return false;
	}
	//user pressed left arrow button
	else if (userData.login[0] == -1) {
		*currentScreen = INPUT_SCREEN;

		return false;
	}

	int authenticationResult = checkAccountDataIsCorrect(userDatabase, userData.login, userData.password);
	
	clearScreen();
	if(authenticationResult == OK_SIGNED_CODE){
		*currentScreen = MAIN_MENU_SCREEN;

		strncpy(account->login, userData.login, strlen(userData.login)+1);
		strncpy(account->password, userData.password, strlen(userData.password)+1);
		
		return true;
	}
	else if(authenticationResult == PASSWORD_IS_NOT_CORRECT_CODE){
		showRedErrorWithMessage("Password is not correct!\n");
		showToPressEnter();
		return false;
	}
	else if(authenticationResult == LOGIN_DOES_NOT_EXIST_CODE){
		showRedErrorWithMessage("Login: %s does not exist!\n", userData.login);
		showToPressEnter();
		return false;
	}
}

AccountData showSignInScreen(void) {
	InputField loginField = {
		.outputLine = "Login: ",
		.isSecurity = false
	};

	InputField passwordField = {
		.outputLine = "Password: ",
		.isSecurity = true
	};

	InputField* fields[] = { &loginField, &passwordField };

	printf("Input login and password of account\n");
	displayMultiInputFields(fields, 2, 1);

	//user pressed left arrow button
	if (loginField.inputedValue[0] == -1) {
		AccountData account = {
			.login[0] = -1,
			.password[0] = -1
		};

		return account;
	}

	AccountData account;

	fillString(account.login, LOGIN_MAX_LENGTH, '\0');
	fillString(account.password, PASSWORD_MAX_LENGTH, '\0');

	strncpy(account.login, loginField.inputedValue, strlen(loginField.inputedValue)+1);
	strncpy(account.password, passwordField.inputedValue, strlen(passwordField.inputedValue)+1);

	return account;
}