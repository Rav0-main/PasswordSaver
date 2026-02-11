#include "fillstrbuffer.h"
#include <ctype.h>
#include <string.h>

void fillString(char* const restrict str, const unsigned int length, const char filling) {
	memset(str, filling, length);
}

void toLowerCase(char* restrict str) {
	while (*str) {
		*str = tolower(*str);
		++str;
	}
}