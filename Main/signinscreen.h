#pragma once

#include "accountdata.h"
#include "../Log2Database/main.h"
#include "screencodes.h"
#include <stdbool.h>

/*
* Returns true if user success signed else false
*/
bool runSignInScreen(
	const Database* const restrict userDatabase, Screen* const restrict currentScreen,
	AccountData* const restrict account
);