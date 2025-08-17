#pragma once

#include "accountdata.h"

/*
* Decrypt all fields of RecordData
*/
void decryptRecord(RecordData* recordData, const char* password);

void outputRecord(const RecordData* recordData);

/*
* Encrypt all fields of RecordData
*/
void encryptRecord(RecordData* recordData, const char* password);