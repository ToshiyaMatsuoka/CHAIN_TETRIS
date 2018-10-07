#include "Render.h"
#include "TetriminoState.h"
#include "Gamescene.h"
#include "TetrisEffect.h"

OBJECT_STATE Block;
TEXTURE_STATE g_Girl = { 1000.f,450.f,150.f };
TEXTURE_STATE Logo = { 320.f, -300.f, 100.f };//���S�}�[�N�̏����ʒu
void TEXTWRITTEN();
int BlockColor;
static char debug[512]{'\0'};
char Score[255];
int g_scene = 0;
void ResultRender(void);

/**
*�`�揈��
*/
void Render() {
	
	static int i = 0;
	static int j = 0;
	TEXTURE textureID;
	//-----------------------------------------------------
	//���_��������--------------------------------------

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

	//��ʂ̏���
	g_pD3Device->Clear(0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0x00, 0x00, 0x00),
		1.0, 0);

	//�`��̊J�n
	g_pD3Device->BeginScene();


	//�w�i
	//�L����
	//UI
	//����
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
	//�w�i�ݒu
	g_pD3Device->SetTexture(0, g_pTexture[BG_MAIN_TEX]);
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, backGroundMain, sizeof(CUSTOMVERTEX));

	g_pD3Device->SetTexture(0, g_pTexture[STAGE_TEX]);
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, tetrisStage, sizeof(CUSTOMVERTEX));

	//PLAYFIELD�̐ݒu
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
	//�T�u�{�[�h�ݒu
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
	//Next�@First
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
	//Next�@Second
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
	//Next�@Third
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
	//�`��̏I��
	g_pD3Device->EndScene();
	//�\��
	g_pD3Device->Present(NULL, NULL, NULL, NULL);
}

void ResultRender(void){
	//���_��������--------------------------------------

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



	//�`��̊J�n
	g_pD3Device->SetTexture(0, g_pTexture[BG_RESULT_TEX]);
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, result, sizeof(CUSTOMVERTEX));

	g_pD3Device->SetTexture(0, g_pTexture[RESULTGIRL_TEX]);
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, resultgirl, sizeof(CUSTOMVERTEX));

	
	sprintf_s(Score, 255, "%d", NowScorePoint);
	g_pFont[ResultScore_FONT]->DrawText(
		NULL,							// NULL
		Score,				// �`��e�L�X�g
		-1,								// �S�ĕ\��
		&ResultScore,							// �\���͈�
		DT_CENTER,     // ����
		D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00)// �ԐF
	);


}



void TEXTWRITTEN() {

	char chaincnt[64];
	sprintf_s(chaincnt, 64, "%d CHAIN!", g_RiseCOUNTER);

	sprintf_s(Score, 255, "%d", g_ScorePoint);

	RECT LOGO = {
		100,		// �����x���W
		500,		// �����y���W
		600,		// �E����x���W
		900			// �E����y���W
	};
	RECT HOLDBOARD = {
		SUBBOARD_LX - BLOCK_HARFWIDTH,		// �����x���W
		HOLDBOARD_Y - BLOCK_HARFWIDTH,		// �����y���W
		SUBBOARD_LX + 200,		// �E����x���W
		HOLDBOARD_Y + 600			// �E����y���W
	};
	RECT NEXTBOARD = {
		SUBBOARD_RX - BLOCK_HARFWIDTH,		// �����x���W
		NEXTBOARD_Y - BLOCK_HARFWIDTH,		// �����y���W
		SUBBOARD_RX + 200,		// �E����x���W
		NEXTBOARD_Y + 600			// �E����y���W
	};
	RECT NEXT2BOARD = {
		SUBBOARD_RX - BLOCK_HARFWIDTH,		// �����x���W
		NEXT2BOARD_Y - BLOCK_HARFWIDTH,		// �����y���W
		SUBBOARD_RX + 200,		// �E����x���W
		NEXT2BOARD_Y + 600			// �E����y���W
	};
	RECT NEXT3BOARD = {
		SUBBOARD_RX - BLOCK_HARFWIDTH,		// �����x���W
		NEXT3BOARD_Y - BLOCK_HARFWIDTH,		// �����y���W
		SUBBOARD_RX + 200,		// �E����x���W
		NEXT3BOARD_Y + 600			// �E����y���W
	};
	RECT SCOREBOARD = {
		SUBBOARD_LX - BLOCK_HARFWIDTH,		// �����x���W
		SCOREBOARD_Y -15,		// �����y���W
		SUBBOARD_LX + 200,		// �E����x���W
		SCOREBOARD_Y + 600			// �E����y���W
	};
	RECT DrawnSCORE = {
		SUBBOARD_LX - BLOCK_HARFWIDTH / 4,		// �����x���W
		SCOREBOARD_Y + BLOCK_HARFWIDTH*2,			// �����y���W
		SUBBOARD_LX + 180 - BLOCK_HARFWIDTH,	// �E����x���W
		SCOREBOARD_Y + 250 - BLOCK_HARFWIDTH	// �E����y���W
	};
	RECT GAMEOVER = {
		150,	// �����x���W
		180,	// �����y���W
		1150,	// �E����x���W
		600		// �E����y���W
	};
	RECT DrawnPAUSE = {
		400,	// �����x���W
		220,	// �����y���W
		1150,	// �E����x���W
		600		// �E����y���W
	};
	RECT DrawnPAUSE_B = {
		380,	// �����x���W
		420,	// �����y���W
		1150,	// �E����x���W
		600		// �E����y���W
	};



	g_pFont[SUBBOARD_FONT]->DrawText(
		NULL,							// NULL
		("HOLD"),					// �`��e�L�X�g
		-1,								// �S�ĕ\��
		&HOLDBOARD,							// �\���͈�
		DT_LEFT,						// ����
		D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)	// ���F
	);
	g_pFont[SUBBOARD_FONT]->DrawText(
		NULL,							// NULL
		("NEXT"),					// �`��e�L�X�g
		-1,								// �S�ĕ\��
		&NEXTBOARD,							// �\���͈�
		DT_LEFT,						// ����
		D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)	// ���F
	);
	g_pFont[SUBBOARD_FONT]->DrawText(
		NULL,							// NULL
		("Second"),					// �`��e�L�X�g
		-1,								// �S�ĕ\��
		&NEXT2BOARD,							// �\���͈�
		DT_LEFT,						// ����
		D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)// ���F
	);
	g_pFont[SUBBOARD_FONT]->DrawText(
		NULL,							// NULL
		("Third"),					// �`��e�L�X�g
		-1,								// �S�ĕ\��
		&NEXT3BOARD,							// �\���͈�
		DT_LEFT,						// ����
		D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)// ���F
	);
	g_pFont[SUBBOARD_FONT]->DrawText(
		NULL,							// NULL
		("SCORE"),					// �`��e�L�X�g
		-1,								// �S�ĕ\��
		&SCOREBOARD,							// �\���͈�
		DT_LEFT,						// ����
		D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)// ���F
	);


	
	g_pFont[Score_FONT]->DrawText(
		NULL,							// NULL
		Score,					// �`��e�L�X�g
		-1,								// �S�ĕ\��
		&DrawnSCORE,							// �\���͈�
		DT_RIGHT,						// ����
		D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)// ���F
	);
	if (GameStop)
	{
		g_pFont[PAUSE_FONT]->DrawText(
			NULL,							// NULL
			("PAUSE"),					// �`��e�L�X�g
			-1,								// �S�ĕ\��
			&DrawnPAUSE,							// �\���͈�
			DT_LEFT,						// ����
			D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)// ���F
		);

		g_pFont[PAUSE2_FONT]->DrawText(
			NULL,							// NULL
			("PUSH Enter or Start"),					// �`��e�L�X�g
			-1,								// �S�ĕ\��
			&DrawnPAUSE_B,							// �\���͈�
			DT_LEFT,						// ����
			D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)// ���F
		);
	}

	if (GameOver) {
		g_pFont[GO_FONT]->DrawText(
			NULL,							// NULL
			("GAME OVER"),					// �`��e�L�X�g
			-1,								// �S�ĕ\��
			&GAMEOVER,						// �\���͈�
			DT_CENTER,						// ����
			D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00)	// color
		);
	}
	if (g_RiseCOUNTER&&ChainEffectOn) {
		g_pFont[GO_FONT]->DrawText(
			NULL,							// NULL
			chaincnt,		// �`��e�L�X�g
			-1,								// �S�ĕ\��
			&GAMEOVER,						// �\���͈�
			DT_CENTER,						// ����
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
				PutDebugX+20,	// �����x���W
				PutDebugY,	// �����y���W
				1200,	// �E����x���W
				700		// �E����y���W
			};


			g_pFont[SUBBOARD_FONT]->DrawText(
				NULL,							// NULL
				debug,							// �`��e�L�X�g
				-1,								// �S�ĕ\��
				&Debug,							// �\���͈�
				DT_LEFT,						// ����
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
		chaincnt,							// �`��e�L�X�g
		-1,								// �S�ĕ\��
		&DebugCHAINCNT,							// �\���͈�
		DT_LEFT,						// ����
		D3DCOLOR_ARGB(0xff, 0x5f, 0x5f, 0x5f)	// color
	);
#endif // DEBUG
	}