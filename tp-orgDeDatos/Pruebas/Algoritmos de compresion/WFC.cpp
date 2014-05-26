/*
 * WFC.cpp
 *
 *  Created on: May 21, 2014
 *      Author: leandro
 */
#include "WFC.h"

WFC::WFC(){
	for (unsigned short i=0;i<256;i++){
		weightedList[i]=((char)i);
		charFrequencies[(char)i]= (unsigned short) 1; //Iniciailza las frecuencias en uno
	}
}

unsigned short WFC::comprimir(char charToTransform){
	unsigned short index = getIndex(charToTransform);
	incrementarFrecuencia(charToTransform);
	updateWeightedList(index);
	return index;
}


unsigned short WFC::getIndex(char charToTransform){
	unsigned short index;
	unsigned short size=256;
    for (unsigned short i = 0; i < size; i++){
        if (weightedList[i] == charToTransform){
            index = i;
        	break;
        }
    }
	return index;
}

void WFC::incrementarFrecuencia(char charToTransform){
	charFrequencies[charToTransform]+=1;
}

void WFC::updateWeightedList(unsigned short indexDelCharActualizado){
	if(indexDelCharActualizado>=256)throw IndexError();
	while (indexDelCharActualizado > 0){
		bool pesoMayor = elPesoDeLaPosicionEsMayor(indexDelCharActualizado,indexDelCharActualizado-1);
		if(pesoMayor){
			char temp = weightedList[indexDelCharActualizado-1];
			weightedList[indexDelCharActualizado-1]=weightedList[indexDelCharActualizado];
			weightedList[indexDelCharActualizado]=temp;
		}
		else{
			break; //Si el peso es < o = no hay que realizar intercambios.
		}
		indexDelCharActualizado--;
	}
}

bool WFC::elPesoDeLaPosicionEsMayor(unsigned short pos1, unsigned short pos2){
	unsigned short pesoDelCharActualizado = charFrequencies[weightedList[pos1]];
	unsigned short pesoDelCharAnterior = charFrequencies[weightedList[pos2]];

	if(pesoDelCharActualizado>pesoDelCharAnterior) return true;
	return false;
}

