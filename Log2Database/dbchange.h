#pragma once

#include "dbstruct.h"

/*
* Returns true if value of key changed on newValue, else false
*/
bool databaseChangeValueByKey(const Database* restrict db, const char* restrict key, const void* restrict newValue);
/*
* Returns true if value by index changed on newValue, else false
*/
bool databaseChangeValueByIndex(const Database* restrict db, const char* restrict key, const void* restrict newValue, const RecordCount index);
