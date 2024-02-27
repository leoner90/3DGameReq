#include "Game.h"
#include "headers/Enemy.h"
#include "headers/Player.h"
#include <math.h> 

//*************** INIT ***************

void Enemy::init(int posX, int poxY, int posZ, int enemyType)
{
	localEnemyType = enemyType;
	if (localEnemyType == 0)  enemyModel.LoadModel("cobra/cobra.md2");
	else if(localEnemyType == 1) enemyModel.LoadModel("Ogro/Ogro.md2");
	
	// enemy model
	enemyModel.SetScale(3.5f); enemyModel.SetToFloor(0);
	enemyModel.PlayAnimation(8, 16, 7, true);
	enemyModel.SetPosition(posX, poxY, posZ);
	enemyModel.SetScale(2.5f);
	enemyModel.SetDirectionAndRotationToPoint(0, 0);
	enemyModel.SetSpeed(100);
}

//*************** UPDATE ***************
void Enemy::OnUpdate(long t, Player &player)
{
	localPlayer = &player;
	if (localEnemyType == 0)
	{
		 //if distance > 50 follow();
		 // else attack();
	 

		enemyModel.SetDirectionAndRotationToPoint(localPlayer->playerModel.GetPositionV().GetX(), localPlayer->playerModel.GetPositionV().GetZ());
	}

	else if (localEnemyType == 1) enemyModel.SetDirectionAndRotationToPoint(-400,-400);
	
	enemyModel.Update(t);
}

//*************** 2D RENDER ***************
void Enemy::OnDraw(CGraphics* g)
{

}

//*************** 3D RENDER ***************
void Enemy::OnRender3D(CGraphics* g)
{
	enemyModel.Draw(g);
}

 
 

 
 


 