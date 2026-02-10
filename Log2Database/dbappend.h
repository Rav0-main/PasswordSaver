#pragma once

#include "dbstruct.h"
#include <stdbool.h>

/*
* Appends given value by key
* Returns true if success append else false
* If key found in database returns false and value of key not changed
*/
bool databaseAppendByKey(
	Database* const restrict db, const char* restrict key, 
	const void* restrict value
);