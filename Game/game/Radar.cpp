#include "Game.h"
#include "headers/Radar.h"
#include "headers/Enemy.h"
#include "headers/Player.h"

Radar::Radar(float w, float h)
{
	radarBg.LoadImage("radarBg.jpg");
	radarBg.SetSize(300, 300);
	radarBg.SetPosition(w - 150, 150);

	// char screen
	dot.LoadImage("dot.png");
	dot.SetSize(10, 10);
	
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
		float radarCenterPos = 150;
		dot.SetColor(CColor::Red());
		//CSprite* enemyDot = new CSprite(dot);

		float enemyPosX = enemy->enemyModel->GetX() / 7500 * 300;
		float resultX = radarCenterPos + (enemyPosX / 2);

		float enemyPosZ = enemy->enemyModel->GetZ() / 7500 * 300;
		float resultZ = radarCenterPos - (enemyPosZ / 2);


		dot.SetPosition(1620 + resultX, resultZ);
		dot.SetSize(10, 10);
		dot.Draw(g);
	}

	//player
	float radarCenterPos = 150;
	float enemyPosX = localPlayer->playerModel.GetX() / 7500 * 300;
	float resultX = radarCenterPos + (enemyPosX / 2);

	float enemyPosZ = localPlayer->playerModel.GetZ() / 7500 * 300;
	float resultZ = radarCenterPos - (enemyPosZ / 2);
	dot.SetPosition(1620 + resultX, resultZ);
	dot.SetSize(10, 10);
	dot.SetColor(CColor::Green());
	dot.Draw(g);


}
