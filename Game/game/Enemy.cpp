#include "Game.h"
#include "headers/Enemy.h"
#include "headers/Player.h"
#include "headers/Map.h"
#include <math.h> 
#include "headers/Portal.h"

//*************** INIT ***************

//Static
CModelMd3* Enemy::enemyModelOne = new CModelMd3();
CModelMd3* Enemy::enemyModelTwo = new CModelMd3();

Enemy::Enemy()
{
	enemyModelOne->LoadModel("enemies/63.md3");
	enemyModelOne->SetScale(15.5f);
	

	enemyModelTwo->LoadModel("enemies/65.md3");
	enemyModelTwo->SetScale(15.5f);
}

Enemy::~Enemy()
{
	delete enemyModel;
}

void Enemy::init(int posX, int poxY, int posZ, int enemyType, Map& map, Portal& portal)
{


	localPortal = &portal;
	enemyModel = new CModelMd3();

	localEnemyType = enemyType;
	if (localEnemyType == 0) 
	{
		enemyMaxHp = enemyCurrentHp = 200;
		enemyModel = enemyModelOne->Clone();
		
		enemyDamage = 50;
		enemySpeed = 300 + rand() % 250;


	}
	else if (localEnemyType == 1)
	{
		enemyDamage = 50;
		enemyMaxHp = enemyCurrentHp = 400;
		enemyModel = enemyModelTwo->Clone();
		enemySpeed = 200 + rand() % 200;
		
		//choose Random Portal Part To Attack
		CVector portalPartsPostions[4]
		{
			{ localPortal->portalPartOne.GetPositionV().GetX(), 0, localPortal->portalPartOne.GetPositionV().GetZ() },
			{ localPortal->portalPartTwo.GetPositionV().GetX(), 0, localPortal->portalPartTwo.GetPositionV().GetZ() },
			{ localPortal->portalPartThree.GetPositionV().GetX(), 0, localPortal->portalPartThree.GetPositionV().GetZ() },
			{ localPortal->portalPartFour.GetPositionV().GetX(), 0, localPortal->portalPartFour.GetPositionV().GetZ() }
		};
		randomPortalPartPos = portalPartsPostions[rand() % 4];
	}
	
	enemyModel->AddAnimation("run", 10, 40);
	enemyModel->AddAnimation("attack", 0, 10);

	// enemy model

	//enemyModel.SetToFloor(0);

	
	
	enemyModel->SetPosition(posX, poxY, posZ);
	
	enemyModel->SetDirectionAndRotationToPoint(0, 0);
	isDead = preDeahAnimation = false;
	
	enemyHpbar.SetSize(35, 3);
	enemyHpbar.SetColor(CColor::Blue());
	enemyHpbar.SetHealth(100);

	deathAnimationTimer = 0;

	//wait at spawn point
	OnSpawnHold = true;

	enemyModel->SetToFloor(0);
	
}

//*************** UPDATE ***************
void Enemy::OnUpdate(Uint32 t, Player &player, Map& map, std::vector<Enemy*>& AllEnemies, int thisEnemyIndex, Portal& portal)
{
	localPortal = &portal;
	//if dead -> return (do nothing)
	if (isDead) return;

	if (preDeahAnimation)
	{
		if (deathAnimationTimer == 0)
		{
			enemyModel->PlayAnimation("attack", 12, true);
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
		if (enemyModel->HitTest(enemy->enemyModel) )
		{
			//enemyModel.SetPosition(enemyModel.GetPositionV().GetX() + 2, enemyModel.GetPositionV().GetY(), enemyModel.GetPositionV().GetZ()+ 2 );
		}
		i++;
	}

	if (localEnemyType == 0 && !OnSpawnHold)
	{	
		CVector displ = localPlayer->playerModel.GetPositionV() - enemyModel->GetPositionV();
		float distance = hypot(displ.GetX(), displ.GetZ());
		if (distance > 60)
		{
			enemyModel->SetDirectionAndRotationToPoint(localPlayer->playerModel.GetPositionV().GetX(), localPlayer->playerModel.GetPositionV().GetZ());
			enemyModel->SetSpeed(enemySpeed);
			enemyModel->PlayAnimation("run", 22, true);
		}
		else 
		{
			enemyModel->SetSpeed(0);
			enemyModel->PlayAnimation("attack", 12, true);
			Attack();
		}
	}

	//Enemy 2
	else if (localEnemyType == 1 && !OnSpawnHold)
	{
		CVector displ = CVector{ randomPortalPartPos.GetX() , 0, randomPortalPartPos.GetZ() } - enemyModel->GetPositionV();
		float distance = hypot(displ.GetX(), displ.GetZ());


		CVector displPlayer = localPlayer->playerModel.GetPositionV() - enemyModel->GetPositionV();
		float distanceToPlayer = hypot(displPlayer.GetX(), displPlayer.GetZ());

		if (localPortal->isPortalReseting && distanceToPlayer > 60)
		{
			enemyModel->SetDirectionAndRotationToPoint(localPlayer->playerModel.GetPositionV().GetX(), localPlayer->playerModel.GetPositionV().GetZ());
			enemyModel->SetSpeed(enemySpeed);
			enemyModel->PlayAnimation("run", 22, true);
		}

		else if (distance > 60 && !localPortal->isPortalReseting)
		{
			enemyModel->SetSpeed(enemySpeed);
			enemyModel->SetDirectionAndRotationToPoint(randomPortalPartPos.GetX(), randomPortalPartPos.GetZ());
			enemyModel->PlayAnimation("run", 22, true);
		}
		else
		{
			enemyModel->PlayAnimation("attack", 12, true);
			enemyModel->SetSpeed(0);
			Attack();
		}
	}
	//if pre-dead -> play animation of death , set dead status true by the end
	float remainingHpInPercentage = enemyCurrentHp / (enemyMaxHp / 100);
	enemyHpbar.SetHealth(remainingHpInPercentage);
	enemyModel->Update(t);
}

//*************** 2D RENDER ***************
void Enemy::OnDraw(CGraphics* g, CVector enemyPos)
{
	if (isDead|| OnSpawnHold) return;
 
	enemyHpbar.SetPosition(enemyPos.x, enemyPos.y + 30);

	CVector displ = localPlayer->playerModel.GetPositionV() - enemyModel->GetPositionV();
	float distance = hypot(displ.GetX(), displ.GetZ());
	
	if(distance < 1200 && !preDeahAnimation) enemyHpbar.Draw(g);
	 
}

void Enemy::Attack()
{
	if (localEnemyType == 0 && attackDelay < localTime)
	{
		// Attack player  
		if (enemyModel->HitTest(&localPlayer->playerModel))
		{
			localPlayer->playerGettingDamage(enemyDamage);
			attackDelay = 2000 + localTime;
		}
	}
	else if (localEnemyType == 1 && attackDelay < localTime)
	{
		// Attack player 
		if (localPortal->isPortalReseting && enemyModel->HitTest(&localPlayer->playerModel))
		{
			localPlayer->playerGettingDamage(enemyDamage);
			attackDelay = 2000 + localTime;
		}
		// Attack Portal 
		else if	
				 (
				enemyModel->HitTest(&localPortal->portalPartOne) ||
				enemyModel->HitTest(&localPortal->portalPartTwo) ||
				enemyModel->HitTest(&localPortal->portalPartThree) ||
				enemyModel->HitTest(&localPortal->portalPartFour)
				)
			{
				localPortal->portalGettingDamaged(enemyDamage);
				attackDelay = 2000 + localTime;
			}
			
	}
}

//*************** 3D RENDER ***************
void Enemy::OnRender3D(CGraphics* g)
{
	if (isDead) return;
	if(!OnSpawnHold) enemyModel->Draw(g);
 
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

 