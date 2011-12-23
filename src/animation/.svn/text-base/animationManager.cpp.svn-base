
#include <animation/animationManager.h>
#include <glObjects/glWidget.h>
#include <glObjects/workspaces/workspaceManager.h>
#include <mainWindow.h>
#include <export/videoConverter.h>
#include <QtCore/QDir>

CAnimationManager* CAnimationManager::instance = NULL;
CAnimationManager::CAnimationManager(){
	iSaveSequenceFPS = 15.0f;
	iGlobalSpeedFactor = 1.0;
	iSaveAnimation = false;
	iTimer = new QTimer();
	iTimer->setInterval(40);//=25 FPS
	iTimer->start();
	QObject::connect(iTimer , SIGNAL(timeout ()), 
		this, SLOT(TimerTimeout( ))); 
}
void CAnimationManager::TimerTimeout()
{
	static int lastTimerCounter =0;
	static int timerCounter =0;
	float interval = iTimer->interval();
	timerCounter += iTimer->interval();
	if(timerCounter>1000)
	{
		timerCounter = 0;
	}
	QMapIterator<CGLImage*, CAnimation*> it(iAnimations);
	//	int maxTimeLength = 0;//the longest time of the animation in the current workspace
	bool animPerformed = false;
	while(it.hasNext())
	{
		CAnimation* anim = it.next().value();

		if(anim->IsActive())
		{
			if(iGlobalSpeedFactor>0)
				animPerformed |= anim->Do(interval*iGlobalSpeedFactor);
		}
	}
if(animPerformed)
{	
	if(CGLWidget::GetInstance())
	{
		CGLWidget::GetInstance()->updateGL();
	}
}
}
CAnimationManager::~CAnimationManager()
{
	instance = NULL;
	QMapIterator<CGLImage*, CAnimation*> it(iAnimations);
	while(it.hasNext())
	{
		CAnimation* anim = it.next().value();
		delete anim;
	}
	iAnimations.clear();
}

CAnimationManager* CAnimationManager::InitInstance()
{
	if(!instance)
	{
		instance  = new CAnimationManager ();
	}
	return instance;
}


CAnimationManager* CAnimationManager::GetInstance()
{
	return instance;
}
/**
* @brief Set true to keep the "temporary" created bitmaps (as a source to the AVI creation process) on filesystem
*/
void CAnimationManager::SetSaveSequenceKeepFrames(bool keepFrames)
{
	iKeepTempFrames = keepFrames;
}

void CAnimationManager::SetSaveSequenceFPS(float fps)
{
	iSaveSequenceFPS = fps;
}
float CAnimationManager::GetSaveSequenceFPS()
{
	return iSaveSequenceFPS;
}


void CAnimationManager::SaveSequence(QString& fileName)
{
	if(fileName.isEmpty())
		return;

	//stop automatic animation update
	iTimer->stop();
	RestartAll();
	//TODO
	//perform animation simulation quickly
	//iSaveAnimation = true;
	int maxTimeLength = 0;
	QMapIterator<CGLImage*, CAnimation*> it(iAnimations);
	//	int maxTimeLength = 0;//the longest time of the animation in the current workspace
	while(it.hasNext())
	{
		CAnimation* anim = it.next().value();
		if(anim->IsActive())
		{
			if(anim->GetOwnerImage().GetParentWorkspace())
			{
				if(anim->GetOwnerImage().GetParentWorkspace() ==CWorkspaceManager::GetInstance()->GetActiveWorkspace())
				{
					int timeLength = (anim->GetStopFrame()-anim->GetStartFrame()) * anim->GetMSInterval();
					if(timeLength>maxTimeLength)
					{
						maxTimeLength = timeLength;
					}
				}
			}
		}
	}
	float FPS = GetSaveSequenceFPS();
	int framesCount = (maxTimeLength*FPS / 1000);
	float interval = 1000.0f/FPS;
	float timerCounter = 0;
	QProgressDialog progress("Saving sequence...", "Abort Saving", 0, framesCount, MainWindow::iSelfS);
	progress.setWindowModality(Qt::WindowModal);
	//save the first frame
	it.toFront();
	while(it.hasNext())
	{
		CAnimation* anim = it.next().value();

		if(anim->IsActive())
		{
			if(anim->GetOwnerImage().GetParentWorkspace())
			{
				if(anim->GetOwnerImage().GetParentWorkspace() ==CWorkspaceManager::GetInstance()->GetActiveWorkspace())
				{
					/*timerCounter +=interval;*/
					anim->Do(0);
				}
			}

		}
	}
	for(int i=0;i<framesCount;i++)
	{

		progress.setValue(i);

		if (progress.wasCanceled())
			break;

		QString frameFileName;
		frameFileName.clear();

		frameFileName.append(fileName.left(fileName.length()-4));
		frameFileName.append(QString::number(i));
		frameFileName.append(".bmp");
		if(CWorkspaceManager::GetInstance())
		{
			CWorkspaceManager::GetInstance()->GetActiveWorkspace()->SaveSnapshot(frameFileName);
		}
		if(CGLWidget::GetInstance())
		{
			CGLWidget::GetInstance()->updateGL();
		}
		it.toFront();

		while(it.hasNext())
		{
			CAnimation* anim = it.next().value();

			if(anim->IsActive())
			{
				if(anim->GetOwnerImage().GetParentWorkspace())
				{
					if(anim->GetOwnerImage().GetParentWorkspace() ==CWorkspaceManager::GetInstance()->GetActiveWorkspace())
					{
						/*timerCounter +=interval;*/
						anim->Do(interval);
					}
				}

			}
		}
	}
	//convert bmp to avis
	//windows EasyBMPtoAVI converter
	QProgressDialog progressVideoConvert("Converting sequence to AVI...", "Abort Converting", 0, 0, MainWindow::iSelfS);
	progress.setWindowModality(Qt::WindowModal);
	VideoConverter::ConvertBitmapsToAvi(fileName.left(fileName.length()-4),0,framesCount-1,(int)FPS,fileName);
	if(!iKeepTempFrames)
	{
		//delete temporary frames
		QString strFilePath(fileName);
	int fileNamePosition = strFilePath.lastIndexOf ( '/' );
	QString strFilename = strFilePath.mid(fileNamePosition+1, fileName.length() - fileNamePosition - 4-1);
	QString strDirectoryPath ( strFilePath.left ( fileNamePosition+1 ) ); //current path
	
		QDir dir(strDirectoryPath);
		QStringList files = dir.entryList();
		for (int i=1;i<files.count() ;i++)
		{	
			if(files.at(i).contains(strFilename) && files.at(i).contains(QString("bmp")))
			{
				int g=1;
				dir.remove(files.at(i));
			}
			
		}
	}
	iTimer->start();
}

void CAnimationManager::PauseAll(bool value)
{
	if(value)
	{
		iTimer->stop();
	}
	else
	{
		iTimer->start();
	}
}
void CAnimationManager::ChangeGlobalSpeedFactor(double value)
{
	iGlobalSpeedFactor = value;
}
void CAnimationManager::RestartAll()
{
	QMapIterator<CGLImage*, CAnimation*> it(iAnimations);
	while(it.hasNext())
	{
		CAnimation* anim = it.next().value();
		anim->Restart();
	}
}
/**
* @brief Returns the animation assigned to the image
*/
CAnimation *CAnimationManager::GetAnimation(CGLImage *image)
{
	if(iAnimations.contains(image))
	{

	}
	else
	{
		CAnimation *newAnimation = new CAnimation(*image);
		iAnimations.insert(image,newAnimation);
	}
	return iAnimations[image];
}

/**
* @brief Remove the animation of the image from the animation manager
*/
void CAnimationManager::RemoveAnimation(CGLImage *image)
{
	if(iAnimations.contains(image))
	{
		iAnimations.remove(image);
	}

}
