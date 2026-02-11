#pragma once

#include "dbstruct.h"

bool databaseIsOpened(const Database* const restrict db);
bool databaseIsClosed(const Database* const restrict db);