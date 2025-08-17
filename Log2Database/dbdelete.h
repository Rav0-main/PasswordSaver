#pragma once

#include "dbstruct.h"

/*
* Deletes a pair key-value from database(not from file) if found and returns true else returns false
*/
bool databaseDeleteValueByKey(const Database* restrict db, const char* restrict key);

/*
* Deletes a pair key-value from database(not from file) if found and returns true else returns false
*/
bool databaseDeleteValueByIndex(const Database* restrict db, const RecordCount index);

/*
* Deletes all pairs but no deletes a file objects
*/
bool databaseClear(const Database* restrict db);