#pragma once
 


class LoadingScreen
{
public:
	LoadingScreen(float w, float h);
	std::string text[15];

	void init();
	void Update(Uint32 t);
	void OnDraw(CGraphics* g);

	CSprite loadingBg;
	CSprite loadingIcon;

	float localH, localW;
	CFont font;
	bool loadingCompleted, loadingStarted;
	int randomTextSelectionNum;

	float loadingTimer;
private:

};

 