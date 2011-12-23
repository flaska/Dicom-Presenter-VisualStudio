#ifndef MWORKSPACELAYOUT_H_asdasd456
#define MWORKSPACELAYOUT_H_asdasd456

#include <GL/glew.h>
#include <QtCore/QString>
#include <QtCore/QPoint>
#include <QtGui/QScrollBar>

#include <dicom/dicomSerieData.h>
#include <3dTexture/dicom3DTexture.h>
#include <glObjects/glImage.h>


//forward
class CGLWorkspace;

//types of layouts
typedef enum
{
ELayoutGrowingGridVertical = 0,
ELayoutGrowingGridHorizontal = 1,
ELayoutFree =2
}
TLayoutType;

class MWorkspaceLayout
{
public:

	virtual ~MWorkspaceLayout() =0;
public:
	//void AddImage(CGLImage *image);	
	virtual bool PrepareNewImageGeometry(CGLImage *image) = 0;
	virtual void RemoveImage(CGLImage *image) = 0;
	virtual void PrepareImageToMove(CGLImage *image) = 0;
	virtual void ImageMoveFinished(CGLImage *image) = 0;
	virtual void ImageMoved(CGLImage *image) = 0;
	virtual void PrepareImageToResize(CGLImage *image) = 0;
	virtual void ImageResized(CGLImage *image) = 0;
	virtual void ImageMovedAlignToWorkspace(CGLImage *image);
	virtual void ImageMovedAlignToOtherImages(CGLImage *image);
	
	virtual void ImageResizedAlignToWorkspace(CGLImage *image);
	virtual void ImageResizedAlignToOtherImages(CGLImage *image);
	
	virtual TLayoutType GetType() = 0;
	virtual bool IsInRange(int a, int b, int tolerance);
	virtual void Do()=0;//redo layou
//	QList<CGLImage*>& GetImages();
	
protected:
	CGLWorkspace *iParentWorkspace;
	CGLImage *iCurrentImage;//being moved resized
	QPointF iImageLastPosition;
	QPointF iImageLastSize;
};
#endif
