#ifndef GLWORKSPACEEXPLORER_H_AWDS
#define GLWORKSPACEEXPLORER_H_AWDS
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

class CGLWorkspace;

class CGLWidget;
class CGLWorkspaceExplorer:public QObject, public CGLObject
{
	Q_OBJECT

private:
	
	CGLWorkspaceExplorer(CGLWidget *parentWindow, QPointF &position, QPointF &size);
public:
	void AddNewWorkspace();
	void CreateNewPlanarWorkspace();
	virtual ~CGLWorkspaceExplorer();
	static CGLWorkspaceExplorer* GetInstance();
	static CGLWorkspaceExplorer* InitInstance(CGLWidget *parent, QPointF& position, QPointF &size );
	void SelectWorkspace(CGLWorkspace *workspace);
	void SelectPlanarWorkspace();
	QPoint GetDefaultWorkspaceSnapshotPos();
		QPoint GetDefaultWorkspaceSnapshotSize();
	void initializeGL();
	
	void DrawSelection();
	void DrawIcons();
	void DrawTexts();
	void paintGL();
	void SetGeometry(int x, int y, int width, int height);
	void mouseMoveEvent(QMouseEvent *event);
	bool IsOnMoveIcon(int x, int y);;
	bool IsOnResizeIcon(int x, int y);;
	void wheelEvent(QWheelEvent *event);	
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	public slots:
		void Scrolled(int value);
private:
	static CGLWorkspaceExplorer* instance;
	//QList<CGLImage*> iImages; // opened images
	//CGLWorkspace *iActiveWorkspace; //object with focus
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
