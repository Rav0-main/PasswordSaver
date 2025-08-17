#pragma once

#include "dbstruct.h"
#include <stdbool.h>

/*
* Returns true, if key found and in buffer read value
* Returns false, if key not found
*/
bool databaseGetValueByKey(const Database* restrict db, const char* restrict key, void* restrict buffer);

/*
* Returns positive RecordCound if found key
* Returns -1 if key not found
*/
RecordCount databaseGetIndexByKey(const Database* restrict db, const char* restrict key);

/*
* Returns the char pointer which created by 'calloc';
* Returns NULL if index >= count of record in database(index out of range);
*/
char* databaseGetKeyByIndex(const Database* restrict db, const RecordCount index);

/*
* Set in memory if index is valid else buffer[0] = '\0'.
*/
void databaseGetKeyByIndexInMemory(const Database* restrict db, const RecordCount index, char* const restrict buffer);

/*
* Returns count of records in database
*/
RecordCount databaseGetCountOfRecords(const Database* restrict db);

/*
* Returns a array of [start_index, end_index] which created a calloc
* If startKey == "" returns [0, record_count-1]
* If startKey not found returns NULL
*/
RecordCount* databaseGetIndexesOfKeysWhichStartWith(const Database* restrict db, const char* restrict startKey);