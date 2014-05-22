/*
 * WFC.cpp
 *
 *  Created on: May 21, 2014
 *      Author: leandro
 */
#include "WFC.h"

WFC::WFC(){
	for (unsigned short i=0;i<256;i++){
		weithedList.push_back((char)i);
		charFrequencies[(char)i]= (unsigned short) 1; //Iniciailza las frecuencias en uno
	}
}

unsigned short WFC::comprimir(char charToTransform){
	unsigned short index = getIndex(charToTransform);
	incrementarFrecuencia(charToTransform);
	updateWeightedList();
	return index;
}

//Se tomo como ref: stackoverflow.com/questions/4604136/how-to-search-for-an-element-in-an-stl-list
unsigned short WFC::getIndex(unsigned long charPos){ //Revice la pos, no el char.
	auto it = find(weithedList.begin(), weithedList.end(), charPos);
	if (it == weithedList.end()) throw notInListError;
	else{
		const short index = distance(weithedList.begin(),it);
		return index;
	}
}

void WFC::incrementarFrecuencia(char charToTransform){
	charFrequencies[charToTransform]+=1;
}

void WFC::updateWeightedList(){

}

