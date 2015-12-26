#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstddef>
#include <bitset>
#include <c++/fstream>


class FileManager {
    public:
        FileManager();

        ~FileManager();

        /*
        * Given a filename, the file is open and its content put in a buffer.
        * To take into account: buffer might exceed physical memory and its not contemplated.
        * Preconditions:
        *  -fileIn must be a valid file name
        *  Postconditions:
        *   -The method modifies the buffer array, addding the totality of the file to process to it. The
        *   data type of the buffer will change depending on the data typy of the file to load on memory. It
        *   will be char if it is to compress, or unsigned short if it is to decompress.
        */
        char *processFile(const char* filename,
                        size_t *size);

        /*
        * Given the file name to process, either to compress or decompress, the file is created and
        * the results are stores into it.
        * Postconditions:
        * 	-returns a FILE* poiting to the output file created.
        */
        void createFileOut(const char* filename,
                         char *output,
                         unsigned int lenght);
};

#endif /* FILEMANAGER_H_ */
