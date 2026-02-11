#include "registerscreen.h"
#include "accountdata.h"
#include "multiinputfield.h"
#include "sha256generator.h"
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
static AccountData showRegisterScreen(void);

void runRegisterScreen(
	Screen* const restrict currentScreen, Database* const restrict userDatabase
) {
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
		*currentScreen = InputScreen;

	//first password equals second password
	else if (newUserData.password[0] != -2) {
		toLowerCase(newUserData.login);

		if (databaseExistsKey(userDatabase, newUserData.login)) {
			clearScreen();
			showRedErrorWithMessage("Login: %s exist in database!\n", newUserData.login);
			showToPressEnter();
		}
		else {
			const char* const restrict hashOfPassword = getSHA256(newUserData.password);
			databaseAppendByKey(userDatabase, newUserData.login, hashOfPassword);
			
			*currentScreen = SignInScreen;
			
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

static AccountData showRegisterScreen(void) {
	InputField loginField = {
		.prompt = "Login: ",
		.isSecurity = false
	};

	InputField passwordField = {
		.prompt = "Password: ",
		.isSecurity = true
	};

	InputField secondPasswordField = {
		.prompt = "Repeat password: ",
		.isSecurity = true
	};

	InputField* fields[] = {
		&loginField, &passwordField, &secondPasswordField
	};

	printf("Input new login and password\n");
	displayMultiInputFields(fields,3, 1);
	
	if (loginField.value[0] == -1) {
		AccountData account = {
			.login[0] = -1,
			.password[0] = -1
		};
		return account;
	}

	else if (strcmp(passwordField.value, secondPasswordField.value) != 0) {
		AccountData account = {
			.password[0] = -2
		};

		strncpy(account.login, loginField.value, strlen(loginField.value));

		return account;
	}

	AccountData account;

	fillString(account.login, LOGIN_MAX_LENGTH, '\0');
	fillString(account.password, PASSWORD_MAX_LENGTH, '\0');

	strncpy(account.login, loginField.value, strlen(loginField.value)+1);
	strncpy(account.password, passwordField.value, strlen(passwordField.value)+1);

	return account;
}
