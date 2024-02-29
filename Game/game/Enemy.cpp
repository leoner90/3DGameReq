#include "Game.h"
#include "headers/Enemy.h"
#include "headers/Player.h"
#include "headers/Map.h"
#include <math.h> 

//*************** INIT ***************

void Enemy::init(int posX, int poxY, int posZ, int enemyType)
{
	localEnemyType = enemyType;
	if (localEnemyType == 0) 
	{
		enemyMaxHp = enemyCurrentHp = 200;
		enemyModel.LoadModel("Cobra/enemyOne.md2");
		enemyDamage = 100;
		enemySpeed = 100 + rand() % 300;
	}
	else if (localEnemyType == 1)
	{
		enemyDamage = 200;
		enemyMaxHp = enemyCurrentHp = 400;
		enemyModel.LoadModel("Ogro/ogro.md2");
		enemySpeed = 100 + rand() % 200;
	}
	
	// enemy model
	enemyModel.SetScale(3.5f); 
	//enemyModel.SetToFloor(0);
	enemyModel.PlayAnimation(1, 35, 12, true);
	enemyModel.SetScale(2.5f);
	enemyModel.SetPosition(posX, poxY, posZ);
	
	enemyModel.SetDirectionAndRotationToPoint(0, 0);
	isDead = false;
	
}

//*************** UPDATE ***************
void Enemy::OnUpdate(long t, Player &player, Map& map)
{
	localPlayer = &player;
	localMap = &map;

	if (localEnemyType == 0)
	{		  
		CVector displ = localPlayer->playerModel.GetPositionV() - enemyModel.GetPositionV();
		float distance = hypot(displ.GetX(), displ.GetZ());
		if (distance > 500)
		{
			enemyModel.SetDirectionAndRotationToPoint(localPlayer->playerModel.GetPositionV().GetX(), localPlayer->playerModel.GetPositionV().GetZ());
			enemyModel.SetSpeed(enemySpeed);
		}
		else 
		{
			enemyModel.SetSpeed(0);
			Attack();
			
		}
	}

	else if (localEnemyType == 1)
	{
		CVector displ = localMap->portal.GetPositionV() - enemyModel.GetPositionV();
		float distance = hypot(displ.GetX(), displ.GetZ());
		if (distance > 200)
		{
			enemyModel.SetSpeed(enemySpeed);
			enemyModel.SetDirectionAndRotationToPoint(localMap->portal.GetPositionV().GetX(), localMap->portal.GetPositionV().GetZ());
		}
		else
		{
			enemyModel.SetSpeed(0);
			Attack();
		}
	
	}
	
	enemyModel.Update(t);
}

//*************** 2D RENDER ***************
void Enemy::OnDraw(CGraphics* g)
{

}

void Enemy::Attack()
{


	if (localEnemyType == 0)
	{
		// Attack player  
		if (enemyModel.HitTest(&localPlayer->playerModel))
		{
			localPlayer->playerGettingDamage(enemyDamage);
		}
	}
	else if (localEnemyType == 1)
	{
		// Attack Portal 
		if (enemyModel.HitTest(&localMap->portal))
		{
			//localPlayer->playerGettingDamage(enemyDamage);
		}
	}
}

//*************** 3D RENDER ***************
void Enemy::OnRender3D(CGraphics* g)
{
	enemyModel.Draw(g);
}

void Enemy::EnemyGetDamage(float damage)
{
}

 
 

 
 


 