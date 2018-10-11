#include "Render.h"

int teamlogorandam = 0;
static TEXTURE_STATE g_texture = { 600.f, 200.f, 200.f };
static TEXTURE_STATE g_texture2 = { 1200.f, 500.f, 200.f };
static float g_tv = 0.f;
static float g3_tu = 1.f;
static float g4_tu = 0.f;
static float teamlogo_y = 0.f;
static int rotate = 0;
static bool move = true;
static int teamlogocount = 0;
static float KEEPX = 0.f;
static float KEEPY = 0.f;
static float teamlogorad = 0.f;
static float logo2x[4];
static float logo2y[4];
static int g_alpha = 255;
static bool ScenrEnd = false;
static bool  JumpLogo = false;
static int g_ya;
static int g_yb;

void TeamLogoControl(void){
	logo2x[0] = g_texture2.x - g_texture2.scale;
	logo2x[1] = g_texture2.x + g_texture2.scale;
	logo2x[2] = g_texture2.x + g_texture2.scale;
	logo2x[3] = g_texture2.x - g_texture2.scale;

	logo2y[0] = g_texture2.y - g_texture2.scale;
	logo2y[1] = g_texture2.y - g_texture2.scale;
	logo2y[2] = g_texture2.y + g_texture2.scale;
	logo2y[3] = g_texture2.y + g_texture2.scale;

	if (teamlogorandam == 1) {
		if (move) {
			if (g_tv <= 1.f) {
				g_tv += 0.01f;
			}
			if (g_tv >= 1 && teamlogocount <= 180) {
				teamlogorad = teamlogorad + 0.1f;
				teamlogo_y -= cos(teamlogorad);
			}
		}
		if (teamlogocount >= 180) {
			move = false;
		}
		if (!move && g_tv >= 0) {
			g_tv -= 0.01;
			if (g_tv < 0) {
				ScenrEnd = true;
			}
		}
		teamlogocount++;
	}
	if (teamlogorandam == 2){
		static bool othercontrol = false, stop = false;
		if (g_texture2.x >= 636 && g_texture2.x <= 644){
			stop = true;
			static int Ecount = 0;
			Ecount++;
			if (Ecount >= 60) {
				othercontrol = true;
			}
		}
		if (stop == false) {
			g_texture2.x -= 5;
			teamlogorad += 0.05575f;

			if (teamlogorad <= 37.5f) {
				g_texture2.scale++;
				for (rotate = 0; rotate < 4; rotate++) {
					logo2x[rotate] -= g_texture2.x;
					logo2y[rotate] -= g_texture2.y;

					KEEPX = logo2x[rotate];
					KEEPY = logo2y[rotate];

					logo2x[rotate] = KEEPX * cos(teamlogorad) + KEEPY * sin(teamlogorad);
					logo2y[rotate] = KEEPX * -sin(teamlogorad) + KEEPY * cos(teamlogorad);

					logo2x[rotate] += g_texture2.x;
					logo2y[rotate] += g_texture2.y;
				}
				if (JumpLogo == true) {
					g_ya = g_texture2.y;
					g_texture2.y += (g_texture2.y - g_yb) + 1;
					g_yb = g_ya;
					if (g_texture2.y == 500) {
						JumpLogo = false;
					}
				}
				if (g_texture2.y == 500 && JumpLogo == false) {
					JumpLogo = true;
					g_yb = g_texture2.y;
					g_texture2.y = g_texture2.y - 20;
				}
			}
		}
		if (othercontrol == true){
			g_texture2.scale += 50;
			if (g_texture2.scale >= 1600){
				othercontrol = false;
				ScenrEnd = true;
			}
		}
	}
	if (teamlogorandam == 3){		
		if (g3_tu >= 0.f){
			g3_tu -= 0.015f;
		}
		if (g4_tu <= 1){
			g4_tu += 0.015f;
		}
		if (g3_tu <= 0.1f){
			g_alpha -= 2;
			if (g_alpha <= 0){
				ScenrEnd = true;
			}
		}		
	}
	if (ScenrEnd) {
		g_scene = STATE_TITLE;
	}
}

/**
*•`‰æˆ—
*/
void TeamLogoRender(void){

	CUSTOMVERTEX TeamLogo1[4]
	{
	{ 100.0f, -150.0f + teamlogo_y, 1.f, 1.f, 0xFFFFFFFF , 0.f, 0.f },
	{ 1180.f, -150.0f + teamlogo_y, 1.f, 1.f, 0xFFFFFFFF , 1.f, 0.f },
	{ 1180.f, 800.0f + teamlogo_y, 1.f, 1.f,0xFFFFFFFF , 1.f, g_tv },
	{ 100.0f, 800.0f + teamlogo_y, 1.f, 1.f, 0xFFFFFFFF , 0.f, g_tv },
	};
	CUSTOMVERTEX TeamLogo2[4]
	{
	{ logo2x[0], logo2y[0], 1.f, 1.f, 0xFFFFFFFF , 0.f, 0.f },
	{ logo2x[1] ,logo2y[1] , 1.f, 1.f, 0xFFFFFFFF , 1.f, 0.f },
	{ logo2x[2] ,logo2y[2] , 1.f, 1.f,0xFFFFFFFF , 1.f, 1.f },
	{ logo2x[3] ,logo2y[3] , 1.f, 1.f, 0xFFFFFFFF , 0.f, 1.f },
	};
	CUSTOMVERTEX TeamLogo3[4]
	{
	{ 100.0f, -150.0f + teamlogo_y, 1.f, 1.f,D3DCOLOR_ARGB(g_alpha,255,255,255) , g3_tu, 0.f },
	{ 1180.f, -150.0f + teamlogo_y, 1.f, 1.f,D3DCOLOR_ARGB(g_alpha,255,255,255), 1.f, 0.f },
	{ 1180.f, 800.f + teamlogo_y, 1.f, 1.f,D3DCOLOR_ARGB(g_alpha,255,255,255), 1.f, 1.f },
	{ 100.0f, 800.f + teamlogo_y, 1.f, 1.f, D3DCOLOR_ARGB(g_alpha,255,255,255) , g3_tu, 1.f },
	};
	CUSTOMVERTEX TeamLogo4[4]
	{
	{ 100.0f, -150.0f + teamlogo_y, 1.f, 1.f,D3DCOLOR_ARGB(g_alpha,255,255,255), 0.f, 0.f },
	{ 1180.f, -150.0f + teamlogo_y, 1.f, 1.f,D3DCOLOR_ARGB(g_alpha,255,255,255), g4_tu, 0.f },
	{ 1180.f, 800.f + teamlogo_y, 1.f, 1.f,D3DCOLOR_ARGB(g_alpha,255,255,255), g4_tu, 1.f },
	{ 100.0f, 800.f + teamlogo_y, 1.f, 1.f,D3DCOLOR_ARGB(g_alpha,255,255,255), 0.f, 1.f },
	};
	//‰æ–Ê‚ÌÁ‹Ž
	g_pD3Device->Clear(0, NULL,	D3DCLEAR_TARGET,D3DCOLOR_XRGB(0x00, 0x00, 0x00),1.0, 0);
	//•`‰æ‚ÌŠJŽn
	g_pD3Device->BeginScene();
	if (!ScenrEnd){
		if (teamlogorandam == 1){
			DrowTexture(TeamLogo1, TEAM_LOGO_TEX);
		}
		if (teamlogorandam == 2){
			DrowTexture(TeamLogo2, TEAM_LOGO_TEX);
		}
		if (teamlogorandam == 3){
			DrowTexture(TeamLogo3, TEAM_LOGO_TEX);
			DrowTexture(TeamLogo4, TEAM_LOGO_TEX);
		}
	}
	//•`‰æ‚ÌI—¹
	g_pD3Device->EndScene();
	//•\Ž¦
	g_pD3Device->Present(NULL, NULL, NULL, NULL);
}

