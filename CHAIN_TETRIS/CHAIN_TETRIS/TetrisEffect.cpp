#include "Main.h"
#include "TetriminoState.h"
#include "Render.h"

int FieldEffectBuff[FIELD_HEIGHT][FIELD_WIDTH];
void chain(int field[FIELD_HEIGHT][FIELD_WIDTH], int chainHeight, int BuffChainCheck[10], bool HorizontalCheck, int ChainCheckCnt, int BuffChainbuffer[10]);
void EffectControl(int field[FIELD_HEIGHT][FIELD_WIDTH]) {
	
	//for (int Hcopy = 0; Hcopy < FIELD_HEIGHT; Hcopy++) {
	//	for (int Wcopy = 0; Wcopy < FIELD_WIDTH; Wcopy++) {
	//		FieldEffectBuff[Hcopy][Wcopy] = field[Hcopy][Wcopy];
	//	}
	//}

	/*static bool Accorded;*/
	static int height = 0;
	static int width = 0;
	static bool clear[10];
	int clearCnt;
	bool canupChain = false;


	for (height = FIELD_HEIGHT - 2; height >= 1; height--) {
		for (width = FIELD_WIDTH - 2; width > 0; width--) {
			if (field[height][width] == 0 ||
				field[height][width] == 6 ||
				field[height][width] == 8) {
				clear[width - 1] = false;
			}
			else {
				clear[width - 1] = true;

			}
		}

		if (clear[0] == true &&
			clear[1] == true &&
			clear[2] == true &&
			clear[3] == true &&
			clear[4] == true &&
			clear[5] == true &&
			clear[6] == true &&
			clear[7] == true &&
			clear[8] == true &&
			clear[9] == true) {

			for (width = 10; width >= 1; width--) {
				field[height][width] = 6;
			}
			
		}
	}
}

void ChainEffect(int field[FIELD_HEIGHT][FIELD_WIDTH]/*int chainHeight*/,int BuffChainCheck[10]) {
	
	static int height = 0;
	static int width = 0;
	static bool clear[10];
	static bool HorizontalCheck = false;
	int clearCnt;
	//int BuffChainCheck[10];
	int BuffChainbuffer[10];
	int BuffChains;

	static int chainHeight = 0;
	static int ClearLineCOUNTER = 0;
	for (height = FIELD_HEIGHT - 2; height >= 1; height--) {
		for (width = FIELD_WIDTH - 2; width > 0; width--) {
			if (field[height][width] == 0 ||
				field[height][width] == 8) {
				clear[width - 1] = false;
			}
			if(field[height][width] == 6 ) {
				clear[width - 1] = true;
			}
			else {
				clear[width - 1] = false;
			}
		}

		if (clear[0] == true &&
			clear[1] == true &&
			clear[2] == true &&
			clear[3] == true &&
			clear[4] == true &&
			clear[5] == true &&
			clear[6] == true &&
			clear[7] == true &&
			clear[8] == true &&
			clear[9] == true) {


			for (width = 10; width >= 1; width--) {
				field[height][width] = 6;
			}
			ClearLineCOUNTER++;
			chainHeight = height - 1;
			
		}
	}

	ClearLineCOUNTER = 0;
	for (int ChainCheckCnt = 10; ChainCheckCnt > 0; ChainCheckCnt--) {

		if ((field[chainHeight][ChainCheckCnt] - BuffChainCheck[ChainCheckCnt - 1] == 1
			|| field[chainHeight][ChainCheckCnt] - BuffChainCheck[ChainCheckCnt - 1] == -3) && field[chainHeight][ChainCheckCnt] != 8) {
			if (!BuffChainCheck[ChainCheckCnt - 1]) {
				continue;
			}

			//上を見る用のバッファに消す色情報入力
			BuffChainbuffer[ChainCheckCnt - 1] = field[chainHeight][ChainCheckCnt];
				field[chainHeight][ChainCheckCnt] = 6;
		}
		
		chain(field, chainHeight, BuffChainCheck, HorizontalCheck, ChainCheckCnt, BuffChainbuffer);
	}
}

void chain(int field[FIELD_HEIGHT][FIELD_WIDTH],int chainHeight, int BuffChainCheck[10], bool HorizontalCheck, int ChainCheckCnt, int BuffChainbuffer[10]) {
	int BuffChainhorizontalbuff; 
	for (int UpperCheck = chainHeight - 1; UpperCheck > 0; UpperCheck--) {
		if ((field[UpperCheck][ChainCheckCnt] - BuffChainbuffer[ChainCheckCnt - 1] == 1
			|| field[UpperCheck][ChainCheckCnt] - BuffChainbuffer[ChainCheckCnt - 1] == -3
			|| field[UpperCheck][ChainCheckCnt] == BuffChainbuffer[ChainCheckCnt - 1])  /*&& !field[UpperCheck][ChainCheckCnt]*/) {

			if (!BuffChainbuffer[ChainCheckCnt - 1]) {
				continue;
			}
			if ((field[UpperCheck][ChainCheckCnt] - BuffChainbuffer[ChainCheckCnt - 1] == 1
				|| field[UpperCheck][ChainCheckCnt] - BuffChainbuffer[ChainCheckCnt - 1] == -3
				|| field[UpperCheck][ChainCheckCnt] == BuffChainbuffer[ChainCheckCnt - 1]) && (chainHeight - UpperCheck> 2)) {

				BuffChainbuffer[ChainCheckCnt - 1] = field[UpperCheck][ChainCheckCnt];
				field[UpperCheck][ChainCheckCnt] = 6;
				HorizontalCheck = true;

			}
			if ((field[UpperCheck][ChainCheckCnt] - BuffChainbuffer[ChainCheckCnt - 1] == 1
				|| field[UpperCheck][ChainCheckCnt] - BuffChainbuffer[ChainCheckCnt - 1] == -3)) {

				BuffChainbuffer[ChainCheckCnt - 1] = field[UpperCheck][ChainCheckCnt];
				field[UpperCheck][ChainCheckCnt] = 6;
			}


			//横の確認
			if (HorizontalCheck) {

				for (int checkHorizontally = ChainCheckCnt; checkHorizontally > FIELD_WIDTH - 2; checkHorizontally++) {
					if (TetrisField[chainHeight][ChainCheckCnt] == TetrisField[chainHeight][checkHorizontally]) {
						for (int checkup = 1; checkup == chainHeight; checkup++) {
							BuffChainhorizontalbuff = field[chainHeight - checkup + 1][checkHorizontally];
							field[chainHeight - checkup + 1][checkHorizontally] = 6;
						}
					}
				}

				for (int checkHorizontally = ChainCheckCnt; checkHorizontally < 1; checkHorizontally--) {
					if (field[chainHeight][ChainCheckCnt] == field[chainHeight][checkHorizontally]) {
						for (int checkup = 1; checkup == chainHeight; checkup++) {
							BuffChainhorizontalbuff = field[chainHeight - checkup + 1][checkHorizontally];
							field[chainHeight - checkup + 1][checkHorizontally] = 6;
						}
					}
				}


			}
		}

	}
}