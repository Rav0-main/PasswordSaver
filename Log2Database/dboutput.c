#include "dboutput.h"
#include "dbget.h"
#include <stdlib.h>

void databaseOutput(
	const Database* const restrict db, 
	const void (*outputValueFunction)(const void* value)
) {
	RecordCount recordCount = databaseGetCountOfRecords(db);

	_fseeki64(db->stream, sizeof(RecordCount), SEEK_SET);

	RecordCount currentByte = _ftelli64(db->stream);
	RecordCount index = 0;
	
	char* const restrict key = (char*)calloc(1, db->keySize);
	char* const restrict value = (char*)calloc(1, db->valueSize);
	if (!key || !value)
		return;

	while (index < recordCount) {
		_fseeki64(db->stream, currentByte, SEEK_SET);

		fread(key, db->keySize, 1, db->stream);
		fread(value, db->valueSize, 1, db->stream);
		
		currentByte = _ftelli64(db->stream);
		
		printf("index=%lld, key=%s, ", index, key);
		outputValueFunction(value);
		putchar('\n');
		
		++index;
	}
}