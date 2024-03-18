#pragma once
class Enemy;
class Player;

class Radar
{
public:
	Radar(float w, float h);
	void init();
	void OnUpdate(Uint32 t, std::vector<Enemy*>& AllEnemies, Player& player);
	void OnDraw(CGraphics* g);

private:
	CSprite radarBg;
	CSprite dot;
	CSpriteList dotList;

	std::vector<Enemy*> localAllEnemies;
	Player* localPlayer;

};

 