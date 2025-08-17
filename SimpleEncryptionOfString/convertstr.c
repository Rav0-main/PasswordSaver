#include "convertstr.h"
#include <string.h>

void convert(char* string, const char* password, int convertingType) {
	if (convertingType >= 0)
		convertingType = 1;
	else
		convertingType = -1;

	unsigned int i = 0;
	unsigned int j = 0;

	unsigned int sourceLen = strlen(string);
	unsigned int passwordLen = strlen(password);

	for (i = 0; i <= sourceLen - 1; i++) {
		string[i] = string[i] + convertingType * (int)password[j];
		j = (j + 1) % passwordLen;
	}
}