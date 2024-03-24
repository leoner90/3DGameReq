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
	playerModel.LoadModel("Player/myra.md3");
	//playerModel.LoadTexture("Player/text.png");
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
	bullet.SetScale(25.f);

	//sounds
	footsteps.Play("playerFootsteps.wav", -1);
	footsteps.SetVolume(3);
	footsteps.Pause();


	//Loot
	lootItemOne.LoadModel("resources/techno.obj");
	lootItemOne.SetScale(25.5f);
	lootItemTwo.LoadModel("resources/bio.obj");
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
	shootingDelay = 250;
	playerDamage = 50;
}

//*************** UPDATE ***************
void Player::OnUpdate(Uint32 t, bool Dkey, bool Akey, bool Wkey, bool Skey, Map& map, std::vector<Enemy*> AllEnemies, CVector& mousePos, Portal& portal)
{
	//if dead -> return (do nothing)
	if (isPlayerDead) return;

	localMouse = &mousePos; 
	localPortal = &portal;
	localMap = &map;
	localTime = t;
	//getDeltaTime
	deltatime = (t - prevFrameTime) / 1000  ; //in sec
	prevFrameTime = t;
 
	

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
		playerModel.SetSpeed(0);
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
	float energyRegenPerSec = 10;
	float armorRegenPerSec = 5;
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
				enemy->EnemyGetDamage(playerDamage, onDamageVfx); // to redo vfx ......
			}
		}
	}

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
		}
	}

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
	lootList.Draw(g);
	dashEffect.Draw(g);
	onHitEffect.Draw(g);
	
}

double magnitude(const std::vector<double>& vec) {
	double sum_of_squares = 0;
	for (double component : vec) {
		sum_of_squares += component * component;
	}
	return sqrt(sum_of_squares);
}

//*************** PLAYER CONTROLER ***************
void Player::PlayerControl(bool Dkey, bool Akey, bool Wkey , bool Skey )
{

	CVector charVelocityV = CVector(playerModel.GetXVelocity(), playerModel.GetYVelocity(), playerModel.GetZVelocity());
	CVector mousePos = CVector(localMouse->GetX(), localMouse->GetY(), localMouse->GetZ());
	CVector displ = mousePos - playerModel.GetPositionV();
	
	CVector normalizedPlayerVelocityV = charVelocityV.Normalized();


	float PlayerMagnitude = charVelocityV.Magnitude();
	float DisplMagnitude = displ.Magnitude();


	float dotProduct = charVelocityV.Dot(displ);


	//float angel = dotProduct / (PlayerMagnitude * DisplMagnitude);
	
	cout << acos(dotProduct) * 180.0 / M_PI << endl;
	
	


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

	if ((playerModel.GetZ() > localMouse->GetZ() || playerModel.GetX() > localMouse->GetX()) && Wkey )
	{
		playerModel.SetSpeed(650);
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
		playerModel.SetSpeed(650);
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
			if (dashCoolDown < localTime && playerCurrentState != INDASH)
			{
				dashCoolDown = localTime + 3000;
				playerCurrentState = INDASH;
				playerModel.PlayAnimation("dash", 150, false);
			 
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
	for (int i = 0; i < 15; i++)
	{
		CModel* p = onDamageVfx.Clone();
		p->SetPositionV(playerModel.GetPositionV() + CVector(0, 100, 0));
		p->SetDirection(rand() % 360);
		p->SetSpeed(rand() % 200);
		p->SetColor(CColor::Red());
		p->Die(250);
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

void Player::addLoot(int enemyType, CVector enemyPos)
{
	float chance = rand() % 100;
	if (chance < 40) return;
	if(enemyType ==  0) lootList.push_back(lootItemOne.Clone());
	else if (enemyType == 1) lootList.push_back(lootItemTwo.Clone());
	else if (enemyType == 2) lootList.push_back(lootItemThree.Clone());
	lootList.back()->SetPositionV(enemyPos);
	lootList.back()->SetStatus(enemyType);
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
	//if (curentSkillSelected == RECALL) curentSkillSelected = DASH;
	//else if (curentSkillSelected == DASH) curentSkillSelected = RECALL;
}

void Player::playerOnWheelDown()
{
	//if (curentSkillSelected == RECALL) curentSkillSelected = DASH;
	//else if (curentSkillSelected == DASH) curentSkillSelected = RECALL;
}
