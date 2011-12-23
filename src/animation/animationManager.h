#ifndef CAnimationManager_H_vcxdf
#define CAnimationManager_H_vcxdf
#include <animation/animation.h>
#include <glObjects/glImage.h>
class CAnimationManager:public QObject
{
	Q_OBJECT
	private:
	CAnimationManager();
	
	
public:
	/**
	* @brief Set the FPS (frames per second) framerate, which would be
	* used when saving sequence (SaveSequence function)
	*/
	void SetSaveSequenceFPS(float fps);

	/**
	* @brief Set true to keep the "temporary" created bitmaps (as a source to the AVI creation process) on filesystem
	*/
	void SetSaveSequenceKeepFrames(bool keepFrames);
	/**
	* @brief Get the current FPS (frames per second) framerate, which would be
	* used when saving sequence (SaveSequence function)
	*/
	float GetSaveSequenceFPS();
	static CAnimationManager* InitInstance();
	static CAnimationManager* GetInstance();
	~CAnimationManager();
private:
	static CAnimationManager *instance;	
public:
	/**
	* @brief Restart all animations
	*/
	void RestartAll();

	/**
	* @brief Pause or unpause  the animations
	*/
	void PauseAll(bool value);

	/**
	* @breif Changes the global animation speed to the value*default speed 
	*/
	void ChangeGlobalSpeedFactor(double value);
	/**
	* @brief Save current workspace animation
	* The animations are all restarted and snapshots are saved at specified FPS as fileName001.bmp - fileName999.bmp files
	* until the longest animation ends, then the 
	
	*/
	void SaveSequence(QString& fileName);
	CAnimation *GetAnimation(CGLImage *image);
	void RemoveAnimation(CGLImage *image);
	public slots:
	void TimerTimeout();
	
private:
	bool iKeepTempFrames;
	float iSaveSequenceFPS;
	double iGlobalSpeedFactor;
	bool iSaveAnimation;//if animation is to be saved
	QTimer *iTimer;
	QMap<CGLImage*,CAnimation*> iAnimations;
};
#endif
