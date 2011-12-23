#include <math.h>
#include <GL/glew.h>
#include <QtOpenGL/QtOpenGL>
#include <plib/fnt.h>

#include <mainWindow.h>
#include <glObjects/glImage.h>
#include <glObjects/glWidget.h>
#include <3dTexture/dicom3DTextureManager.h>
#include <animation/animation.h>
#include <infoPanel.h>
#include <settings.h>
#include <glObjects/workspaces/workspaceManager.h>

#include "dpDebug/dpDebug.h"
debug::Log ImLog;

CGLImage::~CGLImage()
{
	CloseMe();
	C3DTextureManager::GetInstance()->ReleaseTexture(iTexture);
}

void CGLImage::Init(CGLObject *parentWindow, QPointF& position, QPointF &size )
{
	ImLog.write("Begin: CGLImage::Init");

	iLastAxialPosition = 0;
	iLastSagittalPosition = 0;
	iLastCoronalPosition = 0;

	iParentWorkspace = NULL;
	iAnimation = NULL;
	iName = new QString();
	SetOrientation(EImageOrientationAxial);
	QString name;
	name.clear();
	name.append("image");
	//name.append(QString::number(CWorkspaceManager::GetInstance()->GetWorkspaces().count()));
	SetName(name);
	iManipulated = EManipNone;
	// create the framebuffer object - make sure to have a current
	// context before creating it
	CGLWidget::GetInstance()->makeCurrent();
	iOwner = NULL;
	//timerId = startTimer(20);
	//setWindowTitle(tr("OpenGL framebuffer objects 2"));
	iParentWorkspace=NULL;
	iScale = 1.0;
	iBias = 0.3;
	iParentWindow = parentWindow;
	iPosition=position;
	iSize=size;
	//slider
	iFrameSlider.position.setX( 10);
	iFrameSlider.size.setY(15);
	iFrameSlider.size.setX(iSize.x() - iFrameSlider.position.x() - 25); //iSize.x() - size.x()
	iFrameSlider.position.setY(iSize.y() - iFrameSlider.size.y()-2);
	iFrameSlider.data=0;
	//	iFrames = NULL;
	iZoomFactor=1.0;
	iImageCenter = QPointF(0.5,0.5);
	iImageWindow.center = 1<<11;
	iImageWindow.width = 1<<11;
	iActualTextureCoords.bottomLeft = CPoint3Df(0,0,0);
	iActualTextureCoords.bottomRight = CPoint3Df(1,0,0);
	iActualTextureCoords.topRight = CPoint3Df(1,1,0);
	iActualTextureCoords.topLeft = CPoint3Df(0,1,0);

	SetMoveability(true);
	SetResizeability(true);
	SetClosability(true);
	SetOpenGLInterpolation(true);
	SetBorders(Settings::GetBordersConstant(EImageBorders));
	SetBorderColor(Settings::GetColorConstant(EImageBorderColor));
	SetInnerColor(Settings::GetColorConstant(EImageInnerColor));
	iDisplayTextSettings = 0;
	SetTextDisplay(EDisplayTextOrientation, false);
	SetTextDisplay(EDisplayTextZoom, true);
	SetTextDisplay(EDisplayTextWindow, false);
	SetTextDisplay(EDisplayTextOrientation, true);
	SetTextDisplay(EDisplayTextWindow, true);
	SetTextDisplay(EDisplayTextZoom, false);
	ImLog.write("End: CGLImage::Init");

}
bool CGLImage::InitSliceTexture()
{
	////// slices x,y,z

	glGenTextures(1, &iSliceZ);
	glBindTexture(GL_TEXTURE_2D, iSliceZ);
	//Set to GL_LINEAR to have smooth texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB16  , 
		iTexture->GetWidth(), iTexture->GetHeight(),
		0, GL_RGB , GL_UNSIGNED_SHORT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint rboIdZ;
	glGenRenderbuffersEXT(1, &rboIdZ);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboIdZ);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
		iTexture->GetWidth(), iTexture->GetHeight());
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	// create a framebuffer objecth

	glGenFramebuffersEXT(1, &iFBOZ);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFBOZ);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_2D, iSliceZ , 0);

	// attach the renderbuffer to depth attachment point
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
		GL_RENDERBUFFER_EXT, rboIdZ);


		glGenTextures(1, &iSliceX);
	glBindTexture(GL_TEXTURE_2D, iSliceX);
	//Set to GL_LINEAR to have smooth texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB16  , 
		iTexture->GetWidth(), iTexture->GetHeight(),
		0, GL_RGB , GL_UNSIGNED_SHORT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint rboIdX;
	glGenRenderbuffersEXT(1, &rboIdX);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboIdX);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
		iTexture->GetWidth(), iTexture->GetHeight());
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	// create a framebuffer object

	glGenFramebuffersEXT(1, &iFBOX);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFBOX);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_2D, iSliceX , 0);

	// attach the renderbuffer to depth attachment point
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
		GL_RENDERBUFFER_EXT, rboIdX);



	glGenTextures(1, &iSliceY);
	glBindTexture(GL_TEXTURE_2D, iSliceY);
	//Set to GL_LINEAR to have smooth texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB16  , 
		iTexture->GetWidth(), iTexture->GetHeight(),
		0, GL_RGB , GL_UNSIGNED_SHORT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint rboIdY;
	glGenRenderbuffersEXT(1, &rboIdY);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboIdY);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
		iTexture->GetWidth(), iTexture->GetHeight());
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	// create a framebuffer object

	glGenFramebuffersEXT(1, &iFBOY);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFBOY);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_2D, iSliceY , 0);

	// attach the renderbuffer to depth attachment point
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
		GL_RENDERBUFFER_EXT, rboIdZ);

	//////





	ImLog.write("Begin: CGLImage::InitSliceTexture");
	iActualSliceTextureInfo.width = iTexture->GetWidth(); //must be 2^n
	iActualSliceTextureInfo.height = iTexture->GetHeight();//must be 2^n

	glGenTextures(1, &iActualSliceTexture);
	glBindTexture(GL_TEXTURE_2D, iActualSliceTexture);
	//Set to GL_LINEAR to have smooth texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB16  , 
		iActualSliceTextureInfo.width, iActualSliceTextureInfo.height,
		0, GL_RGB , GL_UNSIGNED_SHORT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint rboId;
	glGenRenderbuffersEXT(1, &rboId);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboId);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
		iActualSliceTextureInfo.width, iActualSliceTextureInfo.height);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	// create a framebuffer object

	glGenFramebuffersEXT(1, &iFBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFBO);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_2D, iActualSliceTexture , 0);

	// attach the renderbuffer to depth attachment point
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
		GL_RENDERBUFFER_EXT, rboId);

	// check FBO status
	bool fboUsed=true;
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT){
		ImLog.write("End: CGLImage::InitSliceTexture");
		return false;
	}

	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	ImLog.write("End: CGLImage::InitSliceTexture");
	return true;
}

/**
Get image from ImageExplorer
*/
CGLImage *CGLImage::GetOwner(){
	return iOwner;
}

void CGLImage::SetOwner(CGLImage *owner)
{
	iOwner=owner;
}

CDicom3DTexture *CGLImage::GetTexture()
{
	return iTexture;
}
CGLImage::CGLImage(CGLObject *parentWindow,QString &file, QPointF& position, QPointF &size ):CGLObject(parentWindow,position,size)
{
	if(!C3DTextureManager::GetInstance())
	{
		throw TextureNotCreatedException(); //flaska  - u pana tintery to hofi tuto vyjymku a program nefunguje
	}
	//TODO solve if series is alredy loaded in 3d texture
	//TODO solve errors
	iTexture = NULL;
	iTexture= C3DTextureManager::GetInstance()->LoadTexture(file);

	if(!iTexture)
	{
		throw TextureNotCreatedException(); //flaska  - u pana tintery to hofi tuto vyjymku a program nefunguje
	}


	Init(parentWindow, position, size);
	iImageWindow = iTexture->GetWindowDefaults();
	//initialize texture with actual slice
	GLenum err = glGetErr();

	if(!InitSliceTexture())
	{
		int err=1;
	}
	err = glGetErr();

	iImageWindow.center=int((iImageWindow.center )/CDicomFrames::iWindowMultiplyFactor);
	//width
	iImageWindow.width=int((iImageWindow.width )/CDicomFrames::iWindowMultiplyFactor);
	//iImageWindow.width=((iFrames->GetMaxColorValue()-iFrames->GetMinColorValue())/2);


	PrepareActualTexture();

	err = glGetErr();
	ImLog.write("End: CGLImage::CGLImage1");
}


CGLImage::CGLImage(CGLObject *parentWindow,CDicom3DTexture *texture, QPointF& position, QPointF &size ):CGLObject(parentWindow,position,size)
{	
	ImLog.write("Begin: CGLImage::CGLImage2");
	if(!texture)
	{
		throw DicomFramesException();
	}
	//TODO solve if series is alredy loaded in 3d texture
	//TODO solve errors
	iTexture = texture;
	//iTexture= C3DTextureManager::GetInstance()->LoadTexture(file);
	Init(parentWindow, position, size);
	iImageWindow = iTexture->GetWindowDefaults();
	//initialize texture with actual slice
	if(!InitSliceTexture())
	{
		int err=1;
	}
	iImageWindow.center=int((iImageWindow.center )/CDicomFrames::iWindowMultiplyFactor);
	//width
	iImageWindow.width=int((iImageWindow.width )/CDicomFrames::iWindowMultiplyFactor);
	//iImageWindow.width=((iFrames->GetMaxColorValue()-iFrames->GetMinColorValue())/2);

	PrepareActualTexture();
	ImLog.write("End: CGLImage::CGLImage2");
}

CGLWorkspace * CGLImage::GetParentWorkspace()
{
	return iParentWorkspace;
}

void CGLImage::SetParentWorkspace(CGLWorkspace *workspace)
{
	iParentWorkspace=workspace;
}

CGLImage::CGLImage(CGLObject *parentWindow, QPointF& position, QPointF &size ):CGLObject(parentWindow,position,size)
{	
	Init(parentWindow, position,size);

	//iFrames= new CDicomFrames(file->toAscii().data());
	//texture= LoadTextureFromDicom (*loader);
}



void CGLImage::DrawImage(){
	ImLog.write("Begin: CGLImage::DrawImage");
	glColor4f (1.0,1.0,1.0,1.0);
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, iActualSliceTexture);
	if(iOpenGLInterpol)
	{

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	float dpt =0.0;
	//compute source texture coords
	int width=iSize.x()-GetBorders().right-GetBorders().left;
	int height=iSize.y()-GetBorders().bottom-GetBorders().top;
	int xcenter = iImageCenter.x()*width;
	int ycenter = iImageCenter.y()*height;
	float xgapbefore = (xcenter - (((float)iTexture->GetWidth()/2)*iZoomFactor));
	float ygapbefore = (ycenter - (((float)iTexture->GetHeight()/2)*iZoomFactor));

	float xgapafter = (((float)iTexture->GetWidth()/2)*iZoomFactor)-(width-xcenter);
	float ygapafter = (((float)iTexture->GetHeight()/2)*iZoomFactor)-(height-ycenter);

	QPointF leftTopTex (
		-xgapbefore/(iTexture->GetWidth() * iZoomFactor),
		-ygapbefore/(iTexture->GetHeight() * iZoomFactor)
		);
	QPointF rightBottomTex (
		1.- (xgapafter/(iTexture->GetWidth()*iZoomFactor)),
		1.- (ygapafter/(iTexture->GetHeight()*iZoomFactor))
		);

	QPointF leftTop (GetBorders().left,iSize.y()-GetBorders().top);

	QPointF rightBottom (iSize.x()-GetBorders().right,GetBorders().bottom);
	//	QPointF leftTop (0,iSize.y());
	//	QPointF rightBottom (iSize.x(),0);


	glBegin( GL_QUADS );
	glTexCoord2d(leftTopTex.x(),rightBottomTex.y()); 
	glVertex2d(leftTop.x(),rightBottom.y());
	glTexCoord2d(rightBottomTex.x(),rightBottomTex.y());
	glVertex2d(rightBottom.x(),rightBottom.y());
	glTexCoord2d(rightBottomTex.x(),leftTopTex.y());
	glVertex2d(rightBottom.x(),leftTop.y());
	glTexCoord2d(leftTopTex.x(),leftTopTex.y());
	glVertex2d(leftTop.x(),leftTop.y());
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColor3f (0.0,0.0,0.0);
	//clip image
	if(xgapafter<0)
	{
		if(iSize.x()+xgapafter>0)
		{
			glRectf(GetBorders().left+width+xgapafter ,
				GetBorders().bottom,
				GetBorders().left+width,
				GetBorders().bottom+height);
		}
		else
		{
			glRectf(GetBorders().left ,GetBorders().bottom,width+GetBorders().left,
				GetBorders().bottom+height);
		}
	}
	if(ygapafter<0)
	{
		if(-ygapafter < height)
		{
			glRectf(GetBorders().left ,GetBorders().bottom,
				GetBorders().left+width,GetBorders().bottom-ygapafter);
		}
		else
		{
			glRectf(GetBorders().left ,GetBorders().bottom,width,height);
		}
	}
	if(ygapbefore >0 && height>ygapbefore)
	{

		glRectf(GetBorders().left ,
			GetBorders().bottom+height-ygapbefore,
			GetBorders().left+width,
			GetBorders().bottom+height);

	}
	if(xgapbefore >0 && width>xgapbefore)
	{

		glRectf(GetBorders().left
			,
			GetBorders().bottom,
			GetBorders().left+xgapbefore,
			GetBorders().bottom+height);;

	}
	ImLog.write("End: CGLImage::DrawImage");
}

void CGLImage::DrawMultiplanarSlice(){
	glColor4f (1.0,1.0,1.0,1.0);
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, iActualSliceTexture);
	glBegin( GL_QUADS );
	glTexCoord2d(0.5,0.5); 
	glVertex2d(0.5,0.5);
	glTexCoord2d(1,0.5);
	glVertex2d(1,0.5);
	glTexCoord2d(1,1);
	glVertex2d(1,1);
	glTexCoord2d(0.5,1);
	glVertex2d(0.5,1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void CGLImage::DrawToTextureSliceZ(TPlanarCrossPosition crossposition){
	float heightCorrection =( 1.0f - 1.0f/((float)(GetActualSliceWidth()) / (float)(GetActualSliceHeight())))/2.0f;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, iSliceZ);
	glBegin(GL_QUADS);
	glColor4f( 1., 0., 0., 1.);
    glTexCoord3d(0,1,crossposition.z); 
	glVertex2d(0,0);
	glTexCoord3d(1,	1,crossposition.z);
	glVertex2d(1,0);
	glTexCoord3d(1,0,crossposition.z);
	glVertex2d(1,1);
	glTexCoord3d(0,0,crossposition.z);
	glVertex2d(0,1);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void CGLImage::DrawManipulationSliceZ(TPlanarCrossPosition crossposition){
	glColor4f( 1., 0., 0., 1.); 
    glLineWidth(1); 
	for (int i=1;i<=2;i++){
		glBegin(GL_LINES); 
		glVertex2f( crossposition.x, 0.); 
		glVertex2f( crossposition.x, 1.); 
		glEnd();

		glColor4f( 1., 0., 0., 1.); 
		glLineWidth(1); 
		glBegin(GL_LINES); 
		glVertex2f( 0., crossposition.y ); 
		glVertex2f( 1., crossposition.y ); 
		glEnd();
	}
}

void CGLImage::DrawSliceZ(){
	glColor4f (1.0,1.0,1.0,1.0);
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, iSliceZ);
	glBegin( GL_QUADS );
	glTexCoord2d(0,0); 
	glVertex2d(0,0.5);
	glTexCoord2d(1,0);
	glVertex2d(0.5,0.5);
	glTexCoord2d(1,1);
	glVertex2d(0.5,1);
	glTexCoord2d(0,1);
	glVertex2d(0,1);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void CGLImage::DrawToTextureSliceX(TPlanarCrossPosition crossposition){
	float heightCorrection =( 1.0f - 1.0f/((float)(GetActualSliceWidth()) / (float)(GetActualSliceHeight())))/2.0f;
	glBindTexture(GL_TEXTURE_2D, iSliceZ);
	glBegin(GL_QUADS);

	glTexCoord3d(crossposition.x,1,0);
	glVertex2d(0,0);

	glTexCoord3d(crossposition.x,1,1);
	glVertex2d(1 ,0);

	glTexCoord3d(crossposition.x,0,1);
	glVertex2d(1,1);

	glTexCoord3d(crossposition.x,0,0);
	glVertex2d(0,1);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLImage::DrawManipulationSliceX(TPlanarCrossPosition crossposition){
	glColor4f( 1., 0., 0., 1.); 
    glLineWidth(1); 
	for (int i=1;i<=2;i++){
		glBegin(GL_LINES); 
		glVertex2f( crossposition.z, 0.); 
		glVertex2f( crossposition.z, 1.); 
		glEnd();

		glColor4f( 1., 0., 0., 1.); 
		glLineWidth(1); 
		glBegin(GL_LINES); 
		glVertex2f( 0., crossposition.y ); 
		glVertex2f( 1., crossposition.y ); 
		glEnd();
	}
}

void CGLImage::DrawSliceX(){
	glColor4f (1.0,1.0,1.0,1.0);
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, iSliceX);
	glBegin( GL_QUADS );
	glTexCoord2d(0,0); 
	//glVertex2d(0.5,0.5);
	glVertex2d(1.,0.5);
	glTexCoord2d(1,0);
	//glVertex2d(1,0.5);
	glVertex2d(1.,1.);
	glTexCoord2d(1,1);
	//glVertex2d(1,1);
	glVertex2d(0.5,1.);
	glTexCoord2d(0,1);
	//glVertex2d(0.5,1);
	glVertex2d(0.5,0.5);
	glEnd();
	glDisable(GL_TEXTURE_2D);	
}

void CGLImage::DrawToTextureSliceY(TPlanarCrossPosition crossposition){
	float heightCorrection =( 1.0f - 1.0f/((float)(GetActualSliceWidth()) / (float)(GetActualSliceHeight())))/2.0f;
	glBindTexture(GL_TEXTURE_2D, iSliceY);
	glBegin(GL_QUADS);

    glTexCoord3d(0,SwitchY(crossposition.y),0); 
	glVertex2d(0,0);

	glTexCoord3d(1,SwitchY(crossposition.y),0);
	glVertex2d(1 ,0);

	glTexCoord3d(1,SwitchY(crossposition.y),1);
	glVertex2d(1,1);

	glTexCoord3d(0,SwitchY(crossposition.y),1);
	glVertex2d(0,1);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLImage::DrawManipulationSliceY(TPlanarCrossPosition crossposition){
	glColor4f( 1., 0., 0., 1.); 
    glLineWidth(1); 
	for (int i=1;i<=2;i++){
		glBegin(GL_LINES); 
		glVertex2f( crossposition.x, 0.); 
		glVertex2f( crossposition.x, 1.); 
		glEnd();

		glColor4f( 1., 0., 0., 1.); 
		glLineWidth(1); 
		glBegin(GL_LINES); 
		glVertex2f( 0., crossposition.z ); 
		glVertex2f( 1., crossposition.z ); 
		glEnd();
	}
}

void CGLImage::DrawSliceY(){
	glColor4f (1.0,1.0,1.0,1.0);
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, iSliceY);
	glBegin( GL_QUADS );
	glTexCoord2d(0,0); 
	glVertex2d(0,0);
	glTexCoord2d(1,0);
	glVertex2d(0.5,0);
	glTexCoord2d(1,1);
	glVertex2d(0.5,0.5);
	glTexCoord2d(0,1);
	glVertex2d(0,0.5);
	glEnd();
	glDisable(GL_TEXTURE_2D);	
}

float CGLImage::ToOpenGLCoords(float y)
{
	return iSize.y()-y;
}

void CGLImage::DrawSlider(){
	ImLog.write("Begin: CGLImage::DrawSlider");
	if(!GetTextDisplay(EDisplayTextFrameData) ){
		ImLog.write("End: CGLImage::DrawSlider");
		return;
	}
	glEnable(GL_BLEND);
	float opaq = 0.5;
	glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
	// nahrat jednotkovou matici
	glDisable( GL_TEXTURE_2D );
	// glPopMatrix();

	//slider - framecount
	glColor4f(0.8, 0.0, 1.0,opaq); 
	glRectf(iFrameSlider.position.x(),iSize.y()-iFrameSlider.position.y()-iFrameSlider.size.y(),iFrameSlider.position.x()+iFrameSlider.size.x(),iSize.y()-iFrameSlider.position.y());
	//draw slider
	glColor4f(0.8, 0.8, 1.0,0.8); 
	float sliderlength = iFrameSlider.size.x();
	float sliderposition = sliderlength * iFrameSlider.data / (GetActualTextureDepth());
	float sliderhandlelenth = (float)sliderlength  / (GetActualTextureDepth());
	glRectf(iFrameSlider.position.x() + sliderposition,iSize.y()-iFrameSlider.position.y()-iFrameSlider.size.y(),iFrameSlider.position.x() + sliderposition+sliderhandlelenth,iSize.y()-iFrameSlider.position.y());
	glDisable (GL_BLEND);
	ImLog.write("End: CGLImage::DrawSlider");
}

QString& CGLImage::GetName()
{
	return *iName;
}
void CGLImage::SetName(const QString& name)

{
	iName->clear();
	iName->append(name);
}
TImageAxisOrientation CGLImage::GetOrientation()
{
	return iImageAxisOrientation;
}
/**
* @brief Sets the new image orientation (3D view)
* Currently the only main three views (axial, coronal, sagittal) are supported
* 
*/
void CGLImage::SetOrientation(TImageAxisOrientation orientation){
	ImLog.write("Begin: CGLImage::SetOrientation");
	if(iImageAxisOrientation == orientation){
		ImLog.write("End: CGLImage::SetOrientation");
		return;
	}
	int frameSliderData;
	switch(orientation)
	{
	case EImageOrientationAxial:
		{
			frameSliderData = iLastAxialPosition;
			iActualTextureCoords.bottomLeft = CPoint3Df(0,0,0);
			iActualTextureCoords.bottomRight = CPoint3Df(1,0,0);
			iActualTextureCoords.topRight = CPoint3Df(1,1,0);
			iActualTextureCoords.topLeft = CPoint3Df(0,1,0);
		}
		break;
	case EImageOrientationSagittal: //zleva doprava
		{
			frameSliderData = iLastSagittalPosition;
			iActualTextureCoords.bottomLeft = CPoint3Df(0,0,0);
			iActualTextureCoords.bottomRight = CPoint3Df(0,1,0);
			iActualTextureCoords.topRight = CPoint3Df(0,1,1);
			iActualTextureCoords.topLeft = CPoint3Df(0,0,1);
		}
		break;
	case EImageOrientationCoronal: //zezadu dopredu
		{
			frameSliderData = iLastCoronalPosition;
			iActualTextureCoords.bottomLeft = CPoint3Df(0,0,0);
			iActualTextureCoords.bottomRight = CPoint3Df(1,0,0);
			iActualTextureCoords.topRight = CPoint3Df(1,0,1);
			iActualTextureCoords.topLeft = CPoint3Df(0,0,1);
		}
		break;
	default:
		break;
	}
	TImageAxisOrientation lastImageOrientation = iImageAxisOrientation;
	switch(lastImageOrientation)
	{
	case EImageOrientationAxial:
		{
			iLastAxialPosition = iFrameSlider.data;
		}
		break;
	case EImageOrientationSagittal: //zleva doprava
		{
			iLastSagittalPosition = iFrameSlider.data;
		}
		break;
	case EImageOrientationCoronal: //zezadu dopredu
		{
			iLastCoronalPosition = iFrameSlider.data;
		}
		break;
	default:
		break;
	}
	iImageAxisOrientation = orientation;
	//iFrameSlider.data=(float)(inDepthPosition*(GetActualTextureDepth()));

	float depth = (float)frameSliderData/(float)GetActualTextureDepth();
	MoveToDepth(depth);
	ImLog.write("End: CGLImage::SetOrientation");
	return;
}
bool CGLImage::GetTextDisplay(TDiplayText displayText)
{
	return ((iDisplayTextSettings & displayText) == displayText);
}
void CGLImage::SetTextDisplay(TDiplayText displayText, bool on)
{
	if(on)
	{
		iDisplayTextSettings |= displayText;
	}
	else
	{
		iDisplayTextSettings &= ~displayText;
	}
	if(iParentWorkspace)
	{
		iParentWorkspace->UpdateTexture();
	}
	CGLWidget::GetInstance()->updateGL();
}
void CGLImage::DrawTexts(){
	ImLog.write("Begin: CGLImage::DrawTexts");
	glEnable(GL_BLEND);
	//TODO improve speed - do not use QGLWidget::renderText but some external library
	/*
	renderText uses bitmaps so you can't rotate it. renderText sucks for 
	everything but the simplest text needs. Plus it's really slow.

	If you want better support, roll your own or try something like FTGL...

	http://homepages.paradise.net.nz/henryj/code/index.html#FTGL
	*/
	//FreeType library
	//http://nehe.ceske-hry.cz/tut_43.php
	glColor3f(0.5f, 1.0f, 0.5f);

	/*iParentWindow->renderText  (iPosition.x()+30,iPosition.y()+35,
	QString("scale: ").append( QString::number(iScale,103,6)),font);
	iParentWindow->renderText  (iPosition.x()+30,iPosition.y()+55,
	QString("bias: ").append( QString::number(iBias,103,6)),font);*/
	if((iDisplayTextSettings & EDisplayTextZoom) == EDisplayTextZoom)
	{
		CGLWidget::GetInstance()->renderText  (5,ToOpenGLCoords(iSize.y() - 35),
			QString("zoom: ").append( QString::number(iZoomFactor,103,4)),15);
	}
	if((iDisplayTextSettings & EDisplayTextWindow) == EDisplayTextWindow)
	{
		CGLWidget::GetInstance()->renderText  (5,ToOpenGLCoords(+iSize.y() - 22),
			QString("c: ").append( QString::number(iImageWindow.center ,103,6)),15);
		CGLWidget::GetInstance()->renderText  (+100,ToOpenGLCoords(+iSize.y() - 22),
			QString("w: ").append( QString::number(iImageWindow.width ,103,6)),15);
	}

	glColor3f(1.0f, 0.5f, 0.5f);

	/*CGLWidget::GetInstance()->renderText  (+30,ToOpenGLCoords(+20),

	QString("").append(iTexture->GetDicomHeader().GetPatientInfo().GetName()),15);

	CGLWidget::GetInstance()->renderText  (+30,ToOpenGLCoords(+40),
	QString("").append(iTexture->GetDicomHeader().GetEquipmentInfo().GetInstitutionName()),15);

	CGLWidget::GetInstance()->renderText  (+30,ToOpenGLCoords(+60),
	QString("").append(QString::number(iTexture->GetDicomHeader().GetImageInfo().GetImageOrientation().row.x)),15);
	*/
	//Draw image orientation info
	//see http://www.dclunie.com/medical-image-faq/html/part2.html

	//should depend on HFS and so on
	if((iDisplayTextSettings & EDisplayTextOrientation) == EDisplayTextOrientation)
	{
		char rightOrientation, leftOrientation;
		try
		{
			int frameNr= GetActualFrameNr();
			if(GetOrientation() != EImageOrientationAxial)
			{
				frameNr = 0;
			}
			TImageOrientation imageOrientation  = iTexture->GetDicomHeader(frameNr).GetImageInfo().GetImageOrientation();

			double absX = fabs(imageOrientation.row.x);
			double absY = fabs(imageOrientation.row.y);
			double absZ = fabs(imageOrientation.row.z);
			if (absX>.0001 && absX>absY && absX>absZ) {
				rightOrientation = imageOrientation.row.x < 0 ? 'R' : 'L';
				leftOrientation = imageOrientation.row.x > 0 ? 'R' : 'L';
			}
			else if (absY>.0001 && absY>absX && absY>absZ) {
				rightOrientation = imageOrientation.row.y < 0 ? 'A' : 'P';
				leftOrientation = imageOrientation.row.y > 0 ? 'A' : 'P';
			}
			else if (absZ>.0001 && absZ>absX && absZ>absY) {
				rightOrientation = imageOrientation.row.z < 0 ? 'F' : 'H';
				leftOrientation = imageOrientation.row.z > 0 ? 'F' : 'H';
			}
			//should depend on HFS and so on
			char bottomOrientation, topOrientation;

			absX = fabs(imageOrientation.column.x);
			absY = fabs(imageOrientation.column.y);
			absZ = fabs(imageOrientation.column.z);
			if (absX>.0001 && absX>absY && absX>absZ) {
				bottomOrientation = imageOrientation.column.x < 0 ? 'R' : 'L';
				topOrientation = imageOrientation.column.x > 0 ? 'R' : 'L';
			}
			else if (absY>.0001 && absY>absX && absY>absZ) {
				bottomOrientation = imageOrientation.column.y < 0 ? 'A' : 'P';
				topOrientation = imageOrientation.column.y > 0 ? 'A' : 'P';
			}
			else if (absZ>.0001 && absZ>absX && absZ>absY) 
			{
				bottomOrientation = imageOrientation.column.z < 0 ? 'F' : 'H';
				topOrientation = imageOrientation.column.z > 0 ? 'F' : 'H';
			}

			glColor3f(1.0f, 1.0f, 0.1f);
			CGLWidget::GetInstance()->renderText  ((+(iSize.x()/2)),ToOpenGLCoords( +20),
				QString("").append(topOrientation),15);
			CGLWidget::GetInstance()->renderText  ((+(iSize.x()/2)-5),ToOpenGLCoords(+(iSize.y()-30)),
				QString("").append(bottomOrientation),15);
			CGLWidget::GetInstance()->renderText  ((+5),ToOpenGLCoords(+(iSize.y()/2)-5),
				QString("").append(leftOrientation),15);
			CGLWidget::GetInstance()->renderText  ((+iSize.x()-15),ToOpenGLCoords(+(iSize.y()/2)-5),
				QString("").append(rightOrientation),15);
		}
		catch (exception *e)
		{
		}
	}

	//Frame data on the slider
	if(GetTextDisplay(EDisplayTextFrameData) )
	{
		glColor3f(1.0f, 1.0f, 0.1f);
		CGLWidget::GetInstance()->renderText  (
			(+(iSize.x()/2)),ToOpenGLCoords(iFrameSlider.position.y()+iFrameSlider.size.y()),
			QString::number(iFrameSlider.data).append("/").append(QString::number(GetActualTextureDepth()-1)),15);

	}
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	ImLog.write("End: CGLImage::DrawTexts");
}

int CGLImage::GetActualSliceWidth()
{
	TImageAxisOrientation orientation = GetOrientation();
	switch(orientation)
	{
	case EImageOrientationAxial:
		{
			return GetTexture()->GetWidth();
		}
		break;
	case EImageOrientationSagittal: //zleva doprava
		{
			return GetTexture()->GetHeight();
		}
		break;
	case EImageOrientationCoronal: //zezadu dopredu
		{
			return GetTexture()->GetWidth();
		}
		break;
	default:
		break;
	}
}
int CGLImage::GetActualSliceHeight()
{
	TImageAxisOrientation orientation = GetOrientation();
	switch(orientation)
	{
	case EImageOrientationAxial:
		{
			return GetTexture()->GetHeight();
		}
		break;
	case EImageOrientationSagittal: //zleva doprava
		{
			return GetTexture()->GetDepth()*GetTexture()->GetDicomHeader(0).GetImageInfo().GetSliceThickness();

		}
		break;
	case EImageOrientationCoronal: //zezadu dopredu
		{
			return GetTexture()->GetDepth()*GetTexture()->GetDicomHeader(0).GetImageInfo().GetSliceThickness();
		}
		break;
	default:
		break;
	}
}

void CGLImage::DrawSliceToTexture()
{
	ImLog.write("Begin: CGLImage::DrawSliceToTexture");
	float heightCorrection =( 1.0f - 1.0f/((float)(GetActualSliceWidth()) / (float)(GetActualSliceHeight())))/2.0f;
	glBegin(GL_QUADS);
	glTexCoord3d(iActualTextureCoords.bottomLeft.GetX(),
		iActualTextureCoords.bottomLeft.GetY(),
		iActualTextureCoords.bottomLeft.GetZ()); 
	glVertex2d(0,0+heightCorrection);

	glTexCoord3d(iActualTextureCoords.bottomRight.GetX(),
		iActualTextureCoords.bottomRight.GetY(),
		iActualTextureCoords.bottomRight.GetZ());
	glVertex2d(1 ,0+heightCorrection);

	glTexCoord3d(iActualTextureCoords.topRight.GetX(),
		iActualTextureCoords.topRight.GetY(),
		iActualTextureCoords.topRight.GetZ());
	glVertex2d(1,1-heightCorrection);

	glTexCoord3d(iActualTextureCoords.topLeft.GetX(),
		iActualTextureCoords.topLeft.GetY(),
		iActualTextureCoords.topLeft.GetZ());
	glVertex2d(0,1-heightCorrection);
	glEnd();
	ImLog.write("End: CGLImage::DrawSliceToTexture");
}

//Function prepare texture fo immediate display 
//this is to save performance - call only on texture coords change or bias and scale (window width and center change)
void CGLImage::PrepareActualTexture(){
	float cd= iTexture->GetWindowDefaults().center;
	float wd =iTexture->GetWindowDefaults().width;
	float bias;
	float c = (iImageWindow.center - (cd -wd/2.)) / wd;
	float w1  = ((iImageWindow.center-iImageWindow.width/2) - (cd -wd/2.)) / wd;
	float w2 = ((iImageWindow.center+iImageWindow.width/2) - (cd -wd/2.)) / wd;
	float w = w2-w1;
	w = iImageWindow.width/wd ;
	float scale = 1./w;
	bias =- (c-w/2)*scale;
	iScale = scale;
	iBias = bias;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0,iActualSliceTextureInfo.width, iActualSliceTextureInfo.height);					// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();							// Reset The Projection Matrix
	glOrtho(0, 1 ,0, 1, 0.1, 90); 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();						// Reset The Modelview Matrix
	glTranslatef(0,0,-50.0f);
	glEnable(GL_TEXTURE_3D);
	glBindTexture( GL_TEXTURE_3D, iTexture->GetTextureID ());
	if(iOpenGLInterpol){
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else{
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	glColor3f(1, 1, 1);
	cgGLBindProgram(CGLWidget::GetInstance()->GetFragmentShaderBiasScaleProgram3D ());
	cgGLEnableProfile(CGLWidget::GetInstance()->GetCgFragmentProfile());
	cgSetParameter1f(CGLWidget::GetInstance()->GetFragmentShaderBiasScale3DBiasParam(), iBias);
	cgSetParameter1f(CGLWidget::GetInstance()->GetFragmentShaderBiasScale3DScaleParam(), iScale);
	cgUpdateProgramParameters (CGLWidget::GetInstance()->GetFragmentShaderBiasScaleProgram3D());

	DrawSliceToTexture();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	cgGLDisableProgramProfiles (CGLWidget::GetInstance()->GetFragmentShaderBiasScaleProgram3D ());
	cgGLDisableProfile (CGLWidget::GetInstance()->GetCgFragmentProfile ());

	glDisable(GL_TEXTURE_3D);
	glBindTexture( GL_TEXTURE_3D, 0);
	glGetErr();

	ImLog.write("End: CGLImage::PrepareActualTexture");
	return;
}

void CGLImage::PreparePlanarSlice(char type, TPlanarCrossPosition crossposition){
	float cd = iTexture->GetWindowDefaults().center;
	float wd = iTexture->GetWindowDefaults().width;
	float bias;
	float c = (iImageWindow.center - (cd -wd/2.)) / wd;
	float w1  = ((iImageWindow.center-iImageWindow.width/2) - (cd -wd/2.)) / wd;
	float w2 = ((iImageWindow.center+iImageWindow.width/2) - (cd -wd/2.)) / wd;
	float w = w2-w1;
	w = iImageWindow.width/wd ;
	float scale = 1./w;
	bias =- (c-w/2)*scale;
	iScale = scale;
	iBias = bias;

	if (type=='z')
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFBOZ);
	if (type=='x')
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFBOX);
	if (type=='y')
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFBOY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int textureDepth = iTexture->GetDepth() * GetTexture()->GetDicomHeader(0).GetImageInfo().GetSliceThickness() * 2.5;

	


	if (type=='z')
		glViewport(0, 0,iTexture->GetWidth(), iTexture->GetHeight());					// Reset The Current Viewport
	if (type=='x'){
		if (textureDepth<iTexture->GetHeight()){
			int trans = (iTexture->GetHeight() - textureDepth)/2;
			glViewport(trans, 0,textureDepth, iTexture->GetHeight());
		}else{
			float resizeRatio = (float)iTexture->GetWidth()/(float)textureDepth;
			int trans = (textureDepth * resizeRatio - iTexture->GetHeight() * resizeRatio)/2;
			glViewport(0, trans,textureDepth * resizeRatio, iTexture->GetHeight() * resizeRatio);
		}
	}
	if (type=='y'){
		if (textureDepth<iTexture->GetWidth()){
			int trans = (iTexture->GetWidth() - textureDepth)/2;
			glViewport(0, trans,iTexture->GetWidth(), textureDepth);	
		}else{
			float resizeRatio = (float)iTexture->GetHeight()/(float)textureDepth;
			int trans = (textureDepth*resizeRatio - iTexture->GetWidth()*resizeRatio)/2;
			glViewport(trans, 0,iTexture->GetWidth()*resizeRatio, textureDepth*resizeRatio);
		}
	}
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();							// Reset The Projection Matrix
	glOrtho(0, 1 ,0, 1, 0.1, 90); 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();						// Reset The Modelview Matrix
	glTranslatef(0,0,-50.0f);
	glEnable(GL_TEXTURE_3D);
	glBindTexture( GL_TEXTURE_3D, iTexture->GetTextureID ());
	if(iOpenGLInterpol){
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else{
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	glColor3f(1, 1, 1);

	cgGLBindProgram(CGLWidget::GetInstance()->GetFragmentShaderBiasScaleProgram3D ());
	cgGLEnableProfile(CGLWidget::GetInstance()->GetCgFragmentProfile());
	cgSetParameter1f(CGLWidget::GetInstance()->GetFragmentShaderBiasScale3DBiasParam(), iBias);
	cgSetParameter1f(CGLWidget::GetInstance()->GetFragmentShaderBiasScale3DScaleParam(), iScale);
	cgUpdateProgramParameters (CGLWidget::GetInstance()->GetFragmentShaderBiasScaleProgram3D());

	if (type=='z')
		DrawToTextureSliceZ(crossposition);
	if (type=='x')
		DrawToTextureSliceX(crossposition);
	if (type=='y')
		DrawToTextureSliceY(crossposition);

	cgGLDisableProgramProfiles (CGLWidget::GetInstance()->GetFragmentShaderBiasScaleProgram3D ());
	cgGLDisableProfile (CGLWidget::GetInstance()->GetCgFragmentProfile ());

	glBindTexture( GL_TEXTURE_3D, 0);
	glDisable(GL_TEXTURE_3D);

	if (type=='z')
		DrawManipulationSliceZ(crossposition);
	if (type=='x')
		DrawManipulationSliceX(crossposition);
	if (type=='y')
		DrawManipulationSliceY(crossposition);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	glGetErr();
	return;
}


void CGLImage::initializeGL()
{
}

/**
/*	Draw some hint that image is being moved or resized
*/
void CGLImage::DrawManipulation()
{
	if(iManipulated == EManipNone)
		return;
	// nahrat jednotkovou matici
	glDisable( GL_TEXTURE_2D );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//disallowed- red
	glColor4f(0.8,0.2,0.2,0.4);
	if(iManipulated==EManipAllowed)
	{
		glColor4f(0.2,0.8,0.2,0.4);
	}

	glRecti(0,0,iSize.x(),iSize.y());
	glDisable(GL_BLEND);
}
void CGLImage::paintGL()
{
	ImLog.write("Begin: CGLImage::paintGL");
	CGLWidget::GetInstance()->resetGLView();


	glDisable(GL_BLEND);
	Translate();
	DrawBorderRect();
	DrawInnerRect();

	glColor3f(1.,
		1., 1.);

	glDisable(GL_BLEND);
	PrepareActualTexture();
	glDisable(GL_BLEND);
	glColor3f(1.,
		1., 1.);

	CGLWidget::GetInstance()->resetGLView();
	Translate();

	DrawImage();
	glColor4f(1,1,1,1);
	//glRectf(0,0,20,20);
	DrawManipulation();
	DrawIcons();
	DrawSlider();
	DrawTexts();
	ImLog.write("End: CGLImage::paintGL");
	return;
}

void CGLImage::SetAnimation(CAnimation *animation)
{
	iAnimation = animation;
}

CAnimation* CGLImage::GetAnimation()
{
	return iAnimation;
}



void CGLImage::MoveToFrame(int frame)
{
	ImLog.write("Begin: CGLImage::MoveToFrame");
	float move = 0;
	int depth = GetActualTextureDepth();
	iFrameSlider.data=frame;
	if(iFrameSlider.data <= 0)
	{
		iFrameSlider.data = 0;
	}
	if(iFrameSlider.data >= depth)
	{
		iFrameSlider.data = depth-1;
	}
	if((depth-1)!=0)
	{
	move=(float)iFrameSlider.data/(depth-1);
	}
	else
	{
		move = 0;
	}
	MoveToDepth(move);
	ImLog.write("End: CGLImage::MoveToFrame");
}
void CGLImage::MoveToDepth(float inDepthPosition)
{
	ImLog.write("Begin: CGLImage::MoveToDepth");
	switch(iImageAxisOrientation)
	{
	case EImageOrientationAxial:
		{

			iActualTextureCoords.bottomLeft.SetZ(inDepthPosition);
			iActualTextureCoords.bottomRight.SetZ(inDepthPosition);
			iActualTextureCoords.topLeft.SetZ(inDepthPosition);
			iActualTextureCoords.topRight.SetZ(inDepthPosition);
		}
		break;
	case EImageOrientationSagittal: //zleva doprava
		{

			iActualTextureCoords.bottomLeft.SetX(inDepthPosition);
			iActualTextureCoords.bottomRight.SetX(inDepthPosition);
			iActualTextureCoords.topLeft.SetX(inDepthPosition);
			iActualTextureCoords.topRight.SetX(inDepthPosition);
		}
		break;
	case EImageOrientationCoronal: //zezadu dopredu
		{

			iActualTextureCoords.bottomLeft.SetY(inDepthPosition);
			iActualTextureCoords.bottomRight.SetY(inDepthPosition);
			iActualTextureCoords.topLeft.SetY(inDepthPosition);
			iActualTextureCoords.topRight.SetY(inDepthPosition);
		}
		break;
	default:
		break;
	}
	//move=(float)iFrameSlider.data/(depth-1);
	iFrameSlider.data=(float)(inDepthPosition*(GetActualTextureDepth()));
	if(iFrameSlider.data>= GetActualTextureDepth())
	{
		iFrameSlider.data = GetActualTextureDepth()-1;
	}
	PrepareActualTexture();
	if(iParentWorkspace)
	{
		iParentWorkspace->UpdateTexture();
	}
	CGLWidget::GetInstance()->updateGL();
	ImLog.write("End: CGLImage::MoveToDepth");
}
bool CGLImage::IsOnFrameSliderIcon(int x, int y)
{
	//if frame slider not visible - return immediately
	if(!GetTextDisplay(EDisplayTextFrameData))
		return false;
	if(x<iFrameSlider.position.x())
		return false;
	if(y<iFrameSlider.position.y())
		return false;
	if(x>(iFrameSlider.position.x()+iFrameSlider.size.x()))
		return false;
	if(y>(iFrameSlider.position.y()+iFrameSlider.size.y()))
		return false;

	float sliderlength = iFrameSlider.size.x();
	float sliderposition = x - iFrameSlider.position.x();
	float depth = sliderposition/sliderlength;
	//move=(float)iFrameSlider.data/(depth-1);
	MoveToDepth(depth);
	//set z coords of texture
	return true;
}

/**
Use SetGeometry for image instead of Set
*/
void CGLImage::SetGeometry(float x, float y, float width, float height)
{
	ImLog.write("Begin: CGLImage::SetGeometry");
	/*if(x<0)x=0;
	if(y<0)y=0;
	if(x>iParentWindow->width()-10)
	x=iParentWindow->width()-10;
	if(y>iParentWindow->height()-10)
	y=iParentWindow->height()-10;*/
	//forbid imgae to be shrinked too much
	/*	if(width <3)
	return;
	if(height<3)
	return;
	if(width <60 && (height<30))
	return;*/
	QPointF oldPos=iPosition;
	QPointF oldSize=iPosition;
	iPosition.setX(x);
	iPosition.setY(y);
	//recompute image center coord to guarantee that image center will be kept
	float oldWidth=iSize.x();
	float oldHeight = iSize.y();
	QPointF oldCenter = iImageCenter;
	float oldCenterX = oldCenter.x()*oldWidth;
	float oldCenterY = oldCenter.y()*oldHeight;
	float oldDifX = oldCenterX-(float)oldWidth/2;
	float oldDifY = oldCenterY-(float)oldHeight/2;
	float newDifX = oldDifX /** (float)(oldWidth/width);*/;
	float newDifY = oldDifY /** (float)(oldHeight/height)*/;
	iImageCenter.setX((float)((float)width/2.+newDifX) / (float)width);
	iImageCenter.setY((float)((float)height/2.+newDifY)/(float)height);
	iSize.setX (width);
	iSize.setY (height);

	//slider
	iFrameSlider.position.setX( 10);

	iFrameSlider.size.setY(15);
	iFrameSlider.size.setX(iSize.x() - iFrameSlider.position.x() - 25); //iSize.x() - size.x()
	iFrameSlider.position.setY(iSize.y() - iFrameSlider.size.y()-2);
	if(iParentWorkspace)
	{
		iParentWorkspace->UpdateTexture();
	}
	resizeGL();
	ImLog.write("End: CGLImage::SetGeometry");
}
int CGLImage::GetActualFrameNr()
{
	return iFrameSlider.data;
}
void CGLImage::wheelEvent(QWheelEvent *event)
{
	if(event->delta() > 0 )
	{
		MoveToFrame(iFrameSlider.data+1);
	}
	else if(event->delta() < 0 )
	{
		MoveToFrame(iFrameSlider.data-1);
	}


}
float CGLImage::GetZoom()
{
	return iZoomFactor;

}
void CGLImage::SetZoom(float zoom)
{
	iZoomFactor=zoom;
	if(iParentWorkspace)
	{
		//iLayout->PrepareImageToMove(iActiveImage);
		iParentWorkspace->UpdateTexture();
	}
	CGLWidget::GetInstance()->updateGL ();
}

void CGLImage::mouseReleaseEvent(QMouseEvent *event)
{
	if(iMouseState==EMouseStateObjectMoving && iParentWorkspace)
	{
		iParentWorkspace->GetLayout().ImageMoveFinished(this);
	}
	iManipulated=EManipNone;
	CGLWidget::GetInstance()->updateGL();
	if(CInfoPanel::GetInstance())
	{
		if(CInfoPanel::GetInstance()->GetSourceImage() == this)
		{
			CInfoPanel::GetInstance()->SetSourceImage(this);
		}
	}
}
void CGLImage::mouseDoubleClickEvent(QMouseEvent *event)
{

}
/**
* @brief Sets the image window center and width settings
*/
void CGLImage::SetImageWindow(TImageWindow window)
{
	iImageWindow = window;
	PrepareActualTexture();
	if(iParentWorkspace)
	{
		iParentWorkspace->UpdateTexture();
	}
	CGLWidget::GetInstance()->updateGL ();
}

TImageWindow CGLImage::GetImageWindow()
{
	return iImageWindow;
}
void CGLImage::mouseMoveEvent(QMouseEvent *event)
{
	int x=event->x() - iPosition.x();
	int y = event->y()- iPosition.y();
	int dx = x-iPreviousMousePosition.x();
	int dy = y-iPreviousMousePosition.y();
	if(EMouseStateImageWindowLeveling==iMouseState)
	{
		QCursor::setPos(iLockedGlobalMousePosition);
		//iScale *=(1.+(float)dx/50.);
		//iBias *=(1.+(float)dy/100.);
		iImageWindow.center+=4*dy;
		if(iImageWindow.center>65596)
			iImageWindow.center = 65096;
		//if(iImageWindow.center<0)
		//	iImageWindow.center = 0;
		iImageWindow.width+=4*dx;
		if(iImageWindow.width>65096)
			iImageWindow.width = 65096;
		if(iImageWindow.width<1)
			iImageWindow.width = 1;

		SetImageWindow(iImageWindow);
		return;
	}
	else if(iMouseState == EMouseStateFrameSliderChanging )
	{
		QCursor::setPos(event->globalPos().x(),iLockedGlobalMousePosition.y());
		IsOnFrameSliderIcon (x,y);

		return;
	}
	else if(iMouseState == EMouseStateImageZooming)
	{
		QCursor::setPos(iLockedGlobalMousePosition);
		SetZoom(iZoomFactor*(1.-(float)dy/100.));
		iImageCenter.setX((x-(x-iLockedImageCenter.x()*iSize.x())*(iZoomFactor/iLockedZoom))/iSize.x());
		iImageCenter.setY((y-(y-iLockedImageCenter.y()*iSize.y())*(iZoomFactor/iLockedZoom))/iSize.y());
		
		return;
	}
	else if(iMouseState == EMouseStateImageMoving)
	{
		iImageCenter.setX(iImageCenter.x()+(float)dx/iSize.x());
		iImageCenter.setY(iImageCenter.y()+(float)dy/iSize.y());
		if(iParentWorkspace)
		{
			iParentWorkspace->UpdateTexture();
		}
		CGLWidget::GetInstance()->updateGL ();
		//return;
	}

	else if(iMouseState == EMouseStateObjectMoving)
	{

		dx=event->globalPos().x()-iLockedGlobalMousePosition.x();
		dy=event->globalPos().y()-iLockedGlobalMousePosition.y();

		SetGeometry(iLockedPosition.x()+dx,
			iLockedPosition.y()+dy ,
			iSize.x(),
			iSize.y());
		if(iParentWorkspace)
		{
			iParentWorkspace->GetLayout().ImageMoved(this);
		}
		CGLWidget::GetInstance()->updateGL ();
		//return; //to avoid updating of previous mouse position
	}
	else if(iMouseState == EMouseStateObjectResizing)
	{

		dx=event->globalPos().x()-iLockedGlobalMousePosition.x();
		dy=event->globalPos().y()-iLockedGlobalMousePosition.y();
		SetGeometry(iPosition.x(),iPosition.y(),
			iLockedSize.x()+dx,
			iLockedSize.y()+dy);
		//iPreviousMousePosition = QPoint(iSize.x(), iSize.y());
		if(iParentWorkspace)
		{
			iParentWorkspace->GetLayout().ImageResized(this);
		}
		CGLWidget::GetInstance()->updateGL ();
		//return;

	}
	iPreviousMousePosition = QPoint(x, y);	
	//iPreviousGlobalMousePosition = event->globalPos();
}
void CGLImage::SetManipulated(TManipulationState manipulated)
{

	iManipulated=manipulated;
}
//Closes the image
void CGLImage::CloseMe()
{
	ImLog.write("Begin: CGLImage::CloseMe");
	if(CAnimationManager::GetInstance())
	{
		//removes the animation if exists
		CAnimationManager::GetInstance()->RemoveAnimation(this);

	}
	if(CInfoPanel::GetInstance())
	{
		if(CInfoPanel::GetInstance()->GetSourceImage() == this)
		{
			CInfoPanel::GetInstance()->SetSourceImage(NULL);
		}
	}
	iMouseState=EMouseStateNone;
	if(iParentWorkspace)
	{
		if(GetOwner())
		{
			GetOwner()->RemoveDerivedImage(this);
		}
		iParentWorkspace->RemoveImage(this);

	}
	if(CGLImageExplorer::GetInstance())
	{
		CGLImageExplorer::GetInstance()->CloseImage(this);
	}
	QListIterator<CGLImage*> derivedImages(iDerivedImages);
	while(derivedImages.hasNext())
	{
		CGLImage* im=derivedImages.next();
		im->SetOwner(NULL);
		im->CloseMe();
		delete im;
	}
	iDerivedImages.clear();
	ImLog.write("End: CGLImage::CloseMe");

}
void CGLImage::mousePressEvent(QMouseEvent *event)
{
	//iSkipMouseMovement = false;
	iMouseState = EMouseStateNone;
	//locked states
	iLockedGlobalMousePosition = event->globalPos();
	//Save previous object state
	iLockedSize = iSize;
	iLockedPosition = iPosition;
	iLockedImageCenter = iImageCenter;
	iLockedZoom=iZoomFactor;
	int x=event->x() - iPosition.x();
	int y = event->y()- iPosition.y();
	iPreviousMousePosition = QPoint(x, y);	


	if(IsOnResizeIcon(x, y))
	{
		iMouseState=EMouseStateObjectResizing;
		if(iParentWorkspace)
		{
			iParentWorkspace->GetLayout().PrepareImageToResize(this);
		}
		iManipulated=EManipAllowed;

		return;		
	}
	if(IsOnMoveIcon(x, y))
	{
		iMouseState=EMouseStateObjectMoving;
		if(iParentWorkspace)
		{
			iParentWorkspace->GetLayout().PrepareImageToMove(this);
		}

		//iManipulated=true;

		return;

	}
	if(IsOnCloseIcon(x, y))
	{
		CloseMe();
		delete this;
		return;

	}
	if(IsOnFrameSliderIcon(x, y))
	{
		iMouseState=EMouseStateFrameSliderChanging;
		return;		
	}
	if(event->button() == Qt::LeftButton )
	{
		iMouseState = EMouseStateImageMoving;

		return;
	}
	if(event->button() == Qt::MidButton )
	{
		iMouseState = EMouseStateImageZooming;
	}

	if(event->button() == Qt::RightButton  )
	{
		iMouseState = EMouseStateImageWindowLeveling;
	}

}
////////////////////
void CGLImage::SetOpenGLInterpolation(bool on)
{
	iOpenGLInterpol = on;
}

float CGLImage::GetActualTextureDepth()
{
	//TODO compute it better ??
	switch(iImageAxisOrientation)
	{
	case EImageOrientationAxial:
		{
			return iTexture->GetDepth();
		}
		break;
	case EImageOrientationSagittal:
		{
			return iTexture->GetWidth();
		}
		break;
	case EImageOrientationCoronal:
		{
			return iTexture->GetHeight();
		}
		break;
	default:
		{
			return iTexture->GetDepth();
		}
	}
}
QList<CGLImage *>& CGLImage::GetDerivedImages()
{
	return iDerivedImages;
}
void CGLImage::RemoveDerivedImage(CGLImage* image)
{
	if(iDerivedImages.contains(image))
	{

		iDerivedImages.removeOne(image);
	}
}
CGLImage* CGLImage::CreateDerivedImage(TImageAxisOrientation orientation)
{
	if(!CWorkspaceManager::GetInstance()->GetActiveWorkspace())
	{
		QMessageBox msgBox(QMessageBox::Warning, QObject::tr("QMessageBox::warning()"),
		QString("No active workspace to add image to"), 0, MainWindow::iSelfS);
		msgBox.addButton(QObject::tr("&Continue"), QMessageBox::RejectRole);
		msgBox.exec();
		return NULL;
	}
	QPointF pos(2,2);
	QPointF size(iSize.x(), iSize.y());
	CGLImage *newImage;
	try
	{
		newImage = new CGLImage(CWorkspaceManager::GetInstance()->GetActiveWorkspace(),
			C3DTextureManager::GetInstance()->GetTexture(this->GetTexture()->GetIdentificationString()),pos,size);
	}
	catch(DicomFramesException &e)
	{
		QMessageBox msgBox(QMessageBox::Warning, QObject::tr("QMessageBox::warning()"),
			QString("Could not allocate texture"), 0, MainWindow::iSelfS);
		msgBox.addButton(QObject::tr("&Continue"), QMessageBox::RejectRole);
		msgBox.exec();
		ImLog.write("End: *CGLImage::CreateDerivedImage");
		return NULL;

	}
	CGLWidget::GetInstance()->updateGL();
	newImage->SetOwner(this);
	QString name("Im:");
	name.append(QString::number(iDerivedImages.count()));
	newImage->SetName(name);
	iDerivedImages.append(newImage);
	newImage->SetOrientation(orientation);
	ImLog.write("End: *CGLImage::CreateDerivedImage");
	return newImage;
	//iDerivedImages;
}

float CGLImage::SwitchY(float ypos){
	return 1-ypos;
}
