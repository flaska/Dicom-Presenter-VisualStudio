#include <GL/glew.h>
#include <glObjects/glObject.h>
#include <glObjects/glWidget.h>

CGLObject::~CGLObject()
{
}

void CGLObject::SetGeometry(float x, float y, float width, float height)
{

	iPosition.setX(x);
	iPosition.setY(y);
	iSize.setX (width);
	iSize.setY (height);
	resizeGL();

}
CGLObject::CGLObject(CGLObject *parentWindow, const QPointF& position, const QPointF &size ):iParentWindow(parentWindow ),iPosition(position ),iSize(size)
{	
	iBorders.left=0;
	iBorders.top=0;
	iBorders.right=0;
	iBorders.bottom=0;
	
	iMoveIcon.position.setX(0);
	iMoveIcon.position.setY(0);
	iMoveIcon.size.setX(20);
	iMoveIcon.size.setY(20);

	iResizeIcon.size.setX(20);
	iResizeIcon.size.setY(20);
	iResizeIcon.position.setX(iSize.x()-iResizeIcon.size.x());
	iResizeIcon.position.setY(iSize.y()-iResizeIcon.size.y());

	iCloseIcon.size.setX(20);
	iCloseIcon.size.setY(20);
	iCloseIcon.position.setX(iSize.x()-iCloseIcon.size.x());
	iCloseIcon.position.setY(0);
}

CGLObject::CGLObject()
{	
	iParentWindow = NULL;
}

void CGLObject::resizeGL()
{
	iMoveIcon.position.setX(GetBorders().left);
	iMoveIcon.position.setY(GetBorders().top);
	iMoveIcon.size.setX(20);
	iMoveIcon.size.setY(20);

	iResizeIcon.size.setX(20);
	iResizeIcon.size.setY(20);
	iResizeIcon.position.setX(iSize.x()-iResizeIcon.size.x()-GetBorders().right);
	iResizeIcon.position.setY(iSize.y()-iResizeIcon.size.y()-GetBorders().bottom);

	iCloseIcon.size.setX(20);
	iCloseIcon.size.setY(20);
	iCloseIcon.position.setX(iSize.x()-iCloseIcon.size.x()-GetBorders().right);
	iCloseIcon.position.setY(0+GetBorders().bottom);
}

void CGLObject::DrawInnerRect()
{

	glColor4f(iInnerColor.r,iInnerColor.g,iInnerColor.b,iInnerColor.a);
	const QRect& r =GetInnerRect();
	int rheight=r.height();
	int rtop=r.top();
	int bottom = iSize.y()-r.height()-r.top();
	int right = r.left()+r.width();
	int top = iSize.y()-r.top();
	glRectf(r.left(),
		bottom,
		right,
		top);
}
void CGLObject::SetInnerColor(TColor color)
{
	iInnerColor=color;
}
void CGLObject::SetInnerColor(float r, float g, float b, float alpha)
{
	iInnerColor.r=r;
	iInnerColor.g=g;
	iInnerColor.b=b;
	iInnerColor.a=alpha;
}
TColor CGLObject::GetInnerColor()
{
	return iInnerColor;
}



void CGLObject::DrawIcons()
{
	glEnable(GL_BLEND);
	float opaq = 0.5;
	glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
	// nahrat jednotkovou matici
	glDisable( GL_TEXTURE_2D );

	//Move icon
	if(iMoveAbility  )
	{
		glColor4f(1.0, 1.0, 1.0,opaq); 
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,CGLWidget::GetInstance()->GetMoveIconTexture ());
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
		glDisable(GL_TEXTURE_2D);
	}
	//Resize icon
	if(iResizeAbility )
	{
		glColor4f(1.0, 1.0, 1.0,opaq); 
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,CGLWidget::GetInstance()->GetResizeIconTexture ());
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
	}
	//Close icon
	if(iCloseAbility )
	{
		glColor4f(1.0, 1.0, 1.0,opaq); 
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,CGLWidget::GetInstance()->GetCloseIconTexture ());
		glBegin(GL_QUADS);
		glTexCoord2d(0,0); 
		glVertex2d(iCloseIcon.position.x(),iSize.y() - iCloseIcon.position.y() - iCloseIcon.size.y());

		glTexCoord2d(1,0);
		glVertex2d(iCloseIcon.position.x()+iCloseIcon.size.x() ,iSize.y() - iCloseIcon.position.y() - iCloseIcon.size.y());

		glTexCoord2d(1,1);
		glVertex2d(iCloseIcon.position.x()+iCloseIcon.size.x(),iSize.y() - iCloseIcon.position.y());

		glTexCoord2d(0,1);
		glVertex2d(iCloseIcon.position.x(),iSize.y() - iCloseIcon.position.y());
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}
	// glPopMatrix();

	
	glDisable (GL_BLEND);

}
void CGLObject::SetSize(const QPointF &size)
{
	//iSize=size;
	SetGeometry(iPosition.x(),iPosition.y(),
		size.x(),size.y());
}
void CGLObject::SetPosition(const QPointF &position)
{
	//iPosition=position;
	SetGeometry(position.x(),position.y(),
		iSize.x(),iSize.y());
}
bool CGLObject::IsOnCloseIcon(int x, int y)
{

	if(!iCloseAbility  )return false;

	if(x<iCloseIcon.position.x())
		return false;
	if(y<iCloseIcon.position.y())
		return false;
	if(x>(iCloseIcon.position.x()+iCloseIcon.size.x()))
		return false;
	if(y>(iCloseIcon.position.y()+iCloseIcon.size.y()))
		return false;
	return true;
}

bool CGLObject::IsOnResizeIcon(int x, int y)
{

	if(!iResizeAbility  )return false;

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

void CGLObject::SetMoveability(bool on)
{
	iMoveAbility = on;
}
void CGLObject::SetClosability(bool on){
	iCloseAbility = on;
}
void CGLObject::SetResizeability(bool on)
{
	iResizeAbility = on;
}
bool CGLObject::IsOnMoveIcon(int x, int y)
{
	if(!iMoveAbility )return false;

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

void CGLObject::DrawSelection()
{
	glDisable(GL_BLEND);
	glColor4f(0.5,1,1,1);
	glRectf(0,0,
		GetBorders().left,
		GetSize().y());
	glRectf(0,0,
		GetSize().x(),
		GetBorders().bottom);

	glRectf(GetSize().x()-GetBorders().right,
		0,
		GetSize().x(),
		GetSize().y());
	
	glRectf(0,
		GetSize().y()-GetBorders().top,
		GetSize().x(),
		GetSize().y());

	glColor4f(0.9,0.9,0.9,1);
	glRectf(0,0,
		1,
		GetSize().y());
	glRectf(0,0,
		GetSize().x(),
		1);

	glRectf(GetSize().x()-1,
		0,
		GetSize().x(),
		GetSize().y());
	
	glRectf(0,
		GetSize().y()-1,
		GetSize().x(),
		GetSize().y());
	glEnable(GL_BLEND);
	/*
	glEnable(GL_BLEND);
	glColor4f(0.5,1,1,0.1);
	glRectf(GetBorders().left,GetBorders().bottom
		,
		GetSize().x()-GetBorders().right,
		GetSize().y()-GetBorders().top);
	glDisable(GL_BLEND);
	*/
}
void CGLObject::DrawBorderRect()
{
	// nahrat jednotkovou matici

	glColor4f(iBorderColor.r,iBorderColor.g,iBorderColor.b,iBorderColor.a);
	glRectf(0,0,iSize.x(),iSize.y());
	/*	glBegin(GL_LINES);
	glVertex2i(0,0);
	glVertex2i(iSize.x(),0);
	glVertex2i(0,1);
	glVertex2i(iSize.x(),1);


	glVertex2i(iSize.x(),0);
	glVertex2i(iSize.x(),iSize.y()-1);
	glVertex2i(iSize.x()-1,0);
	glVertex2i(iSize.x()-1,iSize.y()-1);

	glVertex2i(iSize.x(),iSize.y()-1);
	glVertex2i(0,iSize.y()-1);
	glVertex2i(iSize.x(),iSize.y()-2);
	glVertex2i(0,iSize.y()-2);

	glVertex2i(0,iSize.y()-1);
	glVertex2i(0,0);
	glVertex2i(1,iSize.y()-1);
	glVertex2i(1,0);
	glEnd();
	*/
}

//set matrix to start position of object
void CGLObject::Translate()
{
	if(iParentWindow==NULL)
		return;
	glMatrixMode(GL_MODELVIEW);                 // bude se menit modelova matice
	glLoadIdentity();   
	//glTranslatef(0,iSize.y(),-50.0f);
	//iParentWindow->height();
	int posy=CGLWidget::GetInstance()->height();
	glTranslatef(iPosition.x(),posy-iPosition.y()-iSize.y(), -50.0f); 
}


void CGLObject::SetBorderColor(TColor color)
{
	iBorderColor=color;

}
void CGLObject::SetBorderColor(float r, float g, float b, float alpha)
{
	iBorderColor.r=r;
	iBorderColor.g=g;
	iBorderColor.b=b;
	iBorderColor.a=alpha;
}

void CGLObject::SetBorders(TBorders border)
{
	iBorders.left = border.left;
	iBorders.top = border.top;
	iBorders.right = border.right;
	iBorders.bottom = border.bottom;
}

const QPointF &CGLObject::GetPosition()
{
	return iPosition;
}
const QPointF &CGLObject::GetSize()
{

	return iSize;
}

const QRect CGLObject::GetInnerRect()
{
	return QRect(
		iBorders.left,
		iBorders.top,
		iSize.x()-iBorders.right-iBorders.left,
		iSize.y()-iBorders.bottom-iBorders.top
		);
}
const int CGLObject::height()
{
	return iSize.y();
}
const int CGLObject::width()
{
	return iSize.x();
}

bool CGLObject::IsPointOnObject(int x, int y)
{
	if(x<iPosition.x())
		return false;
	if(y<iPosition.y())
		return false;
	if(x>(iPosition.x()+iSize.x()))
		return false;
	if(y>(iPosition.y()+iSize.y()))
		return false;
	return true;

}


