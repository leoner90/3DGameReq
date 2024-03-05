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
	
	enemyModel.AddAnimation("run", 1, 35);
	enemyModel.AddAnimation("attack", 23, 86);

	// enemy model
	enemyModel.SetScale(3.5f); 
	//enemyModel.SetToFloor(0);
	
	enemyModel.SetScale(2.5f);
	enemyModel.SetPosition(posX, poxY, posZ);
	
	enemyModel.SetDirectionAndRotationToPoint(0, 0);
	isDead = false;
	
	

	enemyHpbar.SetSize(35, 3);
	enemyHpbar.SetColor(CColor::Blue());
	enemyHpbar.SetHealth(100);

}

//*************** UPDATE ***************
void Enemy::OnUpdate(long t, Player &player, Map& map)
{

	if (isDead) return;
	localPlayer = &player;
	localMap = &map;
 
	float remainingHpInPercentage = enemyCurrentHp / (enemyMaxHp / 100);
	enemyHpbar.SetHealth(remainingHpInPercentage);
	if (localEnemyType == 0)
	{		  
		CVector displ = localPlayer->playerModel.GetPositionV() - enemyModel.GetPositionV();
		float distance = hypot(displ.GetX(), displ.GetZ());
		if (distance > 500)
		{
			enemyModel.SetDirectionAndRotationToPoint(localPlayer->playerModel.GetPositionV().GetX(), localPlayer->playerModel.GetPositionV().GetZ());
			enemyModel.SetSpeed(enemySpeed);
			enemyModel.PlayAnimation("run", 12, true);
		}
		else 
		{
			enemyModel.SetSpeed(0);
			enemyModel.PlayAnimation("attack", 12, true);
			Attack();
			
		}
	}

	else if (localEnemyType == 1)
	{
		CVector displ = localMap->portal.GetPositionV() - enemyModel.GetPositionV();
		float distance = hypot(displ.GetX(), displ.GetZ());
		if (distance > 50)
		{
			enemyModel.SetSpeed(enemySpeed);
			enemyModel.SetDirectionAndRotationToPoint(localMap->portal.GetPositionV().GetX(), localMap->portal.GetPositionV().GetZ());
			enemyModel.PlayAnimation("run", 12, true);
		}
		else
		{
			enemyModel.PlayAnimation("attack", 12, true);
			enemyModel.SetSpeed(0);
			Attack();
		}
	
	}
 
	enemyModel.Update(t);
}

//*************** 2D RENDER ***************
void Enemy::OnDraw(CGraphics* g, CVector enemyPos)
{
	if (isDead) return;
 
	enemyHpbar.SetPosition(enemyPos.x, enemyPos.y + 30);

	CVector displ = localPlayer->playerModel.GetPositionV() - enemyModel.GetPositionV();
	float distance = hypot(displ.GetX(), displ.GetZ());

	if(distance < 1200) enemyHpbar.Draw(g);
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
			localMap->portal.SetHealth(localMap->portal.GetHealth() - 5);
		}
	}
}

//*************** 3D RENDER ***************
void Enemy::OnRender3D(CGraphics* g)
{
	if (isDead) return;
	enemyModel.Draw(g);
}

void Enemy::EnemyGetDamage(float damage)
{
	enemyCurrentHp -= damage;
	//enemyCurrentHp > 0 ? enemyHpbar.SetHealth(enemyCurrentHp) : enemyHpbar.SetHealth(0);

	if (enemyCurrentHp <= 0)
	{
		if (localEnemyType == 0) localPlayer->weaponComponents++;
		else if (localEnemyType == 1) localPlayer->armorComponents++;
		isDead = true;
	}
}

 