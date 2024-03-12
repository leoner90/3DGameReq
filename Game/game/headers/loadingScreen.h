#pragma once
 


class LoadingScreen
{
public:
	LoadingScreen();
	std::string text[15];

	void init(float w, float h);
	void Update(float t);
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

 