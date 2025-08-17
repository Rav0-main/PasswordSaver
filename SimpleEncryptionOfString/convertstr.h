#pragma once

/*
* Converts string by password
* Converting type is 1 or -1: to encrypt convertingType = 1, to decrypt convertingType = -1
* Changes a string on needed convertion
*/
void convert(char* string, const char* password, int convertingType);