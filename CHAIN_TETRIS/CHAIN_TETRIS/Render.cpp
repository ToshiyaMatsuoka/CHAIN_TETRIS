#include "Render.h"
#include "TetriminoState.h"
#include "Gamescene.h"
#include "TetrisEffect.h"

OBJECT_STATE Block;
TEXTURE_STATE g_Girl = { 1000.f,450.f,150.f };
TEXTURE_STATE Logo = { 320.f, -300.f, 100.f };//ロゴマークの初期位置
void TEXTWRITTEN();
int BlockColor;
static char debug[512]{'\0'};
char Score[255];
int g_scene = 0;
void ResultRender(void);

/**
*描画処理
*/
void Render() {
	
	static int i = 0;
	static int j = 0;
	TEXTURE textureID;
	//-----------------------------------------------------
	//頂点情報を入れる--------------------------------------

	//STAGE_STATE StagePosition{ 640.f,360.f,200.f,250.f };
	
	
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

	//CUSTOMVERTEX TetorisBlock[4]
	//{
	//{ BlockPosition.x - BLOCK_HARFWIDTH*5,BlockPosition.y - BLOCK_HARFWIDTH*5, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	//{ BlockPosition.x + BLOCK_HARFWIDTH*5,BlockPosition.y - BLOCK_HARFWIDTH*5, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	//{ BlockPosition.x + BLOCK_HARFWIDTH*5,BlockPosition.y + BLOCK_HARFWIDTH*5, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	//{ BlockPosition.x - BLOCK_HARFWIDTH*5,BlockPosition.y + BLOCK_HARFWIDTH*5, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	//};

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
	g_pD3Device->Clear(0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0x00, 0x00, 0x00),
		1.0, 0);

	//描画の開始
	g_pD3Device->BeginScene();


	//背景
	//キャラ
	//UI
	//文字
	if (g_scene == STATE_TITLE)
	{
		g_pD3Device->SetTexture(0, g_pTexture[BG_MAIN_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, backGround, sizeof(CUSTOMVERTEX));

		LogoControl();

		g_pD3Device->SetTexture(0, g_pTexture[LOGO_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2,logo, sizeof(CUSTOMVERTEX));

		static int count = 0;
		count = count++;
		if (count > 30){
			g_pD3Device->SetTexture(0, g_pTexture[START_TEX]);
			g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, start, sizeof(CUSTOMVERTEX));
			if (count == 60)
			{
				count = 0;
			}
		}

	}

if (g_scene == STATE_RESULT)
	{
		TEXTURE_STATE Logo = { 230.f, -300.f, 100.f };
		ResultRender();
	}

if (g_scene == STATE_MAIN) {
	//背景設置
	g_pD3Device->SetTexture(0, g_pTexture[BG_MAIN_TEX]);
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, backGroundMain, sizeof(CUSTOMVERTEX));

	g_pD3Device->SetTexture(0, g_pTexture[STAGE_TEX]);
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, tetrisStage, sizeof(CUSTOMVERTEX));

	//PLAYFIELDの設置
	if (!EffectOn&&!ChainEffectOn) {
		for (i = 0; i < FIELD_HEIGHT; i++) {
			for (j = 0; j < FIELD_WIDTH; j++) {

				switch (TetrisField[i][j])
				{
				case 0:
					textureID = UNITS_TEX;
					BlockColor = BLACK;
					break;
				case 1:
				case 11:
					textureID = BLOCK_RED_TEX;
					BlockColor = BASEED;
					break;
				case 2:
				case 12:
					textureID = BLOCK_BLUE_TEX;
					BlockColor = BASEED;
					break;
				case 3:
				case 13:
					textureID = BLOCK_GREEN_TEX;
					BlockColor = BASEED;
					break;
				case 4:
				case 14:
					textureID = BLOCK_YELLOW_TEX;
					BlockColor = BASEED;
					break;
				case 5:
				case 15:
					textureID = UNITS_TEX;
					BlockColor = MAGENTA;
					break;
				case 6:
				case 16:
					textureID = UNITS_TEX;
					BlockColor = CYAN;
					break;
				case 7:
				case 17:
					textureID = UNITS_TEX;
					BlockColor = ORANGE;
					break;
				case 8:
					textureID = UNITS_TEX;
					BlockColor = INVISIBLE;
					break;
				default:
					textureID = UNITS_TEX;
					BlockColor = INVISIBLE;
					break;
				}
				OBJECT_STATE Block = { j * (BLOCK_HARFWIDTH * 2) + FieldInitPosX, i * (BLOCK_HARFWIDTH * 2) + FieldInitPosY, BLOCK_HARFWIDTH };
				CUSTOMVERTEX BLOCK_VTX[4]
				{
				{ Block.x - Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 0.f, 0.f },
				{ Block.x + Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 1.f, 0.f },
				{ Block.x + Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 1.f, 1.f },
				{ Block.x - Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 0.f, 1.f }
				};
				g_pD3Device->SetTexture(0, g_pTexture[textureID]);
				g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, BLOCK_VTX, sizeof(CUSTOMVERTEX));
			}
		}
	}
	//EFFECT
	if (EffectOn|| ChainEffectOn) {
		for (i = 0; i < FIELD_HEIGHT; i++) {
			for (j = 0; j < FIELD_WIDTH; j++) {

				switch (FieldEffectBuff[i][j])
				{
				case 0:
					textureID = UNITS_TEX;
					BlockColor = INVISIBLE;
					break;
				case 1:
				case 11:
					textureID = BLOCK_RED_TEX;
					BlockColor = BASEED;
					break;
				case 2:
				case 12:
					textureID = BLOCK_BLUE_TEX;
					BlockColor = BASEED;
					break;
				case 3:
				case 13:
					textureID = BLOCK_GREEN_TEX;
					BlockColor = BASEED;
					break;
				case 4:
				case 14:
					textureID = BLOCK_YELLOW_TEX;
					BlockColor = BASEED;
					break;
				case 5:
				case 15:
					textureID = UNITS_TEX;
					BlockColor = MAGENTA;
					break;
				case 6:
				case 16:
					textureID = EFFECT_TEX;
					BlockColor = BASEED;
					break;
				case 7:
				case 17:
					textureID = UNITS_TEX;
					BlockColor = ORANGE;
					break;
				case 8:
					textureID = UNITS_TEX;
					BlockColor = INVISIBLE;
					break;
				default:
					textureID = UNITS_TEX;
					BlockColor = INVISIBLE;
					break;
				}
				OBJECT_STATE Block = { j * (BLOCK_HARFWIDTH * 2) + FieldInitPosX, i * (BLOCK_HARFWIDTH * 2) + FieldInitPosY, BLOCK_HARFWIDTH };
				CUSTOMVERTEX BLOCK_VTX[4]
				{
				{ Block.x - Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 0.f, 0.f },
				{ Block.x + Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 1.f, 0.f },
				{ Block.x + Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 1.f, 1.f },
				{ Block.x - Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 0.f, 1.f }
				};
				g_pD3Device->SetTexture(0, g_pTexture[textureID]);
				g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, BLOCK_VTX, sizeof(CUSTOMVERTEX));
			}
		}
	}
	//サブボード設置
	//HOLD
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {

			switch (HoldBoard[i][j])
			{
			case 0:
				textureID = BORD_TEX;
				BlockColor = BLACK;
				break;
			case 1:
			case 11:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 2:
			case 12:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 3:
			case 13:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 4:
			case 14:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 5:
			case 15:
				textureID = UNITS_TEX;
				BlockColor = MAGENTA;
				break;
			case 6:
			case 16:
				textureID = UNITS_TEX;
				BlockColor = CYAN;
				break;
			case 7:
			case 17:
				textureID = UNITS_TEX;
				BlockColor = ORANGE;
				break;
			case 8:
				textureID = UNITS_TEX;
				BlockColor = WHITE;
				break;
			default:
				textureID = UNITS_TEX;
				BlockColor = INVISIBLE;
				break;
			}
			Block = { j *  (BLOCK_HARFWIDTH * 2) + SUBBOARD_LX, i *  (BLOCK_HARFWIDTH * 2) + HOLDBOARD_Y, BLOCK_HARFWIDTH };
			CUSTOMVERTEX BLOCK02_VTX[4]
			{
			{ Block.x - Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 0.f, 0.f },
			{ Block.x + Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 1.f, 0.f },
			{ Block.x + Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 1.f, 1.f },
			{ Block.x - Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 0.f, 1.f }
			};

			g_pD3Device->SetTexture(0, g_pTexture[textureID]);
			g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, BLOCK02_VTX, sizeof(CUSTOMVERTEX));
		}
	}
	//Next　First
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {

			switch (NextBoard[i][j])
			{
			case 0:
				textureID = BORD_TEX;
				BlockColor = BLACK;
				break;
			case 1:
			case 11:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 2:
			case 12:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 3:
			case 13:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 4:
			case 14:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 5:
			case 15:
				textureID = UNITS_TEX;
				BlockColor = MAGENTA;
				break;
			case 6:
			case 16:
				textureID = UNITS_TEX;
				BlockColor = CYAN;
				break;
			case 7:
			case 17:
				textureID = UNITS_TEX;
				BlockColor = ORANGE;
				break;
			case 8:
				textureID = UNITS_TEX;
				BlockColor = WHITE;
				break;
			default:
				textureID = UNITS_TEX;
				BlockColor = INVISIBLE;
				break;
			}
			Block = { j * (BLOCK_HARFWIDTH * 2) + SUBBOARD_RX, i * (BLOCK_HARFWIDTH * 2) + NEXTBOARD_Y, BLOCK_HARFWIDTH };
			CUSTOMVERTEX BLOCK02_VTX[4]
			{
			{ Block.x - Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 0.f, 0.f },
			{ Block.x + Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 1.f, 0.f },
			{ Block.x + Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 1.f, 1.f },
			{ Block.x - Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 0.f, 1.f }
			};

			g_pD3Device->SetTexture(0, g_pTexture[textureID]);
			g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, BLOCK02_VTX, sizeof(CUSTOMVERTEX));
		}
	}
	//Next　Second
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {

			switch (NextBoard2[i][j])
			{
			case 0:
				textureID = BORD_TEX;
				BlockColor = BLACK;
				break;
			case 1:
			case 11:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 2:
			case 12:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 3:
			case 13:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 4:
			case 14:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 5:
			case 15:
				textureID = UNITS_TEX;
				BlockColor = MAGENTA;
				break;
			case 6:
			case 16:
				textureID = UNITS_TEX;
				BlockColor = CYAN;
				break;
			case 7:
			case 17:
				textureID = UNITS_TEX;
				BlockColor = ORANGE;
				break;
			case 8:
				textureID = UNITS_TEX;
				BlockColor = WHITE;
				break;
			default:
				textureID = UNITS_TEX;
				BlockColor = INVISIBLE;
				break;
			}
			Block = { j * (BLOCK_HARFWIDTH * 2) + SUBBOARD_RX, i * (BLOCK_HARFWIDTH * 2) + NEXT2BOARD_Y, BLOCK_HARFWIDTH };
			CUSTOMVERTEX BLOCK03_VTX[4]
			{
			{ Block.x - Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 0.f, 0.f },
			{ Block.x + Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 1.f, 0.f },
			{ Block.x + Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 1.f, 1.f },
			{ Block.x - Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 0.f, 1.f }
			};

			g_pD3Device->SetTexture(0, g_pTexture[textureID]);
			g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, BLOCK03_VTX, sizeof(CUSTOMVERTEX));
		}
	}
	//Next　Third
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {

			switch (NextBoard3[i][j])
			{
			case 0:
				textureID = BORD_TEX;
				BlockColor = BLACK;
				break;
			case 1:
			case 11:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 2:
			case 12:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 3:
			case 13:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 4:
			case 14:
				textureID = BLOCK_BRUNK_TEX;
				BlockColor = BASEED;
				break;
			case 5:
			case 15:
				textureID = UNITS_TEX;
				BlockColor = MAGENTA;
				break;
			case 6:
			case 16:
				textureID = UNITS_TEX;
				BlockColor = CYAN;
				break;
			case 7:
			case 17:
				textureID = UNITS_TEX;
				BlockColor = ORANGE;
				break;
			case 8:
				textureID = UNITS_TEX;
				BlockColor = WHITE;
				break;
			default:
				textureID = UNITS_TEX;
				BlockColor = INVISIBLE;
				break;
			}
			Block = { j *  (BLOCK_HARFWIDTH * 2) + SUBBOARD_RX, i * (BLOCK_HARFWIDTH * 2) + NEXT3BOARD_Y, BLOCK_HARFWIDTH };
			CUSTOMVERTEX BLOCK04_VTX[4]
			{
			{ Block.x - Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 0.f, 0.f },
			{ Block.x + Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 1.f, 0.f },
			{ Block.x + Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 1.f, 1.f },
			{ Block.x - Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 0.f, 1.f }
			};

			g_pD3Device->SetTexture(0, g_pTexture[textureID]);
			g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, BLOCK04_VTX, sizeof(CUSTOMVERTEX));
		}
	}
	//Score
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {

			switch (ScoreBoard[i][j])
			{
			case 0:
				textureID = BORD_TEX;
				BlockColor = BLACK;
				break;
			case 1:
			case 11:
				textureID = BLOCK_RED_TEX;
				BlockColor = BASEED;
				break;
			case 2:
			case 12:
				textureID = BLOCK_BLUE_TEX;
				BlockColor = BASEED;
				break;
			case 3:
			case 13:
				textureID = BLOCK_GREEN_TEX;
				BlockColor = BASEED;
				break;
			case 4:
			case 14:
				textureID = BLOCK_YELLOW_TEX;
				BlockColor = BASEED;
				break;
			case 5:
			case 15:
				textureID = UNITS_TEX;
				BlockColor = MAGENTA;
				break;
			case 6:
			case 16:
				textureID = UNITS_TEX;
				BlockColor = CYAN;
				break;
			case 7:
			case 17:
				textureID = UNITS_TEX;
				BlockColor = ORANGE;
				break;
			case 8:
			case 10:
				textureID = UNITS_TEX;
				BlockColor = WHITE;
				break;
			default:
				textureID = UNITS_TEX;
				BlockColor = INVISIBLE;
				break;
			}
			Block = { j * (BLOCK_HARFWIDTH * 2) + SUBBOARD_LX, i *  (BLOCK_HARFWIDTH * 2) + SCOREBOARD_Y, BLOCK_HARFWIDTH };
			CUSTOMVERTEX BLOCK04_VTX[4]
			{
			{ Block.x - Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 0.f, 0.f },
			{ Block.x + Block.scale, Block.y - Block.scale, 1.f, 1.f, BlockColor, 1.f, 0.f },
			{ Block.x + Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 1.f, 1.f },
			{ Block.x - Block.scale, Block.y + Block.scale, 1.f, 1.f, BlockColor, 0.f, 1.f }
			};

			g_pD3Device->SetTexture(0, g_pTexture[textureID]);
			g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, BLOCK04_VTX, sizeof(CUSTOMVERTEX));
		}

	}
		g_pD3Device->SetTexture(0, g_pTexture[CHAIN_COLOR_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, CAHINCOLOR, sizeof(CUSTOMVERTEX));
	if (GameStop)
	{
		g_pD3Device->SetTexture(0, g_pTexture[PAUSE_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, backGroundPause, sizeof(CUSTOMVERTEX));
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

	RECT ResultScore =
	{
		240,
		315,
		900,
		500
	};

	//-----------------------------------------------------



	//描画の開始
	g_pD3Device->SetTexture(0, g_pTexture[BG_RESULT_TEX]);
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, result, sizeof(CUSTOMVERTEX));

	g_pD3Device->SetTexture(0, g_pTexture[RESULTGIRL_TEX]);
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, resultgirl, sizeof(CUSTOMVERTEX));

	
	sprintf_s(Score, 255, "%d", NowScorePoint);
	g_pFont[ResultScore_FONT]->DrawText(
		NULL,							// NULL
		Score,				// 描画テキスト
		-1,								// 全て表示
		&ResultScore,							// 表示範囲
		DT_CENTER,     // 左寄せ
		D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00)// 赤色
	);


}



void TEXTWRITTEN() {

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
		SCOREBOARD_Y -15,		// 左上のy座標
		SUBBOARD_LX + 200,		// 右下のx座標
		SCOREBOARD_Y + 600			// 右下のy座標
	};
	RECT DrawnSCORE = {
		SUBBOARD_LX - BLOCK_HARFWIDTH / 4,		// 左上のx座標
		SCOREBOARD_Y + BLOCK_HARFWIDTH*2,			// 左上のy座標
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



	g_pFont[SUBBOARD_FONT]->DrawText(
		NULL,							// NULL
		("HOLD"),					// 描画テキスト
		-1,								// 全て表示
		&HOLDBOARD,							// 表示範囲
		DT_LEFT,						// 左寄せ
		D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)	// 白色
	);
	g_pFont[SUBBOARD_FONT]->DrawText(
		NULL,							// NULL
		("NEXT"),					// 描画テキスト
		-1,								// 全て表示
		&NEXTBOARD,							// 表示範囲
		DT_LEFT,						// 左寄せ
		D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)	// 白色
	);
	g_pFont[SUBBOARD_FONT]->DrawText(
		NULL,							// NULL
		("Second"),					// 描画テキスト
		-1,								// 全て表示
		&NEXT2BOARD,							// 表示範囲
		DT_LEFT,						// 左寄せ
		D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)// 白色
	);
	g_pFont[SUBBOARD_FONT]->DrawText(
		NULL,							// NULL
		("Third"),					// 描画テキスト
		-1,								// 全て表示
		&NEXT3BOARD,							// 表示範囲
		DT_LEFT,						// 左寄せ
		D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)// 白色
	);
	g_pFont[SUBBOARD_FONT]->DrawText(
		NULL,							// NULL
		("SCORE"),					// 描画テキスト
		-1,								// 全て表示
		&SCOREBOARD,							// 表示範囲
		DT_LEFT,						// 左寄せ
		D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)// 白色
	);


	
	g_pFont[Score_FONT]->DrawText(
		NULL,							// NULL
		Score,					// 描画テキスト
		-1,								// 全て表示
		&DrawnSCORE,							// 表示範囲
		DT_RIGHT,						// 左寄せ
		D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)// 白色
	);
	if (GameStop)
	{
		g_pFont[PAUSE_FONT]->DrawText(
			NULL,							// NULL
			("PAUSE"),					// 描画テキスト
			-1,								// 全て表示
			&DrawnPAUSE,							// 表示範囲
			DT_LEFT,						// 左寄せ
			D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)// 白色
		);

		g_pFont[PAUSE2_FONT]->DrawText(
			NULL,							// NULL
			("PUSH Enter or Start"),					// 描画テキスト
			-1,								// 全て表示
			&DrawnPAUSE_B,							// 表示範囲
			DT_LEFT,						// 左寄せ
			D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)// 白色
		);
	}

	if (GameOver) {
		g_pFont[GO_FONT]->DrawText(
			NULL,							// NULL
			("GAME OVER"),					// 描画テキスト
			-1,								// 全て表示
			&GAMEOVER,						// 表示範囲
			DT_CENTER,						// 左寄せ
			D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00)	// color
		);
	}
	if (g_RiseCOUNTER&&ChainEffectOn) {
		g_pFont[GO_FONT]->DrawText(
			NULL,							// NULL
			chaincnt,		// 描画テキスト
			-1,								// 全て表示
			&GAMEOVER,						// 表示範囲
			DT_CENTER,						// 左寄せ
			D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00)	// color
		);
	}
#ifdef _DEBUG

	RECT DebugField;
	float PutDebugX = 0;
	float PutDebugY = 0;
	for (int height = 0; height < FIELD_HEIGHT; height++) {
		for (int width = 0; width < FIELD_WIDTH; width++) {
			PutDebugX = width * 25;
			PutDebugY = height * 20;

			sprintf_s(debug, 512, "%d", TetrisField[height][width]);
			
			RECT Debug = {
				PutDebugX+20,	// 左上のx座標
				PutDebugY,	// 左上のy座標
				1200,	// 右下のx座標
				700		// 右下のy座標
			};


			g_pFont[SUBBOARD_FONT]->DrawText(
				NULL,							// NULL
				debug,							// 描画テキスト
				-1,								// 全て表示
				&Debug,							// 表示範囲
				DT_LEFT,						// 左寄せ
				D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0xff)	// color
			);
		}
	}
	RECT DebugCHAINCNT{
	1000,
	500,
	1200,
	700
	};
	g_pFont[MAIN_FONT]->DrawText(
		NULL,							// NULL
		chaincnt,							// 描画テキスト
		-1,								// 全て表示
		&DebugCHAINCNT,							// 表示範囲
		DT_LEFT,						// 左寄せ
		D3DCOLOR_ARGB(0xff, 0x5f, 0x5f, 0x5f)	// color
	);
#endif // DEBUG
	}