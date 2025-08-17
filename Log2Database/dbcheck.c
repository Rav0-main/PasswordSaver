#include "dbstruct.h"

bool databaseIsOpened(const Database* restrict db) {
	return db->isOpened;
}

bool databaseIsClosed(const Database* restrict db) {
	return !(db->isOpened);
}