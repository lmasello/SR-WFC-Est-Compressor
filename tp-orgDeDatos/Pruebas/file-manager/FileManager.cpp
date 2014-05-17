/*
 * FileManager.cpp
 *
 *  Created on: 17 May 2014
 *      Author: lmasel
 */

#include "FileManager.h"

void FileManager::procesarFile(char filename[], char buffer[]){
	FILE* fileIn;
	unsigned int lSize;
	unsigned int result;

	fileIn = fopen(filename, "r");
	if (fileIn==NULL){fputs ("FIle error",stderr); exit(1);}

	//obtain file size:
	fseek (fileIn , 0 , SEEK_END);
	lSize = ftell (fileIn);
	rewind (fileIn));

	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

	// copy the file into the buffer:
	result = fread (buffer,1,lSize,fileIn);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

	/* the whole file is now loaded in the memory buffer. */
	fclose (fileIn);
}

FILE* FileManager::createFileOut(string filename){

}

