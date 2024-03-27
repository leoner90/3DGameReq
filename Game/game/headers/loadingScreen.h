#pragma once
 
class TextConverter;

class LoadingScreen
{
public:
	LoadingScreen(float w, float h);
	std::string text[15];

	//main Functions
	void init();
	void Update(Uint32 t);
	void OnDraw(CGraphics* g);

	bool loadingCompleted, loadingStarted;

private:
	CSprite loadingBg;
	CSprite loadingIcon;
	CFont font;
	TextConverter* textConverter;

	float localH, localW;
	int randomTextSelectionNum;
	float loadingTimer;
};