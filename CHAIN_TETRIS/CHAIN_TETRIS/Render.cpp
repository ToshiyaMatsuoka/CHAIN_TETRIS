#include "Render.h"
#include "TetriminoState.h"
#include "Gamescene.h"
#include "TetrisEffect.h"

//サブボード
#define SUBBOARD_LX 200.f
#define SUBBOARD_RX 1000.f
#define NEXTBOARD_Y 100.f
#define NEXT2BOARD_Y 250.f
#define NEXT3BOARD_Y  400.f
#define HOLDBOARD_Y 100.f
#define SCOREBOARD_Y 300.f
//カラーセット
#define MAGENTA 0xffff00ff
#define CYAN 0xff00ffff
#define ORANGE 0xffffa500
#define BLACK 0x60000000
#define WHITE 0x00ffffff
#define INVISIBLE 0x00ffffff
#define BASEED 0xffffffff
//フィールド左端
#define FieldInitPosX    500.f
#define FieldInitPosY    -64.f
//ブロック
#define BLOCK_HARFWIDTH 16.f

OBJECT_STATE Block;
TEXTURE_STATE g_Girl = { 1000.f,450.f,150.f };
TEXTURE_STATE Logo = { 320.f, -300.f, 100.f };//ロゴマークの初期位置
void TEXTWRITTEN();
static DWORD BlockColor;
static char debug[512]{'\0'};

int g_scene = 0;
void ResultRender(void);
void SetBoardTextureID(int Board, int i, int j, TEXTURE* textureID);
void SetFieldTextureID(int Board, int i, int j, TEXTURE* textureID);

/**
*描画処理
*/
void Render() {
	
	TEXTURE textureID =BLOCK_BRUNK_TEX;
	//-----------------------------------------------------
	//頂点情報を入れる--------------------------------------	
	CUSTOMVERTEX backGround[4]
	{
	{   0.f,   0.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ WIDTH,   0.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ WIDTH,HEIGHT, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{   0.f,HEIGHT, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX backGroundMain[4]
	{
	{ 0.f,   0.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ WIDTH,   0.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ WIDTH,HEIGHT, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 0.f,HEIGHT, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX tetrisStage[4]
	{
	{ FieldInitPosX - (BLOCK_HARFWIDTH * 2) - 5,			  -50,													1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ FIELD_WIDTH * (BLOCK_HARFWIDTH * 2) + FieldInitPosX + 5,-50,													1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ FIELD_WIDTH * (BLOCK_HARFWIDTH * 2) + FieldInitPosX + 5,FIELD_HEIGHT * (BLOCK_HARFWIDTH * 2) + FieldInitPosY, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ FieldInitPosX - (BLOCK_HARFWIDTH * 2) - 5,			  FIELD_HEIGHT * (BLOCK_HARFWIDTH * 2) + FieldInitPosY, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX logo[4]
	{	
	{ Logo.x - Logo.scale,			Logo.y - Logo.scale,         1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ Logo.x + Logo.scale + 800.f,  Logo.y - Logo.scale,         1.f, 1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ Logo.x + Logo.scale + 800.f,  Logo.y + Logo.scale + 400.f, 1.f, 1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ Logo.x - Logo.scale,			Logo.y + Logo.scale + 400.f, 1.f, 1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX start[4]
	{
	{ 210.f,   460.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 770.f,   460.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 770.f, 620.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 210.f, 620.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX CAHINCOLOR[4]
	{
	{ 100.f,  400.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 400.f,  400.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 400.f, 700.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 100.f, 700.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX backGroundPause[4]
	{
	{ 0.0f, 0.0f, 1.f, 1.f, D3DCOLOR_ARGB(150,0,0,0) , 0.f, 0.f },
	{ WIDTH, 0.0f, 1.f, 1.f, D3DCOLOR_ARGB(150,0,0,0) , 1.f, 0.f },
	{ WIDTH, HEIGHT, 1.f, 1.f,D3DCOLOR_ARGB(150,0,0,0) , 1.f, 1.f },
	{ 0.0f, HEIGHT, 1.f, 1.f,D3DCOLOR_ARGB(150,0,0,0) , 1.f, 1.f },
	};

	//画面の消去
	g_pD3Device->Clear(0, NULL,	D3DCLEAR_TARGET,D3DCOLOR_XRGB(0x00, 0x00, 0x00),1.0, 0);
	//描画の開始
	g_pD3Device->BeginScene();

	if (g_scene == STATE_TITLE)
	{
		DrowTexture(backGround, BG_MAIN_TEX);

		LogoControl();

		DrowTexture(logo, LOGO_TEX);

		static int count = 0;
		count++;
		if (count > 30){
			DrowTexture(start, START_TEX);
			if (count == 60){
				count = 0;
			}
		}
	}

if (g_scene == STATE_RESULT){
		TEXTURE_STATE Logo = { 230.f, -300.f, 100.f };
		ResultRender();
	}

if (g_scene == STATE_MAIN) {
	//背景設置
	DrowTexture(backGroundMain, BG_MAIN_TEX);
	DrowTexture(tetrisStage, STAGE_TEX);

	//PLAYFIELDの設置
	if (!EffectOn&&!ChainEffectOn) {
		for (int i = 0; i < FIELD_HEIGHT; i++) {
			for (int j = 0; j < FIELD_WIDTH; j++) {
				SetFieldTextureID(TetrisField[i][j], i, j, &textureID);
				OBJECT_STATE Block = { j * (BLOCK_HARFWIDTH * 2) + FieldInitPosX, i * (BLOCK_HARFWIDTH * 2) + FieldInitPosY, BLOCK_HARFWIDTH };
				CUSTOMVERTEX BLOCK_VTX[4]
				{
				{ Block.x - Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 0.f, 0.f },
				{ Block.x + Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 1.f, 0.f },
				{ Block.x + Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 1.f, 1.f },
				{ Block.x - Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 0.f, 1.f }
				};
				DrowTexture(BLOCK_VTX, textureID);
			}
		}
	}
	//EFFECT
	if (EffectOn|| ChainEffectOn) {
		for (int i = 0; i < FIELD_HEIGHT; i++) {
			for (int j = 0; j < FIELD_WIDTH; j++) {
				SetFieldTextureID(FieldEffectBuff[i][j], i, j, &textureID);
				OBJECT_STATE Block = { j * (BLOCK_HARFWIDTH * 2) + FieldInitPosX, i * (BLOCK_HARFWIDTH * 2) + FieldInitPosY, BLOCK_HARFWIDTH };
				CUSTOMVERTEX BLOCK_VTX[4]
				{
				{ Block.x - Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 0.f, 0.f },
				{ Block.x + Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 1.f, 0.f },
				{ Block.x + Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 1.f, 1.f },
				{ Block.x - Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 0.f, 1.f }
				};
				DrowTexture(BLOCK_VTX, textureID);
			}
		}
	}
	//サブボード設置
	//HOLD
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			SetBoardTextureID(HoldBoard[i][j], i, j, &textureID);
			Block = { j *  (BLOCK_HARFWIDTH * 2) + SUBBOARD_LX, i *  (BLOCK_HARFWIDTH * 2) + HOLDBOARD_Y, BLOCK_HARFWIDTH };
			CUSTOMVERTEX BLOCK02_VTX[4]
			{
			{ Block.x - Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 0.f, 0.f },
			{ Block.x + Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 1.f, 0.f },
			{ Block.x + Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 1.f, 1.f },
			{ Block.x - Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 0.f, 1.f }
			};
			DrowTexture(BLOCK02_VTX, textureID);
		}
	}
	//Next　First
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			SetBoardTextureID(NextBoard[i][j], i, j, &textureID);

			Block = { j * (BLOCK_HARFWIDTH * 2) + SUBBOARD_RX, i * (BLOCK_HARFWIDTH * 2) + NEXTBOARD_Y, BLOCK_HARFWIDTH };
			CUSTOMVERTEX BLOCK02_VTX[4]
			{
			{ Block.x - Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 0.f, 0.f },
			{ Block.x + Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 1.f, 0.f },
			{ Block.x + Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 1.f, 1.f },
			{ Block.x - Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 0.f, 1.f }
			};
			DrowTexture(BLOCK02_VTX, textureID);
		}
	}
	//Next　Second
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			SetBoardTextureID(NextBoard2[i][j], i, j, &textureID);

			Block = { j * (BLOCK_HARFWIDTH * 2) + SUBBOARD_RX, i * (BLOCK_HARFWIDTH * 2) + NEXT2BOARD_Y, BLOCK_HARFWIDTH };
			CUSTOMVERTEX BLOCK03_VTX[4]
			{
			{ Block.x - Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 0.f, 0.f },
			{ Block.x + Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 1.f, 0.f },
			{ Block.x + Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 1.f, 1.f },
			{ Block.x - Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 0.f, 1.f }
			};
			DrowTexture(BLOCK03_VTX, textureID);
		}
	}
	//Next　Third
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			SetBoardTextureID(NextBoard3[i][j], i, j, &textureID);

			Block = { j *  (BLOCK_HARFWIDTH * 2) + SUBBOARD_RX, i * (BLOCK_HARFWIDTH * 2) + NEXT3BOARD_Y, BLOCK_HARFWIDTH };
			CUSTOMVERTEX BLOCK04_VTX[4]
			{
			{ Block.x - Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 0.f, 0.f },
			{ Block.x + Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 1.f, 0.f },
			{ Block.x + Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 1.f, 1.f },
			{ Block.x - Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 0.f, 1.f }
			};
			DrowTexture(BLOCK04_VTX, textureID);
		}
	}
	//Score
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			SetBoardTextureID(ScoreBoard[i][j], i, j, &textureID);
			Block = { j * (BLOCK_HARFWIDTH * 2) + SUBBOARD_LX, i *  (BLOCK_HARFWIDTH * 2) + SCOREBOARD_Y, BLOCK_HARFWIDTH };
			CUSTOMVERTEX BLOCK04_VTX[4]
			{
			{ Block.x - Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 0.f, 0.f },
			{ Block.x + Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 1.f, 0.f },
			{ Block.x + Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 1.f, 1.f },
			{ Block.x - Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 0.f, 1.f }
			};
			DrowTexture(BLOCK04_VTX, textureID);
		}
	}
	DrowTexture(CAHINCOLOR, CHAIN_COLOR_TEX);
	if (GameStop){
		DrowTexture(backGroundPause, PAUSE_TEX);
	}
	TEXTWRITTEN();
}
	//描画の終了
	g_pD3Device->EndScene();
	//表示
	g_pD3Device->Present(NULL, NULL, NULL, NULL);
}

void ResultRender(void){
	//頂点情報を入れる--------------------------------------
	CUSTOMVERTEX result[4]{
	{ 0.0f, 0.0f, 1.f, 1.f, 0xFFFFFFFF , 0.f, 0.f },
	{ 1280.f, 0.0f, 1.f, 1.f, 0xFFFFFFFF , 1.f, 0.f },
	{ 1280.f, 720.f, 1.f, 1.f,0xFFFFFFFF , 1.f, 1.f },
	{ 0.0f, 720.f, 1.f, 1.f, 0xFFFFFFFF , 0.f, 1.f },
	};

	CUSTOMVERTEX resultgirl[4]{
	{ g_Girl.x - g_Girl.scale, g_Girl.y - g_Girl.scale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ g_Girl.x + g_Girl.scale, g_Girl.y - g_Girl.scale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ g_Girl.x + g_Girl.scale, g_Girl.y + g_Girl.scale, 1.f, 1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ g_Girl.x - g_Girl.scale, g_Girl.y + g_Girl.scale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 1.f }
	};

	RECT ResultScore = { 240,315,900,500 };

	//-----------------------------------------------------
	//描画の開始
	DrowTexture(result, BG_RESULT_TEX);
	DrowTexture(resultgirl, RESULTGIRL_TEX);
	char Score[255];
	sprintf_s(Score, 255, "%d", NowScorePoint);
	DrowWord(ResultScore, Score, ResultScore_FONT, DT_CENTER,D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00));
}
void TEXTWRITTEN() {
	char Score[255];
	char chaincnt[64];
	sprintf_s(chaincnt, 64, "%d CHAIN!", g_RiseCOUNTER);

	sprintf_s(Score, 255, "%d", g_ScorePoint);

	RECT LOGO = {
		100,		// 左上のx座標
		500,		// 左上のy座標
		600,		// 右下のx座標
		900			// 右下のy座標
	};
	RECT HOLDBOARD = {
		SUBBOARD_LX - BLOCK_HARFWIDTH,		// 左上のx座標
		HOLDBOARD_Y - BLOCK_HARFWIDTH,		// 左上のy座標
		SUBBOARD_LX + 200,		// 右下のx座標
		HOLDBOARD_Y + 600			// 右下のy座標
	};
	RECT NEXTBOARD = {
		SUBBOARD_RX - BLOCK_HARFWIDTH,		// 左上のx座標
		NEXTBOARD_Y - BLOCK_HARFWIDTH,		// 左上のy座標
		SUBBOARD_RX + 200,		// 右下のx座標
		NEXTBOARD_Y + 600			// 右下のy座標
	};
	RECT NEXT2BOARD = {
		SUBBOARD_RX - BLOCK_HARFWIDTH,		// 左上のx座標
		NEXT2BOARD_Y - BLOCK_HARFWIDTH,		// 左上のy座標
		SUBBOARD_RX + 200,		// 右下のx座標
		NEXT2BOARD_Y + 600			// 右下のy座標
	};
	RECT NEXT3BOARD = {
		SUBBOARD_RX - BLOCK_HARFWIDTH,		// 左上のx座標
		NEXT3BOARD_Y - BLOCK_HARFWIDTH,		// 左上のy座標
		SUBBOARD_RX + 200,		// 右下のx座標
		NEXT3BOARD_Y + 600			// 右下のy座標
	};
	RECT SCOREBOARD = {
		SUBBOARD_LX - BLOCK_HARFWIDTH,		// 左上のx座標
		SCOREBOARD_Y - 15,		// 左上のy座標
		SUBBOARD_LX + 200,		// 右下のx座標
		SCOREBOARD_Y + 600			// 右下のy座標
	};
	RECT DrawnSCORE = {
		SUBBOARD_LX - BLOCK_HARFWIDTH / 4,		// 左上のx座標
		SCOREBOARD_Y + BLOCK_HARFWIDTH * 2,			// 左上のy座標
		SUBBOARD_LX + 180 - BLOCK_HARFWIDTH,	// 右下のx座標
		SCOREBOARD_Y + 250 - BLOCK_HARFWIDTH	// 右下のy座標
	};
	RECT GAMEOVER = {
		150,	// 左上のx座標
		180,	// 左上のy座標
		1150,	// 右下のx座標
		600		// 右下のy座標
	};
	RECT DrawnPAUSE = {
		400,	// 左上のx座標
		220,	// 左上のy座標
		1150,	// 右下のx座標
		600		// 右下のy座標
	};
	RECT DrawnPAUSE_B = {
		380,	// 左上のx座標
		420,	// 左上のy座標
		1150,	// 右下のx座標
		600		// 右下のy座標
	};

	DrowWord(HOLDBOARD, ("HOLD"), SUBBOARD_FONT);
	DrowWord(NEXTBOARD, ("NEXT"), SUBBOARD_FONT);
	DrowWord(NEXT2BOARD, ("Second"), SUBBOARD_FONT);
	DrowWord(NEXT3BOARD, ("Third"), SUBBOARD_FONT);
	DrowWord(SCOREBOARD, ("SCORE"), SUBBOARD_FONT);
	DrowWord(DrawnSCORE, Score, Score_FONT, DT_RIGHT);
	if (GameStop)
	{
		DrowWord(DrawnPAUSE, ("PAUSE"), PAUSE_FONT, DT_LEFT);
		DrowWord(DrawnPAUSE_B, ("PUSH Enter or Start"), PAUSE2_FONT, DT_LEFT);
	}

	if (GameOver) {
		DrowWord(GAMEOVER,("GAME OVER"), GO_FONT, DT_CENTER, D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00));
	}
	if (g_RiseCOUNTER&&ChainEffectOn) {
		DrowWord(GAMEOVER, chaincnt,GO_FONT, DT_CENTER, D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00));
	}
#ifdef _DEBUG
	float PutDebugX = 0;
	float PutDebugY = 0;
	for (int height = 0; height < FIELD_HEIGHT; height++) {
		for (int width = 0; width < FIELD_WIDTH; width++) {
			PutDebugX = width * 25;
			PutDebugY = height * 20;
			sprintf_s(debug, 512, "%d", TetrisField[height][width]);
			RECT Debug = {
				PutDebugX + 20,	// 左上のx座標
				PutDebugY,	// 左上のy座標
				1200,	// 右下のx座標
				700		// 右下のy座標
			};
			DrowWord(Debug, debug, SUBBOARD_FONT, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0xff));
		}
	}
	RECT DebugCHAINCNT{
	1000,
	500,
	1200,
	700
	};
	DrowWord(DebugCHAINCNT, chaincnt, MAIN_FONT, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x5f, 0x5f, 0x5f));
#endif // DEBUG
}
void DrowTexture(CUSTOMVERTEX vertex[4],int textureNumber) {
	g_pD3Device->SetTexture(0, g_pTexture[textureNumber]);
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertex, sizeof(CUSTOMVERTEX));
}
void DrowWord(RECT rect,LPCSTR text,int FontNumber,int TextFormat,DWORD color) {
	g_pFont[FontNumber]->DrawText(
		NULL,		// NULL
		text,		// 描画テキスト
		-1,			// 全て表示
		&rect,		// 表示範囲
		TextFormat,	// 左寄せ
		color		// color
	);
}
void SetBoardTextureID(int Board,int i,int j,TEXTURE* textureID) {
	switch (Board)
	{
	case 0:
		*textureID = BORD_TEX;
		BlockColor = BLACK;
		break;
	case 1:
	case 11:
		*textureID = BLOCK_BRUNK_TEX;
		BlockColor = BASEED;
		break;
	case 2:
	case 12:
		*textureID = BLOCK_BRUNK_TEX;
		BlockColor = BASEED;
		break;
	case 3:
	case 13:
		*textureID = BLOCK_BRUNK_TEX;
		BlockColor = BASEED;
		break;
	case 4:
	case 14:
		*textureID = BLOCK_BRUNK_TEX;
		BlockColor = BASEED;
		break;
	case 5:
	case 15:
		*textureID = UNITS_TEX;
		BlockColor = MAGENTA;
		break;
	case 6:
	case 16:
		*textureID = EFFECT_TEX;
		BlockColor = CYAN;
		break;
	case 7:
	case 17:
		*textureID = UNITS_TEX;
		BlockColor = ORANGE;
		break;
	case 8:
	case 10:
		*textureID = UNITS_TEX;
		BlockColor = WHITE;
		break;
	default:
		*textureID = UNITS_TEX;
		BlockColor = INVISIBLE;
		break;
	}
}
void SetFieldTextureID(int Board, int i, int j, TEXTURE* textureID) {
	switch (Board)
	{
	case 0:
		*textureID = UNITS_TEX;
		BlockColor = BLACK;
		break;
	case 1:
	case 11:
		*textureID = BLOCK_RED_TEX;
		BlockColor = BASEED;
		break;
	case 2:
	case 12:
		*textureID = BLOCK_BLUE_TEX;
		BlockColor = BASEED;
		break;
	case 3:
	case 13:
		*textureID = BLOCK_GREEN_TEX;
		BlockColor = BASEED;
		break;
	case 4:
	case 14:
		*textureID = BLOCK_YELLOW_TEX;
		BlockColor = BASEED;
		break;
	case 5:
	case 15:
		*textureID = UNITS_TEX;
		BlockColor = MAGENTA;
		break;
	case 6:
	case 16:
		*textureID = EFFECT_TEX;
		BlockColor = CYAN;
		break;
	case 7:
	case 17:
		*textureID = UNITS_TEX;
		BlockColor = ORANGE;
		break;
	case 8:
	case 10:
		*textureID = UNITS_TEX;
		BlockColor = WHITE;
		break;
	default:
		*textureID = UNITS_TEX;
		BlockColor = INVISIBLE;
		break;
	}

}