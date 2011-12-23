#include "animation.h"
CAnimation::CAnimation(CGLImage &image):iOwnerImage(image)
{
	iFrameNr=0;
	iLastTimerCounter=0;
	iActualFrame=1;
	iSuspended = false;
	iStartFrame = 1;
	iStopFrame = image.GetActualTextureDepth();
	iMSInterval = 40.0f;
	iActive = false;
	iLoop = true;
}
CAnimation::~CAnimation()
{
}

void CAnimation::SetActive(bool active)
{
	iActive = active;
}
void CAnimation::Restart()
{
	iFrameNr = GetStartFrame();

}
void CAnimation::Start()
{
	iSuspended=false;
}
void CAnimation::Stop()
{
	iSuspended=true;
}
bool CAnimation::IsActive()
{
	return iActive;
}

int CAnimation::GetStopFrame()
{
	return iStopFrame;
}

void CAnimation::SetStopFrame(int stopFrame){
	if(stopFrame<1)
		stopFrame=1;

	if(stopFrame>(iOwnerImage.GetActualTextureDepth()))
	{
		stopFrame=iOwnerImage.GetActualTextureDepth();
	}

	iStopFrame = stopFrame;
}

void CAnimation::SetStartFrame(int startFrame){
	if(startFrame<1)
		startFrame = 1;
	iStartFrame = startFrame;
}
int CAnimation::GetStartFrame(){
	return iStartFrame;}


CGLImage &CAnimation::GetOwnerImage(){
	return iOwnerImage;}
void CAnimation::SetLoop(bool loop){
	iLoop = loop;}

float CAnimation::GetMSInterval()
{
	return iMSInterval;
}
void CAnimation::SetMSInterval(int msInterval)
{
	if(msInterval <0)
	{
		msInterval = 1;
	}
	iMSInterval = msInterval;
}

bool CAnimation::Do(float timeElapsed){

	if(iFrameNr<GetStartFrame())
		iFrameNr=GetStartFrame();
	if(iActive && !iSuspended)
	{


		float fps=	GetMSInterval();
		float msInterval = 1000.f/fps;
		msInterval = GetMSInterval();
		//	if((timerCounter-lastTimerCounter)>=msInterval)
		{//update only if FPS reached

			iFrameNr += (timeElapsed)/msInterval;
			if(iFrameNr>GetStopFrame())
			{	
				if(iLoop)
				{
					iFrameNr = GetStartFrame();
				}
				else
				{
					iActive = false;
					iFrameNr = GetStopFrame();
				}
			}
			int depth = iOwnerImage.GetActualTextureDepth()-1;
			float move;
			if(depth==0.0f)
			{
				move = 0;
			}
			else
			{
				move=	(iFrameNr-1) / ((float)depth);
			}

			iOwnerImage.MoveToDepth(move);
		}
return true; //animation was performed
	}
	return false; // animation was not performed
}
