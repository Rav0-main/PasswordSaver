#include "dbchange.h"
#include "dbget.h"
#include "dbcheck.h"

bool databaseChangeValueByKey(const Database* restrict db, const char* restrict key, const void* restrict newValue) {
	if (databaseIsClosed(db))
		return false;

	RecordCount index = databaseGetIndexByKey(db, key);
	if (index == -1)
		return false;

	return databaseChangeValueByIndex(db, key, newValue, index);
}

bool databaseChangeValueByIndex(const Database* restrict db, const char* restrict key, const void* restrict newValue, const RecordCount index) {
	if (!(db->isOpened))
		return false;

	const RecordCount recordCount;
	_fseeki64(db->stream, 0, SEEK_SET);
	fread(&recordCount, sizeof(RecordCount), 1, db->stream);

	if (index >= recordCount || index < 0)
		return false;

	//move to byte which start of value by given index
	_fseeki64(db->stream,
		(int64_t)sizeof(RecordCount) + index * (RecordCount)(db->keySize + db->valueSize) + db->keySize, SEEK_SET);

	fwrite(newValue, db->valueSize, 1, db->stream);

	return true;
}