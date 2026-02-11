#pragma once

#include "accountdata.h"
#include "log2database.h"
#include "screencodes.h"
#include <stdbool.h>

/*
* Returns true if user success signed else false
*/
bool runSignInScreen(
	const Database* const restrict users, Screen* const restrict currentScreen,
	AccountData* const restrict account
);