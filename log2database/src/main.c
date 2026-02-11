#define _CRT_SECURE_NO_WARNINGS

#include "log2database.h"

Database databaseOpen(
	const char* restrict filepath, const KeySize keySize,
	const ValueSize valueSize
) {
	FILE* databaseFile = fopen(filepath, "rb+");

	if (databaseFile == NULL) {
		//create the database file
		databaseFile = fopen(filepath, "wb");

		if (!databaseFile) {
			Database errorDatabase = { 
				.stream = NULL, .keySize = -1,
				.valueSize = -1, .isOpened = false 
			};

			return errorDatabase;
		}

		RecordCount recordCount = 0;

		fwrite(&recordCount, sizeof(RecordCount), 1, databaseFile);
		
		fclose(databaseFile);
		databaseFile = fopen(filepath, "rb+");
	}

	return (Database) {
		.stream = databaseFile, .keySize = keySize,
		.valueSize = valueSize, .isOpened = true
	};
}

bool databaseClose(Database* const restrict db) {
	if (db->isOpened && !fclose(db->stream)) {
		db->isOpened = false;

		return true;
	}
	return false;
}