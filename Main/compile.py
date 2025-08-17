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

if(len(argv) <= 1):
    print("Need input the program name!")

else:
    programName: str = argv[1]
    if(not checkExpansionOf(programName, "exe")):
        programName = programName + ".exe"

    libraryIncludes: str = ""
    for i in range(2, len(argv), 2):
        libraryIncludes = libraryIncludes + f"-L{argv[i]} -l{argv[i+1]} "

    currentWorkDir: str = os.getcwd()
    strCFiles: str = ""
    for file in getFilesInDirectory(currentWorkDir):
        if(checkExpansionOf(file, "c")):
            length = len(file)
            strCFiles = strCFiles + f"{file} "

    length = len(strCFiles)
    strCFiles = strCFiles[:length-1]

    os.system(f"gcc {strCFiles} icon.o -o {programName} {libraryIncludes}")

print("Program is finished!")