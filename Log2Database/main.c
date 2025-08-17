#define _CRT_SECURE_NO_WARNINGS

#include "main.h"

Database databaseOpen(const char* restrict filePath, const KeySize keySize, const ValueSize valueSize) {
	FILE* databaseFile = fopen(filePath, "rb+");

	if (databaseFile == NULL) {
		//create the database file
		databaseFile = fopen(filePath, "wb");

		if (databaseFile == NULL) {
			Database errorDatabase = { .stream = NULL,
				.keySize = -1, .valueSize = -1, .isOpened = false };
			return errorDatabase;
		}

		RecordCount recordCount = 0;

		fwrite(&recordCount, sizeof(RecordCount), 1, databaseFile);
		
		fclose(databaseFile);
		databaseFile = fopen(filePath, "rb+");
	}

	Database database = {.stream = databaseFile, .keySize = keySize,
		.valueSize = valueSize, .isOpened = true};

	return database;
}

bool databaseClose(Database* restrict db) {
	if (db->isOpened && fclose(db->stream) == 0) {
		db->isOpened = false;
		return true;
	}
	return false;
}