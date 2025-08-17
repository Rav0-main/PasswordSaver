#include "dbdelete.h"
#include "dbget.h"
#include "dbcheck.h"

#include <stdlib.h>

bool databaseDeleteValueByKey(const Database* restrict db, const char* restrict key) {
	if (databaseIsClosed(db))
		return false;

	const RecordCount indexOfKey = databaseGetIndexByKey(db, key);

	if (indexOfKey == -1)
		return false;

	return databaseDeleteValueByIndex(db, indexOfKey);
}

bool databaseDeleteValueByIndex(const Database* restrict db, const RecordCount index) {
	if (databaseIsClosed(db))
		return false;

	const RecordCount pastRecordCount;
	
	_fseeki64(db->stream, 0, SEEK_SET);
	fread(&pastRecordCount, sizeof(RecordCount), 1, db->stream);

	if (index >= pastRecordCount || index < 0)
		return false;
	
	char* const restrict tempKey = (char*)calloc(db->keySize, sizeof(char));
	char* const restrict tempValue = (char*)calloc(1, db->valueSize);

	if (tempKey == NULL || tempValue == NULL)
		return false;

	register RecordCount currentIndex = index;
	while (currentIndex < pastRecordCount - 1) {
		_fseeki64(db->stream,
			(RecordCount)sizeof(RecordCount) + (currentIndex+1) * (RecordCount)(db->keySize + db->valueSize), SEEK_SET);

		fread(tempKey, db->keySize, 1, db->stream);
		fread(tempValue, db->valueSize, 1, db->stream);

		_fseeki64(db->stream, (- 2)* (RecordCount)(db->keySize + db->valueSize), SEEK_CUR);
		
		fwrite(tempKey, db->keySize, 1, db->stream);
		fwrite(tempValue, db->valueSize, 1, db->stream);

		currentIndex++;
	}

	_fseeki64(db->stream, 0, SEEK_SET);
	const RecordCount newRecordCount = pastRecordCount - 1;
	fwrite(&newRecordCount, sizeof(RecordCount), 1, db->stream);

	free(tempKey);
	free(tempValue);

	return true;
}

bool databaseClear(const Database* restrict db) {
	if (databaseIsClosed(db))
		return false;

	_fseeki64(db->stream, 0, SEEK_SET);

	const RecordCount newRecordCount = 0;
	fwrite(&newRecordCount, sizeof(RecordCount), 1, db->stream);

	return true;
}