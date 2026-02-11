from databaselib import Log2DatabaseImpl
from random import randint
from os import remove as removeFile
import ctypes
import unittest

VALUE_SIZE: int = ctypes.sizeof(ctypes.c_int) + 1

def generateRandomSymbolicKey(length: int) -> str:
    result: str = ""
    for i in range(0, length):
        result = result + ALPHABET[randint(0, ALPHABET_LENGTH-1)]

    return result

def getRandomLinieredInt(base: int) -> int:
    k: int = randint(-8, 8)
    b: int = randint(-128, 128)

    return k * base + b

SMALL_KEY_COUNT: int = 256
HUGE_KEY_COUNT: int = 1536
ALPHABET: str = "qwertyuiopasdfghjklzxcvbnm"
ALPHABET_LENGTH: int = len(ALPHABET)

class TestOfGetCountOfRecordsOfLog2DatabaseWithSmallCountKeys(unittest.TestCase):
    databaseFileName: str = "tstDbToCntOfRcrdsWthSmlCntKeys.logdb"
    keyCount: int = SMALL_KEY_COUNT

    def test_GettingCountOfRecordsWithUniqueRandomKeysWithEqualLength(self):
        keyLength: int = 31
        self.database = Log2DatabaseImpl(self.databaseFileName, keyLength+1, VALUE_SIZE)

        keysAndValues: list[tuple[str, int]] = []
        hashedKeys: dict[str, bool] = {}
        key: str = ""

        while(len(keysAndValues) < self.keyCount):
            key = generateRandomSymbolicKey(keyLength)
            if(key not in hashedKeys):
                keysAndValues.append((key, len(keysAndValues)))
                hashedKeys[key] = True

        for uniqueKeyInformation in keysAndValues:
            uniqueKey = uniqueKeyInformation[0]
            truthValue = uniqueKeyInformation[1]
            self.database.appendByKey(uniqueKey, ctypes.pointer(ctypes.c_int(truthValue)))

        countOfRecords = self.database.getCountOfRecords()
        self.assertEqual(len(keysAndValues), countOfRecords, f"In database must be {len(keysAndValues)} but found {countOfRecords}")

    def test_GettingCountOfRecordsWithUniqueRandomKeysWithDifferentLength(self):
        maxKeyLength: int = 127
        minKeyLength: int = 7

        self.database = Log2DatabaseImpl(self.databaseFileName, maxKeyLength+1, VALUE_SIZE)

        keysAndValues: list[tuple[str, int]] = []
        hashedKeys: dict[str, bool] = {}
        key: str = ""
        randomLength: int = 0

        while(len(keysAndValues) < self.keyCount):
            randomLength = randint(minKeyLength, maxKeyLength)
            key = generateRandomSymbolicKey(randomLength)
            if(key not in hashedKeys):
                keysAndValues.append((key, len(keysAndValues)))
                hashedKeys[key] = True

        for uniqueKeyInformation in keysAndValues:
            uniqueKey = uniqueKeyInformation[0]
            truthValue = uniqueKeyInformation[1]
            self.database.appendByKey(uniqueKey, ctypes.pointer(ctypes.c_int(truthValue)))

        countOfRecords = self.database.getCountOfRecords()
        self.assertEqual(len(keysAndValues), countOfRecords, f"In database must be {len(keysAndValues)} but found {countOfRecords}")

    def tearDown(self) -> None:
        self.database.close()
        removeFile(self.databaseFileName)

class TestOfGetCountOfRecordsOfLog2DatabaseWithBigCountKeys(TestOfGetCountOfRecordsOfLog2DatabaseWithSmallCountKeys):
    databaseFileName: str = "tstDbToCntOfRcrdsWthBigCntKeys.logdb"
    keyCount: int = HUGE_KEY_COUNT
    
class TestOfAppendingOfLog2DatabaseWithSmallCountKeys(unittest.TestCase):
    databaseFileName: str = "tstDbToAppendWthSmlCntKeys.logdb"
    keyCount = SMALL_KEY_COUNT

    def test_AppendingUniqueRandomKeysWithEqualLength(self):
        keyLength: int = 31
        self.database = Log2DatabaseImpl(self.databaseFileName, keyLength+1, VALUE_SIZE)

        keysAndValues: list[tuple[str, int]] = []
        hashedKeys: dict[str, bool] = {}
        key: str = ""

        while(len(keysAndValues) < self.keyCount):
            key = generateRandomSymbolicKey(keyLength)
            if(key not in hashedKeys):
                keysAndValues.append((key, len(keysAndValues)))
                hashedKeys[key] = True

        for uniqueKeyInformation in keysAndValues:
            uniqueKey = uniqueKeyInformation[0]
            truthValue = uniqueKeyInformation[1]
            self.database.appendByKey(uniqueKey, ctypes.pointer(ctypes.c_int(truthValue)))

        buffer: ctypes.c_int = ctypes.c_int(-1)
        bufferPointer: ctypes._Pointer = ctypes.pointer(buffer)

        for uniqueKeyInformation in keysAndValues:
            uniqueKey = uniqueKeyInformation[0]
            truthValue = uniqueKeyInformation[1]

            self.database.getValueByKey(uniqueKey, bufferPointer)

            self.assertEqual(truthValue, buffer.value,
            f"{uniqueKey} from list have value of {truthValue}, but from database {buffer.value}")    

    def test_AppendingUniqueRandomKeysWithDifferentLength(self):
        maxKeyLength: int = 127
        minKeyLength: int = 7

        self.database = Log2DatabaseImpl(self.databaseFileName, maxKeyLength+1, VALUE_SIZE)

        keysAndValues: list[tuple[str, int]] = []
        hashedKeys: dict[str, bool] = {}
        key: str = ""
        randomLength: int = 0

        while(len(keysAndValues) < self.keyCount):
            randomLength = randint(minKeyLength, maxKeyLength)
            key = generateRandomSymbolicKey(randomLength)
            if(key not in hashedKeys):
                keysAndValues.append((key, len(keysAndValues)))
                hashedKeys[key] = True

        for keyAndValue in keysAndValues:
            key = keyAndValue[0]
            value = keyAndValue[1]
            self.database.appendByKey(key, ctypes.pointer(ctypes.c_int(value)))

        buffer: ctypes.c_int = ctypes.c_int(-1)
        bufferPointer: ctypes._Pointer = ctypes.pointer(buffer)
        for keyAndValue in keysAndValues:
            uniqueKey = keyAndValue[0]
            truthValue = keyAndValue[1]

            self.database.getValueByKey(uniqueKey, bufferPointer)

            self.assertEqual(truthValue, buffer.value,
            f"{uniqueKey} from list have value of {truthValue}, but from database {buffer.value}")

    def test_AppendingRandomSymbolicKeysWithEqualLength(self):
        keyLength: int = 2
        self.database = Log2DatabaseImpl(self.databaseFileName, keyLength+1, VALUE_SIZE)

        keys: list[str] = []
        key: str = ""

        while(len(keys) < self.keyCount):
            key = generateRandomSymbolicKey(keyLength)
            keys.append(key)

        settedKeys: set[str] = set(keys)
        wasAppendedKeys: dict[str, int] = {}
        #check appended keys
        for index, key in enumerate(keys, start=0):
            self.database.appendByKey(key, ctypes.pointer(ctypes.c_int(index)))
            
            if(key not in wasAppendedKeys):
                wasAppendedKeys[key] = index

        buffer: ctypes.c_int = ctypes.c_int(-1)
        bufferPointer: ctypes._Pointer = ctypes.pointer(buffer)
        for key in wasAppendedKeys.keys():
            self.database.getValueByKey(key, bufferPointer)
            self.assertEqual(wasAppendedKeys[key], buffer.value,
            f"{key} from list have value of {wasAppendedKeys[key]}, but from database {buffer.value}")

        countOfRecords = self.database.getCountOfRecords()
        self.assertEqual(len(settedKeys), countOfRecords, f"Count of unique keys: {len(settedKeys)}, but in database {countOfRecords}")

        #check keys which not appended in database
        notHaveKeys: list[str] = []
        while(len(notHaveKeys) < self.keyCount):
            key = generateRandomSymbolicKey(keyLength)
            if(key not in settedKeys):
                notHaveKeys.append(key)

        for notHaveKey in notHaveKeys:
            haveInDatabase = self.database.existsKey(key)
            self.assertEqual(False, haveInDatabase, f"{notHaveKey} found in database, but he not appended early")

    def test_AppendingRandomSymbolicKeysWithDifferentLength(self):
        minKeyLength: int = 2
        maxKeyLength: int = 31
        self.database = Log2DatabaseImpl(self.databaseFileName, maxKeyLength+1, VALUE_SIZE)

        keys: list[str] = []
        key: str = ""
        keyLength: int = 0

        while(len(keys) < self.keyCount):
            keyLength: int = randint(minKeyLength, maxKeyLength)
            key = generateRandomSymbolicKey(keyLength)
            keys.append(key)

        settedKeys: set[str] = set(keys)
        wasAppendedKeys: dict[str, int] = {}
        #check appended keys
        for index, key in enumerate(keys, start=0):
            self.database.appendByKey(key, ctypes.pointer(ctypes.c_int(index)))
            
            if(key not in wasAppendedKeys):
                wasAppendedKeys[key] = index

        buffer: ctypes.c_int = ctypes.c_int(-1)
        bufferPointer: ctypes._Pointer = ctypes.pointer(buffer)
        for key in wasAppendedKeys.keys():
            self.database.getValueByKey(key, bufferPointer)
            self.assertEqual(wasAppendedKeys[key], buffer.value,
            f"{key} from list have value of {wasAppendedKeys[key]}, but from database {buffer.value}")

        countOfRecords = self.database.getCountOfRecords()
        self.assertEqual(len(settedKeys), countOfRecords, f"Count of unique keys: {len(settedKeys)}, but in database {countOfRecords}")

        #check keys which not appended in database
        notHaveKeys: list[str] = []
        while(len(notHaveKeys) < self.keyCount):
            key = generateRandomSymbolicKey(keyLength)
            if(key not in settedKeys):
                notHaveKeys.append(key)

        for notHaveKey in notHaveKeys:
            haveInDatabase = self.database.existsKey(key)
            self.assertEqual(False, haveInDatabase, f"{notHaveKey} found in database, but he not appended early")

    def tearDown(self) -> None:
        self.database.close()
        removeFile(self.databaseFileName)

class TestOfAppendingOfLog2DatabaseWithBigCountKeys(TestOfAppendingOfLog2DatabaseWithSmallCountKeys):
    databaseFileName: str = "tstDbToAppendWthBigCntKeys.logdb"
    keyCount = HUGE_KEY_COUNT

class TestOfExistingKeyOfLog2DatabaseWithSmallCountKeys(unittest.TestCase):
    databaseFileName: str = "tstDbToExistWthSmlCntKeys.logdb"
    keyCount: int = SMALL_KEY_COUNT

    def test_ExistingKeyWithEqualLength(self):
        keyLength: int = 31
        self.database = Log2DatabaseImpl(self.databaseFileName, keyLength+1, VALUE_SIZE)

        keysAndValues: list[tuple[str, int]] = []
        hashedKeys: dict[str, bool] = {}
        key: str = ""

        while(len(keysAndValues) < self.keyCount):
            key = generateRandomSymbolicKey(keyLength)
            if(key not in hashedKeys):
                keysAndValues.append((key, len(keysAndValues)))
                hashedKeys[key] = True

        for keyAndValue in keysAndValues:
            key = keyAndValue[0]
            index = keyAndValue[1]
            self.database.appendByKey(key, ctypes.pointer(ctypes.c_int(index)))

        keysAndValues.sort(key=lambda x: x[0])
        for keyAndValue in keysAndValues:
            key = keyAndValue[0]
            haveInDatabase = self.database.existsKey(key)
            self.assertEqual(True, haveInDatabase, f"Key {key} not in database, but he appended")

        notHaveKeys: list[str] = []
        hashedNotHavedKeys: dict[str, bool] = {}
        while(len(notHaveKeys) < self.keyCount):
            key = generateRandomSymbolicKey(keyLength)
            if(key not in hashedNotHavedKeys and key not in hashedKeys):
                notHaveKeys.append(key)
                hashedNotHavedKeys[key] = True

        for key in notHaveKeys:
            haveInDatabase = self.database.existsKey(key)
            self.assertEqual(False, haveInDatabase, f"{key} not in database, but founded")

    def test_ExistingKeyWithDifferentLength(self):
        minKeyLength: int = 7
        maxKeyLength: int = 31

        self.database = Log2DatabaseImpl(self.databaseFileName, maxKeyLength+1, VALUE_SIZE)

        keysAndValues: list[tuple[str, int]] = []
        hashedKeys: dict[str, bool] = {}
        key: str = ""
        keyLength: int = 0

        while(len(keysAndValues) < self.keyCount):
            keyLength: int = randint(minKeyLength, maxKeyLength)
            key = generateRandomSymbolicKey(keyLength)
            if(key not in hashedKeys):
                keysAndValues.append((key, len(keysAndValues)))
                hashedKeys[key] = True

        for keyAndValue in keysAndValues:
            key = keyAndValue[0]
            index = keyAndValue[1]
            self.database.appendByKey(key, ctypes.pointer(ctypes.c_int(index)))

        keysAndValues.sort(key=lambda x: x[0])
        for keyAndValue in keysAndValues:
            key = keyAndValue[0]
            haveInDatabase = self.database.existsKey(key)
            self.assertEqual(True, haveInDatabase, f"Key {key} not in database, but he appended")

        notHaveKeys: list[str] = []
        hashedNotHavedKeys: dict[str, bool] = {}
        while(len(notHaveKeys) < self.keyCount):
            keyLength = randint(minKeyLength, maxKeyLength)
            key = generateRandomSymbolicKey(keyLength)
            if(key not in hashedNotHavedKeys and key not in hashedKeys):
                notHaveKeys.append(key)
                hashedNotHavedKeys[key] = True

        for key in notHaveKeys:
            haveInDatabase = self.database.existsKey(key)
            self.assertEqual(False, haveInDatabase, f"{key} not in database, but founded")

    def tearDown(self) -> None:
        self.database.close()
        removeFile(self.databaseFileName)

class TestOfExistingKeyOfLog2DatabaseWithBigCountKeys(TestOfExistingKeyOfLog2DatabaseWithSmallCountKeys):
    databaseFileName: str = "tstDbToExistWthBigCntKeys.logdb"
    keyCount: int = HUGE_KEY_COUNT

class TestOfChangingValueOfLog2DatabaseWithSmallCountKeys(unittest.TestCase):
    databaseFileName: str = "tstDbToChngValsWthSmlCntKeys.logdb"
    keyCount: int = SMALL_KEY_COUNT

    def test_ChangingValuesWithEqualLengthOfUniqueKeys(self):
        keyLength: int = 127
        self.database = Log2DatabaseImpl(self.databaseFileName, keyLength+1, VALUE_SIZE)

        keysAndValues: list[tuple[str, int]] = []
        hashedKeys: dict[str, bool] = {}
        key: str = ""

        while(len(keysAndValues) < self.keyCount):
            key = generateRandomSymbolicKey(keyLength)
            if(key not in hashedKeys):
                keysAndValues.append((key, len(keysAndValues)))
                hashedKeys[key] = True

        for keyAndValue in keysAndValues:
            key = keyAndValue[0]
            index = keyAndValue[1]
            self.database.appendByKey(key, ctypes.pointer(ctypes.c_int(index)))

        newKeysAndValues: list[tuple[str, int]] = []
        for i in range(0, len(keysAndValues)):
            newKeysAndValues.append((keysAndValues[i][0], getRandomLinieredInt(keysAndValues[i][1])))

        for keyAndValue in newKeysAndValues:
            key = keyAndValue[0]
            value = keyAndValue[1]
            self.database.changeValueByKey(key, ctypes.pointer(ctypes.c_int(value)))

        buffer: ctypes.c_int = ctypes.c_int(-1)
        bufferPointer: ctypes._Pointer = ctypes.pointer(buffer)
        for uniqueKeyInformation in newKeysAndValues:
            uniqueKey = uniqueKeyInformation[0]
            truthValue = uniqueKeyInformation[1]

            self.database.getValueByKey(uniqueKey, bufferPointer)

            self.assertEqual(truthValue, buffer.value,
            f"{uniqueKey} from list have value of {truthValue}, but from database {buffer.value}")

    def test_ChangingValuesWithDifferentLengthOfUniqueKeys(self):
        minKeyLength: int = 2
        maxKeyLength: int = 127
        self.database = Log2DatabaseImpl(self.databaseFileName, maxKeyLength+1, VALUE_SIZE)

        keysAndValues: list[tuple[str, int]] = []
        hashedKeys: dict[str, bool] = {}
        key: str = ""

        while(len(keysAndValues) < self.keyCount):
            keyLength: int = randint(minKeyLength, maxKeyLength)
            key = generateRandomSymbolicKey(keyLength)
            if(key not in hashedKeys):
                keysAndValues.append((key, len(keysAndValues)))
                hashedKeys[key] = True

        for keyAndValue in keysAndValues:
            key = keyAndValue[0]
            index = keyAndValue[1]
            self.database.appendByKey(key, ctypes.pointer(ctypes.c_int(index)))

        newKeysAndValues: list[tuple[str, int]] = []
        for i in range(0, len(keysAndValues)):
            newKeysAndValues.append((keysAndValues[i][0], getRandomLinieredInt(keysAndValues[i][1])))

        for keyAndValue in newKeysAndValues:
            key = keyAndValue[0]
            value = keyAndValue[1]
            self.database.changeValueByKey(key, ctypes.pointer(ctypes.c_int(value)))

        buffer: ctypes.c_int = ctypes.c_int(-1)
        bufferPointer: ctypes._Pointer = ctypes.pointer(buffer)
        for uniqueKeyInformation in newKeysAndValues:
            uniqueKey = uniqueKeyInformation[0]
            truthValue = uniqueKeyInformation[1]

            self.database.getValueByKey(uniqueKey, bufferPointer)

            self.assertEqual(truthValue, buffer.value,
            f"{uniqueKey} from list have value of {truthValue}, but from database {buffer.value}")

    def tearDown(self) -> None:
        self.database.close()
        removeFile(self.databaseFileName)

class TestOfChangingValueOfLog2DatabaseWithBigCountKeys(TestOfChangingValueOfLog2DatabaseWithSmallCountKeys):
    databaseFileName: str = "tstDbToChngValsWthBigCntKeys.logdb"
    keyCount: int = HUGE_KEY_COUNT

class TestOfFindingKeysWhichStartWithOfLog2DatabaseWithSmallCountKeys(unittest.TestCase):
    databaseFileName: str = "tstDbToFndngKeysWthSmlCntKeys.logdb"
    keyCount: int = SMALL_KEY_COUNT

    def test_FindingKeysWithEqualLengthOfUniqueKeys(self):
        keyLength: int = 127
        self.database = Log2DatabaseImpl(self.databaseFileName, keyLength+1, VALUE_SIZE)

        keysAndValues: list[tuple[str, int]] = []
        hashedKeys: dict[str, bool] = {}
        hashedHalfKeys: dict[str, bool] = {}
        key: str = ""

        while(len(keysAndValues) < 3*self.keyCount//4):
            key = generateRandomSymbolicKey(keyLength)
            halfKey = key[0:keyLength//2]
            if(key not in hashedKeys and halfKey not in hashedHalfKeys):
                keysAndValues.append((key, len(keysAndValues)))
                hashedKeys[key] = True
                hashedHalfKeys[halfKey] = True

        mainHalfKey = generateRandomSymbolicKey(keyLength//2)
        while(mainHalfKey in hashedHalfKeys):
            mainHalfKey = generateRandomSymbolicKey(keyLength//2)

        while(len(keysAndValues) < self.keyCount):
            secondHalfKey = generateRandomSymbolicKey(keyLength-keyLength//2)
            key = mainHalfKey + secondHalfKey
            if(key not in hashedKeys):
                keysAndValues.append((key, len(keysAndValues)))
                hashedKeys[key] = True
        
        for keyAndValue in keysAndValues:
            self.database.appendByKey(keyAndValue[0], ctypes.pointer(ctypes.c_int(keyAndValue[1])))
        
        keysAndValues.sort(key=lambda x: x[0])

        left: int = 0
        while(keysAndValues[left][0][0:keyLength//2] != mainHalfKey):
            left += 1

        right = left
        while(keysAndValues[right][0][0:keyLength//2] == mainHalfKey):
            right += 1

        right -= 1

        resultPtr = self.database.getIndexesOfKeysWhichStartWith(mainHalfKey)
        result = resultPtr[:2]

        result = (int(result[0]), int(result[1]))

        self.assertEqual(left, result[0], f"In array left index have value: {left} but in database: {result[0]}")
        self.assertEqual(right, result[1], f"In array right index have value: {right} but in database: {result[1]}")

    def test_FindingKeysWithDifferentLengthOfUniqueKeys(self):
        maxKeyLength: int = 127
        self.database = Log2DatabaseImpl(self.databaseFileName, maxKeyLength+1, VALUE_SIZE)

        keysAndValues: list[tuple[str, int]] = []
        hashedKeys: dict[str, bool] = {}
        hashedHalfKeys: dict[str, bool] = {}
        key: str = ""

        while(len(keysAndValues) < 3*self.keyCount//4):
            key = generateRandomSymbolicKey(maxKeyLength)
            halfKey = key[0:maxKeyLength//2]
            if(key not in hashedKeys and halfKey not in hashedHalfKeys):
                keysAndValues.append((key, len(keysAndValues)))
                hashedKeys[key] = True
                hashedHalfKeys[halfKey] = True

        mainHalfKey = generateRandomSymbolicKey(maxKeyLength//2)
        while(mainHalfKey in hashedHalfKeys):
            mainHalfKey = generateRandomSymbolicKey(maxKeyLength//2)

        while(len(keysAndValues) < self.keyCount):
            secondHalfKey = generateRandomSymbolicKey(randint(0, maxKeyLength-maxKeyLength//2-1))
            key = mainHalfKey + secondHalfKey
            if(key not in hashedKeys):
                keysAndValues.append((key, len(keysAndValues)))
                hashedKeys[key] = True
        
        for keyAndValue in keysAndValues:
            self.database.appendByKey(keyAndValue[0], ctypes.pointer(ctypes.c_int(keyAndValue[1])))
        
        keysAndValues.sort(key=lambda x: x[0])

        left: int = 0
        while(keysAndValues[left][0][0:maxKeyLength//2] != mainHalfKey):
            left += 1

        right = left
        while(keysAndValues[right][0][0:maxKeyLength//2] == mainHalfKey):
            right += 1

        right -= 1

        resultPtr = self.database.getIndexesOfKeysWhichStartWith(mainHalfKey)
        result = resultPtr[:2]

        result = (int(result[0]), int(result[1]))

        self.assertEqual(left, result[0], f"In array left index have value: {left} but in database: {result[0]}")
        self.assertEqual(right, result[1], f"In array right index have value: {right} but in database: {result[1]}")

    def tearDown(self):
        self.database.close()
        removeFile(self.databaseFileName)

class TestOfFindingKeysWhichStartWithOfLog2DatabaseWithBigCountKeys(TestOfFindingKeysWhichStartWithOfLog2DatabaseWithSmallCountKeys):
    databaseFileName: str = "tstDbToFndngKeysWthBigCntKeys.logdb"
    keyCount: int = HUGE_KEY_COUNT

class TestOfDeletingValueOfLog2DatabaseWithSmallCountKeys(unittest.TestCase):
    databaseFileName: str = "tstDbToDltValsWthSmlCntKeys.logdb"
    keyCount: int = SMALL_KEY_COUNT

    def test_DeletingValuesWithEqualLengthOfUniqueKeys(self):
        keyLength: int = 127
        self.database = Log2DatabaseImpl(self.databaseFileName, keyLength+1, VALUE_SIZE)

        keysAndValues: list[tuple[str, int]] = []
        hashedKeys: dict[str, bool] = {}
        key: str = ""

        while(len(keysAndValues) < self.keyCount):
            key = generateRandomSymbolicKey(keyLength)
            if(key not in hashedKeys):
                keysAndValues.append((key, len(keysAndValues)))
                hashedKeys[key] = True

        for keyAndValue in keysAndValues:
            key = keyAndValue[0]
            index = keyAndValue[1]
            self.database.appendByKey(key, ctypes.pointer(ctypes.c_int(index)))

        minIndexToDelete = randint(0, self.keyCount//4)
        maxIndexToDelete = randint(minIndexToDelete+1, self.keyCount-1)

        for i in range(minIndexToDelete, maxIndexToDelete+1):
            key = keysAndValues[i][0]
            self.database.deleteValueByKey(key)
            keysAndValues[i] = (key, -1)

        buffer: ctypes.c_int = ctypes.c_int(-1)
        bufferPointer: ctypes._Pointer = ctypes.pointer(buffer)
        for i in range(0, self.keyCount):
            key = keysAndValues[i][0]

            if(i >= minIndexToDelete and i <= maxIndexToDelete):
                haveInDatabase = self.database.existsKey(key)
                self.assertEqual(False, haveInDatabase, f"{key} deleted but in database found")
            else:
                haveInDatabase = self.database.existsKey(key)
                self.assertEqual(True, haveInDatabase, f"{key} not deleted but in database not found")
                self.database.getValueByKey(key, bufferPointer)
                self.assertEqual(keysAndValues[i][1], bufferPointer.contents.value, f"{key} in list have {keysAndValues[i][1]} but in from database {bufferPointer.contents.value}")

    def test_DeletingValuesWithDifferentLengthOfUniqueKeys(self):
        minKeyLength: int = 7
        maxKeyLength: int = 127
        self.database = Log2DatabaseImpl(self.databaseFileName, maxKeyLength+1, VALUE_SIZE)

        keysAndValues: list[tuple[str, int]] = []
        hashedKeys: dict[str, bool] = {}
        key: str = ""

        while(len(keysAndValues) < self.keyCount):
            keyLength = randint(minKeyLength, maxKeyLength)
            key = generateRandomSymbolicKey(keyLength)
            if(key not in hashedKeys):
                keysAndValues.append((key, len(keysAndValues)))
                hashedKeys[key] = True

        for keyAndValue in keysAndValues:
            key = keyAndValue[0]
            index = keyAndValue[1]
            self.database.appendByKey(key, ctypes.pointer(ctypes.c_int(index)))

        minIndexToDelete = randint(0, self.keyCount//4)
        maxIndexToDelete = randint(minIndexToDelete+1, self.keyCount-1)

        for i in range(minIndexToDelete, maxIndexToDelete+1):
            key = keysAndValues[i][0]
            self.database.deleteValueByKey(key)
            keysAndValues[i] = (key, -1)

        buffer: ctypes.c_int = ctypes.c_int(-1)
        bufferPointer: ctypes._Pointer = ctypes.pointer(buffer)
        for i in range(0, self.keyCount):
            key = keysAndValues[i][0]

            if(i >= minIndexToDelete and i <= maxIndexToDelete):
                haveInDatabase = self.database.existsKey(key)
                self.assertEqual(False, haveInDatabase, f"{key} deleted but in database found")
            else:
                haveInDatabase = self.database.existsKey(key)
                self.assertEqual(True, haveInDatabase, f"{key} not deleted but in database not found")
                self.database.getValueByKey(key, bufferPointer)
                self.assertEqual(keysAndValues[i][1], bufferPointer.contents.value, f"{key} in list have {keysAndValues[i][1]} but in from database {bufferPointer.contents.value}")

    def test_DeletingValuesWhichNotAppendedInDatabase(self):
        minKeyLength: int = 7
        maxKeyLength: int = 127
        self.database = Log2DatabaseImpl(self.databaseFileName, maxKeyLength+1, VALUE_SIZE)

        keysAndValues: list[tuple[str, int]] = []
        hashedKeys: dict[str, bool] = {}
        key: str = ""

        while(len(keysAndValues) < self.keyCount):
            keyLength = randint(minKeyLength, maxKeyLength)
            key = generateRandomSymbolicKey(keyLength)
            if(key not in hashedKeys):
                keysAndValues.append((key, len(keysAndValues)))
                hashedKeys[key] = True

        for i in range(0, self.keyCount):
            key = keysAndValues[i][0]

            self.database.deleteValueByKey(key)
            haveInDatabase = self.database.existsKey(key)
            self.assertEqual(False, haveInDatabase, f"{key} not appended in database but he found")

    def tearDown(self) -> None:
        self.database.close()
        removeFile(self.databaseFileName)

class TestOfDeletingValueOfLog2DatabaseWithBigCountKeys(TestOfDeletingValueOfLog2DatabaseWithSmallCountKeys):
    databaseFileName: str = "tstDbToDltValsWthBigCntKeys.logdb"
    keyCount: int = HUGE_KEY_COUNT

if __name__ == "__main__":
    unittest.main()