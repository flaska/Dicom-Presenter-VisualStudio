#ifndef GLIMAGEEXPLORER_H_AWDS
#define GLIMAGEEXPLORER_H_AWDS
#include <Qt/qobject.h>
#include <GL/glew.h>
#include <QtCore/QString>
#include <QtCore/QPoint>
#include <QtGui/QScrollBar>
#include <dicom/dicomSerieData.h>
#include <3dTexture/dicom3DTexture.h>
#include <glObjects/glObject.h>



//forward
class CGLWidget;
class CGLImage;


class CGLWidget;
class CGLImageExplorer: public QObject, public CGLObject
{
	Q_OBJECT

private:
	CGLImageExplorer(CGLWidget *parentWindow, QPointF &position, QPointF &size);
public:
	virtual ~CGLImageExplorer();
	static CGLImageExplorer* GetInstance();
	static CGLImageExplorer* InitInstance(CGLWidget *parent, QPointF& position, QPointF &size );
	void OpenImage(QString &fileName);
	void CloseImage(CGLImage *image);
	void SelectImage(CGLImage *image);
	void initializeGL();

	void DrawIcons();
	void DrawTexts();
	void paintGL();
	void SetGeometry(int x, int y, int width, int height);
	void mouseMoveEvent(QMouseEvent *event);
	bool IsOnMoveIcon(int x, int y);;
	bool IsOnResizeIcon(int x, int y);;
	void wheelEvent(QWheelEvent *event);	
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
		
	void mouseReleaseEvent(QMouseEvent *event);
CGLImage* GetSelectedImage();
	public slots:
	void Scrolled(int value);
private:
	static CGLImageExplorer* instance;
	QList<CGLImage*> iImages; // opened images
	CGLImage *iActiveImage; //object with focus
	QScrollBar *iScrollBar;
	//
	QPoint iPreviousMousePosition;
	QPoint iLockedGlobalMousePosition;
	
	TMouseState iMouseState;
	TGLIcon iMoveIcon;
	TGLIcon iResizeIcon;
	//bool iSkipMouseMovement;
};
#endif
