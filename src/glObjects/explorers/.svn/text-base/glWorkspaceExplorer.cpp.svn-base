#include <GL/glew.h>
#include <QtOpenGL/QtOpenGL>

#include <glObjects/explorers/glWorkspaceExplorer.h>
#include <glObjects/glWidget.h>
#include <glObjects/glImage.h>
#include <infoPanel.h>
#include <glObjects/workspaces/workspaceManager.h>
#include <glObjects/workspaces/glWorkspaceSnapshot.h>
#include <glObjects/workspaces/glPlanarWorkspace.h>

#include "dpDebug/dpDebug.h"
debug::Log WSLog;

CGLWorkspaceExplorer* CGLWorkspaceExplorer::instance = NULL;
const int KScrollbarHeight = 15;
CGLWorkspaceExplorer::~CGLWorkspaceExplorer()
{
	instance = NULL;
	if(iScrollBar)
		delete iScrollBar;
	/*QListIterator<CGLImage*> images(iImages);
	images.toBack ();
	while (images.hasPrevious())
	{
	CGLImage* obj = images.previous();
	if(obj)
	{
	delete obj;
	obj = NULL;
	}
	}*/
}

void CGLWorkspaceExplorer::Scrolled(int value)
{
	if(!CGLWidget::GetInstance())
		return;
	QListIterator<CGLWorkspace*> workspaces(CWorkspaceManager::GetInstance()->GetWorkspaces());

	workspaces.toFront ();
	int relativeX=0;
	while (workspaces.hasNext())
	{
		CGLWorkspace* obj = workspaces.next();
		if(obj)
		{
			obj->GetSnapshot().SetPosition(QPoint(relativeX-value,obj->GetSnapshot().GetPosition().y()));
			relativeX+=obj->GetSnapshot().GetSize().x();
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
CGLWorkspaceExplorer* CGLWorkspaceExplorer::InitInstance(CGLWidget *parent, QPointF& position, QPointF &size )
{
	if(!instance)
	{
		instance = new CGLWorkspaceExplorer(parent, position, size);
	}
	return instance;
}
QPoint CGLWorkspaceExplorer::GetDefaultWorkspaceSnapshotPos()
{
	return QPoint(GetPosition().x()+GetBorders().left,GetPosition().y()+GetBorders().top);

}
QPoint CGLWorkspaceExplorer::GetDefaultWorkspaceSnapshotSize()
{
	if(!CGLWidget::GetInstance())
		return QPoint(0,0);
	int height=GetSize().y()-iScrollBar->height()-+GetBorders().bottom-+GetBorders().top;
	int width=height*CGLWidget::GetInstance()->GetDefaultWorkspaceSize().x()/CGLWidget::GetInstance()->GetDefaultWorkspaceSize().y();
	return QPoint(width,height);
}
/*
get singleton instance
*/
CGLWorkspaceExplorer* CGLWorkspaceExplorer::GetInstance()
{
	return instance;
}

CGLWorkspaceExplorer::CGLWorkspaceExplorer(CGLWidget *parentWindow, QPointF& position, QPointF &size ):CGLObject (parentWindow, position, size)
{	
	WSLog.write("Begin: CGLWorkspaceExplorer::CGLWorkspaceExplorer");
	iScrollBar = new QScrollBar(Qt::Horizontal,parentWindow);

	iScrollBar->setMinimum(0);
	iScrollBar->setMaximum(0);
	iScrollBar->setValue(0);
	iScrollBar->setGeometry (iPosition.x()+GetBorders().left,
		iPosition.y()+iSize.y()-KScrollbarHeight-GetBorders().bottom,
		iSize.x()-GetBorders().left-GetBorders().right,
		KScrollbarHeight);
	QObject::connect(iScrollBar , SIGNAL(valueChanged(int)), 
		this, SLOT(Scrolled(int))); 
	//Init(parentWindow, position,size);
	//iFrames= new CDicomFrames(file->toAscii().data());
	//texture= LoadTextureFromDicom (*loader);
	SetBorders(Settings::GetBordersConstant(EWorkspaceExplorerBorders));
	SetBorderColor(Settings::GetColorConstant(EWorkspaceExplorerBorderColor));
	SetInnerColor(Settings::GetColorConstant(EWorkspaceExplorerInnerColor));
	WSLog.write("End: CGLWorkspaceExplorer::CGLWorkspaceExplorer");
}

void CGLWorkspaceExplorer::SelectWorkspace(CGLWorkspace *workspace)
{
	if(!CGLWidget::GetInstance())
		return;
	//CInfoPanel::GetInstance()->SetSource(image);
	if(CWorkspaceManager::GetInstance()->GetActiveWorkspace())
	{
		CWorkspaceManager::GetInstance()->GetActiveWorkspace()->StopAnimations();
	}
	CWorkspaceManager::GetInstance()->SetActiveWorkspace(workspace);
	if(workspace)
	{
		workspace->StartAnimations();	
	}
	if(CInfoPanel::GetInstance())
		CInfoPanel::GetInstance()->SetWorkspaceExplorerInfoView();
}

void CGLWorkspaceExplorer::SelectPlanarWorkspace()
{
	if(!CGLWidget::GetInstance())
		return;
	if(!CWorkspaceManager::GetInstance()->PlanarWorkspaceExists())
		return;
	SelectWorkspace(NULL);
	CGLWidget::GetInstance()->updateGL();
}









void CGLWorkspaceExplorer::DrawSelection()
{
	WSLog.write("Begin: CGLWorkspaceExplorer::DrawSelection");
	if(!CWorkspaceManager::GetInstance())
		return;
	if(!CWorkspaceManager::GetInstance()->GetActiveWorkspace())
		return;
	// nahrat jednotkovou matici
	CWorkspaceManager::GetInstance()->GetActiveWorkspace()->GetSnapshot().Translate();
	CWorkspaceManager::GetInstance()->GetActiveWorkspace()->GetSnapshot().DrawSelection();
	WSLog.write("End: CGLWorkspaceExplorer::DrawSelection");
}
void CGLWorkspaceExplorer::DrawIcons()
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

void CGLWorkspaceExplorer::DrawTexts()
{
	TColor color= Settings::GetColorConstant(EWorkspaceExplorerTextColor);
	glColor4f(color.r, color.g, color.b, color.a);
	int fontSize =15;
		CGLWidget::GetInstance()->renderText  (5,iSize.y()-fontSize-3,
			QString("Workspace Explorer: "),fontSize);
		glColor4f(1,1,1,1);
}

void CGLWorkspaceExplorer::initializeGL()
{

}
void CGLWorkspaceExplorer::paintGL()
{
	WSLog.write("Begin: CGLWorkspaceExplorer::paintGL");
	if(!CGLWidget::GetInstance())
		return;
	CGLWidget::GetInstance()->resetGLView();
	Translate();
	DrawBorderRect();	
	DrawInnerRect();

	int snapWidthSum = 0;
	glDisable(GL_BLEND);
	CGLWidget::GetInstance()->resetGLView();
	QListIterator<CGLWorkspace*> workspaces(CWorkspaceManager::GetInstance()->GetWorkspaces());
	while (workspaces.hasNext())
	{
		CGLWorkspaceSnapshot &snap = workspaces.next()->GetSnapshot();
		snapWidthSum += snap.GetSize().x();
		if(snap.GetPosition().x()+snap.GetSize().x()>0 
			&& snap.GetPosition().x()<=iSize.x())
		{
			snap.paintGL();
		}
	}
	CGLWidget::GetInstance()->resetGLView();
	DrawSelection();
	
	iScrollBar->setVisible(true);
	if(snapWidthSum >iSize.x())
	{
		iScrollBar->setMaximum(snapWidthSum-iSize.x());
		iScrollBar->setPageStep(iSize.x());
	}
	else
	{
		iScrollBar->setMaximum(0);
	}

	CGLWidget::GetInstance()->resetGLView();
	Translate();
	DrawTexts();
	WSLog.write("End: CGLWorkspaceExplorer::paintGL");
	return;
}


bool CGLWorkspaceExplorer::IsOnMoveIcon(int x, int y)
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

bool CGLWorkspaceExplorer::IsOnResizeIcon(int x, int y)
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

void CGLWorkspaceExplorer::SetGeometry(int x, int y, int width, int height)
{
	WSLog.write("Begin: CGLWorkspaceExplorer::SetGeometry");
	if(!CWorkspaceManager::GetInstance()){
		WSLog.write("End: CGLWorkspaceExplorer::SetGeometry");
		return;
	}
	QPointF oldPos = iPosition;
	QPointF oldSize = iSize;
	CGLObject::SetGeometry(x,y,width,height);
	iScrollBar->setGeometry (iPosition.x()+GetBorders().left,
		iPosition.y()+iSize.y()-KScrollbarHeight-GetBorders().bottom,
		iSize.x()-GetBorders().left-GetBorders().right,
		KScrollbarHeight);

	QListIterator<CGLWorkspace*> workspaces((CWorkspaceManager::GetInstance()->GetWorkspaces()));
	int posx;
	if(workspaces.hasNext())
	{
		posx=workspaces.next()->GetSnapshot().GetPosition().x();
	}
	workspaces.toFront();
	while(workspaces.hasNext())
	{
		CGLWorkspace* ws = workspaces.next();

		QPoint size=GetDefaultWorkspaceSnapshotSize();
		QPoint pos = GetDefaultWorkspaceSnapshotPos();

		ws->GetSnapshot().SetGeometry(posx,pos.y(), size.x(),size.y());
		posx+=size.x();

	}
	WSLog.write("End: CGLWorkspaceExplorer::SetGeometry");
}

void CGLWorkspaceExplorer::wheelEvent(QWheelEvent *event)
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
void CGLWorkspaceExplorer::mouseReleaseEvent(QMouseEvent *event)
{
	if(!CWorkspaceManager::GetInstance())
		return;
	if(CWorkspaceManager::GetInstance()->GetActiveWorkspace()){
		CWorkspaceManager::GetInstance()->GetActiveWorkspace()->GetSnapshot().mouseReleaseEvent(event);
	}
}
void CGLWorkspaceExplorer::AddNewWorkspace()
{
	WSLog.write("Begin: CGLWorkspaceExplorer::AddNewWorkspace");
	if(!CGLWidget::GetInstance())
		return;
	CGLWidget* glwidget =CGLWidget::GetInstance();
	try
	{
		CGLWorkspace *newWorkspace = new CGLWorkspace(glwidget,glwidget->GetDefaultWorkspacePosition(),glwidget->GetDefaultWorkspaceSize());
		CWorkspaceManager::GetInstance()->AddWorkspace(newWorkspace);
		SelectWorkspace(newWorkspace);
	}
	catch (TextureNotCreatedException &e)
	{
		//workspace not created
	}
	//ownership transferred
	WSLog.write("End: CGLWorkspaceExplorer::AddNewWorkspace");
}
void CGLWorkspaceExplorer::CreateNewPlanarWorkspace()
{
	if(!CGLWidget::GetInstance())
		return;
	CGLWidget* glwidget = CGLWidget::GetInstance();
	try
	{
		CGLPlanarWorkspace *newWorkspace = new CGLPlanarWorkspace(glwidget,glwidget->GetDefaultWorkspacePosition(),glwidget->GetDefaultWorkspaceSize());
		CWorkspaceManager::GetInstance()->CreatePlanarWorkspace(newWorkspace);
		//SelectWorkspace(NULL);
		SelectPlanarWorkspace();
	}
	catch (TextureNotCreatedException &e)
	{
		//workspace not created
	}
}
void CGLWorkspaceExplorer::mouseDoubleClickEvent(QMouseEvent *event)
{
	AddNewWorkspace();


}
void CGLWorkspaceExplorer::mouseMoveEvent(QMouseEvent *event)
{
	if(!CWorkspaceManager::GetInstance())
		return;
	if(CWorkspaceManager::GetInstance()->GetActiveWorkspace())
	{
		CWorkspaceManager::GetInstance()->GetActiveWorkspace()->GetSnapshot().mouseMoveEvent(event);
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
void CGLWorkspaceExplorer::mousePressEvent(QMouseEvent *event)
{
	//iActiveWorkspace = NULL;
	if(!CWorkspaceManager::GetInstance())
		return;
	if(!CInfoPanel::GetInstance())
		return;
	CInfoPanel::GetInstance()->SetWorkspaceExplorerInfoView();
	QListIterator<CGLWorkspace*> workspaces(CWorkspaceManager::GetInstance()->GetWorkspaces());
	workspaces.toBack ();
	while (workspaces.hasPrevious())
	{
		CGLWorkspace* obj = workspaces.previous();
		if(obj->GetSnapshot().IsPointOnObject(event->x(),event->y()))
		{
			//	iActiveWorkspace = obj;
			obj->GetSnapshot().mousePressEvent(event);
			return;
		}
	}
	
}
