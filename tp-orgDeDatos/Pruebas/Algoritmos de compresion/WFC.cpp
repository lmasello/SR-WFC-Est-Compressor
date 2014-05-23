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
	updateWeightedList();	//Evaluar la opcion de hacer que se actualice cada N transformaciones
	return index;
}

//Referencia: http://stackoverflow.com/questions/7875581/c-get-index-of-char-element-in-array
unsigned short WFC::getIndex(char charToTransform){
	unsigned short index;
	unsigned short size=256;
    for (unsigned short i = 0; i < size; i++){
        if (weightedList[i] == charToTransform){
            index = i;
        	break;
        }
    }

	cout<<"En la lista de WFC, el char: "<<charToTransform<<" esta en la pos: "<<index<<"\n";

	return index;
}

void WFC::incrementarFrecuencia(char charToTransform){
	charFrequencies[charToTransform]+=1;
}

void WFC::updateWeightedList(){
	quickSort(0,255);

	cout<<"La lista se actualiza a: \n";
	for(int i=0;i<256;i++)
		cout<<weightedList[i]<<"\b";

}

//Referencia: http://www.algolist.net/Algorithms/Sorting/Quicksort
void WFC::quickSort(int left, int right){
	  int i = left, j = right;
	  int tmp;
	  char pivot = weightedList[(left + right) / 2];

	  /* partition */
	  while (i <= j) {
		  	char pesoDelCaracterIzq = charFrequencies[weightedList[i]];
		  	char pesoDelCaracterDer = charFrequencies[weightedList[j]];
		  	char pesoDelPivot = charFrequencies[pivot];

			while (pesoDelCaracterIzq > pesoDelPivot){
				i++;
				pesoDelCaracterIzq = charFrequencies[weightedList[i]];
			}
			while (pesoDelCaracterDer < pesoDelPivot){
				j--;
				pesoDelCaracterDer = charFrequencies[weightedList[j]];
			}
			if (i <= j) {
				  tmp = weightedList[i];
				  weightedList[i] = weightedList[j];
				  weightedList[j] = tmp;
				  i++;
				  j--;
			}
	  };
	  /* recursion */
	  if (left < j)
			quickSort(left, j);
	  if (i < right)
			quickSort(i, right);
}
