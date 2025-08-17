#include "dbget.h"
#include "dbcheck.h"

#include <stdlib.h>
#include <string.h>

bool databaseGetValueByKey(const Database* restrict db, const char* restrict key, void* restrict buffer) {
	if (databaseIsClosed(db))
		return false;

	_fseeki64(db->stream, 0, SEEK_SET);
	const RecordCount recordCount;

	fread(&recordCount, sizeof(RecordCount), 1, db->stream);
	if (recordCount == 0)
		return false;

	char* const currentKey = (char*)calloc(db->keySize, sizeof(char));
	if (currentKey == NULL)
		return false;

	const RecordCount indexOfKey = databaseGetIndexByKey(db, key);
	if (indexOfKey == -1)
		return false;

	_fseeki64(db->stream,
	(RecordCount)sizeof(RecordCount) + indexOfKey * (RecordCount)(db->keySize + db->valueSize) + db->keySize, SEEK_SET);
	fread(buffer, db->valueSize, 1, db->stream);

	return true;
}

RecordCount databaseGetIndexByKey(const Database* restrict db, const char* restrict key) {
	if (databaseIsClosed(db))
		return -1;

	_fseeki64(db->stream, 0, SEEK_SET);
	const RecordCount recordCount;

	fread(&recordCount, sizeof(RecordCount), 1, db->stream);
	if (recordCount == 0)
		return -1;

	char* const restrict currentKey = (char*)calloc(db->keySize, sizeof(char));
	if (currentKey == NULL)
		return -1;

	int resultCmp;
	register RecordCount leftIndex = 0;
	register RecordCount rightIndex = recordCount - 1;
	register RecordCount middleIndex;
	register int64_t currentByte;

	while (leftIndex < rightIndex) {
		middleIndex = (leftIndex + rightIndex) / 2;

		currentByte = sizeof(RecordCount) + middleIndex * (db->valueSize + db->keySize);

		_fseeki64(db->stream, currentByte, SEEK_SET);
		fread(currentKey, db->keySize, 1, db->stream);

		resultCmp = strcmp(key, currentKey);
		if (resultCmp == 0) {
			free(currentKey);
			return middleIndex;
		}
		else if (resultCmp > 0)
			leftIndex = middleIndex + 1;
		else
			rightIndex = middleIndex - 1;
	}

	middleIndex = (leftIndex + rightIndex) / 2;
	currentByte = sizeof(RecordCount) + middleIndex * (db->valueSize + db->keySize);

	_fseeki64(db->stream, currentByte, SEEK_SET);
	fread(currentKey, db->keySize, 1, db->stream);

	resultCmp = strcmp(key, currentKey);
	free(currentKey);

	if (resultCmp == 0) 
		return middleIndex;

	return -1;
}

char* databaseGetKeyByIndex(const Database* restrict db, const RecordCount index) {
	if (databaseIsClosed(db))
		return NULL;

	char* const restrict key = (char*)calloc(db->keySize, sizeof(char));
	if (key == NULL)
		return NULL;

	databaseGetKeyByIndexInMemory(db, index, key);

	if (key[0] == '\0')
		return NULL;

	return key;
}

void databaseGetKeyByIndexInMemory(const Database* restrict db, const RecordCount index, char* const restrict buffer) {
	if (databaseIsClosed(db)) {
		buffer[0] = '\0';
		return;
	}

	_fseeki64(db->stream, 0, SEEK_SET);
	const RecordCount recordCount;

	fread(&recordCount, sizeof(RecordCount), 1, db->stream);
	if (index >= recordCount || index < 0) {
		buffer[0] = '\0';
		return;
	}

	_fseeki64(db->stream, (db->keySize + db->valueSize) * index, SEEK_CUR);
	fread(buffer, db->keySize, 1, db->stream);
}

RecordCount databaseGetCountOfRecords(const Database* restrict db) {
	_fseeki64(db->stream, 0, SEEK_SET);

	RecordCount recordCount;
	fread(&recordCount, sizeof(RecordCount), 1, db->stream);

	return recordCount;
}

RecordCount* databaseGetIndexesOfKeysWhichStartWith(const Database* restrict db, const char* restrict startKey) {
	if (databaseIsClosed(db))
		return NULL;

	RecordCount* restrict result = (RecordCount*)calloc(2, sizeof(RecordCount));

	if (result == NULL)
		return NULL;

	result[0] = 0;
	result[1] = 0;

	_fseeki64(db->stream, 0, SEEK_SET);
	const RecordCount recordCount;

	fread(&recordCount, sizeof(RecordCount), 1, db->stream);
	if (recordCount == 0) {
		free(result);
		return NULL;
	}

	if (strcmp(startKey, "") == 0) {
		result[0] = 0;
		result[1] = recordCount - 1;
		return result;
	}

	const register size_t startKeyLength = strlen(startKey);
	char* const currentKey = (char*)calloc(db->keySize, sizeof(char));
	if (currentKey == NULL) {
		free(result);
		return NULL;
	}

	int resultCmp;
	register RecordCount leftIndex = 0;
	register RecordCount rightIndex = recordCount - 1;
	register RecordCount middleIndex;
	register RecordCount pointIndex = -1;

	while (leftIndex <= rightIndex) {
		middleIndex = (leftIndex + rightIndex) / 2;

		databaseGetKeyByIndexInMemory(db, middleIndex, currentKey);

		currentKey[startKeyLength] = '\0';
		resultCmp = strcmp(startKey, currentKey);

		if (resultCmp == 0) {
			pointIndex = middleIndex;
			break;
		}
		else if (resultCmp > 0)
			leftIndex = middleIndex + 1;
		else
			rightIndex = middleIndex - 1;
	}

	if (pointIndex == -1) {
		free(result);
		free(currentKey);
		return NULL;
	}
	//move to left to find leftest key which starts with startKey
	leftIndex = pointIndex+1;
	resultCmp = 0;

	while (resultCmp == 0 && leftIndex >= 1) {
		leftIndex--;
		databaseGetKeyByIndexInMemory(db, leftIndex, currentKey);

		currentKey[startKeyLength] = '\0';
		resultCmp = strcmp(startKey, currentKey);
	}

	databaseGetKeyByIndexInMemory(db, leftIndex, currentKey);

	currentKey[startKeyLength] = '\0';
	resultCmp = strcmp(startKey, currentKey);

	if (resultCmp != 0)
		leftIndex++;

	//move to right to find rightest key which starts with startKey
	rightIndex = pointIndex-1;
	resultCmp = 0;

	while (resultCmp == 0 && rightIndex <= recordCount-2) {
		rightIndex++;
		databaseGetKeyByIndexInMemory(db, rightIndex, currentKey);

		currentKey[startKeyLength] = '\0';
		resultCmp = strcmp(startKey, currentKey);
	}

	databaseGetKeyByIndexInMemory(db, rightIndex, currentKey);

	currentKey[startKeyLength] = '\0';
	resultCmp = strcmp(startKey, currentKey);

	free(currentKey);

	if (resultCmp != 0)
		rightIndex--;

	result[0] = leftIndex;
	result[1] = rightIndex;

	return result;
}