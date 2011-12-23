#include <glObjects/workspaces/glWorkspaceSnapshot.h>
#include <glObjects/glWidget.h>
#include <glObjects/workspaces/workspaceManager.h>
#include <settings.h>

//workspace snapshot
CGLWorkspaceSnapshot::CGLWorkspaceSnapshot(CGLObject *parentWindow,CGLWorkspace *workspace, const QPointF &pos, const QPointF &size):CGLObject(parentWindow,pos,size)
{
	iWorkspace=workspace;
	SetBorderColor(Settings::GetColorConstant(EWorkspaceSnapshotBorderColor));
	SetInnerColor(Settings::GetColorConstant(EWorkspaceSnapshotInnerColor));
	SetBorders(Settings::GetBordersConstant(EWorkspaceSnapshotBorders));
	SetMoveability(false);
	SetResizeability(false);
	SetClosability(true);
}
CGLWorkspaceSnapshot::~CGLWorkspaceSnapshot()
{
}
void CGLWorkspaceSnapshot::SetWorkspace(CGLWorkspace *workspace)
{
	iWorkspace=workspace;
}
CGLWorkspace& CGLWorkspaceSnapshot::GetWorkspace()
{
	return *iWorkspace;
}
void CGLWorkspaceSnapshot::initializeGL()
{
}

void CGLWorkspaceSnapshot::paintGL()
{
	CGLWidget::GetInstance()->resetGLView();
	Translate();
	DrawBorderRect();
	DrawInnerRect();

	int ypos= CGLWidget::GetInstance()->GetSize().y()-iSize.y()-iPosition.y()+GetBorders().bottom;
	glColor4f(1,1,1,1);
	glViewport(iPosition.x()+GetBorders().left,
		ypos ,
		iSize.x()-GetBorders().left-GetBorders().right
		, iSize.y()-GetBorders().top-GetBorders().bottom);					// Reset The Current Viewport

	//glViewport(iPosition.x(),ypos ,iSize.x(), iSize.y());					// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();							// Reset The Projection Matrix
	glOrtho(0, 1 , 0,1, 0.1, 90); 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();				
	glTranslatef(0,0,-50);
	iWorkspace->DrawFromTexture();
	CGLWidget::GetInstance()->resetGLView();
	Translate();
	DrawIcons();

}
void CGLWorkspaceSnapshot::mouseMoveEvent(QMouseEvent *event)
{
}
void CGLWorkspaceSnapshot::wheelEvent(QWheelEvent *event)
{
}
void  CGLWorkspaceSnapshot::CloseMe()
{
	if(iWorkspace)
	{
	CWorkspaceManager::GetInstance()->RemoveWorkspace(iWorkspace);
	}
}
void CGLWorkspaceSnapshot::mousePressEvent(QMouseEvent *event)
{
	int x=event->x() - iPosition.x();
	int y = event->y()- iPosition.y();
	if(IsOnCloseIcon(x, y))
	{
		CloseMe();
		return;

	}
	CGLWorkspaceExplorer::GetInstance()->SelectWorkspace(&this->GetWorkspace());
//	SelectWorkspace(obj);
}
void CGLWorkspaceSnapshot::mouseReleaseEvent(QMouseEvent *e)
{
}
void CGLWorkspaceSnapshot::mouseDoubleClickEvent(QMouseEvent *event)
{
}
