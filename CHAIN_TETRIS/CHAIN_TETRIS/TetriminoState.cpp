#include "TetriminoState.h"
#include "TetrisEffect.h"

//int TetoriminoColer;
int TetrisField[FIELD_HEIGHT][FIELD_WIDTH];
int ClearBoard[FIELD_HEIGHT][FIELD_WIDTH];
int NextBoard[4][6];
int NextBoard2[4][6];
int NextBoard3[4][6];
int HoldBoard[4][6];
int ScoreBoard[4][6];
int CurrentTetrimino, TetriminoNext, TetriminoNext2, TetriminoNext3, HoldTetrimino;
int moveY;
int moveX;
int ClearLineCOUNTER = 0;
int g_ScorePoint = 0;
int g_RiseCOUNTER = 0;
static int height = 0;
static int width = 0;
bool EffectOn = false;
bool ChainEffectOn = false;
bool GameScene = false;
bool GameOver = false;
bool GAMEOPEN = true;
TETRIMINO MovingTetrimino;
TETRIMINO tetrimino[8];


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
void CreateBoard(){
	for (height = 0; height < 4; height++) {
		for (width = 0; width < 6; width++) {
			NextBoard[height][width] = 0;
		}
	}
	for (height = 0; height < 4; height++) {
		for (width = 0; width < 6; width++) {
			NextBoard2[height][width] = 0;
		}
	}
	for (height = 0; height < 4; height++) {
		for (width = 0; width < 6; width++) {
			NextBoard3[height][width] = 0;
		}
	}
	for (height = 0; height < 4; height++) {
		for (width = 0; width < 6; width++) {
			HoldBoard[height][width] = 0;
		}
	}
	for (height = 0; height < 4; height++) {
		for (width = 0; width < 6; width++) {
			ScoreBoard[height][width] = 0;
		}
	}
}

void SelectTetrimino(void) {
	//���߂ɏo�Ă���e�g���X�A1���A�Q���A3���̃e�g���~�m�����߂�
	srand((unsigned int)time(NULL));
	CurrentTetrimino = rand() % 7 ;
	TetriminoNext = rand() % 7 ;
	TetriminoNext2 = rand() % 7 ;
	TetriminoNext3 = rand() % 7 ;
	HoldTetrimino =  7;
}

void ControlTetris(void) {

	static int CountMoveTime = 0;
	static int moveYtoCentral = 1;
	static int moveXtoCentral = 2;

	static bool isCreated = true;
	static bool canMoveDown1 = false;
	static bool canMoveDown2 = false;
	static bool canMoveDown3 = false;
	static bool canMoveDown4 = false;
	static bool canMoveDrop1 = false;
	static bool canMoveDrop2 = false;
	static bool canMoveDrop3 = false;
	static bool canMoveDrop4 = false;
	static bool canMoveRight1 = false;
	static bool canMoveRight2 = false;
	static bool canMoveRight3 = false;
	static bool canMoveRight4 = false;
	static bool canMoveLeft1 = false;
	static bool canMoveLeft2 = false;
	static bool canMoveLeft3 = false;
	static bool canMoveLeft4 = false;
	static bool SetDown = false;
	static bool canRotate1 = false;
	static bool canRotate2 = false;
	static bool canRotate3 = false;
	static bool canRotate4 = false;
	static bool canRotate5 = false;
	static bool canRotate6 = false;
	static bool canRotate7 = false;
	static bool canRotate8 = false;
	static bool canRotate9 = false;
	static bool canRotate10 = false;
	static bool canRotate11 = false;
	static bool canRotate12 = false;
	static bool canRotate13 = false;
	static bool canRotate14 = false;
	static bool canRotate15 = false;
	static bool canRotate16 = false;
	static bool canHold = false;
	static bool OnlyOnceHold = true;

	static int BlockColorSelector = 0;


	TETRIMINO tetrimino[8]{
	{ -1,0,  0,0,  1,0,  2,0, BlockColorSelector },		//�e�g���~�mI�@
	{ -1,0,  0,0,  0,1,  1,1, BlockColorSelector },		//�e�g���~�mS�@
	{ -1,1,  0,0,  0,1,  1,0, BlockColorSelector },		//�e�g���~�mZ�@
	{ -1,0,  0,0,  0,1,  1,0, BlockColorSelector },		//�e�g���~�mT�@
	{ -1,0,  0,0,  1,0, -1,1, BlockColorSelector },		//�e�g���~�mJ�@
	{ -1,0,  0,0,  1,0,  1,1, BlockColorSelector },		//�e�g���~�mL�@
	{  0,1,  0,0,  1,1,  1,0, BlockColorSelector },		//�e�g���~�mO�@
	{  0,0,  0,0,  0,0,  0,0, 0 }			//HOLD
	};

	TETRIMINO RotedBuffer;

	////�e�g���~�m�̍쐬
	//MovingTetrimino.x1 = tetrimino[CurrentTetrimino].x1;
	//MovingTetrimino.x2 = tetrimino[CurrentTetrimino].x2;
	//MovingTetrimino.x3 = tetrimino[CurrentTetrimino].x3;
	//MovingTetrimino.x4 = tetrimino[CurrentTetrimino].x4;
	//MovingTetrimino.y1 = tetrimino[CurrentTetrimino].y1;
	//MovingTetrimino.y2 = tetrimino[CurrentTetrimino].y2;
	//MovingTetrimino.y3 = tetrimino[CurrentTetrimino].y3;
	//MovingTetrimino.y4 = tetrimino[CurrentTetrimino].y4;

	//RotedBuffer.x1 = tetrimino[CurrentTetrimino].x1;
	//RotedBuffer.x2 = tetrimino[CurrentTetrimino].x2;
	//RotedBuffer.x3 = tetrimino[CurrentTetrimino].x3;
	//RotedBuffer.x4 = tetrimino[CurrentTetrimino].x4;
	//RotedBuffer.y1 = tetrimino[CurrentTetrimino].y1;
	//RotedBuffer.y2 = tetrimino[CurrentTetrimino].y2;
	//RotedBuffer.y3 = tetrimino[CurrentTetrimino].y3;
	//RotedBuffer.y4 = tetrimino[CurrentTetrimino].y4;



	if (GameScene) {
		CountMoveTime++;

		if (!SetDown) {
		//�e�g���~�m������Ă��Ȃ�������@�����_���Ńe�g���~�m��I��
		if (!isCreated && !GameOver) {
			if (CountMoveTime >= CREATE_INTERVAL) {

				CurrentTetrimino = TetriminoNext;
				TetriminoNext = TetriminoNext2;
				TetriminoNext2 = TetriminoNext3;
				TetriminoNext3 = rand() % 7;

				moveX = 5;
				moveY = 1;
				isCreated = true;
				CountMoveTime = 0;

				BlockColorSelector = rand() % 4 + 1;

				//�e�g���~�m�����z�Ő�ɓ������Ƃ��Ɏg�����߂̃o�b�t�@
				MovingTetrimino.x1 = tetrimino[CurrentTetrimino].x1;
				MovingTetrimino.y1 = tetrimino[CurrentTetrimino].y1;
				MovingTetrimino.x2 = tetrimino[CurrentTetrimino].x2;
				MovingTetrimino.y2 = tetrimino[CurrentTetrimino].y2;
				MovingTetrimino.x3 = tetrimino[CurrentTetrimino].x3;
				MovingTetrimino.y3 = tetrimino[CurrentTetrimino].y3;
				MovingTetrimino.x4 = tetrimino[CurrentTetrimino].x4;
				MovingTetrimino.y4 = tetrimino[CurrentTetrimino].y4;
				RotedBuffer.x1 = tetrimino[CurrentTetrimino].x1;
				RotedBuffer.y1 = tetrimino[CurrentTetrimino].y1;
				RotedBuffer.x2 = tetrimino[CurrentTetrimino].x2;
				RotedBuffer.y2 = tetrimino[CurrentTetrimino].y2;
				RotedBuffer.x3 = tetrimino[CurrentTetrimino].x3;
				RotedBuffer.y3 = tetrimino[CurrentTetrimino].y3;
				RotedBuffer.x4 = tetrimino[CurrentTetrimino].x4;
				RotedBuffer.y4 = tetrimino[CurrentTetrimino].y4;

				CreateBoard();

				canMoveDown1 = true;
				canMoveDown2 = true;
				canMoveDown3 = true;
				canMoveDown4 = true;

			}
		}
		
		//�e�g���~�m��HOLD����A�܂���HOLD�ƌ�������
		if (isHold) {
			if (CountMoveTime >= KEY_RESTAIN) {

				if (!canHold) {
					if (OnlyOnceHold) {
						for (height = 0; height < 4; height++) {
							for (width = 0; width < 6; width++) {
								HoldBoard[height][width] = 0;
							}
						}

						HoldTetrimino = CurrentTetrimino;

						TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = 0;
						TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = 0;
						TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = 0;
						TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = 0;

						CurrentTetrimino = TetriminoNext;
						TetriminoNext = TetriminoNext2;
						TetriminoNext2 = TetriminoNext3;
						TetriminoNext3 = rand() % 7;

						MovingTetrimino.x1 = tetrimino[CurrentTetrimino].x1;
						MovingTetrimino.y1 = tetrimino[CurrentTetrimino].y1;
						MovingTetrimino.x2 = tetrimino[CurrentTetrimino].x2;
						MovingTetrimino.y2 = tetrimino[CurrentTetrimino].y2;
						MovingTetrimino.x3 = tetrimino[CurrentTetrimino].x3;
						MovingTetrimino.y3 = tetrimino[CurrentTetrimino].y3;
						MovingTetrimino.x4 = tetrimino[CurrentTetrimino].x4;
						MovingTetrimino.y4 = tetrimino[CurrentTetrimino].y4;
						RotedBuffer.x1 = tetrimino[CurrentTetrimino].x1;
						RotedBuffer.y1 = tetrimino[CurrentTetrimino].y1;
						RotedBuffer.x2 = tetrimino[CurrentTetrimino].x2;
						RotedBuffer.y2 = tetrimino[CurrentTetrimino].y2;
						RotedBuffer.x3 = tetrimino[CurrentTetrimino].x3;
						RotedBuffer.y3 = tetrimino[CurrentTetrimino].y3;
						RotedBuffer.x4 = tetrimino[CurrentTetrimino].x4;
						RotedBuffer.y4 = tetrimino[CurrentTetrimino].y4;

						moveX = 5;
						moveY = 1;

						CreateBoard();
						OnlyOnceHold = false;
						isHold = false;
					}
					canHold = true;
				}

				if (canHold) {
					if (OnlyOnceHold) {
						for (height = 0; height < 4; height++) {
							for (width = 0; width < 6; width++) {
								HoldBoard[height][width] = 0;
							}
						}

						TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = 0;
						TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = 0;
						TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = 0;
						TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = 0;

						int tmp = CurrentTetrimino;
						CurrentTetrimino = HoldTetrimino;
						HoldTetrimino = tmp;

						MovingTetrimino.x1 = tetrimino[CurrentTetrimino].x1;
						MovingTetrimino.y1 = tetrimino[CurrentTetrimino].y1;
						MovingTetrimino.x2 = tetrimino[CurrentTetrimino].x2;
						MovingTetrimino.y2 = tetrimino[CurrentTetrimino].y2;
						MovingTetrimino.x3 = tetrimino[CurrentTetrimino].x3;
						MovingTetrimino.y3 = tetrimino[CurrentTetrimino].y3;
						MovingTetrimino.x4 = tetrimino[CurrentTetrimino].x4;
						MovingTetrimino.y4 = tetrimino[CurrentTetrimino].y4;
						RotedBuffer.x1 = tetrimino[CurrentTetrimino].x1;
						RotedBuffer.y1 = tetrimino[CurrentTetrimino].y1;
						RotedBuffer.x2 = tetrimino[CurrentTetrimino].x2;
						RotedBuffer.y2 = tetrimino[CurrentTetrimino].y2;
						RotedBuffer.x3 = tetrimino[CurrentTetrimino].x3;
						RotedBuffer.y3 = tetrimino[CurrentTetrimino].y3;
						RotedBuffer.x4 = tetrimino[CurrentTetrimino].x4;
						RotedBuffer.y4 = tetrimino[CurrentTetrimino].y4;

						moveX = 5;
						moveY = 1;

						CreateBoard();
						isHold = false;
						OnlyOnceHold = false;
					}
				}
				CountMoveTime = 0;
			}
			isHold = false;
		}

		//�uZ�v�Ńe�g���~�m����]
		if (isRotateL) {

			RotedBuffer.x1 = tetrimino[CurrentTetrimino].x1;
			RotedBuffer.y1 = tetrimino[CurrentTetrimino].y1;
			RotedBuffer.x2 = tetrimino[CurrentTetrimino].x2;
			RotedBuffer.y2 = tetrimino[CurrentTetrimino].y2;
			RotedBuffer.x3 = tetrimino[CurrentTetrimino].x3;
			RotedBuffer.y3 = tetrimino[CurrentTetrimino].y3;
			RotedBuffer.x4 = tetrimino[CurrentTetrimino].x4;
			RotedBuffer.y4 = tetrimino[CurrentTetrimino].y4;
			//��]�P
			int tmpL = RotedBuffer.x1;
			RotedBuffer.x1 = RotedBuffer.y1;
			RotedBuffer.y1 = -tmpL;
			tmpL = RotedBuffer.x2;
			RotedBuffer.x2 = RotedBuffer.y2;
			RotedBuffer.y2 = -tmpL;
			tmpL = RotedBuffer.x3;
			RotedBuffer.x3 = RotedBuffer.y3;
			RotedBuffer.y3 = -tmpL;
			tmpL = RotedBuffer.x4;
			RotedBuffer.x4 = RotedBuffer.y4;
			RotedBuffer.y4 = -tmpL;
			if (TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == 0 ||
				TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate1 = true;
			}
			else canRotate1 = false;
			if (TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == 0 ||
				TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate2 = true;
			}
			else canRotate2 = false;
			if (TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == 0 ||
				TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate3 = true;
			}
			else canRotate3 = false;
			if (TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == 0 ||
				TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate4 = true;
			}
			else canRotate4 = false;
			//��]2
			tmpL = RotedBuffer.x1;
			RotedBuffer.x1 = RotedBuffer.y1;
			RotedBuffer.y1 = -tmpL;
			tmpL = RotedBuffer.x2;
			RotedBuffer.x2 = RotedBuffer.y2;
			RotedBuffer.y2 = -tmpL;
			tmpL = RotedBuffer.x3;
			RotedBuffer.x3 = RotedBuffer.y3;
			RotedBuffer.y3 = -tmpL;
			tmpL = RotedBuffer.x4;
			RotedBuffer.x4 = RotedBuffer.y4;
			RotedBuffer.y4 = -tmpL;
			if (TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == 0 ||
				TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate5 = true;
			}
			else canRotate5 = false;

			if (TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == 0 ||
				TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate6 = true;
			}
			else canRotate6 = false;

			if (TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == 0 ||
				TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate7 = true;
			}
			else canRotate7 = false;

			if (TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == 0 ||
				TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate8 = true;
			}
			else canRotate8 = false;
			//��]3
			tmpL = RotedBuffer.x1;
			RotedBuffer.x1 = RotedBuffer.y1;
			RotedBuffer.y1 = -tmpL;
			tmpL = RotedBuffer.x2;
			RotedBuffer.x2 = RotedBuffer.y2;
			RotedBuffer.y2 = -tmpL;
			tmpL = RotedBuffer.x3;
			RotedBuffer.x3 = RotedBuffer.y3;
			RotedBuffer.y3 = -tmpL;
			tmpL = RotedBuffer.x4;
			RotedBuffer.x4 = RotedBuffer.y4;
			RotedBuffer.y4 = -tmpL;
			if (TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == 0 ||
				TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate9 = true;
			}
			else canRotate9 = false;
			if (TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == 0 ||
				TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate10 = true;
			}
			else canRotate10 = false;
			if (TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == 0 ||
				TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate11 = true;
			}
			else canRotate11 = false;
			if (TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == 0 ||
				TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate12 = true;
			}
			else canRotate12 = false;
			//��]4
			tmpL = RotedBuffer.x1;
			RotedBuffer.x1 = RotedBuffer.y1;
			RotedBuffer.y1 = -tmpL;
			tmpL = RotedBuffer.x2;
			RotedBuffer.x2 = RotedBuffer.y2;
			RotedBuffer.y2 = -tmpL;
			tmpL = RotedBuffer.x3;
			RotedBuffer.x3 = RotedBuffer.y3;
			RotedBuffer.y3 = -tmpL;
			tmpL = RotedBuffer.x4;
			RotedBuffer.x4 = RotedBuffer.y4;
			RotedBuffer.y4 = -tmpL;
			if (TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == 0 ||
				TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate13 = true;
			}
			else canRotate13 = false;
			if (TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == 0 ||
				TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate14 = true;
			}
			else canRotate14 = false;
			if (TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == 0 ||
				TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate15 = true;
			}
			else canRotate15 = false;
			if (TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == 0 ||
				TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate16 = true;
			}
			else canRotate16 = false;

			if (CountMoveTime >= KEY_RESTAIN) {
				if (canRotate1 &&
					canRotate2 &&
					canRotate3 &&
					canRotate4 &&
					canRotate5 &&
					canRotate6 &&
					canRotate7 &&
					canRotate8 &&
					canRotate9 &&
					canRotate10 &&
					canRotate11 &&
					canRotate12 &&
					canRotate13 &&
					canRotate14 &&
					canRotate15 &&
					canRotate16) {

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

					canRotate1 = false;
					canRotate2 = false;
					canRotate3 = false;
					canRotate4 = false;
					//canRotate5 = false;
					//canRotate6 = false;
					//canRotate7 = false;
					//canRotate8 = false;
					//canRotate9 = false;
					//canRotate10 = false;
					//canRotate11 = false;
					//canRotate12 = false;
					//canRotate13 = false;
					//canRotate14 = false;
					//canRotate15 = false;
					//canRotate16 = false;
				}
				CountMoveTime = 0;
			}
			isRotateL = false;
		}

		//�u���v�Ńe�g���~�m�E��]
		if (isRotateR) {

			RotedBuffer.x1 = tetrimino[CurrentTetrimino].x1;
			RotedBuffer.y1 = tetrimino[CurrentTetrimino].y1;
			RotedBuffer.x2 = tetrimino[CurrentTetrimino].x2;
			RotedBuffer.y2 = tetrimino[CurrentTetrimino].y2;
			RotedBuffer.x3 = tetrimino[CurrentTetrimino].x3;
			RotedBuffer.y3 = tetrimino[CurrentTetrimino].y3;
			RotedBuffer.x4 = tetrimino[CurrentTetrimino].x4;
			RotedBuffer.y4 = tetrimino[CurrentTetrimino].y4;
			//��]�P
			int tmpR = RotedBuffer.y1;
			RotedBuffer.y1 = RotedBuffer.x1;
			RotedBuffer.x1 = -tmpR;
			tmpR = RotedBuffer.y2;
			RotedBuffer.y2 = RotedBuffer.x2;
			RotedBuffer.x2 = -tmpR;
			tmpR = RotedBuffer.y3;
			RotedBuffer.y3 = RotedBuffer.x3;
			RotedBuffer.x3 = -tmpR;
			tmpR = RotedBuffer.y4;
			RotedBuffer.y4 = RotedBuffer.x4;
			RotedBuffer.x4 = -tmpR;
			if (TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == 0 ||
				TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate1 = true;
			}
			else canRotate1 = false;
			if (TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == 0 ||
				TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate2 = true;
			}
			else canRotate2 = false;
			if (TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == 0 ||
				TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate3 = true;
			}
			else canRotate3 = false;
			if (TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == 0 ||
				TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate4 = true;
			}
			else canRotate4 = false;
			//��]2
			tmpR = RotedBuffer.y1;
			RotedBuffer.y1 = RotedBuffer.x1;
			RotedBuffer.x1 = -tmpR;
			tmpR = RotedBuffer.y2;
			RotedBuffer.y2 = RotedBuffer.x2;
			RotedBuffer.x2 = -tmpR;
			tmpR = RotedBuffer.y3;
			RotedBuffer.y3 = RotedBuffer.x3;
			RotedBuffer.x3 = -tmpR;
			tmpR = RotedBuffer.y4;
			RotedBuffer.y4 = RotedBuffer.x4;
			RotedBuffer.x4 = -tmpR;
			if (TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == 0 ||
				TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate5 = true;
			}
			else canRotate5 = false;

			if (TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == 0 ||
				TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate6 = true;
			}
			else canRotate6 = false;

			if (TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == 0 ||
				TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate7 = true;
			}
			else canRotate7 = false;

			if (TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == 0 ||
				TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate8 = true;
			}
			else canRotate8 = false;
			//��]3
			tmpR = RotedBuffer.y1;
			RotedBuffer.y1 = RotedBuffer.x1;
			RotedBuffer.x1 = -tmpR;
			tmpR = RotedBuffer.y2;
			RotedBuffer.y2 = RotedBuffer.x2;
			RotedBuffer.x2 = -tmpR;
			tmpR = RotedBuffer.y3;
			RotedBuffer.y3 = RotedBuffer.x3;
			RotedBuffer.x3 = -tmpR;
			tmpR = RotedBuffer.y4;
			RotedBuffer.y4 = RotedBuffer.x4;
			RotedBuffer.x4 = -tmpR;
			if (TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == 0 ||
				TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate9 = true;
			}
			else canRotate9 = false;
			if (TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == 0 ||
				TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate10 = true;
			}
			else canRotate10 = false;
			if (TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == 0 ||
				TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate11 = true;
			}
			else canRotate11 = false;
			if (TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == 0 ||
				TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate12 = true;
			}
			else canRotate12 = false;
			//��]4
			tmpR = RotedBuffer.y1;
			RotedBuffer.y1 = RotedBuffer.x1;
			RotedBuffer.x1 = -tmpR;
			tmpR = RotedBuffer.y2;
			RotedBuffer.y2 = RotedBuffer.x2;
			RotedBuffer.x2 = -tmpR;
			tmpR = RotedBuffer.y3;
			RotedBuffer.y3 = RotedBuffer.x3;
			RotedBuffer.x3 = -tmpR;
			tmpR = RotedBuffer.y4;
			RotedBuffer.y4 = RotedBuffer.x4;
			RotedBuffer.x4 = -tmpR;
			if (TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == 0 ||
				TetrisField[RotedBuffer.y1 + moveY][RotedBuffer.x1 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate13 = true;
			}
			else canRotate13 = false;
			if (TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == 0 ||
				TetrisField[RotedBuffer.y2 + moveY][RotedBuffer.x2 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate14 = true;
			}
			else canRotate14 = false;
			if (TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == 0 ||
				TetrisField[RotedBuffer.y3 + moveY][RotedBuffer.x3 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate15 = true;
			}
			else canRotate15 = false;
			if (TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == 0 ||
				TetrisField[RotedBuffer.y4 + moveY][RotedBuffer.x4 + moveX] == tetrimino[CurrentTetrimino].color) {
				canRotate16 = true;
			}
			else canRotate16 = false;

			if (CountMoveTime >= KEY_RESTAIN) {
				if (canRotate1 &&
					canRotate2 &&
					canRotate3 &&
					canRotate4 &&
					canRotate5 &&
					canRotate6 &&
					canRotate7 &&
					canRotate8 &&
					canRotate9 &&
					canRotate10 &&
					canRotate11 &&
					canRotate12 &&
					canRotate13 &&
					canRotate14 &&
					canRotate15 &&
					canRotate16) {

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

					canRotate1 = false;
					canRotate2 = false;
					canRotate3 = false;
					canRotate4 = false;
					//canRotate5 = false;
					//canRotate6 = false;
					//canRotate7 = false;
					//canRotate8 = false;
					//canRotate9 = false;
					//canRotate10 = false;
					//canRotate11 = false;
					//canRotate12 = false;
					//canRotate13 = false;
					//canRotate14 = false;
					//canRotate15 = false;
					//canRotate16 = false;
				}
				CountMoveTime = 0;
			}
			isRotateR = false;
		}
		//�u���v�Ńe�g���~�m�������
		if (isLeft) {

			if (CountMoveTime >= KEY_RESTAIN) {
				if (TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX - 1] == 0 ||
					TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX - 1] == tetrimino[CurrentTetrimino].color) {
					canMoveLeft1 = true;
				}
				else {
					canMoveLeft1 = false;
				}
				if (TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX - 1] == 0 ||
					TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX - 1] == tetrimino[CurrentTetrimino].color) {
					canMoveLeft2 = true;
				}
				else {
					canMoveLeft2 = false;
				}
				if (TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX - 1] == 0 ||
					TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX - 1] == tetrimino[CurrentTetrimino].color) {
					canMoveLeft3 = true;
				}
				else {
					canMoveLeft3 = false;
				}
				if (TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX - 1] == 0 ||
					TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX - 1] == tetrimino[CurrentTetrimino].color) {
					canMoveLeft4 = true;
				}
				else {
					canMoveLeft4 = false;
				}
				if (canMoveLeft1 == true &&
					canMoveLeft2 == true &&
					canMoveLeft3 == true &&
					canMoveLeft4 == true) {
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
				CountMoveTime = 0;
			}

		}
		//�u���v�Ńe�g���~�m����E��
		if (isRight)
		{

			if (CountMoveTime >= KEY_RESTAIN) {
				if (TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX + 1] == 0 ||
					TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX + 1] == tetrimino[CurrentTetrimino].color) {
					canMoveRight1 = true;
				}
				else {
					canMoveRight1 = false;
				}
				if (TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX + 1] == 0 ||
					TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX + 1] == tetrimino[CurrentTetrimino].color) {
					canMoveRight2 = true;
				}
				else {
					canMoveRight2 = false;
				}
				if (TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX + 1] == 0 ||
					TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX + 1] == tetrimino[CurrentTetrimino].color) {
					canMoveRight3 = true;
				}
				else {
					canMoveRight3 = false;
				}
				if (TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX + 1] == 0 ||
					TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX + 1] == tetrimino[CurrentTetrimino].color) {
					canMoveRight4 = true;
				}
				else {
					canMoveRight4 = false;
				}


				if (canMoveRight1 == true &&
					canMoveRight2 == true &&
					canMoveRight3 == true &&
					canMoveRight4 == true)
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
				CountMoveTime = 0;
			}
		}



		//�u���v�Ńe�g���~�m�������
		if (isDown&&moveY > 1) {

			if (CountMoveTime >= KEY_RESTAIN) {
				if (TetrisField[MovingTetrimino.y1 + moveY + 1][MovingTetrimino.x1 + moveX] == 0 ||
					TetrisField[MovingTetrimino.y1 + moveY + 1][MovingTetrimino.x1 + moveX] == tetrimino[CurrentTetrimino].color) {
					canMoveDown1 = true;
				}
				else {
					canMoveDown1 = false;
				}
				if (TetrisField[MovingTetrimino.y2 + moveY + 1][MovingTetrimino.x2 + moveX] == 0 ||
					TetrisField[MovingTetrimino.y2 + moveY + 1][MovingTetrimino.x2 + moveX] == tetrimino[CurrentTetrimino].color) {
					canMoveDown2 = true;
				}
				else {
					canMoveDown2 = false;
				}
				if (TetrisField[MovingTetrimino.y3 + moveY + 1][MovingTetrimino.x3 + moveX] == 0 ||
					TetrisField[MovingTetrimino.y3 + moveY + 1][MovingTetrimino.x3 + moveX] == tetrimino[CurrentTetrimino].color) {
					canMoveDown3 = true;
				}
				else {
					canMoveDown3 = false;
				}
				if (TetrisField[MovingTetrimino.y4 + moveY + 1][MovingTetrimino.x4 + moveX] == 0 ||
					TetrisField[MovingTetrimino.y4 + moveY + 1][MovingTetrimino.x4 + moveX] == tetrimino[CurrentTetrimino].color) {
					canMoveDown4 = true;
				}
				else {
					canMoveDown4 = false;
				}

				if (canMoveDown1 == true &&
					canMoveDown2 == true &&
					canMoveDown3 == true &&
					canMoveDown4 == true) {

					TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = 0;
					TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = 0;
					TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = 0;
					TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = 0;

					moveY += 1;

					TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color;
					TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color;
					TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color;
					TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color;
					CountMoveTime = 0;
				}
				if (canMoveDown1 == false ||
					canMoveDown2 == false ||
					canMoveDown3 == false ||
					canMoveDown4 == false) {

					SetDown = true;

					TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color + 10;
					TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color + 10;
					TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color + 10;
					TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color + 10;
				}
			}
		}
		//�uspace�v�Ńe�g���~�m����ԉ���
		if (isDrop) {

			if (CountMoveTime >= MOVING_COUNTER) {
				int droploop;
				for (droploop = 0; droploop < 20; droploop++) {
					if (TetrisField[MovingTetrimino.y1 + moveY + 1][MovingTetrimino.x1 + moveX] == 0 ||
						TetrisField[MovingTetrimino.y1 + moveY + 1][MovingTetrimino.x1 + moveX] == tetrimino[CurrentTetrimino].color) {
						canMoveDrop1 = true;
					}
					else {
						canMoveDrop1 = false;
					}
					if (TetrisField[MovingTetrimino.y2 + moveY + 1][MovingTetrimino.x2 + moveX] == 0 ||
						TetrisField[MovingTetrimino.y2 + moveY + 1][MovingTetrimino.x2 + moveX] == tetrimino[CurrentTetrimino].color) {
						canMoveDrop2 = true;
					}
					else {
						canMoveDrop2 = false;
					}
					if (TetrisField[MovingTetrimino.y3 + moveY + 1][MovingTetrimino.x3 + moveX] == 0 ||
						TetrisField[MovingTetrimino.y3 + moveY + 1][MovingTetrimino.x3 + moveX] == tetrimino[CurrentTetrimino].color) {
						canMoveDrop3 = true;
					}
					else {
						canMoveDrop3 = false;
					}
					if (TetrisField[MovingTetrimino.y4 + moveY + 1][MovingTetrimino.x4 + moveX] == 0 ||
						TetrisField[MovingTetrimino.y4 + moveY + 1][MovingTetrimino.x4 + moveX] == tetrimino[CurrentTetrimino].color) {
						canMoveDrop4 = true;
					}
					else {
						canMoveDrop4 = false;
					}

					if (canMoveDrop1 == true &&
						canMoveDrop2 == true &&
						canMoveDrop3 == true &&
						canMoveDrop4 == true)
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
				if (canMoveDown1 == false ||
					canMoveDown2 == false ||
					canMoveDown3 == false ||
					canMoveDown4 == false)
				{
					SetDown = true;

					TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color + 10;
					TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color + 10;
					TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color + 10;
					TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color + 10;
				}
				CountMoveTime = 0;
				isDrop = false;
			}
		}

		//�����ŉ��ɍs������
		static int countFalling;
		countFalling++;
		if (countFalling >= 50) {
			if (TetrisField[MovingTetrimino.y1 + moveY + 1][MovingTetrimino.x1 + moveX] == 0 ||
				TetrisField[MovingTetrimino.y1 + moveY + 1][MovingTetrimino.x1 + moveX] == tetrimino[CurrentTetrimino].color) {
				canMoveDown1 = true;
			}
			else {
				canMoveDown1 = false;
			}
			if (TetrisField[MovingTetrimino.y2 + moveY + 1][MovingTetrimino.x2 + moveX] == 0 ||
				TetrisField[MovingTetrimino.y2 + moveY + 1][MovingTetrimino.x2 + moveX] == tetrimino[CurrentTetrimino].color) {
				canMoveDown2 = true;
			}
			else {
				canMoveDown2 = false;
			}
			if (TetrisField[MovingTetrimino.y3 + moveY + 1][MovingTetrimino.x3 + moveX] == 0 ||
				TetrisField[MovingTetrimino.y3 + moveY + 1][MovingTetrimino.x3 + moveX] == tetrimino[CurrentTetrimino].color) {
				canMoveDown3 = true;
			}
			else {
				canMoveDown3 = false;
			}
			if (TetrisField[MovingTetrimino.y4 + moveY + 1][MovingTetrimino.x4 + moveX] == 0 ||
				TetrisField[MovingTetrimino.y4 + moveY + 1][MovingTetrimino.x4 + moveX] == tetrimino[CurrentTetrimino].color) {
				canMoveDown4 = true;
			}
			else {
				canMoveDown4 = false;
			}

			if (canMoveDown1 == true &&
				canMoveDown2 == true &&
				canMoveDown3 == true &&
				canMoveDown4 == true) {

				TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = 0;
				TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = 0;
				TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = 0;
				TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = 0;

				moveY += 1;

				TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color;
				TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color;
				TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color;
				TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color;
				countFalling = 0;
			}
		}

		TetrisField[MovingTetrimino.y1 + moveY][MovingTetrimino.x1 + moveX] = tetrimino[CurrentTetrimino].color;
		TetrisField[MovingTetrimino.y2 + moveY][MovingTetrimino.x2 + moveX] = tetrimino[CurrentTetrimino].color;
		TetrisField[MovingTetrimino.y3 + moveY][MovingTetrimino.x3 + moveX] = tetrimino[CurrentTetrimino].color;
		TetrisField[MovingTetrimino.y4 + moveY][MovingTetrimino.x4 + moveX] = tetrimino[CurrentTetrimino].color;

		HoldBoard[tetrimino[HoldTetrimino].y1 + moveYtoCentral][tetrimino[HoldTetrimino].x1 + moveXtoCentral] = tetrimino[HoldTetrimino].color;
		HoldBoard[tetrimino[HoldTetrimino].y2 + moveYtoCentral][tetrimino[HoldTetrimino].x2 + moveXtoCentral] = tetrimino[HoldTetrimino].color;
		HoldBoard[tetrimino[HoldTetrimino].y3 + moveYtoCentral][tetrimino[HoldTetrimino].x3 + moveXtoCentral] = tetrimino[HoldTetrimino].color;
		HoldBoard[tetrimino[HoldTetrimino].y4 + moveYtoCentral][tetrimino[HoldTetrimino].x4 + moveXtoCentral] = tetrimino[HoldTetrimino].color;

		NextBoard[tetrimino[TetriminoNext].y1 + moveYtoCentral][tetrimino[TetriminoNext].x1 + moveXtoCentral] = tetrimino[TetriminoNext].color;
		NextBoard[tetrimino[TetriminoNext].y2 + moveYtoCentral][tetrimino[TetriminoNext].x2 + moveXtoCentral] = tetrimino[TetriminoNext].color;
		NextBoard[tetrimino[TetriminoNext].y3 + moveYtoCentral][tetrimino[TetriminoNext].x3 + moveXtoCentral] = tetrimino[TetriminoNext].color;
		NextBoard[tetrimino[TetriminoNext].y4 + moveYtoCentral][tetrimino[TetriminoNext].x4 + moveXtoCentral] = tetrimino[TetriminoNext].color;

		NextBoard2[tetrimino[TetriminoNext2].y1 + moveYtoCentral][tetrimino[TetriminoNext2].x1 + moveXtoCentral] = tetrimino[TetriminoNext2].color;
		NextBoard2[tetrimino[TetriminoNext2].y2 + moveYtoCentral][tetrimino[TetriminoNext2].x2 + moveXtoCentral] = tetrimino[TetriminoNext2].color;
		NextBoard2[tetrimino[TetriminoNext2].y3 + moveYtoCentral][tetrimino[TetriminoNext2].x3 + moveXtoCentral] = tetrimino[TetriminoNext2].color;
		NextBoard2[tetrimino[TetriminoNext2].y4 + moveYtoCentral][tetrimino[TetriminoNext2].x4 + moveXtoCentral] = tetrimino[TetriminoNext2].color;

		NextBoard3[tetrimino[TetriminoNext3].y1 + moveYtoCentral][tetrimino[TetriminoNext3].x1 + moveXtoCentral] = tetrimino[TetriminoNext3].color;
		NextBoard3[tetrimino[TetriminoNext3].y2 + moveYtoCentral][tetrimino[TetriminoNext3].x2 + moveXtoCentral] = tetrimino[TetriminoNext3].color;
		NextBoard3[tetrimino[TetriminoNext3].y3 + moveYtoCentral][tetrimino[TetriminoNext3].x3 + moveXtoCentral] = tetrimino[TetriminoNext3].color;
		NextBoard3[tetrimino[TetriminoNext3].y4 + moveYtoCentral][tetrimino[TetriminoNext3].x4 + moveXtoCentral] = tetrimino[TetriminoNext3].color;


	}

		//�e�g���~�m�����n�������ǂ���
		if (!canMoveDown1 ||
			!canMoveDown2 ||
			!canMoveDown3 ||
			!canMoveDown4)
		{
			SetDown = true;

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
			canMoveDown1 = true;
			canMoveDown2 = true;
			canMoveDown3 = true;
			canMoveDown4 = true;
		}


		if (!ChainEffectOn) {
			g_RiseCOUNTER = 0;
		}

		if (SetDown) {

			bool isAccorded = false;
			bool canupChain = false;
			static bool canClear = true;
			static bool CanCopy = true;
			static bool clear[10];
			static bool isDeleted = false;
			int clearbuff[4]{ 0,0,0,0 };
			static int chainHeight = 0;
			static int chainEffectHeight = 0;
			int clearRise;
			static int ChainCheck[10];
			int ChainCnt = 0;
			static int chainbuffer[10];
			static int RisenCount = 0;
			static int EffectRoop = 0;
			//�����������
			//�܂�����`�F�b�N���āA��񂪑S��TRUE�Ȃ炻�̗������
			if (CanCopy) {
				for (int Hcopy = 0; Hcopy < FIELD_HEIGHT; Hcopy++) {
					for (int Wcopy = 0; Wcopy < FIELD_WIDTH; Wcopy++) {
						FieldEffectBuff[Hcopy][Wcopy] = TetrisField[Hcopy][Wcopy];
					}
				}
			}
			if(canClear)
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

					isAccorded = true;
					canClear = false;
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
						//canupChain = true;
					}
					
				}//isAccorded true
			}
			if (EffectOn) {
				
				EffectControl(FieldEffectBuff);
				EffectRoop++;
				if (EffectRoop > EFFECTTIME) {//���[�v�������Ƃ��ɗ����������Ȃ��o�O����
					EffectRoop = 0;
				}
				if (!EffectRoop) {
					canupChain = true;
					
					EffectOn = false;

				}
			}
			if (!EffectOn) {
				bool isup = false;
				int ChainCheckCnt = 0;
				if (canupChain) {
					for (ChainCheckCnt = 10; ChainCheckCnt > 0; ChainCheckCnt--) {
						if ((TetrisField[chainHeight][ChainCheckCnt] - ChainCheck[ChainCheckCnt - 1] == 1)
							|| (TetrisField[chainHeight][ChainCheckCnt] - ChainCheck[ChainCheckCnt - 1] == -3)) {
							ChainEffectOn = true;
							//�������p�̃o�b�t�@�ɏ����F������
							chainbuffer[ChainCheckCnt - 1] = TetrisField[chainHeight][ChainCheckCnt];
							RisenCount++;
							//������ׂė��Ƃ�
							for (int checkup = chainHeight; checkup > 0; checkup--) {
								TetrisField[checkup][ChainCheckCnt] = TetrisField[checkup - 1][ChainCheckCnt];
							}
							TetrisField[0][ChainCheckCnt] = 0;
						}
						else continue;
						//�A������
							//�A���c����
						for (int UpperCheck = chainHeight; UpperCheck > 0; UpperCheck--) {
							if ((TetrisField[UpperCheck][ChainCheckCnt] - chainbuffer[ChainCheckCnt - 1] == 1
								|| TetrisField[UpperCheck][ChainCheckCnt] - chainbuffer[ChainCheckCnt - 1] == -3)) {

								if ((chainHeight - UpperCheck > 2) && (TetrisField[UpperCheck][ChainCheckCnt] == chainbuffer[ChainCheckCnt - 1])) {
									isup = true;
									RisenCount++;
									chainbuffer[ChainCheckCnt - 1] = TetrisField[chainHeight][ChainCheckCnt];
									UpperCheck++;
									for (int checkup = chainHeight; checkup > 0; checkup--) {
										TetrisField[checkup][ChainCheckCnt] = TetrisField[checkup - 1][ChainCheckCnt];
									}
									TetrisField[0][ChainCheckCnt] = 0;
								}
								else {
									isup = true;
									RisenCount++;
									UpperCheck++;
									chainbuffer[ChainCheckCnt - 1] = TetrisField[chainHeight][ChainCheckCnt];

									for (int checkup = chainHeight; checkup > 0; checkup--) {
										TetrisField[checkup][ChainCheckCnt] = TetrisField[checkup - 1][ChainCheckCnt];
									}
									TetrisField[0][ChainCheckCnt] = 0;

								}

								//���̊m�F
								//	�񂩂�3�i�ڈȏ�Ȃ�Ή�������
								if (chainHeight - UpperCheck > 2) {
									for (int checkHorizontally = ChainCheckCnt; checkHorizontally > FIELD_WIDTH - 2; checkHorizontally++) {
										if (TetrisField[chainHeight][ChainCheckCnt] == TetrisField[chainHeight][checkHorizontally]) {
											for (int checkup = 1; checkup == chainHeight; checkup++) {
												TetrisField[chainHeight - checkup + 1][checkHorizontally] = TetrisField[chainHeight - checkup][checkHorizontally];
												RisenCount++;
											}
										}
									}
									for (int checkHorizontally = ChainCheckCnt; checkHorizontally < 1; checkHorizontally--) {
										if (TetrisField[chainHeight][ChainCheckCnt] == TetrisField[chainHeight][checkHorizontally]) {
											for (int checkup = 1; checkup == chainHeight; checkup++) {
												TetrisField[chainHeight - checkup + 1][checkHorizontally] = TetrisField[chainHeight - checkup][checkHorizontally];
												RisenCount++;
											}
										}
									}
								}
							}
						}
						isup = false;
					}
					g_RiseCOUNTER = RisenCount;
				}
			}
			if (ChainEffectOn) {
				//static int effectroop = 0;
				ChainEffect(FieldEffectBuff/* chainEffectHeight*/, ChainCheck);
				EffectRoop++;
				if (EffectRoop > EFFECTTIME) {
					EffectRoop = 0;
				}
				if (!EffectRoop) {
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
			
			if (!EffectOn&&!ChainEffectOn) {
				//��A����1.25�{�A�A�������邲�Ƃ�0.25�����ꂽ���ɂȂ� �X�R�A*(1+0.25*�A����
				switch (ClearLineCOUNTER) {
				case 1:
					g_ScorePoint += 100 * (1 + RisenCount * 0.25);
					break;
				case 2:
					g_ScorePoint += 300 * (1 + RisenCount * 0.25);
					break;
				case 3:
					g_ScorePoint += 500 * (1 + RisenCount * 0.25);
					break;
				case 4:
					g_ScorePoint += 800 * (1 + RisenCount * 0.25);
					break;
				}

				RisenCount = 0;
				isCreated = false;
				if (GameOver) {
					isCreated = true;
				}
				CanCopy = true;
				SetDown = false;
				canClear = true;
				OnlyOnceHold = true;
				ClearLineCOUNTER = 0;
				for (int Hcopy = 0; Hcopy < FIELD_HEIGHT; Hcopy++) {
					for (int Wcopy = 0; Wcopy < FIELD_WIDTH; Wcopy++) {
						FieldEffectBuff[Hcopy][Wcopy] = 0;
					}
				}
			}
		}
	}
			//GAME OVER�̏���//
		
			
	static int CheckGOHeight;
	static int CheckGOWidth;
	for (CheckGOHeight = 1; CheckGOHeight < FIELD_HEIGHT - 1; CheckGOHeight++) {
		for (CheckGOWidth = 1; CheckGOWidth < FIELD_WIDTH - 1; CheckGOWidth++) {

			for (int BlockCheck = 1; BlockCheck < 8; BlockCheck++) {
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
					
					canHold = false;
					isCreated = true;
					GameOver = true;
					GameScene = false;
					GAMEOPEN = true;
					
				}
			}
		}
	}
}