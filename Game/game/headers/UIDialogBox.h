#pragma once
#include <iostream>
 
class UIDialogBox
{
public:
	//Set current Speacker
	enum currentSpeaker {  BLINKY, MYNE};
	int speaker;
	bool isBoxShowen;
	bool autoHideTimer;

	void init(float w, float h);
	void OnUpdate(long t);
	void OnDraw(CGraphics* g);
	void showBox(int speakerId, int textId, int priority, float autohideBoxin = -1 );
	void hideBox();
	float dialogBoxHideSpeed, dialogBoxShowSpeed;
	CSprite dialogBoxBg;
	int currentPriority;
private:
	float localW, localH;

	int dialogNumber;
	std::string text[5], name[2];
	
	CFont font;
	//all Speakers logo
	CSprite speakerImg, MyneImg;
	

	float localTime;
	bool textShow;
	
	float hideInSec;
	bool onTop;
	 
};

 