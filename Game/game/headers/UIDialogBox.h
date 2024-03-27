#pragma once
class TextConverter;

class UIDialogBox
{
public:
	UIDialogBox(float w, float h);

	//Main Functions
	void init();
	void OnUpdate(long t, bool fullWidth);
	void OnDraw(CGraphics* g);

	//Show Hide Dialog Box
	void showBox(int speakerId, int textIdStart, int textIdEnd, int priority, float autohideBoxin = -1 );
	void hideBox();

	bool isBoxShowen;
	int currentPriority;

private:
	//settings
	bool autoHideTimer;
	int speaker;
	float dialogBoxHideSpeed, dialogBoxShowSpeed;
	float speakerImgMarginTop, speakerImgMarginLeft, textMarginLeft;
	float hideInSec;
	CSprite dialogBoxBg;

	//Set current Speacker
	enum currentSpeaker { MYNE, BLINKY };

	float localW, localH;

	int dialogNumber, dialogEndNumber;
	std::string text[27], name[2];
	
	CFont font;
	//all Speakers logo
	CSprite speakerImg, MyneImg;
	
	vector2f initPosDialogBox, initPosSpeakerImages;

	float localTime;
	bool textShow;


	float localautohideTime;
	TextConverter* textConverter;	 
};