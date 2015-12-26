#include <iostream>

#include "../../headers/FileManager/FileManager.h"

using namespace std;

FileManager::FileManager() {
}

FileManager::~FileManager() {
}

char * FileManager::processFile(const char* filename,
                  size_t *size) {
    FILE *fileIn;
    unsigned int lSize;
    unsigned int result;
    char *buffer;

    fileIn = fopen(filename, "rb");

    if (!fileIn) {
        fputs("FileIn error", stderr);
        exit(1);
    }

    // The file size if obtained:
    fseek(fileIn, 0, SEEK_END);

    lSize = ftell(fileIn);

    rewind(fileIn);

    buffer = new char[lSize];

    result = fread(buffer, 1, lSize, fileIn);

    if (result != lSize) {
        fputs("Reading error", stderr);
        exit(3);
    }

    fclose(fileIn);

    (*size) = lSize;

    return buffer;
}

void FileManager::createFileOut(const char* filename,
                   char *output,
                   unsigned int lenght) {

    FILE *fileOut = fopen(filename, "wb");

    if (!fileOut) {
        fputs("FileOut error", stderr);
        exit(1);
    }

    fwrite(output, sizeof(char), lenght, fileOut);
    fclose(fileOut);

    return;
}

