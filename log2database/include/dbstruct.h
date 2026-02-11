#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/*
* Type for count of records in database(count pairs of key-value)
*/
typedef int64_t RecordCount;

/*
* Type for size of keys in database(in bytes)
*/
typedef int64_t KeySize;

/*
* Type for size of values in database(in bytes)
*/
typedef int64_t ValueSize;

/*
* main structure of Database to work with log2database
*/
struct Log2Database {
	FILE* stream;
	KeySize keySize;
	ValueSize valueSize;
	bool isOpened;
};

typedef struct Log2Database Database;