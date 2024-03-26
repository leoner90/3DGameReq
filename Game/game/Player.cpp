#include "Game.h"
#include "headers/Player.h"
#include "headers/map.h"
#include "headers/Enemy.h"
#include "headers/Portal.h"
#include "headers/UIDialogBox.h"
#include "headers/Shop.h"
#include "headers/Player.h"


//*************** INIT ***************

Player::Player()
{

	dashVfx.LoadModel("rainDrop/rainDrop.obj");
	dashVfx.SetScale(0.2f);

	onDamageVfx.LoadModel("rainDrop/rainDrop.obj");
	onDamageVfx.SetScale(0.2f);


	isPlayerDead = playerPreDeahAnimation = false;
	font.LoadDefault();

	//model
	playerModel.LoadModel("Player/myne2.md3");
	playerModel.LoadTexture("Player/skin1.png");

	playerModel.SetScale(25.5f);


	//animation
	playerModel.AddAnimation("runF", 42, 63);
	playerModel.AddAnimation("runB", 64, 80);
	playerModel.AddAnimation("runR", 82, 99);
	playerModel.AddAnimation("runL", 101, 117);
	playerModel.AddAnimation("hit", 119, 200);
	playerModel.AddAnimation("idle", 1, 40);
	playerModel.AddAnimation("death", 202, 312);
	playerModel.AddAnimation("dash", 314, 330);

	//Shooting
	bullet.LoadModel("bullet/Bullet2.obj");
	bullet.SetScale(8.f);

	//sounds
	footsteps.Play("footsteps.wav", -1);
	footsteps.SetVolume(20);
	footsteps.Pause();


	//Loot
	lootItemOne.LoadModel("resources/bio.obj"); // weapon 
	lootItemOne.SetScale(25.5f);
	lootItemTwo.LoadModel("resources/techno.obj"); // armor
	lootItemTwo.SetScale(25.5f);
	lootItemThree.LoadModel("resources/bioCORE.obj");
	lootItemThree.SetScale(25.5f);


}

void Player::init()
{
	//reset
	playerModel.SetPosition(5461, 0, 4275);
	isPlayerMoving = false;
	playerShots.delete_all();
	playerCurrentState = UNOCCUPIED;
	curentSkillSelected = DASH;
	currentExp = 0;

	//stats
	playerMaxHp = playerCurrentHp = 500;
	maxEnergy = CurrentEnergy = 100;
	playerCurrentArmor = playerMaxArmor = 100;

	//resources
	armorComponents = weaponComponents = bossLoot = 0;

	isPlayerDead = playerPreDeahAnimation =  false;
	playerdeathAnimationTimer = 0;
	onStartGameEvent = firstBlinkyMeet = false;

	onHitEffect.delete_all();
	dashCoolDown = 0;

	//shooting
	shootingDelay = 400;
	playerDamage = 150;
	chargedDamage = 7; // x

	//Energy Recovery
	energyRegenPerSec = 10;
	armorRegenPerSec = 5;


	//charged Shot
	ShotChargeBar.SetSize(35, 1);
	ShotChargeBar.SetColors(CColor::Yellow(), CColor::Black(), CColor::Black());
	ShotChargeBar.SetHealth(0);

	isPlayerInDamage = false;
	chargedShot = false;
	isShotCharged = false;
	InDamageStunDelayTimer = 0;
	startChargedShotTimer = 0;
	totalTimeToCharge = 2000;
	ShotChargeBarOffset = 150;

	repeatStunDelayTimer = 0;
	dashTimer = 0;

	footsteps.Pause();

}

//*************** UPDATE ***************
void Player::OnUpdate(Uint32 t, bool Dkey, bool Akey, bool Wkey, bool Skey, Map& map, std::vector<Enemy*> AllEnemies, CVector& mousePos, Portal& portal, CVector playerWorldPos)
{
	//if dead -> return (do nothing)
	if (isPlayerDead) return;

	//mirroring
	localMouse = &mousePos; 
	localPortal = &portal;
	localEnemies = AllEnemies;
	localMap = &map;
	localTime = t;

	//delta time
	deltatime = (t - prevFrameTime) / 1000  ; //in sec
	prevFrameTime = t;
 
	

	//if pre-dead -> play animation of death , set dead status true by the end
	float remainingHpInPercentage = playerCurrentHp / (playerMaxHp / 100);
	playerModel.SetHealth(remainingHpInPercentage);

	if (playerPreDeahAnimation)
	{
		if (playerdeathAnimationTimer == 0)
		{
			playerModel.PlayAnimation("death", 43, false);
			playerdeathAnimationTimer = 3000 + t;
		}
		else if (playerdeathAnimationTimer < t) isPlayerDead = true;
		playerModel.SetSpeed(0);
		playerModel.Update(t);
		return;
	}

	//if moving play start footsteps sound
	if ( isPlayerMoving)
	{
		footsteps.Resume();
	}

	//Energy Recovery
 
	if (CurrentEnergy < maxEnergy) CurrentEnergy += deltatime * energyRegenPerSec;
	if (playerCurrentArmor < playerMaxArmor) playerCurrentArmor += deltatime * armorRegenPerSec;

	
	//recovery from hit
	if (isPlayerInDamage && InDamageStunDelayTimer < t)
	{
		isPlayerInDamage = false;
		playerCurrentState = savedPrevPlayerState;
	} 
	if (isPlayerInDamage) 
	{
		playerModel.Update(t);
		return;
	}
 
	//Attack Delay 
	if (attackDelay - t <= 0 && playerCurrentState == INATTACK) playerCurrentState = UNOCCUPIED;
	
	playerCollision(AllEnemies);
	PlayerControl(Dkey, Akey, Wkey, Skey);
	playerShotsHandler();
	lootHandler();

	//Update and deletes
	lootList.delete_if(true);
	lootList.Update(t);

	playerShots.delete_if(true);
	playerShots.Update(t);

	lastFramePos = playerModel.GetPositionV();
	playerModel.Update(t);

	dashEffect.Update(t);
	dashEffect.delete_if(true);

	onHitEffect.Update(t);
	onHitEffect.delete_if(true);




	if (startChargedShotTimer != 0 && !isShotCharged)
	{
		float currentChargeinSec = totalTimeToCharge - (startChargedShotTimer - t);
		int chargePercent = currentChargeinSec / totalTimeToCharge * 100;

		if (chargePercent < 50) ShotChargeBar.SetColor(CColor::Red());
		else if (chargePercent < 100) ShotChargeBar.SetColor(CColor::Yellow());
		else if (chargePercent >= 100) ShotChargeBar.SetColor(CColor::Green());
		ShotChargeBar.SetHealth(chargePercent);


		if (startChargedShotTimer < t)
		{
			isShotCharged = true;
		}
	}

	//WorldToScreenCoordinate(playerModel->enemyModel->GetPositionV())
	ShotChargeBar.SetPosition(playerWorldPos.x, playerWorldPos.y + ShotChargeBarOffset);
	ShotChargeBar.Update(t);
}

//*************** 2D RENDER ***************
void Player::OnDraw(CGraphics* g, UIDialogBox& dialogBox, Shop& shop)
{
	if (!onStartGameEvent) 
	{
		onStartGameEvent = true;
		dialogBox.showBox(0, 15, 17, 2, 4000);
	}

	if (!firstBlinkyMeet)
	{
		CVector displ =  playerModel.GetPositionV() - shop.testRobot.GetPositionV();
		float distance = hypot(displ.GetX(), displ.GetZ());

		if (distance < 300)
		{
			firstBlinkyMeet = true;
			dialogBox.showBox(1, 18, 20, 3, 4000);
		}
	}

	
	if(ShotChargeBar.GetHealth() > 0 ) ShotChargeBar.Draw(g);
}

//*************** 3D RENDER ***************
void Player::OnRender3D(CGraphics* g, CCamera& world)
{
	playerShots.Draw(g);
	playerModel.Draw(g);
	lootList.Draw(g);
	dashEffect.Draw(g);
	onHitEffect.Draw(g);


	
}

 

//*************** PLAYER CONTROLER ***************
void Player::PlayerControl(bool Dkey, bool Akey, bool Wkey , bool Skey )
{
	if (playerModel.AnimationFinished() && playerCurrentState == INDASH) playerCurrentState = UNOCCUPIED;
	if (playerCurrentState == INDASH) return;

	//moving animation
	if (isPlayerMoving)
	{
		CVector charDirectionV = playerModel.GetDirectionV();

		CVector mousePos = CVector(localMouse->GetX(), 0, localMouse->GetZ());
		CVector displ = mousePos - playerModel.GetPositionV();
		CVector normDisp = displ.Normalized();

		float dotProduct = charDirectionV.Dot(normDisp);
		float angle = acos(dotProduct) * 180.0 / M_PI;


	 
		if (angle < 45)
		{
			playerModel.SetSpeed(650);
			playerModel.PlayAnimation("runF", 22, true);
		}
		else if (angle < 135)
		{
			playerModel.SetSpeed(500);
			playerModel.PlayAnimation("runL", 22, true);
		}
		else if (angle > 135)
		{
			playerModel.SetSpeed(500);
			playerModel.PlayAnimation("runB", 22, true);
		}
	}
 


	//moving direction
	if (Wkey)
	{
		isPlayerMoving = true;
		if (Dkey) playerModel.SetDirection(1, -1);
		else if (Akey) playerModel.SetDirection(-1, -1);
		else playerModel.SetDirection(0, -1);	
	}

	else if (Skey)
	{
		isPlayerMoving = true;
		if (Dkey) playerModel.SetDirection(1, 1);
		else if (Akey) playerModel.SetDirection(-1, 1);
		else playerModel.SetDirection(0, 1);
	}

	else if (Dkey) 
	{
		isPlayerMoving = true;
		playerModel.SetDirection(1, 0);
	}
	else if (Akey)
	{
		isPlayerMoving = true;
		playerModel.SetDirection(-1, 0);
	}
	else 
	{
		footsteps.Pause();
		playerModel.SetSpeed(0);
		isPlayerMoving = false;
		playerModel.PlayAnimation("idle", 20, true);
	}
	
}


void Player::OnKeyDown(SDLKey sym, CVector currentMousePos)
{
	if (sym == SDLK_q)
	{
		switch (curentSkillSelected)
		{
		case RECALL:
			if (CurrentEnergy >= 50)
			{
				playerModel.SetPositionV(localPortal->portal.GetPositionV());
				CurrentEnergy -= 50;
			}
			break;
		case DASH:
			if (dashCoolDown < localTime && playerCurrentState != INDASH)
			{
				dashCoolDown = localTime + 3000;
				//dashTimer = 500 + localTime;
				playerCurrentState = INDASH;
				dashSound.Play("dash.wav", 1);
				dashSound.SetVolume(75);
				playerModel.PlayAnimation("dash", 85, false);
			 
				CVector displ = CVector(localMouse->GetX() - playerModel.GetX(), 0, localMouse->GetZ() - playerModel.GetZ());
				CVector dash = displ.Normalized();
				for (int i = 0; i < 120; i++)
				{
					CModel* p = dashVfx.Clone();
					p->SetPositionV(playerModel.GetPositionV() + CVector(0,100,0));
					p->SetColor(CColor::Blue());
					p->SetDirection(rand() % 360);
					p->SetSpeed(rand() % 400);
					p->Die(300);
					dashEffect.push_back(p);
				}
			 
				playerModel.SetPositionV(playerModel.GetPositionV() + dash * 500);
				playerModel.SetSpeed(0);
			}
			break;
		default:
			break;
		}
	}
}

void Player::playerGettingDamage(float damage)
{
	if (playerPreDeahAnimation) return;

	for (int i = 0; i < 25; i++)
	{
		CModel* p = onDamageVfx.Clone();
		p->SetPositionV(playerModel.GetPositionV() + CVector(0, 100, 0));
		p->SetDirection(rand() % 360);
		p->SetSpeed(rand() % 200);
		p->SetColor(CColor::Red());
		p->Die(270);
		onHitEffect.push_back(p);
	}

	playerCurrentArmor -= damage;

	if (playerCurrentArmor < 0)
	{
		playerCurrentHp -= damage;
		playerCurrentArmor = 0;
	}

	if (playerCurrentHp <= 0)
	{
		playerCurrentHp = 0;
		playerPreDeahAnimation = true;
		deathSound.Play("PlayerDeath3.wav", 1);
	}
	else
	{
		if (repeatStunDelayTimer < localTime)
		{
			hitSound.Play("PlayerHurt.wav", 1);
			hitSound.SetVolume(75);
			repeatStunDelayTimer = 1500 + localTime;
			playerModel.PlayAnimation("hit", 162, false);
			InDamageStunDelayTimer = 500 + localTime;
			savedPrevPlayerState = playerCurrentState;
			playerCurrentState = INDAMAGE;
			isPlayerInDamage = true;
			playerModel.SetSpeed(0);
		}
	
	}
 
}

void Player::playerCollision(std::vector<Enemy*> AllEnemies)
{
	//end of the map
	for (auto wallSeg : localMap->mapCollision)
	{
		if (playerModel.HitTest(wallSeg))
		{
			playerModel.SetPositionV(lastFramePos);
		}
	}

	//map objects
	for (auto collidingObj : localMap->collidingObjects)
	{
		float distance = 100;
		if (collidingObj->GetStatus() == 4) distance = 300; // if rock
		if (playerModel.HitTest(collidingObj->GetPositionV(), distance))
		{
			playerModel.SetPositionV(lastFramePos);
		}
	}
	 
	//enemies
	/*for (auto enemy : AllEnemies) {
		if (enemy->deathAnimationTimer) continue;
		if (playerModel.HitTest(enemy->enemyModel)) 
		{
			playerModel.SetPositionV(lastFramePos);
		}
	}*/

}

void Player::addLoot(int enemyType, CVector enemyPos)
{
	float chance = rand() % 100;
	if (chance < 40 && enemyType != 2) return;
	if(enemyType ==  0) lootList.push_back(lootItemOne.Clone());
	else if (enemyType == 1) lootList.push_back(lootItemTwo.Clone());
	else if (enemyType == 2) lootList.push_back(lootItemThree.Clone());
	lootList.back()->SetPositionV(enemyPos);
	lootList.back()->SetStatus(enemyType);
}

void Player::playerShotsHandler()
{
	for (CModel* pShot : playerShots)
	{
		for (auto enemy : localEnemies)
		{
			if (pShot->HitTest(enemy->enemyModel->GetPositionV(), 250)) //add distance
			{
				if (enemy->preDeahAnimation) continue;
				pShot->Delete();
				float damage;

				if (chargedShot)  
				{
					damage = playerDamage * chargedDamage;
					chargedShot = false;
				}
				else damage = playerDamage;

				enemy->EnemyGetDamage(damage, onDamageVfx); // to redo vfx ......
			}
		}
	}
}

void Player::lootHandler()
{
	//LOOT - DROP
	for (auto item : lootList)
	{
		if (playerModel.HitTest(item->GetPositionV(), 150))
		{
			int itemType = item->GetStatus();
			switch (itemType)
			{
			case 0:
				armorComponents++;
				break;
			case 1:
				weaponComponents++;
				break;
			case 2:
				bossLoot++;
				break;
			default:
				break;
			}
			item->Delete();
			PickUpLootSound.Play("pickUp.wav", 1);
		}
	}
}

void Player::performShot()
{
	playerCurrentState = INATTACK;
	attackDelay = localTime + shootingDelay;
	//chargedShot

	CModel* pShot = bullet.Clone();
	pShot->SetPositionV(playerModel.GetPositionV() + CVector(0, 100, 0));
	pShot->SetDirectionAndRotationToPoint(localMouse->GetX(), localMouse->GetZ());
	pShot->SetSpeed(3000);
	pShot->Die(500);
	playerShots.push_back(pShot);

	//sound
	shotSound.Play("shot.wav", 1);
	shotSound.SetVolume(30);
 
}

 
void Player::OnMouseMove(CVector currentMousePos)
{
	playerModel.SetRotationToPoint(currentMousePos.x, currentMousePos.z);
}

void Player::OnLButtonDown(CVector pos, CVector currentMousePos, long t)
{
	if (playerCurrentState == UNOCCUPIED &&  CurrentEnergy >= 5)
	{
		CurrentEnergy -= 5;
		performShot();
	}
}


void Player::OnRButtonDown(long t)
{
	if (playerCurrentState == UNOCCUPIED && CurrentEnergy > 60)
	{
		//chargedShot
		if (startChargedShotTimer == 0) startChargedShotTimer = t + totalTimeToCharge;
		charginShotSound.Play("chargedshot.wav");
	}
}


void Player::OnRButtonUp()
{
	startChargedShotTimer = 0;
	ShotChargeBar.SetHealth(0);

	if (isShotCharged)
	{
		//setStatus
		CurrentEnergy -= 60;
		chargedShot = true;
		performShot();
		isShotCharged = false;
	}
}

 


 
void Player::playerOnWheelUp()
{
	//if (curentSkillSelected == RECALL) curentSkillSelected = DASH;
	//else if (curentSkillSelected == DASH) curentSkillSelected = RECALL;
}

void Player::playerOnWheelDown()
{
	//if (curentSkillSelected == RECALL) curentSkillSelected = DASH;
	//else if (curentSkillSelected == DASH) curentSkillSelected = RECALL;
}
