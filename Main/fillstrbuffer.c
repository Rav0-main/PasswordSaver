#include "fillstrbuffer.h"
#include <ctype.h>
#include <string.h>

void fillString(char* str, unsigned int length, const char filling) {
	for (unsigned int i = 0; i <= length; i++)
		str[i] = filling;
}

void toLowerCase(char* str) {
	const unsigned int length = strlen(str);
	for (unsigned int i = 0; i <= length - 1; i++)
		str[i] = tolower(str[i]);
}