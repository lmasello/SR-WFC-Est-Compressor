#include "gtest.h"
#include "Algoritmos de compresion/SymbolRanking.h"

namespace{
	class SymbolRankingTest : public ::testing::Test {
	protected:
		unsigned int size = 24;
		short *salidaCompresor = (short*) malloc (sizeof(short) * size);
		char *salidaDescompresor = (char*) malloc (sizeof(char) * size);
	};

	TEST_F(SymbolRankingTest,comprimirMaxCtx1){
		SymbolRanking sr(1);
		char* aComprimir = "abcdacdabcddbd";
		sr.comprimir(aComprimir,salidaCompresor,14);
		short expected[]={97,98,99,100,0,3,0,0,1,0,0,3,5,1};

		for(unsigned int i=0;i<14;i++){
			ASSERT_EQ(expected[i],salidaCompresor[i]);
		}
	}

	TEST_F(SymbolRankingTest,comprimirMaxCtx3){
		SymbolRanking sr(3);
		char* aComprimir = "abcdeabdeaeaneabdeabdeab";
		sr.comprimir(aComprimir,salidaCompresor,size);
		short expected[]={97,98,99,100,101,0,0,4,0,0,4,0,112,1,0,2,0,0,0,1,0,0,0,0};

		for(unsigned int i=0;i<24;i++){
			ASSERT_EQ(expected[i],salidaCompresor[i]);
		}
	}

	TEST_F(SymbolRankingTest,comprimirMaxCtx4){
		SymbolRanking sr(4);
		char* aComprimir = "abcdeabcdabcdebcdacdeabc";
		sr.comprimir(aComprimir,salidaCompresor,size);
		short expected[]={97,98,99,100,101,0,0,0,0,1,0,0,0,1,2,0,0,1,2,0,1,1,0,0};

		for(unsigned int i=0;i<24;i++){
			ASSERT_EQ(expected[i],salidaCompresor[i]);
		}
	}

	TEST_F(SymbolRankingTest,descomprimirMaxCtx1){
		SymbolRanking sr(1);
		unsigned short aDescomprimir[]={97,98,99,100,0,3,0,0,1,0,0,3,5,1};
		sr.descomprimir(aDescomprimir,salidaDescompresor,14);

		char expected[]="abcdacdabcddbd";

		for(unsigned int i=0;i<14;i++){
			ASSERT_EQ(expected[i],salidaDescompresor[i]);
		}
	}


	TEST_F(SymbolRankingTest,descomprimirMaxCtx3){
		SymbolRanking sr(3);
		unsigned short aDescomprimir[]={97,98,99,100,101,0,0,4,0,0,4,0,112,1,0,2,0,0,0,1,0,0,0,0};
		sr.descomprimir(aDescomprimir,salidaDescompresor,size);

		char expected[]="abcdeabdeaeaneabdeabdeab";

		for(unsigned int i=0;i<24;i++){
			ASSERT_EQ(expected[i],salidaDescompresor[i]);
		}
	}

	TEST_F(SymbolRankingTest,descomprimirMaxCtx4){
		SymbolRanking sr(4);
		unsigned short aDescomprimir[]={97,98,99,100,101,0,0,0,0,1,0,0,0,1,2,0,0,1,2,0,1,1,0,0};
		sr.descomprimir(aDescomprimir,salidaDescompresor,size);

		char expected[]="abcdeabcdabcdebcdacdeabc";

		for(unsigned int i=0;i<24;i++){
			ASSERT_EQ(expected[i],salidaDescompresor[i]);
		}
	}

}
