import ctypes

EncryptStr = ctypes.CDLL(".\\strconverter.dll")

EncryptStr.encrypt.restype = None
EncryptStr.encrypt.argtypes = [ctypes.POINTER(ctypes.c_char), ctypes.POINTER(ctypes.c_char)]

EncryptStr.decrypt.restype = None
EncryptStr.decrypt.argtypes = [ctypes.POINTER(ctypes.c_char), ctypes.POINTER(ctypes.c_char)]

def encrypt(source: ctypes.Array, password: ctypes.Array) -> None:
    EncryptStr.encrypt(source, password)

def decrypt(encryption: ctypes.Array, password: ctypes.Array) -> None:
    EncryptStr.decrypt(encryption, password)