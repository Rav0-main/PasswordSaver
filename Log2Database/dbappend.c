#define _CRT_SECURE_NO_WARNINGS

#include "dbappend.h"
#include "dbget.h"
#include "dbcheck.h"

#include <string.h>
#include <stdlib.h>

static RecordCount _getIndexToAppend(const Database* restrict db , const char* restrict key);
static bool _databaseAppendByIndex(const Database* restrict db, const char* restrict key, const void* restrict value, const RecordCount index);
static void _swapKeyAndValue(char* restrict key1, char* restrict value1, char* restrict key2, char* restrict value2, const KeySize keySize, const ValueSize valueSize);
static void _swapValuesWithEqualSize(char* restrict str1, char* restrict str2, const int64_t size);
static void _copyBytesTo(char* restrict where, const char* restrict source, const int64_t size);

bool databaseAppendByKey(const Database* restrict db, const char* restrict key, const void* restrict value) {
	if (databaseIsClosed(db))
		return false;

	_fseeki64(db->stream, 0, SEEK_SET);

	const RecordCount pastRecordCount;
	fread(&pastRecordCount, sizeof(RecordCount), 1, db->stream);

	if (pastRecordCount == 0) {
		//append first element
		const RecordCount newRecordCount = 1;
		_fseeki64(db->stream, 0, SEEK_SET);
		fwrite(&newRecordCount, sizeof(RecordCount), 1, db->stream);

		fwrite(key, db->keySize, 1, db->stream);
		fwrite(value, db->valueSize, 1, db->stream);

		return true;
	}
	
	RecordCount futureIndex = _getIndexToAppend(db, key);
	if (futureIndex == -1)
		return false;
	
	_databaseAppendByIndex(db, key, value, futureIndex);
	return true;
}

static RecordCount _getIndexToAppend(const Database* restrict db, const char* restrict key) {
	_fseeki64(db->stream, 0, SEEK_SET);

	const RecordCount pastRecordCount;
	fread(&pastRecordCount, sizeof(RecordCount), 1, db->stream);

	char* const currentKey = (char*)calloc(db->keySize, sizeof(char));
	if (currentKey == NULL)
		return -1;

	databaseGetKeyByIndexInMemory(db, 0, currentKey);
	
	int resultCmp = strcmp(key, currentKey);

	if (resultCmp == 0)
		return -1;
	else if (resultCmp < 0)
		return 0;

	databaseGetKeyByIndexInMemory(db, pastRecordCount - 1, currentKey);

	resultCmp = strcmp(key, currentKey);

	if (resultCmp == 0)
		return -1;
	else if (resultCmp > 0) 
		return pastRecordCount;

	//use binary search to find future index of future (key,value)
	register RecordCount leftIndex = 1;
	register RecordCount rightIndex = pastRecordCount - 1;
	register RecordCount middleIndex;

	while (leftIndex < rightIndex) {
		middleIndex = (rightIndex + leftIndex) / 2;

		databaseGetKeyByIndexInMemory(db, middleIndex, currentKey);
		resultCmp = strcmp(key, currentKey);

		if (resultCmp == 0)
			return -1;
		else if (resultCmp > 0)
			leftIndex = middleIndex + 1;
		else
			rightIndex = middleIndex - 1;
	}

	databaseGetKeyByIndexInMemory(db, leftIndex - 1, currentKey);

	const char* currentKey2;
	currentKey2 = databaseGetKeyByIndex(db, leftIndex);

	int resultCmpRight = strcmp(key, currentKey2);
	int resultCmpLeft = strcmp(key, currentKey);
	
	free(currentKey);
	free(currentKey2);

	if (resultCmpLeft > 0 && resultCmpRight < 0)
		return leftIndex;
	else if (resultCmpLeft > 0 && resultCmpRight > 0)
		return leftIndex + 1;
	else if (resultCmpLeft < 0 && resultCmpRight < 0)
		return leftIndex - 1;
	else
		return -1;
}

static bool _databaseAppendByIndex(const Database* restrict db, const char* restrict key, const void* restrict value, const RecordCount index) {
	const RecordCount pastRecordCount;

	_fseeki64(db->stream, 0, SEEK_SET);

	fread(&pastRecordCount, sizeof(RecordCount), 1, db->stream);
	const RecordCount newRecordCount = pastRecordCount + 1;

	if (index > pastRecordCount)
		return false;

	else if (index == pastRecordCount) {
		//append in end
		_fseeki64(db->stream, 0, SEEK_SET);
		fwrite(&newRecordCount, sizeof(RecordCount), 1, db->stream);

		_fseeki64(db->stream, (db->keySize+db->valueSize)*index, SEEK_CUR);
		fwrite(key, db->keySize, 1, db->stream);
		fwrite(value, db->valueSize, 1, db->stream);

		return true;
	}
	char* const restrict tempKey = (char*)calloc(db->keySize, sizeof(char));
	char* const restrict tempValue = (char*)malloc(db->valueSize);

	char* const restrict currentDatabaseKey = (char*)calloc(db->keySize, sizeof(char));
	char* const restrict currentDatabaseValue = (char*)malloc(db->valueSize);

	_fseeki64(db->stream, (RecordCount)sizeof(RecordCount) + index * (RecordCount)(db->keySize + db->valueSize), SEEK_SET);

	fread(tempKey, sizeof(char), db->keySize, db->stream);
	fread(tempValue, db->valueSize, 1, db->stream);

	_fseeki64(db->stream, -(RecordCount)(db->keySize + db->valueSize), SEEK_CUR);
	
	fwrite(key, sizeof(char), db->keySize, db->stream);
	fwrite(value, db->valueSize, 1, db->stream);

	register RecordCount currentIndex = index+1;
	register int64_t currentByte = (RecordCount)sizeof(RecordCount) + currentIndex * (RecordCount)(db->keySize + db->valueSize);

	//shifting all keys which have i >= index+1 on 1 to append a new key
	while (currentIndex <= pastRecordCount-1) {
		_fseeki64(db->stream, currentByte, SEEK_SET);

		fread(currentDatabaseKey, sizeof(char), db->keySize, db->stream);
		fread(currentDatabaseValue, db->valueSize, 1, db->stream);

		_fseeki64(db->stream, -(RecordCount)(db->keySize + db->valueSize), SEEK_CUR);
		
		fwrite(tempKey, sizeof(char), db->keySize, db->stream);
		fwrite(tempValue, db->valueSize, 1, db->stream);
		currentByte += (db->keySize + db->valueSize);

		_swapKeyAndValue(tempKey, tempValue, currentDatabaseKey, currentDatabaseValue, db->keySize, db->valueSize);

		currentIndex++;
	}

	_fseeki64(db->stream, 0, SEEK_SET);
	fwrite(&newRecordCount, sizeof(RecordCount), 1, db->stream);

	_fseeki64(db->stream, 0, SEEK_END);
	fwrite(tempKey, sizeof(char), db->keySize, db->stream);
	fwrite(tempValue, db->valueSize, 1, db->stream);

	free(tempKey);
	free(tempValue);
	free(currentDatabaseKey);
	free(currentDatabaseValue);

	return true;
}

static void _swapKeyAndValue(char* restrict key1, char* restrict value1, char* restrict key2, char* restrict value2, const KeySize keySize, const ValueSize valueSize) {
	_swapValuesWithEqualSize(key1, key2, keySize);
	_swapValuesWithEqualSize(value1, value2, valueSize);
}

static void _swapValuesWithEqualSize(char* restrict str1, char* restrict str2, const int64_t size) {
	char* temp = (char*)malloc(size*sizeof(char));

	_copyBytesTo(temp, str1, size);
	_copyBytesTo(str1, str2, size);
	_copyBytesTo(str2, temp, size);

	free(temp);
}

static void _copyBytesTo(char* restrict where, const char* restrict source, const int64_t size) {
	for (int64_t i = 0; i <= size - 1; i++)
		where[i] = source[i];
}