#include "../Log2Database/main.h"
#include "loadingscreen.h"
#include "inputscreen.h"
#include "helpscreen.h"
#include "signinscreen.h"
#include "registerscreen.h"
#include "mainmenuscreen.h"
#include "createrecordscreen.h"
#include "outputallrecordsscreen.h"
#include "findrecordscreen.h"
#include "deleterecordscreen.h"
#include "changerecordscreen.h"
#include "accountdata.h"
#include "screencodes.h"
#include "generalscreenfuncts.h"
#include "deleteaccountscreen.h"
#include "authentication.h"
#include "fillstrbuffer.h"

#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>

#define USERS_DATABASE_FILENAME "users.logdb"
#define PROGRAM_TITLE "Password Saver"

void handleCloseProgram(int sig);
void closeProgramData(const Database* userDatabase, const Database* accountDatabase, AccountData* const restrict accountData);

int main(const int argc, char* const argv[]) {
    titleProcess(PROGRAM_TITLE);

    showLoadingScreen();

    const Database users = databaseOpen(
        USERS_DATABASE_FILENAME, 
        LOGIN_MAX_LENGTH + 1, 
        HASH_OF_PASSWORD_LENGTH + 1
    );
    
    AccountData currentAccount;
    Database currentAccountDatabase = { .isOpened = false };
    
    bool run = true;
    Screen currentScreen = InputScreen;

    resetAccount(&currentAccount);

    if (argc >= 3) {
        clearScreen();

        char* const login = argv[1];
        char* const password = argv[2];

        AuthenticationStatus status = checkAccountDataIsCorrect(&users, login, password);
        
        switch (status) {
        case AccountDataOk:
            currentScreen = MainMenuScreen;
            
            strncpy(currentAccount.login, login, strlen(login) + 1);
            strncpy(currentAccount.password, password, strlen(password) + 1);

            currentAccountDatabase = signInAccount(&currentAccount);
                
            break;

        case PasswordIsNotCorrect:
             showRedErrorWithMessage("Password is not correct!\n");
             showToPressEnter();
                
             break;

        case LoginDoesNotExists:
             showRedErrorWithMessage("Login: %s does not exist!\n", login);
             showToPressEnter();
                
             break;
        }
    }

    signal(SIGINT, handleCloseProgram);

    while (run) {
        clearScreen();

        switch (currentScreen) {
        case ExitScreen:
            run = false;
            break;

        case HelpScreen:
            runHelpScreen(&currentScreen);

            break;

        case InputScreen:
            runInputScreen(&currentScreen);

            break;

        case SignInScreen:
            bool successSignedIn = runSignInScreen(
                 &users, &currentScreen,
                 &currentAccount
            );
            if (successSignedIn)
                currentAccountDatabase = signInAccount(&currentAccount);

            break;

        case RegisterScreen:
            runRegisterScreen(&currentScreen, &users);

            break;

        case MainMenuScreen:
            runMainMenuScreen(&currentScreen, currentAccount.login);
            if (currentScreen == SignInScreen)
                exitFromAccount(&currentAccount, &currentAccountDatabase);
            
            break;
            
        case CreateRecordScreen:
            runCreateRecordScreen(
                 &currentAccountDatabase, &currentScreen,
                 currentAccount.password
            );

            break;

        case FindRecordScreen:
             runFindRecordScreen(
                 &currentAccountDatabase, &currentScreen,
                  currentAccount.password
             );

             break;

        case OutputAllRecordsScreen:
             runOutputAllRecordsScreen(
                 &currentAccountDatabase, &currentScreen,
                  currentAccount.password
             );

             break;

        case ChangeRecordScreen:
             runChangeRecordScreen(
                  &currentAccountDatabase, &currentScreen,
                  currentAccount.password
             );

             break;

        case DeleteRecordScreen:
            runDeleteRecordScreen(&currentAccountDatabase, &currentScreen);
            
            break;

        case DeleteAccountScreen:
            runDeleteAccountScreen(
                 &currentAccountDatabase, &currentAccount,
                 &users, &currentScreen
            );

            break;
        }
    }
    
    closeProgramData(&users, &currentAccountDatabase, &currentAccount);
    showToPressEnter();

    return 0;
}

void handleCloseProgram(int sig) {
    clearScreen();
    showGreenSuccessWithMessage("Program is finished!\n");

    exit(0);
}

void closeProgramData(const Database* userDatabase, const Database* accountDatabase, AccountData* const restrict accountData) {
    clearScreen();

    resetAccount(accountData);

    if (databaseIsOpened(accountDatabase))
        databaseClose(accountDatabase);

    if (databaseIsOpened(userDatabase))
        databaseClose(userDatabase);

    showGreenSuccessWithMessage("Program is finished!\n");
}