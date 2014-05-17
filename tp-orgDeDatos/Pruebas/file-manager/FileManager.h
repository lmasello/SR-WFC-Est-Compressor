/*
 * FileManager.h
 *
 *  Created on: 17 May 2014
 *      Author: lmasel
 */

#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

class FileManager {
	public:
		FileManager();
		void processFile(char fileIn[], char buffer[]);
		/* Dado un filename, lo abre y vuelca su contenido en el buffer.
		 * Precondiciones:
		 *  -fileIn debe ser un nombre de archivo valido, ubicado en el mismo directorio del
		 *   proyecto
		 *  -buffer todavia no contemplo la cantidad de memoria necesaria
		 *  Postcondiciones:
		 *  -El metodo modifica el array buffer, agregando el total del contenido del archivo a procesar
		 * */
};




#endif /* FILEMANAGER_H_ */
