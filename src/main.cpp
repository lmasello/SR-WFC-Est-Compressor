#include <iostream>
#include <iomanip>
#include "../headers/FileManager/FileManager.h"
#include "../headers/Compressor/Compressor.h"

using namespace std;

/*
 * Prints out the usage of the program.
 */
void help() {
    cout << "Parameters: executable c/d FileIn" << endl;

    cerr << "Parameter error" << endl;
    exit(1);
}

int main(int argc, char *argv[]) {
    // argv variables --> compress/decompress, input file
    if (argc != 3) {
        help();
    }

    string operationToDo = argv[1];
    string fileIn = argv[2];
    string fileOut = fileIn;

    // Get start time
    clock_t start = clock();
    FileManager fileManager;
    size_t size;
    char *buffer = fileManager.processFile(fileIn.c_str(), &size);

    cout << "The file has been opened and put to memory correctly" << endl;

    pair<char *, unsigned int> pairOut;
    Compressor *compressor = new Compressor();

    if (operationToDo.compare("c") == 0) {
        pairOut = compressor->compress(buffer, size);

        fileOut += ".sr";

        /*
         * After the compression the fileManager is called in order to
         * save the results in the output file.
         */
        fileManager.createFileOut(fileOut.c_str(), pairOut.first, pairOut.second);

        cout << "The compression process has ended successfully." << endl;
    }
    else if (operationToDo.compare("d") == 0) {
        pairOut = compressor->decompress(buffer, size);

        fileOut.erase(fileOut.end() - 3, fileOut.end());

        /*
         * After the decompression the fileManager is called in order to
         * save the results in the output file.
         */
        fileManager.createFileOut(fileOut.c_str(), pairOut.first, pairOut.second);

        cout << "The decompression process has ended successfully." << endl;
    }
    else {
        help();
    }

    // Report result
    cout << endl << fileIn << " to " << fileOut << " in ";
    printf("%1.2f", double(clock() - start) / CLOCKS_PER_SEC);

    delete[] buffer;
    delete[] pairOut.first;
    delete compressor;

    return 0;
}

