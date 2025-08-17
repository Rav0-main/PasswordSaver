#pragma once

#include "dbstruct.h"

/*
* Output all elements by style as "index=i, key=str_key, ...(outputValueFunction)\n"
*/
void databaseOutput(const Database* restrict db, const void (*outputValueFunction)(const void* value));