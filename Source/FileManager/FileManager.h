
#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstddef>
#include <bitset>

using namespace std;

template<typename dato>
class FileManager
{
    public:
        FileManager()
        {
        }

        ~FileManager()
        {
        }

        /*
         *  Dado un filename, lo abre y vuelca su contenido en el buffer.
         * Precondiciones:
         *  -fileIn debe ser un nombre de archivo valido, ubicado en el mismo directorio del
         *   proyecto
         *  -buffer todavia no contemplo la cantidad de memoria necesaria
         *  Postcondiciones:
         *  -El metodo modifica el array buffer, agregando el total del contenido del archivo a procesar.
         *   el tipo de dato del buffer va a depender del tipo de datos que contenga el file a cargar en memoria.
         *   char si se trata del compresor o unsigned short si se trata del descompresor
         */
        char * processFile(const char * filename,
                           size_t *     size)
        {
            FILE *       fileIn;
            unsigned int lSize;
            unsigned int result;
            char *       buffer;

            fileIn = fopen(filename, "rb");

            if (!fileIn)
            {
                fputs("FileIn error", stderr);
                exit(1);
            }

            // Obtenemos el tamaño del archivo:
            fseek(fileIn, 0, SEEK_END);

            lSize = ftell(fileIn);

            rewind(fileIn);

            buffer = new char[lSize];

            if (buffer == NULL)
            {
                fputs("Memory error", stderr);
                exit(2);
            }

            result = fread(buffer, 1, lSize, fileIn);

            if (result != lSize)
            {
                fputs("Reading error", stderr);
                exit(3);
            }

            fclose(fileIn);

            (*size) = lSize;

            return buffer;
        }

        /*
         * Dado el nombre del archivo a procesar, ya sea a comprimir o descomprimir, se crea el archivo
         * y se guardan los resultados.
         * Postcondiciones:
         *  -devuelve un FILE* apuntando al archivo de salida creado
         */
        void createFileOut(const char * filename,
                           char *       salida,
                           unsigned int lenght)
        {
            FILE * fileOut = fopen(filename, "wb");

            if (!fileOut)
            {
                fputs("FileOut error", stderr);
                exit(1);
            }

            fwrite(salida, sizeof(char), lenght, fileOut);
            fclose(fileOut);

            return;
        }
};
#endif /* FILEMANAGER_H_ */
