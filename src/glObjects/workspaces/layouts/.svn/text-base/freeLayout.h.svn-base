#ifndef CFreeLayout_H_asdasd456
#define CFreeLayout_H_asdasd456

#include <GL/glew.h>
#include <QtCore/QString>
#include <QtCore/QPoint>
#include <QtGui/QScrollBar>
#include <dicom/dicomSerieData.h>
#include <3dTexture/dicom3DTexture.h>

#include <glObjects/glObject.h>
#include <glObjects/glImage.h>
#include <glObjects/workspaces/layouts/workspaceLayout.h>

//forward
class CGLWorkspace;


class CFreeLayout:public MWorkspaceLayout
{
public:
	CFreeLayout(CGLWorkspace *parentWorkspace);

	virtual ~CFreeLayout();
public:
	void Do();
	//void AddImage(CGLImage *image);	
	virtual bool PrepareNewImageGeometry(CGLImage *image);
	virtual void RemoveImage(CGLImage *image);
	virtual void PrepareImageToMove(CGLImage *image);
	virtual void ImageMoveFinished(CGLImage *image);
	virtual void ImageMoved(CGLImage *image);
	virtual void PrepareImageToResize(CGLImage *image);
	virtual void ImageResized(CGLImage *image);
	
	virtual TLayoutType GetType();
//	QList<CGLImage*>& GetImages();
	
protected:
};
#endif
