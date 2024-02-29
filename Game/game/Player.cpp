#include "Game.h"
#include "headers/Player.h"
#include "headers/map.h"
#include "headers/Enemy.h"

//*************** INIT ***************

void Player::init()
{
	font.LoadDefault();
	//Player 3D
	playerModel.LoadModel("Player/player.md3");
	playerModel.SetScale(15.5f);
	playerModel.SetPosition(800, 0, 300);
	playerModel.AddAnimation("run", 1, 22);
	playerModel.AddAnimation("idle", 23, 86);

	//delete Shots
	playerShots.delete_all();

	//bullet
	bullet.LoadModel("grass/grass.obj");
	bullet.SetScale(0.2f);

	mouseControl = false;
	playerCurrentState = UNOCCUPIED;
	curentSkillSelected = RECALL;
	shootingDelay = 1000;
	maxHp = currentHp = 1000;
	maxEnergy = CurrentEnergy = 100;
	playerDamage = 100;
}

//*************** UPDATE ***************
void Player::OnUpdate( long t, bool Dkey, bool Akey, bool Wkey, bool Skey, Map& map , std::vector<Enemy*> AllEnemies)
{
	
	localMap = &map;
	//Attack Delay 
	if (attackDelay - t <= 0) playerCurrentState = UNOCCUPIED;

	//playerShots.delete_if(deleted);
	PlayerControl(Dkey, Akey, Wkey, Skey);
	playerCollision(AllEnemies);

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
				enemy->enemyModel.SetHealth(0);
				enemy->isDead = true;
			}
		}
	}

	

	playerShots.delete_if(true);

	playerShots.Update(t);
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
		//footsteps.Play("playerFootsteps.wav");
		playerModel.PlayAnimation("run", 30, true);
		playerModel.SetSpeed(400);
	}
	else if (Skey) 
	{
		playerModel.SetSpeed(-200);
		playerModel.PlayAnimation("run", 30, true);
	}
	else 
	{
		footsteps.Stop();
		playerModel.SetSpeed(0);
		playerModel.PlayAnimation("idle", 30, true);
		
	}

	//model rotation
	if (Dkey) playerModel.Rotate(0, -3, 0);
	if (Akey) playerModel.Rotate(0, 3, 0);
 

	// align direction with rotation
	playerModel.SetDirectionV(playerModel.GetRotationV());
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
		

			playerModel.SetPositionV(playerModel.GetPositionV() + CVector(xVel, 0 , zVel));
			break;
		default:
			break;
		}
	}

 
}

void Player::playerGettingDamage(float damage)
{

}

void Player::playerCollision(std::vector<Enemy*> AllEnemies)
{
	for (auto wallSeg : localMap->mapCollision)
	{
		if (playerModel.HitTest(wallSeg))
		{
			playerModel.SetSpeed(0);
		}
	}

	for (auto enemy : AllEnemies) {
		if (playerModel.HitTest(&enemy->enemyModel)) 
		{
			//playerModel.SetSpeed(0);
			//playerModel.PlayAnimation("stand", 7, true);
		}
	}

	
}


void Player::OnLButtonDown(CVector pos, CVector currentMousePos, long t)
{
	if (playerCurrentState == UNOCCUPIED)
	{
		playerCurrentState = INATTACK;
		attackDelay = t + shootingDelay;
		playerModel.Stop();
		//playerModel.SetRotationToPoint(currentMousePos.x, currentMousePos.z);
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
		shotSound.Stop();
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
