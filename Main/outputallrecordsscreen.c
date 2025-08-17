#include "../Log2Database/main.h"
#include "../SimpleEncryptionOfString/main.h"
#include "accountdata.h"
#include "generalscreenfuncts.h"
#include "screencodes.h"
#include "recorddatamanipulations.h"
#include <stdlib.h>
#include <stdio.h>

void runOutputAllRecordsScreen(const Database* accountDatabase, int* currentScreen, const char* password) {
	const RecordCount recordCount = databaseGetCountOfRecords(accountDatabase);

	if (recordCount == 0) {
		printf("You have not records in database\n");
		showToPressEnter();
		*currentScreen = MAIN_MENU_SCREEN;
		return;
	}

	RecordData currentRecord;
	char* key;
	printf("Record count: %lld\n", recordCount);
	printf("\n");

	for (RecordCount index = 0; index <= recordCount-1; index++) {
		key = databaseGetKeyByIndex(accountDatabase, index);
		databaseGetValueByKey(accountDatabase, key, &currentRecord);

		decryptRecord(&currentRecord, password);

		if(index+1 == 1)
			printf("\033[3m%lld-st record\033[0m:\n", index + 1);

		else if(index + 1 == 2)
			printf("\033[3m%lld-nd record\033[0m:\n", index + 1);

		else if(index + 1 == 3)
			printf("\033[3m%lld-rd record\033[0m:\n", index + 1);

		else
			printf("\033[3m%lld-th record\033[0m:\n", index + 1);

		outputRecord(&currentRecord);
		printf("\n");
		
		free(key);
	}
	
	*currentScreen = MAIN_MENU_SCREEN;

	showToPressEnter();
}