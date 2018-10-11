#include "Main.h"
#include "Render.h"
#include "Control.h"
#include "DX9.h"
#include "TetriminoState.h"
#include "Gamescene.h"

void Gamescene() {
	if (!GameStop) {
		if (GAMEOPEN) {
			CreateField();
			SelectTetrimino();
			CreateField();
			GameOver = false;
			GameScene = true;
			GAMEOPEN = false;
			g_ScorePoint = 0;
			NowScorePoint = 0;
		}
		ControlTetris();
	}
}