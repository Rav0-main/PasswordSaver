/*
* For encryption and decryption strings by password.
*/

#pragma once

#include "convertstr.h"

inline static void decrypt(
	char* restrict encryption, const char* const restrict password
) {
	convert(encryption, password, -1);
}

inline static void encrypt(
	char* restrict source, const char* const restrict password
) {
	convert(source, password, 1);
}