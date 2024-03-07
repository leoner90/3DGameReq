#include "Game.h"
#include "headers/Enemy.h"
#include "headers/Player.h"
#include "headers/Map.h"
#include <math.h> 

//*************** INIT ***************

void Enemy::init(int posX, int poxY, int posZ, int enemyType, Map& map)
{
	localEnemyType = enemyType;
	if (localEnemyType == 0) 
	{
		enemyMaxHp = enemyCurrentHp = 200;
		enemyModel.LoadModel("Cobra/enemyOne.md2");
		enemyDamage = 20;
		enemySpeed = 100 + rand() % 300;
	}
	else if (localEnemyType == 1)
	{
		enemyDamage = 20;
		enemyMaxHp = enemyCurrentHp = 400;
		enemyModel.LoadModel("Ogro/ogro.md2");
		enemySpeed = 100 + rand() % 200;

		//choose Random Portal Part To Attack
		CVector portalPartsPostions[4]
		{
			{ map.portalPartOne.GetPositionV().GetX(), 0, map.portalPartOne.GetPositionV().GetZ() },
			{ map.portalPartTwo.GetPositionV().GetX(), 0, map.portalPartTwo.GetPositionV().GetZ() },
			{ map.portalPartThree.GetPositionV().GetX(), 0, map.portalPartThree.GetPositionV().GetZ() },
			{ map.portalPartFour.GetPositionV().GetX(), 0, map.portalPartFour.GetPositionV().GetZ() }
		};
		randomPortalPartPos = portalPartsPostions[rand() % 4];
	}
	
	enemyModel.AddAnimation("run", 1, 35);
	enemyModel.AddAnimation("attack", 23, 86);

	// enemy model
	enemyModel.SetScale(3.5f); 
	//enemyModel.SetToFloor(0);
	
	enemyModel.SetScale(2.5f);
	enemyModel.SetPosition(posX, poxY, posZ);
	
	enemyModel.SetDirectionAndRotationToPoint(0, 0);
	isDead = preDeahAnimation = false;
	
	

	enemyHpbar.SetSize(35, 3);
	enemyHpbar.SetColor(CColor::Blue());
	enemyHpbar.SetHealth(100);

	deathAnimationTimer = 0;

	//wait at spawn point
	OnSpawnHold = true;


	//TO delete
	spawnPoint.LoadModel("portal/portalPart.obj");
	spawnPoint.SetScale(8.f);
	spawnPoint.SetPosition(2500, 0, 700);

	spawnPoint2.LoadModel("portal/portalPart.obj");
	spawnPoint2.SetScale(8.f);
	spawnPoint2.SetPosition(3800, 0, 400);

}

//*************** UPDATE ***************
void Enemy::OnUpdate(long t, Player &player, Map& map, std::vector<Enemy*>& AllEnemies, int thisEnemyIndex)
{
	//if dead -> return (do nothing)
	if (isDead) return;

	if (preDeahAnimation)
	{
		if (deathAnimationTimer == 0)
		{
			enemyModel.PlayAnimation("attack", 12, true);
			deathAnimationTimer = 1000 + t;
		}
		else if(deathAnimationTimer < t) isDead = true;
	 
		return;
	}

	localPlayer = &player;
	localMap = &map;
	localTime = t;

	int i = 0;
	for (auto enemy : AllEnemies)
	{
		if (i == thisEnemyIndex) continue;
		if (enemyModel.HitTest(&enemy->enemyModel) )
		{
			//
			//enemyModel.SetPosition(enemyModel.GetPositionV().GetX() + 2, enemyModel.GetPositionV().GetY(), enemyModel.GetPositionV().GetZ()+ 2 );
		}
		i++;
	}

	if (localEnemyType == 0 && !OnSpawnHold)
	{	
		CVector displ = localPlayer->playerModel.GetPositionV() - enemyModel.GetPositionV();
		float distance = hypot(displ.GetX(), displ.GetZ());
		if (distance > 60)
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

	else if (localEnemyType == 1 && !OnSpawnHold)
	{
		CVector displ = CVector{ randomPortalPartPos.GetX() , 0, randomPortalPartPos.GetZ() } - enemyModel.GetPositionV();
		float distance = hypot(displ.GetX(), displ.GetZ());

		if (distance > 60)
		{
			
			enemyModel.SetSpeed(enemySpeed);
			enemyModel.SetDirectionAndRotationToPoint(randomPortalPartPos.GetX(), randomPortalPartPos.GetZ());
			enemyModel.PlayAnimation("run", 12, true);
		}
		else
		{
			enemyModel.PlayAnimation("attack", 12, true);
			enemyModel.SetSpeed(0);
			Attack();
		}
	
	}
	//if pre-dead -> play animation of death , set dead status true by the end
	float remainingHpInPercentage = enemyCurrentHp / (enemyMaxHp / 100);
	enemyHpbar.SetHealth(remainingHpInPercentage);

	enemyModel.Update(t);
}

//*************** 2D RENDER ***************
void Enemy::OnDraw(CGraphics* g, CVector enemyPos)
{
	if (isDead) return;
 
	enemyHpbar.SetPosition(enemyPos.x, enemyPos.y + 30);

	CVector displ = localPlayer->playerModel.GetPositionV() - enemyModel.GetPositionV();
	float distance = hypot(displ.GetX(), displ.GetZ());
	
	if(distance < 1200 && !preDeahAnimation) enemyHpbar.Draw(g);

	//todelete

}

void Enemy::Attack()
{

	if (localEnemyType == 0 && attackDelay < localTime)
	{
		// Attack player  
		if (enemyModel.HitTest(&localPlayer->playerModel))
		{
			localPlayer->playerGettingDamage(enemyDamage);
			attackDelay = 2000 + localTime;
		}
	}
	else if (localEnemyType == 1 && attackDelay < localTime)
	{
		// Attack Portal 

		if (
				enemyModel.HitTest(&localMap->portalPartOne) || 
				enemyModel.HitTest(&localMap->portalPartTwo) ||
				enemyModel.HitTest(&localMap->portalPartThree) ||
				enemyModel.HitTest(&localMap->portalPartFour)
			)
		{
			localMap->portal.SetHealth(localMap->portal.GetHealth() - 5);
			attackDelay = 2000 + localTime;
		}
	}
}

//*************** 3D RENDER ***************
void Enemy::OnRender3D(CGraphics* g)
{
	if (isDead) return;
	enemyModel.Draw(g);
	spawnPoint.Draw(g);
	spawnPoint2.Draw(g);
}

void Enemy::EnemyGetDamage(float damage)
{
	OnSpawnHold = false; // in case attacking at spawn point

	enemyCurrentHp -= damage;
	//enemyCurrentHp > 0 ? enemyHpbar.SetHealth(enemyCurrentHp) : enemyHpbar.SetHealth(0);

	if (enemyCurrentHp <= 0)
	{
		if (localEnemyType == 0) localPlayer->weaponComponents++;
		else if (localEnemyType == 1) localPlayer->armorComponents++;
		preDeahAnimation = true;
	}
}

 