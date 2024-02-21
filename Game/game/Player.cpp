#include "Game.h"
#include "headers/Player.h"
#include "headers/map.h"
#include "headers/Enemy.h"

//*************** INIT ***************

void Player::init()
{
	//Player 3D
	playerModel.LoadModel("test/test.obj");
	playerModel.SetScale(3.5f);
	playerModel.SetPosition(400, 100, 300);

	//delete Shots
	playerShots.delete_all();

	//bullet
	bullet.LoadModel("grass/grass.obj");
	bullet.SetScale(0.2f);

	mouseControl = false;
	playerCurrentState = UNOCCUPIED;
}

//*************** UPDATE ***************
void Player::OnUpdate( long t, bool Dkey, bool Akey, bool Wkey, bool Skey, Map& map , std::vector<Enemy*> AllEnemies)
{
	
	
	//Attack Delay 
	if (attackDelay - t <= 0) playerCurrentState = UNOCCUPIED;

	//playerShots.delete_if(deleted);
	PlayerControl(Dkey, Akey, Wkey, Skey);
	playerCollision(AllEnemies);

	for (CModel* pShot : playerShots)
	{
		// remove shots when hitting the house	    
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


	if (!mouseControl)
	{ 
		//moving back/forward
		if (Wkey)
		{
			playerModel.SetSpeed(500);
			playerModel.PlayAnimation("run", 7, true);
		}
		else if (Skey) 
		{
			playerModel.SetSpeed(-200);
			playerModel.PlayAnimation("run", 2, true);
		}
		else 
		{
			playerModel.SetSpeed(0);
			playerModel.PlayAnimation("stand", 7, true);
		}

		//model rotation
		if (Dkey) playerModel.Rotate(0, -3, 0);
		if (Akey) playerModel.Rotate(0, 3, 0);
	}

	//Mouse Movement
	else
	{

		if (playerModel.IsAutoMoving())
		{
			playerModel.PlayAnimation("run", 7, true);
		}
		else
		{
			//footsteps.Stop();
			playerModel.SetSpeed(0);
			playerModel.PlayAnimation("stand", 7, true);
		}
	}

	// align direction with rotation
	playerModel.SetDirectionV(playerModel.GetRotationV());
}


void Player::OnKeyDown(SDLKey sym, CVector currentMousePos)
{
	//SHOTS
	if (sym == SDLK_q && mouseControl)
	{
		playerModel.Stop();
		playerModel.SetRotationToPoint(currentMousePos.x, currentMousePos.z);
		// create a new shot as a short line
		CModel* pShot = bullet.Clone();
		pShot->SetPositionV(playerModel.GetPositionV());
		// rotation and direction same as the player
		pShot->SetDirection(playerModel.GetRotation());
		pShot->SetRotation(playerModel.GetRotation());
		pShot->SetSpeed(2000);
		pShot->Die(2000);
		playerShots.push_back(pShot);
		//sound
		shotSound.Stop();
		shotSound.Play("shotSound.wav");
		shotSound.SetVolume(20);
	}

	if (sym == SDLK_p) mouseControl = !mouseControl;


}

void Player::playerCollision(std::vector<Enemy*> AllEnemies)
{
	for (auto enemy : AllEnemies) {
		if (playerModel.HitTest(&enemy->enemyModel)) 
		{
			playerModel.SetSpeed(0);
			playerModel.PlayAnimation("stand", 7, true);
		}
	}

	
}


void Player::OnRButtonDown(CVector pos, CVector currentMousePos, long t)
{
	if (mouseControl)
	{
		playerModel.SetRotationToPoint(pos.x, pos.z);
		//	player->playerModel.SetDirection(player->playerModel.GetRotation());
		playerModel.MoveTo(pos.x, pos.z, 400);
		footsteps.Stop();
		footsteps.Play("playerFootsteps.wav", -1);
		footsteps.SetVolume(10);
	}
	else if(playerCurrentState == UNOCCUPIED)
	{
		playerCurrentState = INATTACK;
		attackDelay = t + 1000;
		playerModel.Stop();
		playerModel.SetRotationToPoint(currentMousePos.x, currentMousePos.z);
		// create a new shot as a short line
		CModel* pShot = bullet.Clone();
		pShot->SetPositionV(playerModel.GetPositionV());
		// rotation and direction same as the player
		pShot->SetDirection(playerModel.GetRotation());
		pShot->SetRotation(playerModel.GetRotation());
		pShot->SetSpeed(2000);
		pShot->Die(2000);
		playerShots.push_back(pShot);
		//sound
		shotSound.Stop();
		shotSound.Play("shotSound.wav");
		shotSound.SetVolume(20);
	}
}