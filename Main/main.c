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

#define USERS_DATABASE_FILE_NAME "users.logdb"
#define PROGRAM_TITLE "Password Saver"

void handleCloseProgram(int sig);
void closeProgramData(Database* userDatabase, Database* accountDatabase, AccountData* accountData);

int main(int argc, char* argv[]) {
    titleProcess(PROGRAM_TITLE);

    showLoadingScreen();

    const Database userDatabase = databaseOpen(USERS_DATABASE_FILE_NAME, LOGIN_MAX_LENGTH + 1, HASH_OF_PASSWORD_LENGTH + 1);

    AccountData currentAccount;
    Database currentAccountDatabase = { .isOpened = false };
    
    bool runProgram = true;
    int currentScreen = INPUT_SCREEN;

    resetAccount(&currentAccount);

    if (argc >= 3) {
        clearScreen();

        char* password;
        char* login;
        const int ACCOUNT_DATA_OK = 0;
        const int PASSWORD_IS_NOT_CORRECT = 1;
        const int LOGIN_DOES_NOT_EXIST = 2;

        login = argv[1];
        password = argv[2];

        int authenticationResult = checkAccountDataIsCorrect(&userDatabase, login, password);
        
        if(authenticationResult == ACCOUNT_DATA_OK){
            currentScreen = MAIN_MENU_SCREEN;
            strncpy(currentAccount.login, login, strlen(login)+1);
            strncpy(currentAccount.password, password, strlen(password)+1);

            currentAccountDatabase = signInAccount(&currentAccount);
        }
        else if(authenticationResult == PASSWORD_IS_NOT_CORRECT){
            showRedErrorWithMessage("Password is not correct!\n");
            showToPressEnter();
        }
        else if(authenticationResult == LOGIN_DOES_NOT_EXIST){
            showRedErrorWithMessage("Login: %s does not exist!\n", login);
            showToPressEnter();
        }
    }

    signal(SIGINT, handleCloseProgram);

    while (runProgram) {
        clearScreen();

        if (currentScreen == EXIT_SCREEN)
            runProgram = false;

        else if (currentScreen == HELP_SCREEN)
            runHelpScreen(&currentScreen);

        else if (currentScreen == INPUT_SCREEN)
            runInputScreen(&currentScreen);
        
        else if (currentScreen == SIGN_IN_SCREEN) {
            bool userSuccessSigned = runSignInScreen(&userDatabase, &currentScreen, &currentAccount);
            if (userSuccessSigned)
                currentAccountDatabase = signInAccount(&currentAccount);
        }
        else if (currentScreen == REGISTER_SCREEN)
            runRegisterScreen(&currentScreen, &userDatabase);
        
        else if (currentScreen == MAIN_MENU_SCREEN) {
            runMainMenuScreen(&currentScreen, currentAccount.login);
            if (currentScreen == SIGN_IN_SCREEN)
                exitFromAccount(&currentAccount, &currentAccountDatabase);
        }
        else if (currentScreen == CREATE_RECORD_SCREEN) 
            runCreateRecordScreen(&currentAccountDatabase, &currentScreen, currentAccount.password);

        else if (currentScreen == FIND_RECORD_SCREEN) 
            runFindRecordScreen(&currentAccountDatabase, &currentScreen, currentAccount.password);

        else if (currentScreen == OUTPUT_ALL_RECORDS_SCREEN)
            runOutputAllRecordsScreen(&currentAccountDatabase, &currentScreen, currentAccount.password);

        else if (currentScreen == CHANGE_RECORD_SCREEN)
            runChangeRecordScreen(&currentAccountDatabase, &currentScreen, currentAccount.password);

        else if (currentScreen == DELETE_RECORD_SCREEN)
            runDeleteRecordScreen(&currentAccountDatabase, &currentScreen);

        else if (currentScreen == DELETE_ACCOUNT_SCREEN)
            runDeleteAccountScreen(&currentAccountDatabase, &currentAccount, &userDatabase, &currentScreen);
    }
    
    closeProgramData(&userDatabase, &currentAccountDatabase, &currentAccount);
    showToPressEnter();

    return 0;
}

void handleCloseProgram(int sig) {
    clearScreen();
    showGreenSuccessWithMessage("Program is finished!\n");
    exit(0);
}

void closeProgramData(Database* userDatabase, Database* accountDatabase, AccountData* accountData) {
    clearScreen();

    resetAccount(accountData);

    if (databaseIsOpened(accountDatabase))
        databaseClose(accountDatabase);

    if (databaseIsOpened(userDatabase))
        databaseClose(userDatabase);

    showGreenSuccessWithMessage("Program is finished!\n");
}