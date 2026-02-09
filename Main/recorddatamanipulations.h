#pragma once

#include "accountdata.h"

/*
* Decrypt all fields of RecordData
*/
void decryptRecord(
	RecordData* const restrict recordData, const char* const restrict password
);

void outputRecord(const RecordData* const restrict recordData);

/*
* Encrypt all fields of RecordData
*/
void encryptRecord(
	RecordData* const restrict recordData, const char* const restrict password
);