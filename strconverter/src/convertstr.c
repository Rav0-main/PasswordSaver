#include "convertstr.h"
#include <string.h>

void convert(char* restrict str, const char* const restrict password, int convertingType) {
	if (convertingType >= 0)
		convertingType = 1;
	else
		convertingType = -1;

	size_t i = 0;
	size_t j = 0;

	size_t strLen = strlen(str);
	size_t passwordLen = strlen(password);

	for (i = 0; i != strLen; ++i, ++str) {
		*str = *str + convertingType * (int)password[j];
		j = (j + 1) % passwordLen;
	}
}