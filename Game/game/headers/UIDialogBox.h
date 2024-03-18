#pragma once
class TextConverter;

class UIDialogBox
{
public:
	UIDialogBox(float w, float h);

	//Set current Speacker
	enum currentSpeaker {MYNE, BLINKY};
	int speaker;
	bool isBoxShowen;
	bool autoHideTimer;

	void init();
	void OnUpdate(long t, bool fullWidth);
	void OnDraw(CGraphics* g);
	void showBox(int speakerId, int textIdStart, int textIdEnd, int priority, float autohideBoxin = -1 );
	void hideBox();
	float dialogBoxHideSpeed, dialogBoxShowSpeed;
	float speakerImgMarginTop, speakerImgMarginLeft;
	CSprite dialogBoxBg;
	int currentPriority;
private:
	float localW, localH;

	int dialogNumber, dialogEndNumber;
	std::string text[27], name[2];
	
	CFont font;
	//all Speakers logo
	CSprite speakerImg, MyneImg;
	
	vector2f initPosDialogBox, initPosSpeakerImages;

	float localTime;
	bool textShow;
	
	float hideInSec;


	float localautohideTime;
	TextConverter* textConverter;

	 
};

 