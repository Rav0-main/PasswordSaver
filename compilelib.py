import os
from sys import argv

def getFilesInDirectory(directory: str):
    for item in os.listdir(directory):
        fullPath = os.path.join(directory, item)
        if os.path.isfile(fullPath):
            yield item

def checkExpansionOf(fileName: str, expansion: str) -> bool:
    fileNameLen = len(fileName)
    expansionLen = len(expansion)
    return fileName[fileNameLen-expansionLen-1:fileNameLen] == ("."+expansion)

def getTruthLibraryName(libraryName: str) -> str:
    if(checkExpansionOf(libraryName, "dll")):
        return libraryName
    
    return libraryName + ".dll"

if(len(argv) <= 1):
    print("Need input the library name!")

else:
    libraryName: str = getTruthLibraryName(argv[1])
    currentWorkDir: str = os.getcwd()
    strCFiles: str = ""
    strOFiles: str = ""
    for file in getFilesInDirectory(currentWorkDir):
        if(checkExpansionOf(file, "c")):
            length = len(file)
            strCFiles = strCFiles + f"{file} "
            strOFiles = strOFiles + f"{file[0:length-2]}.o "

    length = len(strCFiles)
    strCFiles = strCFiles[:length-1]

    length = len(strOFiles)
    strOFiles = strOFiles[:length-1]
    
    os.system(f"gcc -c -fPIC {strCFiles}")
    os.system(f"gcc -shared -o {libraryName} {strOFiles}")

print("Program is finished!")