#pragma once

/*
* Creates the sha256 of data
* Returned pointer created by 'malloc'
*/
char* getSHA256(const char* restrict data);

void getSHA256InMemory(const char* restrict data, char* restrict dist);