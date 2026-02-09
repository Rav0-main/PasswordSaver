#pragma once

#define FIELD_COUNT_IN_RECORD_DATA 4

#define LOGIN_MAX_LENGTH 64
#define PASSWORD_MAX_LENGTH 64
#define RECORD_NAME_MAX_LENGTH 64
#define DESCRIPTION_MAX_LENGTH 64

typedef struct _AccountData {
	char login[LOGIN_MAX_LENGTH + 1];
	char password[PASSWORD_MAX_LENGTH + 1];
} AccountData;

typedef struct _RecordData {
	char recordName[RECORD_NAME_MAX_LENGTH + 1];
	char login[LOGIN_MAX_LENGTH + 1];
	char password[PASSWORD_MAX_LENGTH + 1];
	char description[DESCRIPTION_MAX_LENGTH + 1];
} RecordData;