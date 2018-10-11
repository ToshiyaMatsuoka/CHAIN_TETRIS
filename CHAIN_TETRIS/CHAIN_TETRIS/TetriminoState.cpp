#include "TetriminoState.h"
#include "TetrisEffect.h"

#define EFFECTTIME 45
//キー入力制限
#define CREATE_INTERVAL 4
#define KEY_RESTAIN 5

static int CurrentTetrimino, TetriminoNext[3], HoldTetrimino;
static int moveY;
static int moveX;
static int height = 0;
static int width = 0;
int TetrisField[FIELD_HEIGHT][FIELD_WIDTH];
int ClearBoard[FIELD_HEIGHT][FIELD_WIDTH];
int NextBoard[4][6];
int NextBoard2[4][6];
int NextBoard3[4][6];
int HoldBoard[4][6];
int ScoreBoard[4][6];
int ClearLineCOUNTER = 0;
int g_ScorePoint = 0;
int g_RiseCOUNTER = 0;
bool EffectOn = false;
bool ChainEffectOn = false;
bool GameScene = false;
bool GameOver = false;
bool GAMEOPEN = true;
TETRIMINO MovingTetrimino;
TETRIMINO tetrimino[8];

void GameOverControl(bool* canHold, bool* isCreated, bool* GameOver, bool* GameScene, bool* GAMEOPEN);
void ScoreCalculation(int* RisenCount, bool* isCreated, bool* CanCopy, bool* SetDown, bool* canClear, bool* OnlyOnceHold);
void ChainProcessing(int* chainHeight, int ChainCheck[], int* RisenCount, bool* ChainEffectOn);
void tetriminoSetDown(bool* SetDown, bool* isCreated, bool* OnlyOnceHold,int* EffectRoop);
void RightMove(int* CountMoveTime, TETRIMINO* tetrimino);
void LeftMove(int* CountMoveTime, TETRIMINO* tetrimino);
void DownMove(int* CountMoveTime, TETRIMINO* tetrimino, bool canMoveDown[], bool* SetDown);
void DropMove(int* CountMoveTime, TETRIMINO* tetrimino, bool canMoveDown[], bool* SetDown);
void RotetionLeftMove(int* CountMoveTime, bool canRotate[], TETRIMINO* RotedBuffer, TETRIMINO* tetrimino);
void RotetionRightMove(int* CountMoveTime, bool canRotate[], TETRIMINO* RotedBuffer, TETRIMINO* tetrimino);
void HoldingTetrimino(int* CountMoveTime, bool* OnlyOnceHold, bool* canHold, TETRIMINO* tetrimino, TETRIMINO* RotedBuffer,TETRIMINO* MovingTetrimino);
void ChoseTetrimino(int* CountMoveTime, TETRIMINO* tetrimino, TETRIMINO* RotedBuffer, bool* isCreated, int* BlockColorSelector, bool canMoveDown[]);
void SetdownCheck(TETRIMINO* tetrimino, bool canMoveDown[], bool* SetDown);
void FalingTetrimino(TETRIMINO* tetrimino, bool canMoveDown[], int* countFalling);
void ReflectTetrimino(TETRIMINO* tetrimino, int moveYtoCentral, int moveXtoCentral);

////////////////////////////////////////
void CreateField(void) {

	for (height = 0; height < FIELD_HEIGHT; height++) {
		for (width = 0; width < FIELD_WIDTH; width++) {
			if (width == 0 || width == FIELD_WIDTH - 1 || height == FIELD_HEIGHT - 1) {
				TetrisField[height][width] = 8;
			}
			else {
				TetrisField[height][width] = 0;
			}
		}
	}
}
void CreateBoard() {
	for (height = 0; height < 4; height++) {
		for (width = 0; width < 6; width++) {
			NextBoard[height][width] = 0;
			NextBoard2[height][width] = 0;
			NextBoard3[height][width] = 0;
			HoldBoard[height][width] = 0;
			ScoreBoard[height][width] = 0;
		}
	}
}
void SelectTetrimino(void) {
	//初めに出てくるテトリス、1つ次、２つ次、3つ次のテトリミノを決める
	srand((unsigned int)time(NULL));
	CurrentTetrimino = rand() % 7 ;
	TetriminoNext[0] = rand() % 7 ;
	TetriminoNext[1] = rand() % 7 ;
	TetriminoNext[2] = rand() % 7 ;
	HoldTetrimino =  7;
}
void ControlTetris(void) {
	static int EffectRoop = 0, CountMoveTime = 0, moveYtoCentral = 1, moveXtoCentral = 2, BlockColorSelector = 0;
	static bool isCreated = true, SetDown = false, canHold = false, OnlyOnceHold = true;
	static bool canMoveDown[4] = { false,false,false,false };
	static bool canRotate[16] = { false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false };

	TETRIMINO tetrimino[8]{
	{ -1,0,  0,0,  1,0,  2,0, BlockColorSelector },		//テトリミノI　
	{ -1,0,  0,0,  0,1,  1,1, BlockColorSelector },		//テトリミノS　
	{ -1,1,  0,0,  0,1,  1,0, BlockColorSelector },		//テトリミノZ　
	{ -1,0,  0,0,  0,1,  1,0, BlockColorSelector },		//テトリミノT　
	{ -1,0,  0,0,  1,0, -1,1, BlockColorSelector },		//テトリミノJ　
	{ -1,0,  0,0,  1,0,  1,1, BlockColorSelector },		//テトリミノL　
	{  0,1,  0,0,  1,1,  1,0, BlockColorSelector },		//テトリミノO　
	{  0,0,  0,0,  0,0,  0,0, 0 }						//HOLD
	};
	TETRIMINO RotedBuffer;

	if (GameScene) {
		CountMoveTime++;
		if (!SetDown) {
			//テトリミノが作られていなかったら　ランダムでテトリミノを選ぶ
			if (!isCreated && !GameOver) {
				ChoseTetrimino(&CountMoveTime,tetrimino,&RotedBuffer,&isCreated,&BlockColorSelector,canMoveDown);
			}
			//テトリミノをHOLDする、またはHOLDと交換する
			if (isHold) {
				HoldingTetrimino(&CountMoveTime,&OnlyOnceHold,&canHold,tetrimino,&RotedBuffer,&MovingTetrimino);
			}
			//「Z」でテトリミノ左回転
			if (isRotateL) {
				RotetionLeftMove(&CountMoveTime, canRotate, &RotedBuffer, tetrimino);
				isRotateL = false;
			}
			//「↑」でテトリミノ右回転
			if (isRotateR) {
				RotetionRightMove(&CountMoveTime, canRotate, &RotedBuffer, tetrimino);
				isRotateR = false;
			}
			//「←」でテトリミノを一つ左へ
			if (isLeft) {
				LeftMove(&CountMoveTime,tetrimino);
				isLeft = false;
			}
			//「→」でテトリミノを一つ右へ
			if (isRight) {
				RightMove(&CountMoveTime, tetrimino);
				isRight = false;
			}
			//「↓」でテトリミノを一つ下へ
			if (isDown&&moveY > 1) {
				DownMove(&CountMoveTime,tetrimino,canMoveDown,&SetDown);
				isDown = false;
			}
			//「space」でテトリミノを一番下へ
			if (isDrop) {
				DropMove(&CountMoveTime,tetrimino,canMoveDown,&SetDown);
				isDrop = false;
			}
		}
		//自動で下に行く処理
		static int countFalling = 0;
		countFalling++;
		if (countFalling >= 50) {
			FalingTetrimino(tetrimino,canMoveDown,&countFalling);
		}
		if (!SetDown) {
			ReflectTetrimino(tetrimino,moveYtoCentral,moveXtoCentral);
		}
	}
	//テトリミノが着地したかどうか
	if (((!canMoveDown[0]) || (!canMoveDown[1]) || (!canMoveDown[2]) || (!canMoveDown[3])) && (!SetDown)){
		SetdownCheck(tetrimino, canMoveDown, &SetDown);
	}
	if (!ChainEffectOn) {
		g_RiseCOUNTER = 0;
	}
	if (SetDown) {
		tetriminoSetDown(&SetDown,&isCreated,&OnlyOnceHold,&EffectRoop);
	}
	//GAME OVERの処理//
	GameOverControl(&canHold, &isCreated, &GameOver, &GameScene, &GAMEOPEN);
}
void tetriminoSetDown(bool* SetDown, bool* isCreated, bool* OnlyOnceHold, int* EffectRoop) {
	static bool canClear = true;
	static bool CanCopy = true;
	static bool clear[10];
	static bool isDeleted = false;
	static int chainHeight = 0;
	static int chainEffectHeight = 0;
	static int ChainCheck[10];
	static int RisenCount = 0;

	int clearbuff[4]{ 0,0,0,0 };
	bool isAccorded = false;
	bool canupChain = false;
	int clearRise;
	if (CanCopy) {
		for (int Hcopy = 0; Hcopy < FIELD_HEIGHT; Hcopy++) {
			for (int Wcopy = 0; Wcopy < FIELD_WIDTH; Wcopy++) {
				FieldEffectBuff[Hcopy][Wcopy] = TetrisField[Hcopy][Wcopy];
			}
		}
	}
	//列を消す処理
	//まず列をチェックして、一列が全部TRUEならその列を消す
	if (canClear) {
		for (height = FIELD_HEIGHT - 2; height >= 1; height--) {
			for (width = FIELD_WIDTH - 2; width > 0; width--) {
				if (TetrisField[height][width] == 0 ||
					TetrisField[height][width] == 8) {
					clear[width - 1] = false;
				}
				else {
					clear[width - 1] = true;
				}
			}

			if (clear[0] == true && clear[1] == true &&clear[2] == true && clear[3] == true &&
				clear[4] == true && clear[5] == true &&clear[6] == true && clear[7] == true &&
				clear[8] == true && clear[9] == true) {

				isAccorded = true;
				
			}
			while (isAccorded) {
				for (width = 10; width > 0; width--) {
					ChainCheck[width - 1] = TetrisField[height][width];
				}
				for (clearRise = height; clearRise > 0; clearRise--) {
					for (width = 10; width > 0; width--) {
						TetrisField[clearRise][width] = TetrisField[clearRise - 1][width];
						TetrisField[0][width] = 0;
					}
				}

				chainHeight = height;
				chainEffectHeight = height;
				height++;
				ClearLineCOUNTER++;
				if (!clearRise) {
					isAccorded = false;
					EffectOn = true;
					CanCopy = false;
				}
			}
		}
		canClear = false;
	}
	if (EffectOn) {

		EffectControl(FieldEffectBuff);
		(*EffectRoop)++;
		if (*EffectRoop > EFFECTTIME) {
			*EffectRoop = 0;
		}
		if (!(*EffectRoop)) {
			canupChain = true;
			EffectOn = false;
		}
	}
	if (!EffectOn&&canupChain) {
		ChainProcessing(&chainHeight, ChainCheck, &RisenCount, &ChainEffectOn);
	}
	if (ChainEffectOn) {
		ChainEffect(FieldEffectBuff, ChainCheck);
		(*EffectRoop)++;
		if (*EffectRoop > EFFECTTIME) {
			*EffectRoop = 0;
		}
		if (!(*EffectRoop)) {
			canupChain = true;

			ChainEffectOn = false;
			CanCopy = true;
			for (height = 0; height < FIELD_HEIGHT; height++) {
				for (width = 0; width < FIELD_WIDTH; width++) {
					if (TetrisField[height][width] == 6) {
						TetrisField[height][width] = 0;
					}
				}
			}

		}
	}

	if ((!EffectOn) && (!ChainEffectOn)) {
		//スコア計算
		ScoreCalculation(&RisenCount, isCreated, &CanCopy, SetDown, &canClear, OnlyOnceHold);
	}
}
void ChainProcessing(int* chainHeight, int ChainCheck[], int* RisenCount,bool* ChainEffectOn) {
	bool isup = false;
	int chainbuffer[10];


	for (int ChainCheckCnt = 10; ChainCheckCnt > 0; ChainCheckCnt--) {
		if ((TetrisField[*chainHeight][ChainCheckCnt] - ChainCheck[ChainCheckCnt - 1] == 1)
			|| (TetrisField[*chainHeight][ChainCheckCnt] - ChainCheck[ChainCheckCnt - 1] == -3)) {
			*ChainEffectOn = true;
			//上を見る用のバッファに消す色情報入力
			chainbuffer[ChainCheckCnt - 1] = TetrisField[*chainHeight][ChainCheckCnt];
			(*RisenCount)++;
			//上方すべて落とす
			for (int checkup = *chainHeight; checkup > 0; checkup--) {
				TetrisField[checkup][ChainCheckCnt] = TetrisField[checkup - 1][ChainCheckCnt];
			}
			TetrisField[0][ChainCheckCnt] = 0;
		}
		else continue;
		//連鎖処理
		//連鎖縦見る
		for (int UpperCheck = *chainHeight; UpperCheck > 0; UpperCheck--) {
			if ((TetrisField[UpperCheck][ChainCheckCnt] - chainbuffer[ChainCheckCnt - 1] == 1
				|| TetrisField[UpperCheck][ChainCheckCnt] - chainbuffer[ChainCheckCnt - 1] == -3)) {

				if ((*chainHeight - UpperCheck > 2) && (TetrisField[UpperCheck][ChainCheckCnt] == chainbuffer[ChainCheckCnt - 1])) {
					isup = true;
					(*RisenCount)++;
					chainbuffer[ChainCheckCnt - 1] = TetrisField[*chainHeight][ChainCheckCnt];
					UpperCheck++;
					for (int checkup = *chainHeight; checkup > 0; checkup--) {
						TetrisField[checkup][ChainCheckCnt] = TetrisField[checkup - 1][ChainCheckCnt];
					}
					TetrisField[0][ChainCheckCnt] = 0;
				}
				else {
					isup = true;
					(*RisenCount)++;
					UpperCheck++;
					chainbuffer[ChainCheckCnt - 1] = TetrisField[*chainHeight][ChainCheckCnt];

					for (int checkup = *chainHeight; checkup > 0; checkup--) {
						TetrisField[checkup][ChainCheckCnt] = TetrisField[checkup - 1][ChainCheckCnt];
					}
					TetrisField[0][ChainCheckCnt] = 0;

				}

				//横の確認
				//	列から3段目以上ならば横も見る
				if (*chainHeight - UpperCheck > 2) {
					for (int checkHorizontally = ChainCheckCnt; checkHorizontally > FIELD_WIDTH - 2; checkHorizontally++) {
						if (TetrisField[*chainHeight][ChainCheckCnt] == TetrisField[*chainHeight][checkHorizontally]) {
							for (int checkup = 1; checkup == *chainHeight; checkup++) {
								TetrisField[*chainHeight - checkup + 1][checkHorizontally] = TetrisField[*chainHeight - checkup][checkHorizontally];
								(*RisenCount)++;
							}
						}
					}
					for (int checkHorizontally = ChainCheckCnt; checkHorizontally < 1; checkHorizontally--) {
						if (TetrisField[*chainHeight][ChainCheckCnt] == TetrisField[*chainHeight][checkHorizontally]) {
							for (int checkup = 1; checkup == *chainHeight; checkup++) {
								TetrisField[*chainHeight - checkup + 1][checkHorizontally] = TetrisField[*chainHeight - checkup][checkHorizontally];
								(*RisenCount)++;
							}
						}
					}
				}
			}
		}
		isup = false;
	}
	g_RiseCOUNTER = (*RisenCount);
}
void GameOverControl(bool* canHold,bool* isCreated, bool* GameOver, bool* GameScene, bool* GAMEOPEN) {
	for (int BlockCheck = 1; BlockCheck < 4; BlockCheck++) {
		if (TetrisField[1][1] == 10 + BlockCheck ||
			TetrisField[1][2] == 10 + BlockCheck ||
			TetrisField[1][3] == 10 + BlockCheck ||
			TetrisField[1][4] == 10 + BlockCheck ||
			TetrisField[1][5] == 10 + BlockCheck ||
			TetrisField[1][6] == 10 + BlockCheck ||
			TetrisField[1][7] == 10 + BlockCheck ||
			TetrisField[1][8] == 10 + BlockCheck ||
			TetrisField[1][9] == 10 + BlockCheck ||
			TetrisField[1][10] == 10 + BlockCheck) {
			*canHold = false;
			*isCreated = true;
			*GameOver = true;
			*GameScene = false;
			*GAMEOPEN = true;

		}
	}

}
void ScoreCalculation(int* RisenCount,bool* isCreated, bool* CanCopy, bool* SetDown, bool* canClear,bool* OnlyOnceHold) {
	//一連鎖で1.25倍、連鎖増えるごとに0.25足された数になる スコア*(1+0.25*連鎖数
	switch (ClearLineCOUNTER) {
	case 1:
		g_ScorePoint += 100 * (1 + (*RisenCount) * 0.25f);
		break;
	case 2:
		g_ScorePoint += 300 * (1 + (*RisenCount) * 0.25f);
		break;
	case 3:
		g_ScorePoint += 500 * (1 + (*RisenCount) * 0.25f);
		break;
	case 4:
		g_ScorePoint += 800 * (1 + (*RisenCount) * 0.25f);
		break;
	}

	*RisenCount = 0;
	*isCreated = false;
	if (GameOver) {
		*isCreated = true;
	}
	*CanCopy = true;
	*SetDown = false;
	*canClear = true;
	*OnlyOnceHold = true;
	ClearLineCOUNTER = 0;
	for (int Hcopy = 0; Hcopy < FIELD_HEIGHT; Hcopy++) {
		for (int Wcopy = 0; Wcopy < FIELD_WIDTH; Wcopy++) {
			FieldEffectBuff[Hcopy][Wcopy] = 0;
		}
	}

}
void RightMove(int* CountMoveTime, TETRIMINO* tetrimino) {
	bool canMoveRight[4] = { false,false,false,false };

	if (*CountMoveTime >= KEY_RESTAIN) {
		if (TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX + 1] == 0 ||
			TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX + 1] == tetrimino[CurrentTetrimino].color) {
			canMoveRight[0] = true;
		}
		else {
			canMoveRight[0] = false;
		}
		if (TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX + 1] == 0 ||
			TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX + 1] == tetrimino[CurrentTetrimino].color) {
			canMoveRight[1] = true;
		}
		else {
			canMoveRight[1] = false;
		}
		if (TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX + 1] == 0 ||
			TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX + 1] == tetrimino[CurrentTetrimino].color) {
			canMoveRight[2] = true;
		}
		else {
			canMoveRight[2] = false;
		}
		if (TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX + 1] == 0 ||
			TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX + 1] == tetrimino[CurrentTetrimino].color) {
			canMoveRight[3] = true;
		}
		else {
			canMoveRight[3] = false;
		}


		if (canMoveRight[0] == true &&
			canMoveRight[1] == true &&
			canMoveRight[2] == true &&
			canMoveRight[3] == true)
		{
			TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = 0;
			TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = 0;
			TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = 0;
			TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = 0;
			moveX += 1;
			TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color;
		}
		*CountMoveTime = 0;
	}

}
void LeftMove(int* CountMoveTime,TETRIMINO* tetrimino) {
	bool canMoveLeft[4] = { false,false,false,false };

	if (*CountMoveTime >= KEY_RESTAIN) {
		if (TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX - 1] == 0 ||
			TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX - 1] == tetrimino[CurrentTetrimino].color) {
			canMoveLeft[0] = true;
		}
		else {
			canMoveLeft[0] = false;
		}
		if (TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX - 1] == 0 ||
			TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX - 1] == tetrimino[CurrentTetrimino].color) {
			canMoveLeft[1] = true;
		}
		else {
			canMoveLeft[1] = false;
		}
		if (TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX - 1] == 0 ||
			TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX - 1] == tetrimino[CurrentTetrimino].color) {
			canMoveLeft[2] = true;
		}
		else {
			canMoveLeft[2] = false;
		}
		if (TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX - 1] == 0 ||
			TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX - 1] == tetrimino[CurrentTetrimino].color) {
			canMoveLeft[3] = true;
		}
		else {
			canMoveLeft[3] = false;
		}
		if (canMoveLeft[0] == true &&
			canMoveLeft[1] == true &&
			canMoveLeft[2] == true &&
			canMoveLeft[3] == true) {
			TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = 0;
			TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = 0;
			TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = 0;
			TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = 0;
			moveX -= 1;
			TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color;
		}
		*CountMoveTime = 0;
	}

}
void DownMove(int* CountMoveTime, TETRIMINO* tetrimino, bool canMoveDown[],bool* SetDown) {
	if (*CountMoveTime >= KEY_RESTAIN) {
		if (TetrisField[MovingTetrimino.y1 + moveY + 1][MovingTetrimino.x1 + moveX] == 0 ||
			TetrisField[MovingTetrimino.y1 + moveY + 1][MovingTetrimino.x1 + moveX] == tetrimino[CurrentTetrimino].color) {
			canMoveDown[0] = true;
		}
		else {
			canMoveDown[0] = false;
		}
		if (TetrisField[MovingTetrimino.y2 + moveY + 1][MovingTetrimino.x2 + moveX] == 0 ||
			TetrisField[MovingTetrimino.y2 + moveY + 1][MovingTetrimino.x2 + moveX] == tetrimino[CurrentTetrimino].color) {
			canMoveDown[1] = true;
		}
		else {
			canMoveDown[1] = false;
		}
		if (TetrisField[MovingTetrimino.y3 + moveY + 1][MovingTetrimino.x3 + moveX] == 0 ||
			TetrisField[MovingTetrimino.y3 + moveY + 1][MovingTetrimino.x3 + moveX] == tetrimino[CurrentTetrimino].color) {
			canMoveDown[2] = true;
		}
		else {
			canMoveDown[2] = false;
		}
		if (TetrisField[MovingTetrimino.y4 + moveY + 1][MovingTetrimino.x4 + moveX] == 0 ||
			TetrisField[MovingTetrimino.y4 + moveY + 1][MovingTetrimino.x4 + moveX] == tetrimino[CurrentTetrimino].color) {
			canMoveDown[3] = true;
		}
		else {
			canMoveDown[3] = false;
		}

		if (canMoveDown[0] == true &&
			canMoveDown[1] == true &&
			canMoveDown[2] == true &&
			canMoveDown[3] == true) {

			TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = 0;
			TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = 0;
			TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = 0;
			TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = 0;

			moveY += 1;

			TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color;
			*CountMoveTime = 0;
		}
		if (canMoveDown[0] == false ||
			canMoveDown[1] == false ||
			canMoveDown[2] == false ||
			canMoveDown[3] == false) {

			*SetDown = true;

			TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color + 10;
			TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color + 10;
			TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color + 10;
			TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color + 10;
		}
	}

}
void DropMove(int* CountMoveTime, TETRIMINO* tetrimino, bool canMoveDown[], bool* SetDown) {
	for (int droploop = 0; droploop < 20; droploop++) {
		bool canMoveDrop[4] = { false,false,false,false };
		if (TetrisField[MovingTetrimino.y1 + moveY + 1][MovingTetrimino.x1 + moveX] == 0 ||
			TetrisField[MovingTetrimino.y1 + moveY + 1][MovingTetrimino.x1 + moveX] == tetrimino[CurrentTetrimino].color) {
			canMoveDrop[0] = true;
		}
		else {
			canMoveDrop[0] = false;
		}
		if (TetrisField[MovingTetrimino.y2 + moveY + 1][MovingTetrimino.x2 + moveX] == 0 ||
			TetrisField[MovingTetrimino.y2 + moveY + 1][MovingTetrimino.x2 + moveX] == tetrimino[CurrentTetrimino].color) {
			canMoveDrop[1] = true;
		}
		else {
			canMoveDrop[1] = false;
		}
		if (TetrisField[MovingTetrimino.y3 + moveY + 1][MovingTetrimino.x3 + moveX] == 0 ||
			TetrisField[MovingTetrimino.y3 + moveY + 1][MovingTetrimino.x3 + moveX] == tetrimino[CurrentTetrimino].color) {
			canMoveDrop[2] = true;
		}
		else {
			canMoveDrop[2] = false;
		}
		if (TetrisField[MovingTetrimino.y4 + moveY + 1][MovingTetrimino.x4 + moveX] == 0 ||
			TetrisField[MovingTetrimino.y4 + moveY + 1][MovingTetrimino.x4 + moveX] == tetrimino[CurrentTetrimino].color) {
			canMoveDrop[3] = true;
		}
		else {
			canMoveDrop[3] = false;
		}

		if (canMoveDrop[0] == true &&
			canMoveDrop[1] == true &&
			canMoveDrop[2] == true &&
			canMoveDrop[3] == true)
		{
			TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = 0;
			TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = 0;
			TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = 0;
			TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = 0;
			if (FIELD_HEIGHT - 2 - moveY < 0) {
				moveY += FIELD_HEIGHT - 2 - moveY;
			}
			else moveY += 1;
			TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color;
		}

	}
	if (canMoveDown[0] == false ||
		canMoveDown[1] == false ||
		canMoveDown[2] == false ||
		canMoveDown[3] == false)
	{
		*SetDown = true;

		TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color + 10;
		TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color + 10;
		TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color + 10;
		TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color + 10;
	}
	*CountMoveTime = 0;

}
void RotetionRightMove(int* CountMoveTime, bool canRotate[], TETRIMINO* RotedBuffer, TETRIMINO* tetrimino) {

	RotedBuffer->x1 = tetrimino[CurrentTetrimino].x1;
	RotedBuffer->y1 = tetrimino[CurrentTetrimino].y1;
	RotedBuffer->x2 = tetrimino[CurrentTetrimino].x2;
	RotedBuffer->y2 = tetrimino[CurrentTetrimino].y2;
	RotedBuffer->x3 = tetrimino[CurrentTetrimino].x3;
	RotedBuffer->y3 = tetrimino[CurrentTetrimino].y3;
	RotedBuffer->x4 = tetrimino[CurrentTetrimino].x4;
	RotedBuffer->y4 = tetrimino[CurrentTetrimino].y4;
	//回転１
	int tmpR = RotedBuffer->y1;
	RotedBuffer->y1 = RotedBuffer->x1;
	RotedBuffer->x1 = -tmpR;
	tmpR = RotedBuffer->y2;
	RotedBuffer->y2 = RotedBuffer->x2;
	RotedBuffer->x2 = -tmpR;
	tmpR = RotedBuffer->y3;
	RotedBuffer->y3 = RotedBuffer->x3;
	RotedBuffer->x3 = -tmpR;
	tmpR = RotedBuffer->y4;
	RotedBuffer->y4 = RotedBuffer->x4;
	RotedBuffer->x4 = -tmpR;
	if (TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == 0 ||
		TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[0] = true;
	}
	else canRotate[0] = false;
	if (TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == 0 ||
		TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[1] = true;
	}
	else canRotate[1] = false;
	if (TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == 0 ||
		TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[2] = true;
	}
	else canRotate[2] = false;
	if (TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == 0 ||
		TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[3] = true;
	}
	else canRotate[3] = false;
	//回転2
	tmpR = RotedBuffer->y1;
	RotedBuffer->y1 = RotedBuffer->x1;
	RotedBuffer->x1 = -tmpR;
	tmpR = RotedBuffer->y2;
	RotedBuffer->y2 = RotedBuffer->x2;
	RotedBuffer->x2 = -tmpR;
	tmpR = RotedBuffer->y3;
	RotedBuffer->y3 = RotedBuffer->x3;
	RotedBuffer->x3 = -tmpR;
	tmpR = RotedBuffer->y4;
	RotedBuffer->y4 = RotedBuffer->x4;
	RotedBuffer->x4 = -tmpR;
	if (TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == 0 ||
		TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[4] = true;
	}
	else canRotate[4] = false;

	if (TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == 0 ||
		TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[5] = true;
	}
	else canRotate[5] = false;

	if (TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == 0 ||
		TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[6] = true;
	}
	else canRotate[6] = false;

	if (TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == 0 ||
		TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[7] = true;
	}
	else canRotate[7] = false;
	//回転3
	tmpR = RotedBuffer->y1;
	RotedBuffer->y1 = RotedBuffer->x1;
	RotedBuffer->x1 = -tmpR;
	tmpR = RotedBuffer->y2;
	RotedBuffer->y2 = RotedBuffer->x2;
	RotedBuffer->x2 = -tmpR;
	tmpR = RotedBuffer->y3;
	RotedBuffer->y3 = RotedBuffer->x3;
	RotedBuffer->x3 = -tmpR;
	tmpR = RotedBuffer->y4;
	RotedBuffer->y4 = RotedBuffer->x4;
	RotedBuffer->x4 = -tmpR;
	if (TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == 0 ||
		TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[8] = true;
	}
	else canRotate[8] = false;
	if (TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == 0 ||
		TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[9] = true;
	}
	else canRotate[9] = false;
	if (TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == 0 ||
		TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[10] = true;
	}
	else canRotate[10] = false;
	if (TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == 0 ||
		TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[11] = true;
	}
	else canRotate[11] = false;
	//回転4
	tmpR = RotedBuffer->y1;
	RotedBuffer->y1 = RotedBuffer->x1;
	RotedBuffer->x1 = -tmpR;
	tmpR = RotedBuffer->y2;
	RotedBuffer->y2 = RotedBuffer->x2;
	RotedBuffer->x2 = -tmpR;
	tmpR = RotedBuffer->y3;
	RotedBuffer->y3 = RotedBuffer->x3;
	RotedBuffer->x3 = -tmpR;
	tmpR = RotedBuffer->y4;
	RotedBuffer->y4 = RotedBuffer->x4;
	RotedBuffer->x4 = -tmpR;
	if (TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == 0 ||
		TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[12] = true;
	}
	else canRotate[12] = false;
	if (TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == 0 ||
		TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[13] = true;
	}
	else canRotate[13] = false;
	if (TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == 0 ||
		TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[14] = true;
	}
	else canRotate[14] = false;
	if (TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == 0 ||
		TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[15] = true;
	}
	else canRotate[15] = false;

	if (*CountMoveTime >= KEY_RESTAIN) {
		if (canRotate[0] &&
			canRotate[1] &&
			canRotate[2] &&
			canRotate[3] &&
			canRotate[4] &&
			canRotate[5] &&
			canRotate[6] &&
			canRotate[7] &&
			canRotate[8] &&
			canRotate[9] &&
			canRotate[10] &&
			canRotate[11] &&
			canRotate[12] &&
			canRotate[13] &&
			canRotate[14] &&
			canRotate[15]) {

			TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = 0;
			TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = 0;
			TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = 0;
			TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = 0;

			int RRoleTmp = MovingTetrimino.y1;
			MovingTetrimino.y1 = MovingTetrimino.x1;
			MovingTetrimino.x1 = -RRoleTmp;
			RRoleTmp = MovingTetrimino.y3;
			MovingTetrimino.y3 = MovingTetrimino.x3;
			MovingTetrimino.x3 = -RRoleTmp;
			RRoleTmp = MovingTetrimino.y4;
			MovingTetrimino.y4 = MovingTetrimino.x4;
			MovingTetrimino.x4 = -RRoleTmp;

			TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color;

			canRotate[0] = false;
			canRotate[1] = false;
			canRotate[2] = false;
			canRotate[3] = false;
			*CountMoveTime = 0;

		}
	}
}
void RotetionLeftMove(int* CountMoveTime, bool canRotate[], TETRIMINO* RotedBuffer, TETRIMINO* tetrimino) {
	RotedBuffer->x1 = tetrimino[CurrentTetrimino].x1;
	RotedBuffer->y1 = tetrimino[CurrentTetrimino].y1;
	RotedBuffer->x2 = tetrimino[CurrentTetrimino].x2;
	RotedBuffer->y2 = tetrimino[CurrentTetrimino].y2;
	RotedBuffer->x3 = tetrimino[CurrentTetrimino].x3;
	RotedBuffer->y3 = tetrimino[CurrentTetrimino].y3;
	RotedBuffer->x4 = tetrimino[CurrentTetrimino].x4;
	RotedBuffer->y4 = tetrimino[CurrentTetrimino].y4;
	//回転１
	int tmpL = RotedBuffer->x1;
	RotedBuffer->x1 = RotedBuffer->y1;
	RotedBuffer->y1 = -tmpL;
	tmpL = RotedBuffer->x2;
	RotedBuffer->x2 = RotedBuffer->y2;
	RotedBuffer->y2 = -tmpL;
	tmpL = RotedBuffer->x3;
	RotedBuffer->x3 = RotedBuffer->y3;
	RotedBuffer->y3 = -tmpL;
	tmpL = RotedBuffer->x4;
	RotedBuffer->x4 = RotedBuffer->y4;
	RotedBuffer->y4 = -tmpL;
	if (TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == 0 ||
		TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[0] = true;
	}
	else canRotate[0] = false;
	if (TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == 0 ||
		TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[1] = true;
	}
	else canRotate[1] = false;
	if (TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == 0 ||
		TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[2] = true;
	}
	else canRotate[2] = false;
	if (TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == 0 ||
		TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[3] = true;
	}
	else canRotate[3] = false;
	//回転2
	tmpL = RotedBuffer->x1;
	RotedBuffer->x1 = RotedBuffer->y1;
	RotedBuffer->y1 = -tmpL;
	tmpL = RotedBuffer->x2;
	RotedBuffer->x2 = RotedBuffer->y2;
	RotedBuffer->y2 = -tmpL;
	tmpL = RotedBuffer->x3;
	RotedBuffer->x3 = RotedBuffer->y3;
	RotedBuffer->y3 = -tmpL;
	tmpL = RotedBuffer->x4;
	RotedBuffer->x4 = RotedBuffer->y4;
	RotedBuffer->y4 = -tmpL;
	if (TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == 0 ||
		TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[4] = true;
	}
	else canRotate[4] = false;

	if (TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == 0 ||
		TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[5] = true;
	}
	else canRotate[5] = false;

	if (TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == 0 ||
		TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[6] = true;
	}
	else canRotate[6] = false;

	if (TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == 0 ||
		TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[7] = true;
	}
	else canRotate[7] = false;
	//回転3
	tmpL = RotedBuffer->x1;
	RotedBuffer->x1 = RotedBuffer->y1;
	RotedBuffer->y1 = -tmpL;
	tmpL = RotedBuffer->x2;
	RotedBuffer->x2 = RotedBuffer->y2;
	RotedBuffer->y2 = -tmpL;
	tmpL = RotedBuffer->x3;
	RotedBuffer->x3 = RotedBuffer->y3;
	RotedBuffer->y3 = -tmpL;
	tmpL = RotedBuffer->x4;
	RotedBuffer->x4 = RotedBuffer->y4;
	RotedBuffer->y4 = -tmpL;
	if (TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == 0 ||
		TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[8] = true;
	}
	else canRotate[8] = false;
	if (TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == 0 ||
		TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[9] = true;
	}
	else canRotate[9] = false;
	if (TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == 0 ||
		TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[10] = true;
	}
	else canRotate[10] = false;
	if (TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == 0 ||
		TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[11] = true;
	}
	else canRotate[11] = false;
	//回転4
	tmpL = RotedBuffer->x1;
	RotedBuffer->x1 = RotedBuffer->y1;
	RotedBuffer->y1 = -tmpL;
	tmpL = RotedBuffer->x2;
	RotedBuffer->x2 = RotedBuffer->y2;
	RotedBuffer->y2 = -tmpL;
	tmpL = RotedBuffer->x3;
	RotedBuffer->x3 = RotedBuffer->y3;
	RotedBuffer->y3 = -tmpL;
	tmpL = RotedBuffer->x4;
	RotedBuffer->x4 = RotedBuffer->y4;
	RotedBuffer->y4 = -tmpL;
	if (TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == 0 ||
		TetrisField[RotedBuffer->y1 + moveY][RotedBuffer->x1 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[12] = true;
	}
	else canRotate[12] = false;
	if (TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == 0 ||
		TetrisField[RotedBuffer->y2 + moveY][RotedBuffer->x2 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[13] = true;
	}
	else canRotate[13] = false;
	if (TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == 0 ||
		TetrisField[RotedBuffer->y3 + moveY][RotedBuffer->x3 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[14] = true;
	}
	else canRotate[14] = false;
	if (TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == 0 ||
		TetrisField[RotedBuffer->y4 + moveY][RotedBuffer->x4 + moveX] == tetrimino[CurrentTetrimino].color) {
		canRotate[15] = true;
	}
	else canRotate[15] = false;


	if (*CountMoveTime >= KEY_RESTAIN) {
		if (canRotate[0] && canRotate[1] &&
			canRotate[2] && canRotate[3] &&
			canRotate[4] && canRotate[5] &&
			canRotate[6] && canRotate[7] &&
			canRotate[8] && canRotate[9] &&
			canRotate[10] && canRotate[11] &&
			canRotate[12] && canRotate[13] &&
			canRotate[14] && canRotate[15]) {

			TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = 0;
			TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = 0;
			TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = 0;
			TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = 0;

			int LRoleTmp = MovingTetrimino.x1;
			MovingTetrimino.x1 = MovingTetrimino.y1;
			MovingTetrimino.y1 = -LRoleTmp;
			LRoleTmp = MovingTetrimino.x3;
			MovingTetrimino.x3 = MovingTetrimino.y3;
			MovingTetrimino.y3 = -LRoleTmp;
			LRoleTmp = MovingTetrimino.x4;
			MovingTetrimino.x4 = MovingTetrimino.y4;
			MovingTetrimino.y4 = -LRoleTmp;

			TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color;
			TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color;

			canRotate[0] = false;
			canRotate[1] = false;
			canRotate[2] = false;
			canRotate[3] = false;
		}
		*CountMoveTime = 0;
	}
	isRotateL = false;
}
void HoldingTetrimino(int* CountMoveTime, bool* OnlyOnceHold,bool* canHold, TETRIMINO* tetrimino, TETRIMINO* RotedBuffer,TETRIMINO* MovingTetrimino) {
	if (*CountMoveTime >= KEY_RESTAIN) {

		if (!(*canHold)) {
			if (OnlyOnceHold) {
				for (height = 0; height < 4; height++) {
					for (width = 0; width < 6; width++) {
						HoldBoard[height][width] = 0;
					}
				}

				HoldTetrimino = CurrentTetrimino;

				TetrisField[MovingTetrimino->y1 + moveY][MovingTetrimino->x1 + moveX] = 0;
				TetrisField[MovingTetrimino->y2 + moveY][MovingTetrimino->x2 + moveX] = 0;
				TetrisField[MovingTetrimino->y3 + moveY][MovingTetrimino->x3 + moveX] = 0;
				TetrisField[MovingTetrimino->y4 + moveY][MovingTetrimino->x4 + moveX] = 0;

				CurrentTetrimino = TetriminoNext[0];
				TetriminoNext[0] = TetriminoNext[1];
				TetriminoNext[1] = TetriminoNext[2];
				TetriminoNext[2] = rand() % 7;

				MovingTetrimino->x1 = tetrimino[CurrentTetrimino].x1;
				MovingTetrimino->y1 = tetrimino[CurrentTetrimino].y1;
				MovingTetrimino->x2 = tetrimino[CurrentTetrimino].x2;
				MovingTetrimino->y2 = tetrimino[CurrentTetrimino].y2;
				MovingTetrimino->x3 = tetrimino[CurrentTetrimino].x3;
				MovingTetrimino->y3 = tetrimino[CurrentTetrimino].y3;
				MovingTetrimino->x4 = tetrimino[CurrentTetrimino].x4;
				MovingTetrimino->y4 = tetrimino[CurrentTetrimino].y4;
				RotedBuffer->x1 = tetrimino[CurrentTetrimino].x1;
				RotedBuffer->y1 = tetrimino[CurrentTetrimino].y1;
				RotedBuffer->x2 = tetrimino[CurrentTetrimino].x2;
				RotedBuffer->y2 = tetrimino[CurrentTetrimino].y2;
				RotedBuffer->x3 = tetrimino[CurrentTetrimino].x3;
				RotedBuffer->y3 = tetrimino[CurrentTetrimino].y3;
				RotedBuffer->x4 = tetrimino[CurrentTetrimino].x4;
				RotedBuffer->y4 = tetrimino[CurrentTetrimino].y4;

				moveX = 5;
				moveY = 1;

				CreateBoard();
				OnlyOnceHold = false;
				isHold = false;
			}
			*canHold = true;
		}

		if (*canHold) {
			if (OnlyOnceHold) {
				for (height = 0; height < 4; height++) {
					for (width = 0; width < 6; width++) {
						HoldBoard[height][width] = 0;
					}
				}
				*canHold = true;
			}

			if (canHold) {
				if (OnlyOnceHold) {
					for (height = 0; height < 4; height++) {
						for (width = 0; width < 6; width++) {
							HoldBoard[height][width] = 0;
						}
					}

					TetrisField[MovingTetrimino->y1 + moveY][MovingTetrimino->x1 + moveX] = 0;
					TetrisField[MovingTetrimino->y2 + moveY][MovingTetrimino->x2 + moveX] = 0;
					TetrisField[MovingTetrimino->y3 + moveY][MovingTetrimino->x3 + moveX] = 0;
					TetrisField[MovingTetrimino->y4 + moveY][MovingTetrimino->x4 + moveX] = 0;

					int tmp = CurrentTetrimino;
					CurrentTetrimino = HoldTetrimino;
					HoldTetrimino = tmp;

					MovingTetrimino->x1 = tetrimino[CurrentTetrimino].x1;
					MovingTetrimino->y1 = tetrimino[CurrentTetrimino].y1;
					MovingTetrimino->x2 = tetrimino[CurrentTetrimino].x2;
					MovingTetrimino->y2 = tetrimino[CurrentTetrimino].y2;
					MovingTetrimino->x3 = tetrimino[CurrentTetrimino].x3;
					MovingTetrimino->y3 = tetrimino[CurrentTetrimino].y3;
					MovingTetrimino->x4 = tetrimino[CurrentTetrimino].x4;
					MovingTetrimino->y4 = tetrimino[CurrentTetrimino].y4;
					RotedBuffer->x1 = tetrimino[CurrentTetrimino].x1;
					RotedBuffer->y1 = tetrimino[CurrentTetrimino].y1;
					RotedBuffer->x2 = tetrimino[CurrentTetrimino].x2;
					RotedBuffer->y2 = tetrimino[CurrentTetrimino].y2;
					RotedBuffer->x3 = tetrimino[CurrentTetrimino].x3;
					RotedBuffer->y3 = tetrimino[CurrentTetrimino].y3;
					RotedBuffer->x4 = tetrimino[CurrentTetrimino].x4;
					RotedBuffer->y4 = tetrimino[CurrentTetrimino].y4;

					moveX = 5;
					moveY = 1;

					CreateBoard();
					isHold = false;
					*OnlyOnceHold = false;
				}
			}
			*CountMoveTime = 0;
		}
		isHold = false;
	}

}
void ChoseTetrimino(int* CountMoveTime, TETRIMINO* tetrimino, TETRIMINO* RotedBuffer, bool* isCreated,int* BlockColorSelector, bool canMoveDown[]) {
	if (*CountMoveTime >= CREATE_INTERVAL) {

		CurrentTetrimino = TetriminoNext[0];
		TetriminoNext[0] = TetriminoNext[1];
		TetriminoNext[1] = TetriminoNext[2];
		TetriminoNext[2] = rand() % 7;

		moveX = 5;
		moveY = 1;
		*isCreated = true;
		*CountMoveTime = 0;

		*BlockColorSelector = rand() % 4 + 1;

		//テトリミノを仮想で先に動かすときに使うためのバッファ
		MovingTetrimino.x1 = tetrimino[CurrentTetrimino].x1;
		MovingTetrimino.y1 = tetrimino[CurrentTetrimino].y1;
		MovingTetrimino.x2 = tetrimino[CurrentTetrimino].x2;
		MovingTetrimino.y2 = tetrimino[CurrentTetrimino].y2;
		MovingTetrimino.x3 = tetrimino[CurrentTetrimino].x3;
		MovingTetrimino.y3 = tetrimino[CurrentTetrimino].y3;
		MovingTetrimino.x4 = tetrimino[CurrentTetrimino].x4;
		MovingTetrimino.y4 = tetrimino[CurrentTetrimino].y4;
		RotedBuffer->x1 = tetrimino[CurrentTetrimino].x1;
		RotedBuffer->y1 = tetrimino[CurrentTetrimino].y1;
		RotedBuffer->x2 = tetrimino[CurrentTetrimino].x2;
		RotedBuffer->y2 = tetrimino[CurrentTetrimino].y2;
		RotedBuffer->x3 = tetrimino[CurrentTetrimino].x3;
		RotedBuffer->y3 = tetrimino[CurrentTetrimino].y3;
		RotedBuffer->x4 = tetrimino[CurrentTetrimino].x4;
		RotedBuffer->y4 = tetrimino[CurrentTetrimino].y4;

		CreateBoard();

		canMoveDown[0] = true;
		canMoveDown[1] = true;
		canMoveDown[2] = true;
		canMoveDown[3] = true;

	}

}
void SetdownCheck(TETRIMINO* tetrimino, bool canMoveDown[], bool* SetDown) {
	*SetDown = true;

	TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color + 10;
	TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color + 10;
	TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color + 10;
	TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color + 10;

	tetrimino[CurrentTetrimino].color = 0;

	isDrop = false;
	isRotateL = false;
	isRotateR = false;
	isDown = false;
	isLeft = false;
	isRight = false;
	memset(canMoveDown, true, 4);
}
void FalingTetrimino(TETRIMINO* tetrimino, bool canMoveDown[],int* countFalling) {
	if (TetrisField[MovingTetrimino.y1 + moveY + 1][MovingTetrimino.x1 + moveX] == 0 ||
		TetrisField[MovingTetrimino.y1 + moveY + 1][MovingTetrimino.x1 + moveX] == tetrimino[CurrentTetrimino].color) {
		canMoveDown[0] = true;
	}
	else {
		canMoveDown[0] = false;
	}
	if (TetrisField[MovingTetrimino.y2 + moveY + 1][MovingTetrimino.x2 + moveX] == 0 ||
		TetrisField[MovingTetrimino.y2 + moveY + 1][MovingTetrimino.x2 + moveX] == tetrimino[CurrentTetrimino].color) {
		canMoveDown[1] = true;
	}
	else {
		canMoveDown[1] = false;
	}
	if (TetrisField[MovingTetrimino.y3 + moveY + 1][MovingTetrimino.x3 + moveX] == 0 ||
		TetrisField[MovingTetrimino.y3 + moveY + 1][MovingTetrimino.x3 + moveX] == tetrimino[CurrentTetrimino].color) {
		canMoveDown[2] = true;
	}
	else {
		canMoveDown[2] = false;
	}
	if (TetrisField[MovingTetrimino.y4 + moveY + 1][MovingTetrimino.x4 + moveX] == 0 ||
		TetrisField[MovingTetrimino.y4 + moveY + 1][MovingTetrimino.x4 + moveX] == tetrimino[CurrentTetrimino].color) {
		canMoveDown[3] = true;
	}
	else {
		canMoveDown[3] = false;
	}


	if (canMoveDown[0] == true &&
		canMoveDown[1] == true &&
		canMoveDown[2] == true &&
		canMoveDown[3] == true) {

		TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = 0;
		TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = 0;
		TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = 0;
		TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = 0;

		moveY += 1;

		TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color;
		TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color;
		TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color;
		TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color;
		*countFalling = 0;
	}

}
void ReflectTetrimino(TETRIMINO* tetrimino, int moveYtoCentral,int moveXtoCentral) {
	TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color;
	TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color;
	TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color;
	TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color;
	//
	HoldBoard[tetrimino[HoldTetrimino].y1 + moveYtoCentral][tetrimino[HoldTetrimino].x1 + moveXtoCentral] = tetrimino[HoldTetrimino].color;
	HoldBoard[tetrimino[HoldTetrimino].y2 + moveYtoCentral][tetrimino[HoldTetrimino].x2 + moveXtoCentral] = tetrimino[HoldTetrimino].color;
	HoldBoard[tetrimino[HoldTetrimino].y3 + moveYtoCentral][tetrimino[HoldTetrimino].x3 + moveXtoCentral] = tetrimino[HoldTetrimino].color;
	HoldBoard[tetrimino[HoldTetrimino].y4 + moveYtoCentral][tetrimino[HoldTetrimino].x4 + moveXtoCentral] = tetrimino[HoldTetrimino].color;

	NextBoard[tetrimino[TetriminoNext[0]].y1 + moveYtoCentral][tetrimino[TetriminoNext[0]].x1 + moveXtoCentral] = tetrimino[TetriminoNext[0]].color;
	NextBoard[tetrimino[TetriminoNext[0]].y2 + moveYtoCentral][tetrimino[TetriminoNext[0]].x2 + moveXtoCentral] = tetrimino[TetriminoNext[0]].color;
	NextBoard[tetrimino[TetriminoNext[0]].y3 + moveYtoCentral][tetrimino[TetriminoNext[0]].x3 + moveXtoCentral] = tetrimino[TetriminoNext[0]].color;
	NextBoard[tetrimino[TetriminoNext[0]].y4 + moveYtoCentral][tetrimino[TetriminoNext[0]].x4 + moveXtoCentral] = tetrimino[TetriminoNext[0]].color;

	NextBoard2[tetrimino[TetriminoNext[1]].y1 + moveYtoCentral][tetrimino[TetriminoNext[1]].x1 + moveXtoCentral] = tetrimino[TetriminoNext[1]].color;
	NextBoard2[tetrimino[TetriminoNext[1]].y2 + moveYtoCentral][tetrimino[TetriminoNext[1]].x2 + moveXtoCentral] = tetrimino[TetriminoNext[1]].color;
	NextBoard2[tetrimino[TetriminoNext[1]].y3 + moveYtoCentral][tetrimino[TetriminoNext[1]].x3 + moveXtoCentral] = tetrimino[TetriminoNext[1]].color;
	NextBoard2[tetrimino[TetriminoNext[1]].y4 + moveYtoCentral][tetrimino[TetriminoNext[1]].x4 + moveXtoCentral] = tetrimino[TetriminoNext[1]].color;

	NextBoard3[tetrimino[TetriminoNext[2]].y1 + moveYtoCentral][tetrimino[TetriminoNext[2]].x1 + moveXtoCentral] = tetrimino[TetriminoNext[2]].color;
	NextBoard3[tetrimino[TetriminoNext[2]].y2 + moveYtoCentral][tetrimino[TetriminoNext[2]].x2 + moveXtoCentral] = tetrimino[TetriminoNext[2]].color;
	NextBoard3[tetrimino[TetriminoNext[2]].y3 + moveYtoCentral][tetrimino[TetriminoNext[2]].x3 + moveXtoCentral] = tetrimino[TetriminoNext[2]].color;
	NextBoard3[tetrimino[TetriminoNext[2]].y4 + moveYtoCentral][tetrimino[TetriminoNext[2]].x4 + moveXtoCentral] = tetrimino[TetriminoNext[2]].color;

}