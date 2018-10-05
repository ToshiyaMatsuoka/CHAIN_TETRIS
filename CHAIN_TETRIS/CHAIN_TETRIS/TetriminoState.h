#ifndef TETRIMINO_STATE_H
#define TETRIMINO_STATE_H
#include "Main.h"
#include "Render.h"
#include "Control.h"
#include "DX9.h"


struct TETRIMINO {

	int x1, y1, x2, y2, x3, y3, x4, y4, color;

};


extern TETRIMINO tetrimino[8];

extern int TetriminoColer;

extern int TetrisField[FIELD_HEIGHT][FIELD_WIDTH];

extern int NextBoard[4][6];
extern int NextBoard2[4][6];
extern int NextBoard3[4][6];
extern int HoldBoard[4][6];
extern int ScoreBoard[4][6];
extern int CurrentTetrimino, TetriminoNext, TetriminoNext2, TetriminoNext3, HoldTetrimino;
extern int moveY;
extern int moveX;

extern bool EffectOn;
extern bool ChainEffectOn;
extern bool GameScene;
extern bool GameOver;
extern bool GAMEOPEN;

extern int g_ScorePoint;
extern int g_RiseCOUNTER;


void CreateField(void);
void CreateBoard(void);
void SelectTetrimino(void);
void ControlTetris(void);


#endif // !TETRIMINO_STATE_H