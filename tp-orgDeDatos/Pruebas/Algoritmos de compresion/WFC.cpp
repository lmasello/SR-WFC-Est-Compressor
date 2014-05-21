/*
 * WFC.cpp
 *
 *  Created on: May 21, 2014
 *      Author: leandro
 */
#include "WFC.h"

WFC::WFC(){
	for (unsigned short i=0;i<256;i++)
		weithedList.push_back((char)i);

	//inicializar en uno el map de frecuencias. Implementar!
}

unsigned short WFC::comprimir(char charToTransform){
	unsigned short index = getIndex(charToTransform);
	incrementarFrecuencia(charToTransform);
	updateWeightedList();
	return index;
}

//Se tomo como ref: stackoverflow.com/questions/4604136/how-to-search-for-an-element-in-an-stl-list
unsigned short WFC::getIndex(char charTobeIndexed){
	auto it = std::find(weithedList.begin(), weithedList.end(), charToTransform);
	if (it == weithedList.end()) throw notInList;
	else{
		const short index = distance(weithedList.begin(),it);
		return index;
	}
}

//iMPLEMENTAR
void WFC::incrementarFrecuencia(char charToTransform){

}

void WFC::updateWeightedList(){

}

