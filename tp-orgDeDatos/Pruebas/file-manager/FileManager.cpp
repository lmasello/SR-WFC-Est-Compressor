#include "FileManager.h"
using namespace std;

void FileManager::processFile(char* filename, char buffer[]){
	FILE* fileIn;
	unsigned long lSize;
	unsigned int result;

	fileIn = fopen(filename, "rb");
	if (!fileIn){fputs ("File error",stderr); exit(1);}

	//obtain file size:
	fseek (fileIn , 0 , SEEK_END);
	lSize = ftell (fileIn);
	rewind (fileIn);

	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

	// copy the file into the buffer:
	result = fread (buffer,1,lSize,fileIn);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

	/* the whole file is now loaded in the memory buffer. */
	fclose (fileIn);

}

FILE* FileManager::createFileOut(char* filename){
	return NULL;
}

