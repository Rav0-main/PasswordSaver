#include "registerscreen.h"
#include "accountdata.h"
#include "multiinputfield.h"
#include "screencodes.h"
#include "../Log2Database/main.h"
#include "../SHA256Generator/main.h"
#include "generalscreenfuncts.h"
#include "fillstrbuffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
* Returns Normal AccountData if user inputed all and first and second passwords are equal
* Returns AccountData {.login[0] = -1, .password[0] = -1} if user pressed left arrow button
* Returns AccountData {.login = inputedLogin, .password[0] = -2} if first and second passwords not equal
*/
AccountData showRegisterScreen(void);

void runRegisterScreen(int* currentScreen, const Database* userDatabase) {
	AccountData newUserData = showRegisterScreen();

	//user not inputed login
	if (newUserData.login[0] == '\0') {
		clearScreen();
		showRedErrorWithMessage("Need input login!\n");
		showToPressEnter();

		return;
	}
	//user not inputed password
	else if (newUserData.password[0] == '\0') {
		clearScreen();
		showRedErrorWithMessage("Need input password!\n");
		showToPressEnter();

		return;
	}
	//user pressed left arrow button
	else if (newUserData.login[0] == -1) 
		*currentScreen = INPUT_SCREEN;

	//first password equals second password
	else if (newUserData.password[0] != -2) {
		toLowerCase(newUserData.login);

		if (databaseExistsKey(userDatabase, newUserData.login)) {
			clearScreen();
			showRedErrorWithMessage("Login: %s exist in database!\n", newUserData.login);
			showToPressEnter();
		}
		else {
			const char* hashOfPassword = getSHA256(newUserData.password);
			databaseAppendByKey(userDatabase, newUserData.login, hashOfPassword);
			
			*currentScreen = SIGN_IN_SCREEN;
			
			free(hashOfPassword);
			
			clearScreen();
			showGreenSuccessWithMessage("New account success created!\n");
			showToPressEnter();
		}
	}
	else {
		clearScreen();
		showRedErrorWithMessage("Password in first field not equals password in second!\n");
		showToPressEnter();
	}
}

AccountData showRegisterScreen(void) {
	InputField loginField = {
		.outputLine = "Login: ",
		.isSecurity = false
	};

	InputField passwordField = {
		.outputLine = "Password: ",
		.isSecurity = true
	};

	InputField secondPasswordField = {
		.outputLine = "Repeat password: ",
		.isSecurity = true
	};

	InputField* fields[] = {
		&loginField, &passwordField, &secondPasswordField
	};

	printf("Input new login and password\n");
	displayMultiInputFields(fields,3, 1);
	
	if (loginField.inputedValue[0] == -1) {
		AccountData account = {
			.login[0] = -1,
			.password[0] = -1
		};
		return account;
	}

	else if (strcmp(passwordField.inputedValue, secondPasswordField.inputedValue) != 0) {
		AccountData account = {
			.password[0] = -2
		};

		strncpy(account.login, loginField.inputedValue, strlen(loginField.inputedValue));

		return account;
	}

	AccountData account;

	fillString(account.login, LOGIN_MAX_LENGTH, '\0');
	fillString(account.password, PASSWORD_MAX_LENGTH, '\0');

	strncpy(account.login, loginField.inputedValue, strlen(loginField.inputedValue)+1);
	strncpy(account.password, passwordField.inputedValue, strlen(passwordField.inputedValue)+1);

	return account;
}
