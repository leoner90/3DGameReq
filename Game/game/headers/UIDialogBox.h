#pragma once

class String;
class UIDialogBox
{
public:
	void init(float w, float h);
	void OnUpdate(long t);
	void OnDraw(CGraphics* g);
	void showBox();
	void hideBox();
private:
	float localW, localH;
	char* text, name;
	CSprite speakerImg;
	CSprite dialogBoxBg;
	bool isBoxShowen;
	CFont font;
	float autoHideTimer;
};

 