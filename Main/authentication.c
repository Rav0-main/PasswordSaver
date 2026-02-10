#include "authentication.h"
#include "../SHA256Generator/main.h"
#include "accountdata.h"
#include "fillstrbuffer.h"
#include <string.h>
#include <stdlib.h>

/*
* created by 'calloc'
*/
inline static char* getDatabaseFileNameOf(const AccountData* const restrict account);

AuthenticationStatus checkAccountDataIsCorrect(
    const Database* const restrict userDatabase, char* const restrict login,
    const char* const restrict password
) {
    toLowerCase(login);

    if (databaseExistsKey(userDatabase, login)) {
        char* const restrict hashOfPasswordFromDatabase = (char*)calloc(HASH_OF_PASSWORD_LENGTH + 1, sizeof(char));

        databaseGetValueByKey(userDatabase, login, hashOfPasswordFromDatabase);
        const char* const restrict hashOfInputedPassword = getSHA256(password);

        if (strcmp(hashOfInputedPassword, hashOfPasswordFromDatabase) != 0) {
            free(hashOfInputedPassword);
            free(hashOfPasswordFromDatabase);

            return PasswordIsNotCorrect;
        }
        else {
            free(hashOfInputedPassword);
            free(hashOfPasswordFromDatabase);
            
            return AccountDataOk;
        }

    }
    else
        return LoginDoesNotExists;
}

Database signInAccount(const AccountData* const restrict account) {
    const char* restrict const filepath = getDatabaseFileNameOf(account);
    
    Database accountDatabase = databaseOpen(
        filepath, RECORD_NAME_MAX_LENGTH + 1, sizeof(RecordData)
    );

    free(filepath);
    return accountDatabase;
}

void exitFromAccount(
    AccountData* const restrict account, Database* const restrict accountDatabase
) {
    resetAccount(account);
    databaseClose(accountDatabase);
}

void resetAccount(AccountData* const restrict account) {
    fillString(account->login, LOGIN_MAX_LENGTH, '\0');
    fillString(account->password, PASSWORD_MAX_LENGTH, '\0');
}

void deleteAccountDatabaseFile(AccountData* const restrict account) {
    const char* const restrict databaseFileName = getDatabaseFileNameOf(account);
    
    remove(databaseFileName);
    free(databaseFileName);
}

static char* getDatabaseFileNameOf(const AccountData* const restrict account) {
    const unsigned int databaseExtensionLength = strlen(DATABASE_EXTENSION);

    char* restrict filepath = calloc(HASH_OF_LOGIN_LENGTH + databaseExtensionLength + 1, sizeof(char));
    if (!filepath)
        return NULL;

    const char* const restrict loginHash = getSHA256(account->login);

    strncpy(filepath, loginHash, HASH_OF_LOGIN_LENGTH);
    strncpy(filepath + HASH_OF_LOGIN_LENGTH, DATABASE_EXTENSION, databaseExtensionLength + 1);

    free(loginHash);
    return filepath;
}