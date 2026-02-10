import ctypes

ConvertStr = ctypes.CDLL(".\\strconverter.dll")

ConvertStr.convert.restype = None
ConvertStr.convert.argtypes = [ctypes.POINTER(ctypes.c_char), ctypes.POINTER(ctypes.c_char), ctypes.c_int]


def encrypt(source: ctypes.Array, password: ctypes.Array) -> None:
    ConvertStr.convert(source, password, 1)

def decrypt(encryption: ctypes.Array, password: ctypes.Array) -> None:
    ConvertStr.convert(encryption, password, -1)