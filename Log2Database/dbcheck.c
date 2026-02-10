#include "dbstruct.h"

bool databaseIsOpened(const Database* const restrict db) {
	return db->isOpened;
}

bool databaseIsClosed(const Database* const restrict db) {
	return !(db->isOpened);
}