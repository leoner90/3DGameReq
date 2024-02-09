#pragma once


class PlayerInterface
{
public:
	//Main Functions
	void init(int gameWidth, int gameHeigth);
	void OnUpdate();
	void OnDraw(CGraphics* g);
	CHealthBar hbar;

private:
	
};

 