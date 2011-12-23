#include <glObjects/workspaces/glWorkspace.h>
#include <glObjects/glWidget.h>
#include <infoPanel.h>
#include <glObjects/workspaces/layouts/growingGridLayout.h>
#include <glObjects/workspaces/layouts/freeLayout.h>
#include <glObjects/workspaces/workspaceManager.h>
#include <glObjects/workspaces/glWorkspaceSnapshot.h>
#include <settings.h>

#include "dpDebug/dpDebug.h"
debug::Log WorkLog;

CGLWorkspace::CGLWorkspace(CGLObject *parent, const QPointF &position, const QPointF &size, TLayoutType layoutType):
CGLObject(parent, position, size)
{
	WorkLog.write("Begin: CGLWorkspace::CGLWorkspace");	
	iLayout = NULL;
	iLastInnerHeight = 0;
	iLastInnerWidth = 0;
	iWorkspaceChanged = true;
	iName = new QString();
	iSaveSnapshot = false;
	iSaveSnapshotFileName = NULL;
	QString name;
	name.clear();
	name.append("WS:");
	name.append(QString::number(CWorkspaceManager::GetInstance()->GetWorkspaces().count()));
	SetName(name);
	iActiveImage = NULL;
	SetLayout(layoutType);
	if(!InitTexture())
	{
		int err=1;
		throw TextureNotCreatedException();
	}
	iWorkspaceSnapshot =new CGLWorkspaceSnapshot(parent,this,QPointF(10,10), QPointF(100,100));
	SetBorders(Settings::GetBordersConstant(EWorkspaceBorders));
	SetBorderColor(Settings::GetColorConstant(EWorkspaceBorderColor));
	SetInnerColor(Settings::GetColorConstant(EWorkspaceInnerColor));
	WorkLog.write("End: CGLWorkspace::CGLWorkspace");	
}
void CGLWorkspace::StartAnimations()
{
	QListIterator<CGLImage*> images(iImages);
	images.toBack();
	while (images.hasPrevious())
	{
		CGLImage* obj = images.previous();
		if(obj)
		{
			if(obj->GetAnimation())
			{
				obj->GetAnimation()->Start();
			}
		}
	}
}
void CGLWorkspace::StopAnimations()
{
	QListIterator<CGLImage*> images(iImages);
	images.toBack ();
	while (images.hasPrevious())
	{
		CGLImage* obj = images.previous();
		if(obj)
		{
			if(obj->GetAnimation())
			{
				obj->GetAnimation()->Stop();
			}
		}
	}
}

void CGLWorkspace::SetGeometry(float x, float y, float w, float h)
{
	WorkLog.write("Begin: CGLWorkspace::SetGeometry");	
	QPointF oldPos = iPosition;
	if(iLastInnerHeight ==0)
	{

		iLastInnerWidth = iSize.x()-GetBorders().left-GetBorders().right;
		iLastInnerHeight = iSize.y()-GetBorders().top-GetBorders().bottom;
	}
	int posxdif = x - oldPos.x();
	int posydif = y - oldPos.y();
	int innerWidth = w - GetBorders().left-GetBorders().right;
	int innerHeight = h- GetBorders().top-GetBorders().bottom;
	float sizeRatioX = (float)(innerWidth)/((float)iLastInnerWidth);
	float sizeRatioY = (float)(innerHeight)/((float)iLastInnerHeight);
	iLastInnerWidth = innerWidth;
	iLastInnerHeight = innerHeight;


	CGLObject::SetGeometry(x,y,w,h);

	QListIterator<CGLImage*> images(iImages);
	images.toFront();
	while(images.hasNext())
	{
		CGLImage* im = images.next();
		QPointF pos(im->GetPosition().x()-GetBorders().left,im->GetPosition().y()-GetBorders().top);
		QPointF size = im->GetSize();
		QPointF newPos;
		newPos.setX(GetBorders().left+pos.x()*sizeRatioX+posxdif*sizeRatioX);
		newPos.setY(GetBorders().top+pos.y()*sizeRatioY+posydif*sizeRatioY);
		QPointF newSize;
		newSize.setX(size.x()*sizeRatioX);
		newSize.setY(size.y()*sizeRatioY);
		im->SetGeometry(newPos.x(),newPos.y(), newSize.x(),newSize.y());
	}
	UpdateTexture();
	WorkLog.write("Begin: CGLWorkspace::SetGeometry");	
}


void CGLWorkspace::SetName( const QString &name)
{
	iName->clear();
	iName->append(name);
}

QString& CGLWorkspace::GetName()
{
	return *iName;
}
void CGLWorkspace::SetLayout(TLayoutType type)
{
	WorkLog.write("Begin: CGLWorkspace::SetLayout");	
	if(iLayout)
	{
		if(iLayout->GetType() == type)
		{
			return;
		}
	}
	if(iLayout)
	{
		delete iLayout;
		iLayout = NULL;
	}
	switch (type)
	{
	case ELayoutFree:
		{
			iLayout= new CFreeLayout(this);
			iLayout->Do();
			break;
		};
	case ELayoutGrowingGridHorizontal:
		{
			iLayout= new CGrowingGridLayout(this,EGrowinGridLayoutHorizontal);
			iLayout->Do();
			break;
		};
	case ELayoutGrowingGridVertical:
		{
			iLayout= new CGrowingGridLayout(this,EGrowinGridLayoutVertical);
			iLayout->Do();
			break;
		};


	}
	WorkLog.write("End: CGLWorkspace::SetLayout");
}
CGLWorkspace::~CGLWorkspace()
{
	delete iActualTextureFBO;
	QListIterator<CGLImage*> images(iImages);
	images.toBack ();
	while (images.hasPrevious())
	{
		CGLImage* obj = images.previous();
		if(obj)
		{
			delete obj;
			obj = NULL;
		}
	}
	if(iName)
	{
		delete iName;
		iName = NULL;
	}
	if(iLayout)
	{
		delete iLayout;
		iLayout = NULL;
	}
	if(iWorkspaceSnapshot)
	{
		delete iWorkspaceSnapshot;
		iWorkspaceSnapshot=NULL;
	}
	if(iSaveSnapshotFileName)
	{
		delete iSaveSnapshotFileName;
		iSaveSnapshotFileName = NULL;
	}
}

/*void CGLWorkspace::SetIdentification(QString &identification)
{
iIdentification = identification;
}
QString &CGLWorkspace::GetIdentification()
{
return iIdentification;
}
*/

void CGLWorkspace::initializeGL(){

}

CGLWorkspaceSnapshot& CGLWorkspace::GetSnapshot()
{
	return *iWorkspaceSnapshot;
}
MWorkspaceLayout &CGLWorkspace::GetLayout()
{
	return *iLayout;
}
/************************************************************************/
/* draw to 0,1 full space                                                                     */
/************************************************************************/
void CGLWorkspace::DrawFromTexture()
{
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, iActualTextureFBO->texture());

	//TODO interpolation setting
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBegin( GL_QUADS );
	glTexCoord2d(0,0); 
	glVertex2d(0,0);
	glTexCoord2d(1,0);
	glVertex2d(1,0);
	glTexCoord2d(1,1);
	glVertex2d(1,1);
	glTexCoord2d(0,1);
	glVertex2d(0,1);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//clip image
	//glColor4f(1,1,0,1);
	//glRectf (0.0,0.0,1,1);

	//copy to texture

}
/**
* @brief Save the actual workspace texture to the file
*/
void CGLWorkspace::SaveSnapshot(QString& fileName)
{
	if(fileName.isEmpty())
		return;
	iSaveSnapshot = true;
	if(iSaveSnapshotFileName)
	{
		delete iSaveSnapshotFileName;
		iSaveSnapshotFileName = NULL;
	}
	iSaveSnapshotFileName = new QString(fileName);
	paintGL();
}

int CGLWorkspace::GetSaveSnapshotWidth()
{
	return iSaveSnapshotWidth;
}
int CGLWorkspace::GetSaveSnapshotHeight()
{
	return iSaveSnapshotHeight;
}

void CGLWorkspace::SetSaveSnapshotWidth(int width)
{
	iSaveSnapshotWidth = width;
}
void CGLWorkspace::SetSaveSnapshotHeight(int height)
{
	iSaveSnapshotHeight = height;
}

/**
* @brief Save the actual workspace texture to the file
*/
void CGLWorkspace::SaveTexture(QString& fileName)
{
	//TODO interpolation
	WorkLog.write("Begin: CGLWorkspace::SaveTexture");
	int texwidth = GetSaveSnapshotWidth();
	int texheight = GetSaveSnapshotHeight();
	if(iActiveImage)
	{
		bool writeToFile=true;
		if(writeToFile)
		{
			QList<QByteArray> list=QImageReader::supportedImageFormats ();
			QListIterator<QByteArray> it(list);
			it.toFront();
			while(it.hasNext())
			{
				const char *format = it.next().data();
				int i=1;
			}
			int bmpwidth=texwidth;
			int bmpheight=bmpwidth*((float)iSize.y()/(float)iSize.x());
			bool b=	iActualTextureFBO->toImage().scaled(bmpwidth,bmpheight).save(fileName); 
			if(b!=true)
			{
				QMessageBox msgBox;
				msgBox.setText("Image was not saved");
				msgBox.exec();
				WorkLog.write("End: CGLWorkspace::SaveTexture");
				return;
			}
		}
	}
	WorkLog.write("End: CGLWorkspace::SaveTexture");
}
void CGLWorkspace::paintGL(){
	CGLWidget::GetInstance()->resetGLView();
	Translate();
	DrawBorderRect();	
	DrawInnerRect();
	DrawToTexture();

	int ypos= CGLWidget::GetInstance()->GetSize().y()
		-iSize.y()
		-iPosition.y()+GetBorders().bottom;

	//CGLWidget::GetInstance()->resetGLView();
	//Translate();
	glViewport(iPosition.x()+GetBorders().left,
		ypos ,
		iSize.x()-GetBorders().left-GetBorders().right
		, iSize.y()-GetBorders().top-GetBorders().bottom);					// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();							// Reset The Projection Matrix
	glOrtho(0, 1 , 0,1, 0.1, 90); 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();				
	glTranslatef(0,0,-50);
	glDisable(GL_BLEND);

	glColor4f(1.,1.,1.,1.);
	DrawFromTexture();
	glEnable(GL_BLEND);
	/*glColor4f(1,1,1,1);
	glRectf(0,0,0.1,0.1);*/
	//DrawSelection();
}

bool CGLWorkspace::InitTexture()
{

	// create a texture object
	//smaller = faster
	iActualTextureInfo.width =Settings::GetIntegerConstant(EWorkspaceTextureWidth); //must be 2^n
	iActualTextureInfo.height = Settings::GetIntegerConstant(EWorkspaceTextureHeight);
	iSaveSnapshotWidth = iActualTextureInfo.width ;
	iSaveSnapshotHeight = iActualTextureInfo.height;
	//	iActualSliceTextureInfo.width = 1024; //must be 2^n
	//iActualSliceTextureInfo.height = 1024;//must be 2^n
	/*glGenTextures(1, &iActualTexture);
	glBindTexture(GL_TEXTURE_2D, iActualTexture);
	//Set to GL_LINEAR to have smooth texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA32F  , 
	iActualTextureInfo.width, iActualTextureInfo.height,
	0, GL_RGBA , GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//GL_frame_buffer_object - core in OpenGL 3.0 http://www.opengl.org/wiki/GL_EXT_framebuffer_object
	// create a renderbuffer object to store depth info
	//new  august 2008
	GLuint rboId;

	glGenRenderbuffersEXT(1, &rboId);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboId);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
	iActualTextureInfo.width, iActualTextureInfo.height);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	*/
	// create a framebuffer object

	iActualTextureFBO = new QGLFramebufferObject(iActualTextureInfo.width, iActualTextureInfo.height);
	/*
	glGenFramebuffersEXT(1, &iFBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFBO);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
	GL_TEXTURE_2D, iActualTexture , 0);

	// attach the renderbuffer to depth attachment point
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
	GL_RENDERBUFFER_EXT, rboId);

	// check FBO status
	bool fboUsed=true;
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
	return false;

	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	return true;
	*/
	return true;

}
void CGLWorkspace::UpdateTexture()
{
	iWorkspaceChanged =true;
}
/************************************************************************/
/* draws workspace to texture                                                                      */
/************************************************************************/
void CGLWorkspace::DrawToTexture()
{
	WorkLog.write("Begin: CGLWorkspace::DrawToTexture");
	if(!iWorkspaceChanged && !iSaveSnapshot){
		WorkLog.write("End: CGLWorkspace::DrawToTexture");
		return;
	}
	iWorkspaceChanged = false;

	//Set up rendering to texture
	//render to ext buffer (texture behind)
	iActualTextureFBO->bind();


	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFBO);
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//draw 3D texture slice to texture
	QListIterator<CGLImage*> images(iImages);
	int imageHeightSum = 0;
	glDisable(GL_BLEND);

	while (images.hasNext())
	{
		glColor4f(1,1,1,1);
		CGLImage *im = images.next();
		imageHeightSum += im->GetSize().y();
		//im->PrepareActualTexture();
		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFBO);
		glViewport(0, 0,iActualTextureInfo.width, iActualTextureInfo.height);					// Reset The Current Viewport
		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();							// Reset The Projection Matrix
		//map to texture as to workspace (in widget)

		int left = GetPosition().x()+GetBorders().left;
		int right = 	GetSize().x()+GetPosition().x()-GetBorders().right;
		int bottom=CGLWidget::GetInstance()->GetSize().y()-GetPosition().y()-GetSize().y()+GetBorders().bottom;
		int top = CGLWidget::GetInstance()->GetSize().y()-GetPosition().y()-GetBorders().top;
		glOrtho(left, 
			right ,
			bottom,
			top,
			0.1, 90);

		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();						// Reset The Modelview Matrix
		im->Translate();
		glColor4f(1,1,1,1);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		im->DrawBorderRect();
		im->DrawInnerRect();

		if(iActiveImage==im)
		{
			im->DrawSelection();
		}
		glColor4f(1,1,1,1);
		im->DrawImage();
		im->DrawManipulation();
		im->DrawIcons();
		im->DrawSlider();
		im->DrawTexts();
	}
	if(iSaveSnapshot)
	{
		iSaveSnapshot = false;
		SaveTexture(*iSaveSnapshotFileName);

	}
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	iActualTextureFBO->release();
	WorkLog.write("End: CGLWorkspace::DrawToTexture");
	return;
}
void CGLWorkspace::mouseReleaseEvent(QMouseEvent *event)
{
	if(iActiveImage)
	{
		iActiveImage->mouseReleaseEvent(event);
		//UpdateTexture();
	}
}
void CGLWorkspace::mouseDoubleClickEvent(QMouseEvent *event)
{
	if(iActiveImage)
	{
		iActiveImage->mouseDoubleClickEvent(event);
		//UpdateTexture();
	}
}
void CGLWorkspace::mouseMoveEvent(QMouseEvent *event)
{
	if(iActiveImage)
	{
		//iLayout->PrepareImageToMove(iActiveImage);
		iActiveImage->mouseMoveEvent(event);
		//UpdateTexture();
		//iLayout->ImageMoved(iActiveImage);
	}
}
void CGLWorkspace::wheelEvent(QWheelEvent *event){
	if(iActiveImage)
	{
		//iLayout->PrepareImageToMove(iActiveImage);
		iActiveImage->wheelEvent(event);

		//iLayout->ImageMoved(iActiveImage);
	}
}

QList<CGLImage*>& CGLWorkspace::GetImages()
{

	return iImages;
}

void CGLWorkspace::SelectImage(CGLImage *image)
{
	if(iImages.contains(image))
	{
		CGLImageExplorer::GetInstance()->SelectImage(image->GetOwner());
		CInfoPanel::GetInstance()->SelectDerivedImage(image);
		//image->Set(image->GetTexture());
		iActiveImage = image;
		UpdateTexture();
	}
	else
	{
		int usererr=1;
	}
}

void CGLWorkspace::mousePressEvent(QMouseEvent *event){
	iActiveImage = NULL;

	QListIterator<CGLImage*> images(iImages);
	images.toBack ();
	while (images.hasPrevious())
	{
		CGLImage* obj = images.previous();
		if(obj->IsPointOnObject(event->x(),event->y()))
		{
			iActiveImage = obj;
			iImages.move(iImages.indexOf(obj),iImages.count()-1);
			obj->mousePressEvent(event);
			SelectImage(obj);
			UpdateTexture();
			//updateGL ();
			break;
		}
	}
	CInfoPanel::GetInstance()->SetWorkspaceInfoView();
}

void CGLWorkspace::AddImage(CGLImage *image)
{
	image->SetParentWorkspace(this);
	iImages.append (image);
	iLayout->PrepareNewImageGeometry(image);
	UpdateTexture();
	if(CGLWidget::GetInstance())
	{
		CGLWidget::GetInstance()->updateGL();
	}

}
void CGLWorkspace::RemoveImage(CGLImage *image)
{
	if(iImages.contains(image))
	{
		iImages.removeOne(image);
	}
	if(image==iActiveImage)
	{
		iActiveImage=NULL;
	}

	iLayout->RemoveImage(image);
	UpdateTexture();
	if(CGLWidget::GetInstance())
	{
		CGLWidget::GetInstance()->updateGL();
	}

}
CGLImage* CGLWorkspace::GetActiveImage()
{
	return iActiveImage;
}
