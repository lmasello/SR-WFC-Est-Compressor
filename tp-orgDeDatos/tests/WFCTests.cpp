#include "gtest.h"
#include "Algoritmos de compresion/WFC.h"

TEST(WFCTest, comprimir) {
	WFC wfc;
	ASSERT_EQ(65,wfc.comprimir('A'));
	ASSERT_EQ(0,wfc.comprimir('A'));
	ASSERT_EQ(66,wfc.comprimir('B'));
	ASSERT_EQ(1,wfc.comprimir('B'));
	ASSERT_EQ(1,wfc.comprimir('B'));
	ASSERT_EQ(0,wfc.comprimir('B'));
	ASSERT_EQ(67,wfc.comprimir('C'));
	ASSERT_EQ(2,wfc.comprimir('C'));
	ASSERT_EQ(2,wfc.comprimir('C'));
	ASSERT_EQ(1,wfc.comprimir('C'));
	ASSERT_EQ(1,wfc.comprimir('C'));
	ASSERT_EQ(0,wfc.comprimir('C'));
}
TEST(WFCTest, valorInicial){
	WFC wfc;
	ASSERT_EQ(68,'D');
}

TEST(WFCTest, incrementarFrecuencia) {
	WFC wfc;
	ASSERT_EQ(65,wfc.comprimir('A'));
	ASSERT_EQ(0,wfc.comprimir('A'));
	wfc.incrementarFrecuencia('B');
	wfc.incrementarFrecuencia('B');
	wfc.incrementarFrecuencia('B');
	ASSERT_EQ(0,wfc.comprimir('B'));
}
TEST(WFCTest, descomprimir) {
	WFC wfc;
	ASSERT_EQ('A',wfc.descomprimir(65));
	ASSERT_EQ('A',wfc.descomprimir(0));
	ASSERT_EQ('B',wfc.descomprimir(66));
	ASSERT_EQ('B',wfc.descomprimir(1));
	ASSERT_EQ('C',wfc.descomprimir(67));
	ASSERT_EQ('C',wfc.descomprimir(2));
	ASSERT_EQ('C',wfc.descomprimir(2));
	ASSERT_EQ('C',wfc.descomprimir(0));
}
