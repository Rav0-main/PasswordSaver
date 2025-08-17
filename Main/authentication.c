#include "authentication.h"
#include "../Log2Database/main.h"
#include "../SHA256Generator/main.h"
#include "accountdata.h"
#include "fillstrbuffer.h"
#include <string.h>
#include <stdlib.h>

/*
* created by 'calloc'
*/
char* _getDatabaseFileNameOf(AccountData* account);

int checkAccountDataIsCorrect(const Database* userDatabase, char* login, char* password) {
    char* hashOfPasswordFromDatabase;
    char* hashOfInputedPassword;

    toLowerCase(login);

    if (databaseExistsKey(userDatabase, login)) {
        hashOfPasswordFromDatabase = (char*)calloc(HASH_OF_PASSWORD_LENGTH + 1, sizeof(char));

        databaseGetValueByKey(userDatabase, login, hashOfPasswordFromDatabase);
        hashOfInputedPassword = getSHA256(password);

        if (strcmp(hashOfInputedPassword, hashOfPasswordFromDatabase) != 0) {
            free(hashOfInputedPassword);
            free(hashOfPasswordFromDatabase);
            return 1;
        }
        else {
            free(hashOfInputedPassword);
            free(hashOfPasswordFromDatabase);
            return 0;
        }

    }
    else
        return 2;
}

Database signInAccount(AccountData* account) {
    const char* filePath = _getDatabaseFileNameOf(account);
    Database accountDatabase = databaseOpen(filePath, RECORD_NAME_MAX_LENGTH + 1, sizeof(RecordData));

    free(filePath);
    return accountDatabase;
}

void exitFromAccount(AccountData* account, Database* accountDatabase) {
    resetAccount(account);
    databaseClose(accountDatabase);
}

void resetAccount(AccountData* account) {
    fillString(account->login, LOGIN_MAX_LENGTH, '\0');
    fillString(account->password, PASSWORD_MAX_LENGTH, '\0');
}

void deleteAccountDatabaseFile(AccountData* account) {
    const char* databaseFileName = _getDatabaseFileNameOf(account);
    remove(databaseFileName);
    free(databaseFileName);
}

char* _getDatabaseFileNameOf(AccountData* account) {
    const unsigned int databaseExtensionLength = strlen(DATABASE_EXTENSION);
    const char* filePath = calloc(HASH_OF_LOGIN_LENGTH + databaseExtensionLength + 1, sizeof(char));
    const char* hashOfLogin = getSHA256(account->login);

    strncpy(filePath, hashOfLogin, HASH_OF_LOGIN_LENGTH);
    strncpy(filePath + HASH_OF_LOGIN_LENGTH, DATABASE_EXTENSION, databaseExtensionLength + 1);

    free(hashOfLogin);
    return filePath;
}