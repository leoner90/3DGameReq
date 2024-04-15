#include "Game.h"
#include "headers/Enemy.h"
#include "headers/Player/Player.h"
#include "headers/Map.h"
#include <math.h> 
#include "headers/Portal.h"

//*************** INIT ***************

Enemy::Enemy()
{

}

Enemy::~Enemy()
{
	delete enemyModel;
}

void Enemy::init(CVector enemyPos, int enemyType, Map& map, Portal& portal, CModelMd3& enemyMod)
{
	localPortal = &portal;
	enemyModel = enemyMod.Clone();

	localEnemyType = enemyType;

	if (localEnemyType == 0) 
	{
		enemyMaxHp = enemyCurrentHp = 200;
		enemyDamage = 50;
		enemySpeed = 400 + rand() % 250;
		enemyModel->AddAnimation("run", 1, 29);
		enemyModel->AddAnimation("attack", 35, 59);
		enemyModel->AddAnimation("dead", 65, 82);
	}
	else if (localEnemyType == 1)
	{
		enemyDamage = 50;
		enemyMaxHp = enemyCurrentHp = 400;
	
		enemySpeed = 350 + rand() % 250;

		enemyModel->AddAnimation("attack", 0, 10);
		enemyModel->AddAnimation("run", 10, 40);
		enemyModel->AddAnimation("dead", 42, 70);
		
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

	//boss
	else if (localEnemyType == 2)
	{
		enemyDamage = 350;
		enemyMaxHp = enemyCurrentHp = 2400;

		enemySpeed = 400 + rand() % 250;

		enemyModel->AddAnimation("run", 10, 44);
		enemyModel->AddAnimation("attack", 50, 72);
	//	enemyModel->AddAnimation("chargingForAttack", 83, 125);
	//	enemyModel->AddAnimation("chargedAttack", 141, 153);
	//	enemyModel->AddAnimation("chargeAttackStop", 160, 165);
		enemyModel->AddAnimation("dead", 170, 199);
	}
	
	// enemy model

	//enemyModel.SetToFloor(0);
	enemyModel->SetPositionV(enemyPos);
	
	enemyModel->SetDirectionAndRotationToPoint(0, 0);
	isDead = preDeahAnimation = false;
	
	enemyHpbar.SetSize(35, 3);
	enemyHpbar.SetColor(CColor::Blue());
	enemyHpbar.SetHealth(100);

	deathAnimationTimer = 0;

	//wait at spawn point
	OnSpawnHold = true;

	enemyModel->SetToFloor(0);

	onHitEffect.delete_all();

	bossChargedAttackDelay = 0;
	isBossChargingAttack = false;
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
			int deathFrames = 16;
			if (localEnemyType == 2) deathFrames = 30;
			enemyModel->PlayAnimation("dead", deathFrames, false);
			enemyModel->SetSpeed(0);
			
			deathAnimationTimer = 1000 + t;

			
		}
		else if(deathAnimationTimer < t) isDead = true;
		
		onHitEffect.Update(t);
		onHitEffect.delete_if(true);
		enemyModel->Update(t);
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

	if ((localEnemyType == 0 || localEnemyType == 2)  && !OnSpawnHold)
	{	
		CVector displ = localPlayer->playerModel.GetPositionV() - enemyModel->GetPositionV();
		float distance = hypot(displ.GetX(), displ.GetZ());
		if (distance > 60)
		{
			enemyModel->SetDirectionAndRotationToPoint(localPlayer->playerModel.GetPositionV().GetX(), localPlayer->playerModel.GetPositionV().GetZ());
			enemyModel->SetSpeed(enemySpeed);
			enemyModel->PlayAnimation("run", 42, true);
		}
		else 
		{
			enemyModel->SetSpeed(0);
			enemyModel->PlayAnimation("attack", 24, true);
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
			enemyModel->PlayAnimation("attack", 10, true);
			enemyModel->SetSpeed(0);
			Attack();
		}
	}
	//if pre-dead -> play animation of death , set dead status true by the end
	float remainingHpInPercentage = enemyCurrentHp / (enemyMaxHp / 100);
	enemyHpbar.SetHealth(remainingHpInPercentage);
	enemyModel->Update(t);

	onHitEffect.Update(t);
	onHitEffect.delete_if(true);
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
			attackDelay = 1000 + localTime;
		}
	}
	else if (localEnemyType == 1 && attackDelay < localTime)
	{
		// Attack player 
		if (localPortal->isPortalReseting && enemyModel->HitTest(&localPlayer->playerModel))
		{
			localPlayer->playerGettingDamage(enemyDamage);
			attackDelay = 1000 + localTime;
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
				attackDelay = 1000 + localTime;
			}
			
	}
	else if (localEnemyType == 2 && attackDelay < localTime)
	{

		/* // CHARGED ATTACK TO DO
		if (bossChargedAttackDelay < localTime && !isBossChargingAttack)
		{
			isBossChargingAttack = true;
			bossChargedAttackDelay = 20000 + localTime; //delay

			chargingAttack = 1000 + localTime;
			enemyModel->PlayAnimation("chargingForAttack", 42, false);
		}

 

		if (isBossChargingAttack && chargingAttack < localTime) 
		{
			isBossChargingAttack = false;
			enemyModel->PlayAnimation("chargedAttack", 12, false);
		}
		else
		{
			return;
		}
		*/
 
		//enemyModel->AddAnimation("chargeAttackStop", 160, 165);


		// Attack player  
		if (enemyModel->HitTest(&localPlayer->playerModel))
		{
			localPlayer->playerGettingDamage(enemyDamage);
			attackDelay = 2000 + localTime;
		}
	}
}

//*************** 3D RENDER ***************
void Enemy::OnRender3D(CGraphics* g)
{
	if (isDead) return;
	if(!OnSpawnHold) enemyModel->Draw(g);
	onHitEffect.Draw(g);
}

void Enemy::EnemyGetDamage(float damage , CModel Vfx)
{
	if (preDeahAnimation) return;
	for (int i = 0; i < 15; i++)
	{
		CModel* p = Vfx.Clone();
		p->SetPositionV(enemyModel->GetPositionV() + CVector(0, 100, 0));
		p->SetDirection(rand() % 360);
		p->SetSpeed(rand() % 200);
		p->SetColor(CColor::Green());
		p->Die(250);
		onHitEffect.push_back(p);
	}



	OnSpawnHold = false; // in case attacking at spawn point

	enemyCurrentHp -= damage;
	//enemyCurrentHp > 0 ? enemyHpbar.SetHealth(enemyCurrentHp) : enemyHpbar.SetHealth(0);

	if (enemyCurrentHp <= 0)
	{
		deathSound.Play("monsterDeath.wav");
		localPlayer->addLoot(localEnemyType, enemyModel->GetPositionV());
		preDeahAnimation = true;
		localPlayer->currentExp += enemyMaxHp;
	}
}

 