#include "Game.h"
#include "headers/Radar.h"
#include "headers/Enemy.h"
#include "headers/Player.h"

Radar::Radar(float w, float h)
{
	radarBg.LoadImage("radarBg.jpg");
	radarBg.SetSize(180, 180);
	radarBg.SetPosition(w - 90, 90);

	// char screen
	dot.LoadImage("dot.png");
	dot.SetSize(7, 7);
	
}

void Radar::init()
{
}

void Radar::OnUpdate(Uint32 t, std::vector<Enemy*>& AllEnemies, Player& player)
{
	localAllEnemies =  AllEnemies;
	localPlayer = &player;
}





void Radar::OnDraw(CGraphics* g)
{
	radarBg.Draw(g);


	for (auto enemy : localAllEnemies)
	{
		if (enemy->OnSpawnHold) continue;

		DrawDot(enemy->enemyModel->GetX(), enemy->enemyModel->GetZ(), CColor::Red(), g);
	}

	//player
	DrawDot(localPlayer->playerModel.GetX(), localPlayer->playerModel.GetZ(), CColor::Green(), g);
}


void Radar::DrawDot(float posX, float posZ, CColor color, CGraphics* g)
{
	float radarCenterPos = 90;
	float PosX = posX / 7500 * 180;
	float resultX = radarCenterPos + (PosX / 2);

	float enemyPosZ = posZ / 7500 * 180;
	float resultZ = radarCenterPos - (enemyPosZ / 2);

	dot.SetPosition(1740 + resultX, resultZ);
	dot.SetColor(color);
	dot.Draw(g);

}