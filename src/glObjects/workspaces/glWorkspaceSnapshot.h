#ifndef CWORKSPACESNAPSHOT_H_sdaczx
#define CWORKSPACESNAPSHOT_H_sdaczx
#include <GL/glew.h>
#include <glObjects/glImage.h>
#include <glObjects/workspaces/glWorkspace.h>

class CGLWorkspace;
class CGLWorkspaceSnapshot:public QWidget, public CGLObject
{
	Q_OBJECT
public:
	CGLWorkspaceSnapshot(CGLObject *parentWindow,CGLWorkspace* workspace, const QPointF &pos, const QPointF &size );
	~CGLWorkspaceSnapshot();
	void SetWorkspace(CGLWorkspace *workspace);
	CGLWorkspace& GetWorkspace();
	void initializeGL();
	void paintGL();
	void mouseMoveEvent(QMouseEvent *event);
	void  CloseMe();
	void wheelEvent(QWheelEvent *event);	
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *event);
private:
	CGLWorkspace *iWorkspace;
};
#endif
