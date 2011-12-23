#ifndef GLWORKSPACE_H_AWDS
#define GLWORKSPACE_H_AWDS
#include <GL/glew.h>
#include <glObjects/glObject.h>
#include <glObjects/glImage.h>
#include <glObjects/workspaces/layouts/workspaceLayout.h>
#include <QtCore/QString>
#include <QtCore/QList>
class CGLWorkspaceSnapshot;
class TextureNotCreatedException: public exception
{
public:
	virtual const char* what() const throw()
	{
		return "The texture could not be created. video memory is probably full";
	};
};
/**
* @class CGLObject
* @brief Abstract class for object that can be drawn onto glWidget
*/
class CGLWorkspace:public QWidget, public CGLObject
{
	Q_OBJECT
public:
	void SetGeometry(float x, float y, float width, float height);
	void SetName( const QString &name);
	void StartAnimations();
	void StopAnimations();
	void UpdateTexture();
	CGLImage* GetActiveImage();
	QString& GetName();
	CGLWorkspace(CGLObject *parentWindow, const QPointF &position, const QPointF &size, TLayoutType layoutType=ELayoutGrowingGridHorizontal);
	~CGLWorkspace();
	void initializeGL();
	void paintGL();
	bool InitTexture();
	void DrawToTexture();
	void DrawFromTexture();
	void SaveSnapshot(QString& fileName);
	//void SetIdentification(QString &identification);
	//QString &GetIdentification();
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);	
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void AddImage(CGLImage *image);
	void RemoveImage(CGLImage *image);
	void SelectImage(CGLImage *image);

	QList<CGLImage*>& GetImages();
	MWorkspaceLayout &GetLayout();
	void SetLayout(TLayoutType type);
	CGLWorkspaceSnapshot& GetSnapshot();

	/**
	* @brief Get the current width, which will be used as the saved image width
	*/
	int GetSaveSnapshotWidth();
	/**
	* @brief Get the current height, which will be used as the saved image height
	*/
	int GetSaveSnapshotHeight();
	/**
	* @brief Set the width, which will be used as the saved image width
	*/
	void SetSaveSnapshotWidth(int width);
	/**
	* @brief Set the height, which will be used as the saved image height
	*/
	void SetSaveSnapshotHeight(int height);
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
	QList<CGLImage*> iImages; // images in workspace
	CGLImage *iActiveImage;

	CGLWorkspaceSnapshot *iWorkspaceSnapshot;
	QString *iName;
};
#endif
