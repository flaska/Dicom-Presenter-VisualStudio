#include <GL/glew.h>
#include <QtOpenGL/QtOpenGL>

#include <glObjects/explorers/glImageExplorer.h>
#include <glObjects/glWidget.h>
#include <glObjects/glImage.h>
#include <infoPanel.h>
#include <glObjects/workspaces/workspaceManager.h>
#include <mainWindow.h>
#include <settings.h>

#include "dpDebug/dpDebug.h"
debug::Log IELog;

CGLImageExplorer* CGLImageExplorer::instance = NULL;

CGLImageExplorer::~CGLImageExplorer()
{
	instance = NULL;
	if(iScrollBar)
		delete iScrollBar;
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

}

void CGLImageExplorer::Scrolled(int value)
{
	QListIterator<CGLImage*> images(iImages);
	images.toFront ();
	int relativeY=0;
	while (images.hasNext())
	{
		CGLImage* obj = images.next();
		if(obj)
		{
			obj->SetPosition(QPoint(obj->GetPosition().x(),relativeY-value));
			relativeY+=obj->GetSize().y();
		}
	}
	if(CGLWidget::GetInstance())
	{
		CGLWidget::GetInstance()->updateGL();
	}
}

/*
get singleton instance
*/
CGLImageExplorer* CGLImageExplorer::InitInstance(CGLWidget *parent, QPointF& position, QPointF &size )
{
	if(!instance)
	{
		instance = new CGLImageExplorer(parent, position, size);
	}
	return instance;
}


CGLImage* CGLImageExplorer::GetSelectedImage()
{
	return iActiveImage;
}
/*
get singleton instance
*/
CGLImageExplorer* CGLImageExplorer::GetInstance()
{
	return instance;
}

CGLImageExplorer::CGLImageExplorer(CGLWidget *parentWindow, QPointF& position, QPointF &size ):CGLObject (parentWindow, position, size)
{	
	IELog.write("Begin: CGLImageExplorer::CGLImageExplorer");	
	iActiveImage = NULL;
	iScrollBar = new QScrollBar(Qt::Vertical,parentWindow);

	iScrollBar->setMinimum(0);
	iScrollBar->setMaximum(0);
	iScrollBar->setValue(0);
	QObject::connect(iScrollBar , SIGNAL(valueChanged(int)), 
		this, SLOT(Scrolled(int))); 
	//Init(parentWindow, position,size);
	//iFrames= new CDicomFrames(file->toAscii().data());
	//texture= LoadTextureFromDicom (*loader);
	SetBorders(Settings::GetBordersConstant(EImageExplorerBorders));
	SetBorderColor(Settings::GetColorConstant(EImageExplorerBorderColor));
	SetInnerColor(Settings::GetColorConstant(EImageExplorerInnerColor));
	IELog.write("End: CGLImageExplorer::CGLImageExplorer");
}

void CGLImageExplorer::CloseImage(CGLImage *image)
{
	IELog.write("Begin: CGLImageExplorer::CloseImage");
	if(!(iImages.contains(image)))
	{
		IELog.write("End: CGLImageExplorer::CloseImage");
		return;
	}
	QListIterator<CGLImage*> images(iImages);
	bool bRemoved=false;
	int posy=0;
	while(images.hasNext())
	{
		CGLImage* im=images.next();
		if(im==image)
		{
			bRemoved=true;
			posy=im->GetPosition().y();

		}
		else if(bRemoved)
		{
			int lastpos = im->GetPosition().y();
			im->SetPosition(QPoint(im->GetPosition().x(),posy));
			posy=lastpos;
		}
	}
	if(iImages.contains(image))
	{
		iImages.removeOne(image);
	}
	if(CInfoPanel::GetInstance())
	{
		CInfoPanel::GetInstance()->SetImageExplorerInfoView();
	}
	IELog.write("End: CGLImageExplorer::CloseImage");
}
void CGLImageExplorer::SelectImage(CGLImage *image)
{
	if(iImages.contains(image))
	{
		iActiveImage = image;
	}
}

void CGLImageExplorer::OpenImage(QString &fileName)
{
	IELog.write("Begin: CGLImageExplorer::OpenImage");
	QPointF pos(GetBorders().left,GetBorders().top);
	if(!iImages.isEmpty())
	{
		CGLImage *lastImage = iImages.back();
		pos.setY (lastImage->GetPosition().y() + lastImage->GetSize().y());
	}
	pos+=iPosition;
	int width = iSize.x()-GetBorders().right-GetBorders().left-iScrollBar->size().width();
	QPointF size(width,width);
	CGLImage *newImage;
	if(fileName.isEmpty()){
		return;
	}
	try
	{
		newImage = new CGLImage(iParentWindow,fileName,pos,size);
	}
	catch(TextureNotCreatedException &e)
	{
		QMessageBox msgBox(QMessageBox::Warning, tr("QMessageBox::warning()"),
			QString("Could not allocate texture - TextureNotCreatedException"), 0, MainWindow::iSelfS);
		msgBox.addButton(tr("&Continue"), QMessageBox::RejectRole);
		msgBox.exec();
		return;

	}
	catch(DicomFramesException &e)
	{
		QMessageBox msgBox(QMessageBox::Warning, tr("QMessageBox::warning()"),
			QString("Could not load the image - bad format"), 0, MainWindow::iSelfS);
		msgBox.addButton(tr("&Continue"), QMessageBox::RejectRole);
		msgBox.exec();
		IELog.write("End: CGLImageExplorer::OpenImage");
		return;
	}


	newImage->SetMoveability (false);
	newImage->SetResizeability (false);
	newImage->SetTextDisplay(EDisplayTextAll, false);

	iImages.append (newImage);
	SelectImage(newImage);
	GLenum err = glGetErr();
	if(CGLWidget::GetInstance())
	{
		CGLWidget::GetInstance()->updateGL();
	}
	err = glGetErr();
	if(CInfoPanel::GetInstance())
	{
		CInfoPanel::GetInstance()->SetImageExplorerInfoView();
	}
	err = glGetErr();
	//CGLImage *im = new CGLImage(iParentWindow,fileName,pos,size);
	IELog.write("End: CGLImageExplorer::OpenImage");
}





void CGLImageExplorer::DrawIcons()
{
	/*glEnable(GL_BLEND);
	float opaq = 0.5;
	glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
	// nahrat jednotkovou matici
	Translate();//glTranslatef(0.0f, 0.0f, -50.0f);  
	glDisable( GL_TEXTURE_2D );

	//Move icon
	glColor4f(1.0, 1.0, 1.0,opaq); 
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,iParentWindow->GetMoveIconTexture ());
	glBegin(GL_QUADS);
	glTexCoord2d(0,0); 
	glVertex2d(iMoveIcon.position.x(),iSize.y()-iMoveIcon.position.y()-iMoveIcon.size.y());

	glTexCoord2d(1,0);
	glVertex2d(iMoveIcon.position.x()+iMoveIcon.size.x() ,iSize.y()-iMoveIcon.position.y()-iMoveIcon.size.y());

	glTexCoord2d(1,1);
	glVertex2d(iMoveIcon.position.x()+iMoveIcon.size.x(),iSize.y()-iMoveIcon.position.y());

	glTexCoord2d(0,1);
	glVertex2d(iMoveIcon.position.x(),iSize.y()-iMoveIcon.position.y());
	glEnd();

	//Resize icon
	glBindTexture(GL_TEXTURE_2D,iParentWindow->GetResizeIconTexture ());
	glBegin(GL_QUADS);
	glTexCoord2d(0,0); 
	glVertex2d(iResizeIcon.position.x(),iSize.y() - iResizeIcon.position.y() - iResizeIcon.size.y());

	glTexCoord2d(1,0);
	glVertex2d(iResizeIcon.position.x()+iResizeIcon.size.x() ,iSize.y() - iResizeIcon.position.y() - iResizeIcon.size.y());

	glTexCoord2d(1,1);
	glVertex2d(iResizeIcon.position.x()+iResizeIcon.size.x(),iSize.y() - iResizeIcon.position.y());

	glTexCoord2d(0,1);
	glVertex2d(iResizeIcon.position.x(),iSize.y() - iResizeIcon.position.y());
	glEnd();

	glDisable(GL_TEXTURE_2D);
	// glPopMatrix();

	//slider - framecount
	glColor4f(0.8, 0.0, 1.0,opaq); 
	glRectf(iFrameSlider.position.x(),iSize.y()-iFrameSlider.position.y()-iFrameSlider.size.y(),iFrameSlider.position.x()+iFrameSlider.size.x(),iSize.y()-iFrameSlider.position.y());
	//draw slider
	glColor4f(0.8, 0.8, 1.0,0.8); 
	float sliderlength = iFrameSlider.size.x();
	float sliderposition = sliderlength * iFrameSlider.data / (iTexture->GetDepth());
	float sliderhandlelenth = (float)sliderlength  / (iTexture->GetDepth());
	glRectf(iFrameSlider.position.x() + sliderposition,iSize.y()-iFrameSlider.position.y()-iFrameSlider.size.y(),iFrameSlider.position.x() + sliderposition+sliderhandlelenth,iSize.y()-iFrameSlider.position.y());

	glDisable (GL_BLEND);*/

}

void CGLImageExplorer::DrawTexts()
{
	IELog.write("Begin: CGLImageExplorer::DrawTexts");
	TColor color= Settings::GetColorConstant(EImageExplorerTextColor);
	glColor4f(color.r, color.g, color.b, color.a);
	int fontSize = 15;
	CGLWidget::GetInstance()->renderText  (5,iSize.y()-fontSize-3,
		QString("Image Explorer: "),fontSize);
	IELog.write("End: CGLImageExplorer::OpenImage");
}

void CGLImageExplorer::initializeGL()
{

}
void CGLImageExplorer::paintGL()
{
	IELog.write("Begin: CGLImageExplorer::paintGL");
	GLenum err = glGetErr();
	if(CGLWidget::GetInstance())
	{
		CGLWidget::GetInstance()->resetGLView();
	}
	Translate();
	DrawBorderRect();	
	DrawInnerRect();


	QListIterator<CGLImage*> images(iImages);
	int imageHeightSum = 0;

	while (images.hasNext())
	{
		CGLImage *im = images.next();
		imageHeightSum += im->GetSize().y();
		if(im->GetPosition().y()+im->GetSize().y()>0 
			&& im->GetPosition().y()<=iSize.y())
		{
			im->paintGL();
		}
	}

	if(iActiveImage)
	{
		iActiveImage->Translate();
		iActiveImage->DrawSelection();
	}
	iScrollBar->setVisible(true);
	if(imageHeightSum >iSize.y())
	{
		iScrollBar->setMaximum(imageHeightSum-iSize.y());
		iScrollBar->setPageStep(iSize.y());
	}
	else
	{
		iScrollBar->setMaximum(0);
	}
	if(CGLWidget::GetInstance())
	{
		CGLWidget::GetInstance()->resetGLView();
	}
	Translate();
	DrawTexts();
	IELog.write("End: CGLImageExplorer::paintGL");
	return;
}


bool CGLImageExplorer::IsOnMoveIcon(int x, int y)
{
	if(x<iMoveIcon.position.x())
		return false;
	if(y<iMoveIcon.position.y())
		return false;
	if(x>(iMoveIcon.position.x()+iMoveIcon.size.x()))
		return false;
	if(y>(iMoveIcon.position.y()+iMoveIcon.size.y()))
		return false;
	return true;
}

bool CGLImageExplorer::IsOnResizeIcon(int x, int y)
{

	if(x<iResizeIcon.position.x())
		return false;
	if(y<iResizeIcon.position.y())
		return false;
	if(x>(iResizeIcon.position.x()+iResizeIcon.size.x()))
		return false;
	if(y>(iResizeIcon.position.y()+iResizeIcon.size.y()))
		return false;
	return true;
}

void CGLImageExplorer::SetGeometry(int x, int y, int width, int height)
{
	IELog.write("Begin: CGLImageExplorer::SetGeometry");
	QPointF oldPos = iPosition;
	QPointF oldSize = iSize;
	CGLObject::SetGeometry(x,y,width,height);
	iScrollBar->setGeometry (
		iPosition.x()+iSize.x()-15-GetBorders().right,
		iPosition.y()+GetBorders().top,
		15,
		iPosition.y()+iSize.y()-GetBorders().top-GetBorders().bottom
		);
	QListIterator<CGLImage*> images(iImages);
	int posy;
	if(images.hasNext())
	{
		posy=images.next()->GetPosition().y();
	}
	images.toFront();
	while(images.hasNext())
	{
		CGLImage* im = images.next();
		int width = iSize.x()-GetBorders().right-GetBorders().left-iScrollBar->size().width();
		QPoint size(width,width);
		int posx=iPosition.x()+GetBorders().left;

		im->SetGeometry(posx,posy, size.x(),size.y());
		posy+=size.y();

	}
	IELog.write("End: CGLImageExplorer::SetGeometry");
}


void CGLImageExplorer::wheelEvent(QWheelEvent *event)
{
	/*float framm=(float)iActualTextureCoords.bottomLeft.GetZ() * (iTexture->GetDepth ());
	int dpt = (float)(iActualTextureCoords.bottomLeft.GetZ() * (iTexture->GetDepth ()+0.5f));


	if(event->delta() > 0 )
	{
	dpt++;
	}
	else
	{
	dpt--;
	}
	if(dpt>=iTexture->GetDepth ())
	{
	dpt=iTexture->GetDepth ()-1;
	}
	if(dpt<0)
	{
	dpt=0;
	}
	//TODO kouknout na framy srovnat s originalem (Nearest neigbour - aby to neinterpolovalo mezi slices)
	iFrameSlider.data =dpt;
	float z=(float)dpt/(iTexture->GetDepth ());
	//set z coords of texture
	iActualTextureCoords.bottomLeft.SetZ(z);
	iActualTextureCoords.bottomRight.SetZ(z);
	iActualTextureCoords.topLeft.SetZ(z);
	iActualTextureCoords.topRight.SetZ(z);
	PrepareActualTexture();
	iParentWindow->updateGL();
	*/
}
void CGLImageExplorer::mouseReleaseEvent(QMouseEvent *event)
{
	if(iActiveImage)
	{
		iActiveImage->mouseReleaseEvent(event);
	}
}
void CGLImageExplorer::mouseDoubleClickEvent ( QMouseEvent * event )
{

	if(MainWindow::iSelfS)
	{
		MainWindow::iSelfS->OpenFile();
	}
}

void CGLImageExplorer::mouseMoveEvent(QMouseEvent *event)
{
	if(iActiveImage)
	{
		iActiveImage->mouseMoveEvent(event);
	}
	/*int x=event->x() - iPosition.x();
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
	//update slice texture
	PrepareActualTexture();
	iParentWindow->updateGL ();
	return;
	}
	else if(iMouseState == EMouseStateFrameSliderChanging )
	{
	QCursor::setPos(event->globalPos().x(),iLockedGlobalMousePosition.y());
	IsOnFrameSliderIcon (x,y);

	return;
	}
	else if(iMouseState == EMouseStateImageMoving)
	{
	iImageCenter.setX(iImageCenter.x()+(float)dx/iSize.x());
	iImageCenter.setY(iImageCenter.y()+(float)dy/iSize.y());
	iParentWindow->updateGL ();
	//return;
	}
	else if(iMouseState == EMouseStateImageZooming)
	{
	QCursor::setPos(iLockedGlobalMousePosition);
	iZoomFactor*=(1.-(float)dy/100.);
	iImageCenter.setX((x-(x-iLockedImageCenter.x()*iSize.x())*(iZoomFactor/iLockedZoom))/iSize.x());
	iImageCenter.setY((y-(y-iLockedImageCenter.y()*iSize.y())*(iZoomFactor/iLockedZoom))/iSize.y());
	iParentWindow->updateGL ();
	return;
	}
	else if(iMouseState == EMouseStateObjectMoving)
	{
	SetGeometry(dx + iPosition.x(),dy + iPosition.y(),iSize.x(),iSize.y());
	iParentWindow->updateGL ();
	return; //to avoid updating of previous mouse position
	}
	else if(iMouseState == EMouseStateObjectResizing)
	{
	SetGeometry(iPosition.x(),iPosition.y(),iSize.x()+dx,iSize.y()+dy);
	iParentWindow->updateGL ();

	}
	iPreviousMousePosition = QPoint(x, y);	
	//iPreviousGlobalMousePosition = event->globalPos();
	*/
}

void CGLImageExplorer::mousePressEvent(QMouseEvent *event)
{
	iActiveImage = NULL;
	QListIterator<CGLImage*> images(iImages);
	images.toBack ();
	while (images.hasPrevious())
	{
		CGLImage* obj = images.previous();
		if(obj->IsPointOnObject(event->x(),event->y()))
		{
			//iActiveImage = obj;
			//iImages.move(iImages.indexOf(obj),iImages.count()-1);
			obj->mousePressEvent(event);
			SelectImage(obj);
			//updateGL ();
			break;
		}
	}
	if(CInfoPanel::GetInstance())
	{

		CInfoPanel::GetInstance()->SetImageExplorerInfoView();
	}

}
