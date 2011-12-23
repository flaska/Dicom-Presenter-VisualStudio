#ifndef CAnimation_H_vcxdf
#define CAnimation_H_vcxdf

#include <glObjects/glImage.h>

class CAnimation
{
public:
	CAnimation(CGLImage &image);
	~CAnimation();
	
public:
	void Restart();
	void Start();
	void Stop();
	float GetMSInterval();
	void SetMSInterval(int msInterval);
	void SetActive(bool active);
	bool IsActive();
	void SetStartFrame(int startFrame);
	int GetStartFrame();
	int GetStopFrame();
	void SetOwnerImage(CGLImage *image);
	CGLImage &GetOwnerImage();
	void SetLoop(bool loop);
	void SetStopFrame(int stopFrame);
	/**
	* @brief Perform animation if appropriate
	* returns true if animation was performed
	*/
	bool Do(float ms);//perform next step in animation- ms how many sm it is from last do
	
private:
		float iFrameNr;
	int iLastTimerCounter;
	int iActualFrame;//in animation
	int iStartFrame, iStopFrame;
	float iMSInterval;
	int iLoop;
	bool iSuspended;
	bool iActive;
	CGLImage &iOwnerImage;
};
#endif
