#pragma once

#include "../Log2Database/main.h"
#include "accountdata.h"

#define HASH_OF_PASSWORD_LENGTH 64
#define HASH_OF_LOGIN_LENGTH 64
#define DATABASE_EXTENSION ".logdb"

/*
* Returns 0 if all correct
* Returns 1 if password not correct
* Returns 2 if login noit found in database
*/
int checkAccountDataIsCorrect(const Database* userDatabase, char* login, char* password);

/*
* Returns the account database
*/
Database signInAccount(AccountData* account);

/*
* Resets account and closes the accountDatabase 
*/
void exitFromAccount(AccountData* account, Database* accountDatabase);

/*
* Sets \0 in account->login and account->password
*/
void resetAccount(AccountData* account);

/*
* Delete the file of account user
*/
void deleteAccountDatabaseFile(AccountData* account);