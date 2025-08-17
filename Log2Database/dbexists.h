#pragma once

#include "dbstruct.h"

/*
* Returns true if key found in database else false
*/
bool databaseExistsKey(const Database* restrict db, const char* restrict key);