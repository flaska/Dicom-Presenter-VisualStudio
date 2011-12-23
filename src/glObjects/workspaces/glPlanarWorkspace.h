#ifndef GLPLANARWORKSPACE_H_AWDS
#define GLPLANARWORKSPACE_H_AWDS
#include <GL/glew.h>
#include <glObjects/glObject.h>
#include <glObjects/glImage.h>
#include <glObjects/workspaces/layouts/workspaceLayout.h>
#include <QtCore/QString>
#include <QtCore/QList>

class CGLWorkspaceSnapshot;

class CGLPlanarWorkspace:public QWidget, public CGLObject
{
	Q_OBJECT
public:
	void SetGeometry(float x, float y, float width, float height);
	void SetName( const QString &name);
	void UpdateTexture();
	QString& GetName();
	CGLPlanarWorkspace(CGLObject *parent, const QPointF &position, const QPointF &size);
	~CGLPlanarWorkspace();
	void initializeGL();
	void paintGL();
	void DrawToTexture();
	void DrawFromTexture();
	void SaveSnapshot(QString& fileName);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);	
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void SelectImage(CGLImage *image);

	QList<CGLImage*>& GetImages();
	MWorkspaceLayout &GetLayout();
	void SetLayout(TLayoutType type);
	CGLWorkspaceSnapshot& GetSnapshot();

	int GetSaveSnapshotWidth();
	int GetSaveSnapshotHeight();
	void SetSaveSnapshotWidth(int width);
	void SetSaveSnapshotHeight(int height);
	void UseImage(CGLImage *image);
private:
	float iLastInnerHeight;
	float iLastInnerWidth;

	/**
	* @Save the texture  to the file
	* is performed when the actual texture is drawn
	*/
	void SaveTexture(QString& fileName);
	bool iSaveSnapshot;
	QString* iSaveSnapshotFileName;
	int iSaveSnapshotHeight;//\current set height, which will be used as height of the image when saving snapshot
	int iSaveSnapshotWidth;//\current set width, which will be used as width of the image when saving snapshot
	bool iWorkspaceChanged ;
	//GLuint iFBO;
	
    QGLFramebufferObject *iActualTextureFBO;
//	GLuint iActualTexture;
	TSliceInfo iActualTextureInfo;
	QString iIdentification;
	MWorkspaceLayout *iLayout;
	CGLImage *iImage;

	CGLWorkspaceSnapshot *iWorkspaceSnapshot;
	QString *iName;

	TPlanarCrossPosition iPlanarCrossPosition;
	void DrawBorders();
	QPoint *iCursorHistory;
	QPoint *iEventHistory;
	char UserManipulatingSlice;
	int iSensitivity;
};
#endif
