import ctypes

class FILE(ctypes.Structure):
    pass

class KeySize(ctypes.c_int64):
    pass

class ValueSize(ctypes.c_int64):
    pass

FILE_POINTER = ctypes.POINTER(FILE)

class Log2DatabaseStruct(ctypes.Structure):
    _fields_ = [("stream", FILE_POINTER),
                ("keySize", KeySize),
                ("valueSize", ValueSize),
                ("isOpened", ctypes.c_bool)]

Log2DatabaseStructPointer = ctypes.POINTER(Log2DatabaseStruct)

Log2Database = ctypes.CDLL(".\\log2database.dll")

Log2Database.databaseOpen.restype = Log2DatabaseStruct
Log2Database.databaseOpen.argtypes = [ctypes.c_char_p, KeySize, ValueSize]

Log2Database.databaseAppendByKey.restype = ctypes.c_bool
Log2Database.databaseAppendByKey.argtypes = [Log2DatabaseStructPointer, ctypes.c_char_p, ctypes.c_void_p]

Log2Database.databaseChangeValueByKey.restype = ctypes.c_bool
Log2Database.databaseChangeValueByKey.argtypes = [Log2DatabaseStructPointer, ctypes.c_char_p, ctypes.c_void_p]

Log2Database.databaseExistsKey.restype = ctypes.c_bool
Log2Database.databaseExistsKey.argtypes = [Log2DatabaseStructPointer, ctypes.c_char_p]

Log2Database.databaseGetValueByKey.restype = ctypes.c_bool
Log2Database.databaseGetValueByKey.argtypes = [Log2DatabaseStructPointer, ctypes.c_char_p, ctypes.c_void_p]

Log2Database.databaseGetIndexByKey.restype = ctypes.c_int64
Log2Database.databaseGetIndexByKey.argtypes = [Log2DatabaseStructPointer, ctypes.c_char_p]

Log2Database.databaseGetKeyByIndex.restype = ctypes.c_char_p
Log2Database.databaseGetKeyByIndex.argtypes = [Log2DatabaseStructPointer, ctypes.c_long]

Log2Database.databaseGetIndexesOfKeysWhichStartWith.restype = ctypes.POINTER(ctypes.c_int64)
Log2Database.databaseGetIndexesOfKeysWhichStartWith.argtypes = [Log2DatabaseStructPointer, ctypes.c_char_p]

Log2Database.databaseGetCountOfRecords.restype = ctypes.c_int64
Log2Database.databaseGetCountOfRecords.argtypes = [Log2DatabaseStructPointer]

Log2Database.databaseDeleteValueByKey.restype = ctypes.c_bool
Log2Database.databaseDeleteValueByKey.argtypes = [Log2DatabaseStructPointer, ctypes.c_char_p]

Log2Database.databaseClose.restype = ctypes.c_bool
Log2Database.databaseClose.argtypes = [Log2DatabaseStructPointer]

class Log2DatabaseImpl:
    def __init__(self, filePath: str, keySize: int, valueSize: int):
        """
        keySize and valueSize in bytes
        """
        self.__databaseObj = Log2Database.databaseOpen(filePath.encode("utf-8"), keySize, valueSize)
        self.__databasePtr = ctypes.byref(self.__databaseObj)
    
    def appendByKey(self, key: str, value: ctypes._Pointer) -> bool:
        return Log2Database.databaseAppendByKey(self.__databasePtr, key.encode("utf-8"), value)
    
    def getValueByKey(self, key: str, buffer: ctypes._Pointer) -> bool:
        return Log2Database.databaseGetValueByKey(self.__databasePtr, key.encode("utf-8"), buffer)
    
    def getIndexByKey(self, key: str) -> int:
        return Log2Database.databaseGetIndexByKey(self.__databasePtr, key.encode("utf-8"))
    
    def getKeyByIndex(self, index: ctypes.c_int64) -> str:
        return Log2Database.databaseGetKeyByIndex(self.__databasePtr, index).decode("utf-8")
    
    def getIndexesOfKeysWhichStartWith(self, startKey: str):
        return Log2Database.databaseGetIndexesOfKeysWhichStartWith(self.__databasePtr, startKey.encode("utf-8"))

    def changeValueByKey(self, key: str, newValue: ctypes._Pointer) -> bool:
        return Log2Database.databaseChangeValueByKey(self.__databasePtr, key.encode("utf-8"), newValue)
    
    def deleteValueByKey(self, key: str) -> bool:
        return Log2Database.databaseDeleteValueByKey(self.__databasePtr, key.encode("utf-8"))
    
    def existsKey(self, key: str) -> bool:
        return Log2Database.databaseExistsKey(self.__databasePtr, key.encode("utf-8"))
    
    def getCountOfRecords(self) -> int:
        return Log2Database.databaseGetCountOfRecords(self.__databasePtr)
    
    def close(self):
        Log2Database.databaseClose(self.__databasePtr)
