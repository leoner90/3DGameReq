#include "Game.h"
#include "headers/Player.h"
#include "headers/map.h"
#include "headers/Enemy.h"

//*************** INIT ***************

void Player::init()
{
	font.LoadDefault();

	

	//model
	playerModel.LoadModel("Player/player.md3");
	playerModel.SetScale(15.5f);
	playerModel.SetPosition(800, 0, 300);

 
	//animation
	playerModel.AddAnimation("run", 1, 75);
	playerModel.AddAnimation("idle", 80, 160);


	//moving 
	footsteps.Play("playerFootsteps.wav");
	footsteps.SetVolume(3);
 

	//Shooting
	bullet.LoadModel("grass/grass.obj");
	bullet.SetScale(0.2f);
	shootingDelay = 1000;
	playerDamage = 100;
	playerShots.delete_all();
 
	playerCurrentState = UNOCCUPIED;
	curentSkillSelected = RECALL;

	//stats
	playerMaxHp = playerCurrentHp = 1000;
	maxEnergy = CurrentEnergy = 100;
	playerCurrentArmor = playerMaxArmor = 100;

	//resources
	armorComponents = weaponComponents = 0;

	isPlayerDead = false;
}

//*************** UPDATE ***************
void Player::OnUpdate( long t, bool Dkey, bool Akey, bool Wkey, bool Skey, Map& map , std::vector<Enemy*> AllEnemies)
{

	//Energy Recovery
	if(CurrentEnergy < maxEnergy) CurrentEnergy += 0.25;
	if (playerCurrentArmor < playerMaxArmor) playerCurrentArmor += 0.1;

	localMap = &map;
	//Attack Delay 
	if (attackDelay - t <= 0) playerCurrentState = UNOCCUPIED;

	//playerShots.delete_if(deleted);
	
	playerCollision(AllEnemies);
	PlayerControl(Dkey, Akey, Wkey, Skey);

	for (CModel* pShot : playerShots)
	{   
		if (pShot->HitTest(&map.portal) && map.portal.GetHealth() > 0)
		{
			pShot->Delete();
			map.portal.SetHealth(map.portal.GetHealth() - 10);
		}

		for (auto enemy : AllEnemies) {
			if (pShot->HitTest(&enemy->enemyModel))
			{
				pShot->Delete();
			 
				enemy->EnemyGetDamage(playerDamage);
			 
			}
		}
	}

	

	playerShots.delete_if(true);

	playerShots.Update(t);
	lastFramePos = playerModel.GetPositionV();
	playerModel.Update(t);

}

//*************** 2D RENDER ***************
void Player::OnDraw(CGraphics* g)
{
	font.DrawText(100, 100, "Current Skill is:", CColor::White(), 22);
	font.DrawText(300, 100, curentSkillSelected == 0 ? "Recall" : "DASH_FORWARD", CColor::White(), 22);
	
}

//*************** 3D RENDER ***************
void Player::OnRender3D(CGraphics* g, CCamera& world)
{
	playerShots.Draw(g);
	playerModel.Draw(g);;	
}

//*************** PLAYER CONTROLER ***************
void Player::PlayerControl(bool Dkey, bool Akey, bool Wkey , bool Skey)
{
	 
	//moving back/forward
 
	if (Wkey)
	{
		playerModel.SetDirection(90);
		playerModel.SetSpeed(400);
		playerModel.PlayAnimation("run", 30, true);
	}
	else if (Skey && !Wkey)
	{
		playerModel.SetDirection(-90);
		playerModel.SetSpeed(400);
		playerModel.PlayAnimation("run", 30, true);
	}

	else if (Dkey && !Wkey)
	{
		playerModel.SetDirection(0);
		playerModel.SetSpeed(400);
		playerModel.PlayAnimation("run", 30, true);
	}
	else if (Akey )
	{
		playerModel.SetDirection(180);
		playerModel.SetSpeed(400);
		playerModel.PlayAnimation("run", 30, true);
	}
 
	else 
	{
		footsteps.Pause();
		playerModel.SetSpeed(0);
		playerModel.PlayAnimation("idle", 30, true);

	}

	//model rotation
	//if (Dkey) playerModel.Rotate(0, -3, 0);
	//if (Akey) playerModel.Rotate(0, 3, 0);



	// align direction with rotation
	//playerModel.SetDirectionV(playerModel.GetRotationV());
}


void Player::OnKeyDown(SDLKey sym, CVector currentMousePos)
{
	if (sym == SDLK_q)
	{
		float xVel = playerModel.GetXVelocity() * 1.2;
		float zVel = playerModel.GetZVelocity() * 1.2;

		switch (curentSkillSelected)
		{
		case RECALL:
			playerModel.SetPositionV(localMap->portal.GetPositionV());
			break;
		case DASH:
			if (CurrentEnergy >= 25)
			{
				CurrentEnergy -= 25;
				playerModel.SetPositionV(playerModel.GetPositionV() + CVector(xVel, 0, zVel));
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
		playerCurrentHp = -playerCurrentArmor;
		playerCurrentArmor = 0;
	}

	if (playerCurrentHp <= 0)
	{
		playerCurrentHp = 0;
		isPlayerDead = true;
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

	for (auto enemy : AllEnemies) {
		if (playerModel.HitTest(&enemy->enemyModel)) 
		{
			playerModel.SetPositionV(lastFramePos);
		}
	}

	
}


void Player::OnLButtonDown(CVector pos, CVector currentMousePos, long t)
{
	if (playerCurrentState == UNOCCUPIED )
	{
		playerCurrentState = INATTACK;
		attackDelay = t + shootingDelay;
		playerModel.Stop();
		playerModel.SetRotationToPoint(currentMousePos.x, currentMousePos.z);
 
	 
		// create a new shot as a short line
		CModel* pShot = bullet.Clone();
		pShot->SetPositionV(playerModel.GetPositionV() + CVector(0, 100, 0));
		// rotation and direction same as the player
		pShot->SetDirection(playerModel.GetRotation());
		//pShot->SetRotation(playerModel.GetRotation());
		pShot->SetSpeed(2000);
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
