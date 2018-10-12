#include "Main.h"
#include "TetriminoState.h"
#include "Render.h"

int FieldEffectBuff[FIELD_HEIGHT][FIELD_WIDTH];
void chain(int field[FIELD_HEIGHT][FIELD_WIDTH], int chainHeight, int BuffChainCheck[10], int ChainCheckCnt, int BuffChainbuffer[10]);

void EffectControl(int field[FIELD_HEIGHT][FIELD_WIDTH]) {
	
	static int width = 0;
	static bool clear[10];
	bool canupChain = false;
	for (int height = FIELD_HEIGHT - 2; height >= 1; height--) {
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
		if (clear[0] == true &&	clear[1] == true &&	clear[2] == true &&	clear[3] == true &&	
			clear[4] == true &&	clear[5] == true &&	clear[6] == true &&	clear[7] == true &&
			clear[8] == true &&	clear[9] == true) {
			for (width = 10; width >= 1; width--) {
				field[height][width] = 6;
			}			
		}
	}
}

void ChainEffect(int field[FIELD_HEIGHT][FIELD_WIDTH],int BuffChainCheck[10]) {
	
	static int width;
	bool clear[10];
	
	int BuffChainbuffer[10];
	static int chainHeight = 0;
	for (int height = FIELD_HEIGHT - 2; height >= 1; height--) {
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
		if (clear[0] == true &&	clear[1] == true &&	clear[2] == true &&	clear[3] == true &&	clear[4] == true &&
			clear[5] == true &&	clear[6] == true &&	clear[7] == true &&	clear[8] == true &&	clear[9] == true) {
			for (width = 10; width >= 1; width--) {
				field[height][width] = 6;
			}
			chainHeight = height - 1;			
		}
	}
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
		chain(field, chainHeight, BuffChainCheck,ChainCheckCnt, BuffChainbuffer);
	}
}

void chain(int field[FIELD_HEIGHT][FIELD_WIDTH],int chainHeight, int BuffChainCheck[10], int ChainCheckCnt, int BuffChainbuffer[10]) {
	for (int UpperCheck = chainHeight - 1; UpperCheck > 0; UpperCheck--) {
		if ((field[UpperCheck][ChainCheckCnt] - BuffChainbuffer[ChainCheckCnt - 1] == 1	|| field[UpperCheck][ChainCheckCnt] - BuffChainbuffer[ChainCheckCnt - 1] == -3
			|| field[UpperCheck][ChainCheckCnt] == BuffChainbuffer[ChainCheckCnt - 1])) {
			bool HorizontalCheck = false;
			if (!BuffChainbuffer[ChainCheckCnt - 1]) {
				continue;
			}
			if( (chainHeight - UpperCheck) <= 2 && (field[UpperCheck][ChainCheckCnt] != BuffChainbuffer[ChainCheckCnt - 1])) {
				BuffChainbuffer[ChainCheckCnt - 1] = field[UpperCheck][ChainCheckCnt];
				field[UpperCheck][ChainCheckCnt] = 6;
			}
			//消去列から2段を超えたら同色を消していく
			else if ( ((chainHeight - UpperCheck) > 2) && (field[UpperCheck-1][ChainCheckCnt] == 6) ) {
				BuffChainbuffer[ChainCheckCnt - 1] = field[UpperCheck][ChainCheckCnt];
				field[UpperCheck][ChainCheckCnt] = 6;
				HorizontalCheck = true;
			}
			//横の確認
			if (HorizontalCheck) {
				int BuffChainhorizontalbuff; 
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