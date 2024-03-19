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
	//rainDrop.SetY(50);
	dashVfx.SetScale(0.5f);


	isPlayerDead = playerPreDeahAnimation = false;
	font.LoadDefault();

	//model
	playerModel.LoadModel("Player/myra.md3");
	playerModel.LoadTexture("Player/myraTextures.png");
	playerModel.SetScale(25.5f);


	//animation
	playerModel.AddAnimation("runF", 255, 276);
	playerModel.AddAnimation("runB", 280, 295);
	playerModel.AddAnimation("runR", 300, 317);
	playerModel.AddAnimation("runL", 320, 336);
	playerModel.AddAnimation("hit", 340, 421);
	playerModel.AddAnimation("idle", 1, 249);
	playerModel.AddAnimation("death", 425, 556);
	playerModel.AddAnimation("dash", 560, 576);

	//Shooting
	bullet.LoadModel("bullet/Bullet2.obj");
	bullet.SetScale(25.f);
	shootingDelay = 500;
	playerDamage = 100;

	//sounds
	footsteps.Play("playerFootsteps.wav", -1);
	footsteps.SetVolume(3);
	footsteps.Pause();

}

void Player::init()
{
	//reset
	playerModel.SetPosition(5461, 0, 4275);
	isPlayerMoving = false;
	playerShots.delete_all();
	playerCurrentState = UNOCCUPIED;
	curentSkillSelected = 0;

	//stats
	playerMaxHp = playerCurrentHp = 1000;
	maxEnergy = CurrentEnergy = 100;
	playerCurrentArmor = playerMaxArmor = 100;

	//resources
	armorComponents = weaponComponents = 0;

	isPlayerDead = playerPreDeahAnimation =  false;
	playerdeathAnimationTimer = 0;
	onStartGameEvent = firstBlinkyMeet = false;
}

//*************** UPDATE ***************
void Player::OnUpdate(Uint32 t, bool Dkey, bool Akey, bool Wkey, bool Skey, Map& map, std::vector<Enemy*> AllEnemies, CVector& mousePos, Portal& portal)
{
	localMouse = &mousePos; 
	localPortal = &portal;
	localMap = &map;

	//getDeltaTime
	deltatime = (t - prevFrameTime) / 1000  ; //in sec
	prevFrameTime = t;
 
	//if dead -> return (do nothing)
	if (isPlayerDead) return;

	//if pre-dead -> play animation of death , set dead status true by the end
	float remainingHpInPercentage = playerCurrentHp / (playerMaxHp / 100);
	playerModel.SetHealth(remainingHpInPercentage);

	if (playerPreDeahAnimation)
	{
		if (playerdeathAnimationTimer == 0)
		{
			playerModel.PlayAnimation("death", 43, true);
			playerdeathAnimationTimer = 3000 + t;
		}
		else if (playerdeathAnimationTimer < t) isPlayerDead = true;
		playerModel.Update(t);
		return;
	}

	if (playerModel.GetSpeed() > 0 && !isPlayerMoving)
	{
		//moving 
		isPlayerMoving = true;
		footsteps.Resume();
	}

	//Energy Recovery
	float energyRegenPerSec = 50;
	float armorRegenPerSec = 10;
	if (CurrentEnergy < maxEnergy) CurrentEnergy += deltatime * energyRegenPerSec;

	 
	if (playerCurrentArmor < playerMaxArmor) playerCurrentArmor += deltatime * armorRegenPerSec;

	
	//Attack Delay 
	if (attackDelay - t <= 0 && playerCurrentState == INATTACK) playerCurrentState = UNOCCUPIED;
	//playerShots.delete_if(deleted);
	
	playerCollision(AllEnemies);
	PlayerControl(Dkey, Akey, Wkey, Skey);

	for (CModel* pShot : playerShots)
	{ 
		for (auto enemy : AllEnemies) 
		{
			if (pShot->HitTest(enemy->enemyModel->GetPositionV() , 250)) //add distance
			{
				if (enemy->preDeahAnimation) continue;
				pShot->Delete();
				enemy->EnemyGetDamage(playerDamage);
			}
		}
	}

	playerShots.delete_if(true);
	playerShots.Update(t);
	lastFramePos = playerModel.GetPositionV();
	playerModel.Update(t);

	dashEffect.Update(t);
	dashEffect.delete_if(true);
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

		if (distance < 1200)
		{
			firstBlinkyMeet = true;
			dialogBox.showBox(1, 18, 20, 3, 4000);
		}
	}
}

//*************** 3D RENDER ***************
void Player::OnRender3D(CGraphics* g, CCamera& world)
{
	playerShots.Draw(g);
	playerModel.Draw(g);

	dashEffect.Draw(g);
	
}

//*************** PLAYER CONTROLER ***************
void Player::PlayerControl(bool Dkey, bool Akey, bool Wkey , bool Skey )
{
	if (playerModel.AnimationFinished() && playerCurrentState == INDASH) playerCurrentState = UNOCCUPIED;
	if (playerCurrentState == INDASH) return;

	//moving back/forward
	if (Wkey)
	{
		if (Dkey)
		{
			playerModel.SetDirection(1, -1);
			playerModel.SetSpeed(400);
		}

		else if (Akey)
		{
			playerModel.SetDirection(-1, -1);
			playerModel.SetSpeed(400);
		}

		else
		{
			playerModel.SetDirection(0, -1);
			playerModel.SetSpeed(400);
		}
	}

	else if (Skey)
	{
		if (Dkey) playerModel.SetDirection(1, 1);
		else if (Akey) playerModel.SetDirection(-1, 1);
		else playerModel.SetDirection(0, 1);
	}

	else if (Dkey) playerModel.SetDirection(1, 0);
	else if (Akey) playerModel.SetDirection(-1,0);

	else 
	{
		footsteps.Pause();
		playerModel.SetSpeed(0);
		isPlayerMoving = false;
		playerModel.PlayAnimation("idle", 20, true);
	}


	float x = playerModel.GetXVelocity();
	float y = playerModel.GetZVelocity();
	//int dotproduct = playerModel.GetXVelocity() * localMouse.GetX() + playerModel.GetZVelocity() * localMouse.GetZ();
	//int crossProduct = playerModel.GetXVelocity() * localMouse.GetZ() + playerModel.GetZVelocity() * localMouse.GetX();

	if ((playerModel.GetZ() > localMouse->GetZ() || playerModel.GetX() > localMouse->GetX()) && Wkey )
	{
		playerModel.SetSpeed(450);
		playerModel.PlayAnimation("runF", 22, true);
	}
	
	else if ((playerModel.GetZ() < localMouse->GetZ() || playerModel.GetX() < localMouse->GetX()) && Wkey)
	{
		playerModel.SetSpeed(400);
		playerModel.PlayAnimation("runB", 22, true);
	}
	
 
	if (playerModel.GetZ() > localMouse->GetZ() && Skey)
	{
		playerModel.SetSpeed(400);
		playerModel.PlayAnimation("runB", 22, true);
	}
		
	else if (playerModel.GetZ() < localMouse->GetZ() && Skey)
	{
		playerModel.SetSpeed(450);
		playerModel.PlayAnimation("runF", 22, true);
	}
		
	if (playerModel.GetX() > localMouse->GetX() && Dkey)
	{
		playerModel.SetSpeed(400);
		playerModel.PlayAnimation("runR", 22, true);
	}

	else if (playerModel.GetX() < localMouse->GetX() && Dkey)
	{
		playerModel.SetSpeed(400);
		playerModel.PlayAnimation("runL", 22, true);
	}

	if (playerModel.GetX() > localMouse->GetX() && Akey)
	{
		playerModel.SetSpeed(400);
		playerModel.PlayAnimation("runL", 22, true);
	}

	else if (playerModel.GetX() < localMouse->GetX() && Akey)
	{
		playerModel.SetSpeed(400);
		playerModel.PlayAnimation("runR", 22, true);
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
			if (CurrentEnergy >= 25 && playerCurrentState != INDASH)
			{
				CurrentEnergy -= 25;
				playerCurrentState = INDASH;
				playerModel.PlayAnimation("dash", 150, false);
			 
				CVector displ = CVector(localMouse->GetX() - playerModel.GetX(), 0, localMouse->GetZ() - playerModel.GetZ());
				CVector dash = displ.Normalized();
				for (int i = 0; i < 50; i++)
				{
					CModel* p = dashVfx.Clone();
					p->SetPositionV(playerModel.GetPositionV() + CVector(0,100,0));
					p->SetDirection(playerModel.GetRotation());
					p->SetSpeed(rand() % 1200);
					p->Die(500);
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
	}
 
}

void Player::playerCollision(std::vector<Enemy*> AllEnemies)
{
	for (auto wallSeg : localMap->mapCollision)
	{
		if (playerModel.HitTest(wallSeg))
		{
			playerModel.SetPositionV(lastFramePos);
		}
	}

	for (auto collidingObj : localMap->collidingObjects)
	{
		float distance = 100;
		if (collidingObj->GetStatus() == 4) distance = 300; // if rock
		if (playerModel.HitTest(collidingObj->GetPositionV(), distance))
		{
			playerModel.SetPositionV(lastFramePos);
		}
	}
	 
	for (auto enemy : AllEnemies) {
		if (enemy->deathAnimationTimer) continue;
		if (playerModel.HitTest(enemy->enemyModel)) 
		{
			playerModel.SetPositionV(lastFramePos);
		}
	}
}

 
void Player::OnMouseMove(CVector currentMousePos)
{
	playerModel.SetRotationToPoint(currentMousePos.x, currentMousePos.z);
}

void Player::OnLButtonDown(CVector pos, CVector currentMousePos, long t)
{
	if (playerCurrentState == UNOCCUPIED )
	{
		playerCurrentState = INATTACK;
		attackDelay = t + shootingDelay;
 
		
		CModel* pShot = bullet.Clone();
		pShot->SetPositionV(playerModel.GetPositionV() + CVector(0, 100, 0));
		pShot->SetDirectionAndRotationToPoint(localMouse->GetX(), localMouse->GetZ());
		pShot->SetSpeed(3000);
		pShot->Die(500);
		playerShots.push_back(pShot);

		//sound
		shotSound.Play("shotSound.wav");
		shotSound.SetVolume(20);
	}
}

void Player::playerOnWheelUp()
{
	if (curentSkillSelected == RECALL) curentSkillSelected = DASH;
	else if (curentSkillSelected == DASH) curentSkillSelected = RECALL;
}

void Player::playerOnWheelDown()
{
	if (curentSkillSelected == RECALL) curentSkillSelected = DASH;
	else if (curentSkillSelected == DASH) curentSkillSelected = RECALL;
}
