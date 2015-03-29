
#include <iostream>
#include <iomanip>
#include "FileManager/FileManager.h"
#include "Compressor/Compressor.h"
#include <string.h>

using namespace std;

/*
 * Prints out the usage of the program.
 */
void help()
{
    cout << "Parameters: executable c/d FileIn FileOut" << endl;

    fputs("Parameter error\n", stderr);
    exit(1);
}

int main(int    argc,
         char * argv[])
{
    // argv variables --> compress/decompress, input file, output file
    if (argc != 4)
    {
        help();
    }

    const char * operationToDo = argv[1];
    const char * fileIn        = argv[2];
    const char * fileOut       = argv[3];

    // Get start time
    clock_t           start = clock();
    FileManager<char> fileManager;
    size_t            size;
    char *            buffer = fileManager.processFile(fileIn, &size);

    cout << "The file has been opened and put to memory correctly" << endl;

    pair<char *, unsigned int> pairOut;
    Compressor *                compressor = new Compressor();

    if (strcmp(operationToDo, "c") == 0)
    {
        pairOut = compressor -> compress(buffer, size);

        /*
         * After the compression the fileManager is called in order to
         * save the results in the output file.
         */
        fileManager.createFileOut(fileOut, pairOut.first, pairOut.second);

        cout << "The compression process has ended succesfully." << endl;
    }
    else if (strcmp(operationToDo, "d") == 0)
    {
        pairOut = compressor -> decompress(buffer, size);

        /*
         * After the decompression the fileManager is called in order to
         * save the results in the output file.
         */
        fileManager.createFileOut(fileOut, pairOut.first, pairOut.second);

        cout << "The decompression process has ended succesfully." << endl;
    }
    else
    {
        help();
    }

    // Report result
    printf("\n%s to %s in %1.2f sec.\n", argv[2], fileOut, double(clock() - start) / CLOCKS_PER_SEC);

    delete[] buffer;
    delete[] pairOut.first;
    delete compressor;

    return 0;
}

