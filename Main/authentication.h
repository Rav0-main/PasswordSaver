#pragma once

#include "../Log2Database/main.h"
#include "accountdata.h"

#define HASH_OF_PASSWORD_LENGTH 64
#define HASH_OF_LOGIN_LENGTH 64
#define DATABASE_EXTENSION ".logdb"

typedef enum {
	AccountDataOk,
	PasswordIsNotCorrect,
	LoginDoesNotExists
} AuthenticationStatus;

AuthenticationStatus checkAccountDataIsCorrect(
	const Database* const restrict userDatabase, char* const restrict login,
	const char* const restrict password
);

/*
* Returns the account database
*/
Database signInAccount(const AccountData* const restrict account);

/*
* Resets account and closes the accountDatabase 
*/
void exitFromAccount(
	AccountData* const restrict account, 
	Database* const restrict accountDatabase
);

/*
* Sets \0 in account->login and account->password
*/
void resetAccount(AccountData* const restrict account);

/*
* Delete the file of account user
*/
void deleteAccountDatabaseFile(AccountData* const restrict account);