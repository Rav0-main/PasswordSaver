#include "convertstr.h"

void decrypt(char* encryption, const char* password) {
	convert(encryption, password, -1);
}