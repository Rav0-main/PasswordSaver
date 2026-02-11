/*
* Simple database on sorted array.
* Saving keys in lexigraphic order.
*/

#pragma once

#include "dbstruct.h"
#include "dbappend.h"
#include "dbget.h"
#include "dbexists.h"
#include "dbcheck.h"
#include "dbchange.h"
#include "dboutput.h"
#include "dbdelete.h"

/*
* Returns the database object
* Creates file with 'filePath' with database if 'filePath' not found in system
*/
Database databaseOpen(
	const char* restrict filepath, const KeySize keySize, 
	const ValueSize valueSize
);

/*
* Closes a database if database not closed before
*/
bool databaseClose(Database* const restrict db);