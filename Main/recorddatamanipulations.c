#include "recorddatamanipulations.h"
#include "../SimpleEncryptionOfString/main.h"
#include <stdio.h>

void decryptRecord(
	RecordData* const restrict recordData, const char* const restrict password
) {
	decrypt(recordData->recordName, password);
	decrypt(recordData->login, password);
	decrypt(recordData->password, password);
	decrypt(recordData->description, password);
}

void outputRecord(const RecordData* const restrict recordData) {
	printf(" - Record name: %s\n", recordData->recordName);
	printf(" - Login: %s\n", recordData->login);
	printf(" - Password: %s\n", recordData->password);
	printf(" - Description: %s\n", recordData->description);
}

void encryptRecord(
	RecordData* const restrict recordData, const char* const restrict password
) {
	encrypt(recordData->recordName, password);
	encrypt(recordData->login, password);
	encrypt(recordData->password, password);
	encrypt(recordData->description, password);
}