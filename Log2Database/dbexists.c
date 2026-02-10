#include "dbexists.h"
#include "dbget.h"
#include "dbcheck.h"

bool databaseExistsKey(
	const Database* const restrict db, const char* restrict key
) {
	if (databaseIsClosed(db))
		return false;

	else if(databaseGetIndexByKey(db, key) == -1)
		return false;

	return true;
}