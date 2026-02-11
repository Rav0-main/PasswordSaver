from random import randint
import strconverterlib
import ctypes
import unittest

ALPHABET: str = "qwertyuiopasdfghjklzxcvbnm1234567890!@#$%^&*()-=+_"
ALPHABET_LENGTH: int = len(ALPHABET)

def generateRandomSymbolicKey(length: int) -> str:
    result: str = ""
    for i in range(0, length):
        result = result + ALPHABET[randint(0, ALPHABET_LENGTH-1)]
    return result

def createBufferPointer(string: str):
    return ctypes.create_string_buffer(string.encode("utf-8"))

class TestOfStringConverting(unittest.TestCase):
    def test_StringConvertingWhenPasswordLengthLessWordLength(self):
        wordLength = randint(64, 128)
        passwordLength = randint(4, 16)

        wordCount = 256
        password = generateRandomSymbolicKey(passwordLength)
        words: list[str] = []
        hashedWord: dict[str, bool] = {}
        while(len(words) < wordCount):
            word = generateRandomSymbolicKey(wordLength)
            if(word not in hashedWord):
                hashedWord[word] = True
                words.append(word)

        passwordPointer = createBufferPointer(password)
        for word in words:
            wordPointer = createBufferPointer(word)

            strconverterlib.encrypt(wordPointer, passwordPointer)
            strconverterlib.decrypt(wordPointer, passwordPointer)

            self.assertEqual(word, wordPointer.value.decode("utf-8"),
                             f"Origin: {word}, but after convertings: {wordPointer.value.decode('utf-8')}")
            
    def test_StringConvertingWhenPasswordLengthBiggerWordLength(self):
        wordLength = randint(4, 16)
        passwordLength = randint(32, 128)

        wordCount = 256
        password = generateRandomSymbolicKey(passwordLength)
        words: list[str] = []
        hashedWord: dict[str, bool] = {}
        while(len(words) < wordCount):
            word = generateRandomSymbolicKey(wordLength)
            if(word not in hashedWord):
                hashedWord[word] = True
                words.append(word)

        passwordPointer = createBufferPointer(password)
        for word in words:
            wordPointer = createBufferPointer(word)

            strconverterlib.encrypt(wordPointer, passwordPointer)
            strconverterlib.decrypt(wordPointer, passwordPointer)

            self.assertEqual(word, wordPointer.value.decode("utf-8"),
                             f"Origin: {word}, but after convertings: {wordPointer.value.decode('utf-8')}")
            
    def test_StringConvertingWhenPasswordLengthEqualWordLength(self):
        wordLength = randint(32, 128)
        passwordLength = wordLength

        wordCount = 256
        password = generateRandomSymbolicKey(passwordLength)
        words: list[str] = []
        hashedWord: dict[str, bool] = {}
        while(len(words) < wordCount):
            word = generateRandomSymbolicKey(wordLength)
            if(word not in hashedWord):
                hashedWord[word] = True
                words.append(word)

        passwordPointer = createBufferPointer(password)
        for word in words:
            wordPointer = createBufferPointer(word)

            strconverterlib.encrypt(wordPointer, passwordPointer)
            strconverterlib.decrypt(wordPointer, passwordPointer)

            self.assertEqual(word, wordPointer.value.decode("utf-8"),
                             f"Origin: {word}, but after convertings: {wordPointer.value.decode('utf-8')}")
            
if __name__ == "__main__":
    unittest.main()