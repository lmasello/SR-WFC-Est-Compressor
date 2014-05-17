#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>

class FileManager {
	private:
		char vector[];
	public:
		FileManager();
		FileManager(char vector[]);
		void procesarFile(FILE* file, char vector[]);
};

#endif
