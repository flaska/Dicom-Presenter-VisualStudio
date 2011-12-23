#include <glObjects/workspaces/layouts/freeLayout.h>
#include <glObjects/glImage.h>
#include <glObjects/workspaces/glWorkspace.h>

CFreeLayout::CFreeLayout(CGLWorkspace *parentWorkspace)
{
iParentWorkspace=parentWorkspace;
}




CFreeLayout::~CFreeLayout()
{

}

void CFreeLayout::Do()
{
}
 void CFreeLayout::RemoveImage(CGLImage *image)
 {
	//
 }

bool CFreeLayout::PrepareNewImageGeometry(CGLImage *image)
{

	QListIterator<CGLImage*> images(iParentWorkspace->GetImages());
	images.toBack();
	if(images.hasPrevious())
	{
		CGLImage*im= images.previous();
		
		image->SetPosition(QPointF(iParentWorkspace->GetPosition().x()+iParentWorkspace->GetBorders().left,
			iParentWorkspace->GetPosition().y()+iParentWorkspace->GetBorders().top));
	}


	if(image->GetSize().x() > iParentWorkspace->GetSize().x())
	{
		image->SetGeometry(image->GetPosition().x(),
			image->GetPosition().y(),
			iParentWorkspace->GetSize().x()-iParentWorkspace->GetBorders().right-iParentWorkspace->GetBorders().left,
			iParentWorkspace->GetSize().y()-iParentWorkspace->GetBorders().top-iParentWorkspace->GetBorders().bottom);
	}
	return true;
}
TLayoutType CFreeLayout::GetType()
{
	return ELayoutFree;
}

void CFreeLayout::PrepareImageToMove(CGLImage *image)
{
	iCurrentImage = image;//being moved resized
	iImageLastPosition=image->GetPosition();
	iImageLastSize=image->GetSize();
}


/**
default layout aligmnment 
1/align image to workspace borders
2/align image to other images in workspace
*/
void CFreeLayout::ImageMoved(CGLImage *image)
{
	ImageMovedAlignToWorkspace(image);
	ImageMovedAlignToOtherImages(image);

}

/**
* Called after user releaser mouse after moving object
*/
void CFreeLayout::ImageMoveFinished(CGLImage *image)
{
//default implementation - do nothing

}

void CFreeLayout::PrepareImageToResize(CGLImage *image)
{
	iCurrentImage = image;//being moved resized
	iImageLastPosition=image->GetPosition();
	iImageLastSize=image->GetSize();
}
/**
default layout aligmnment 
1/align image to workspace borders
2/align image to other images in workspace
*/
void CFreeLayout::ImageResized(CGLImage *image)
{
	ImageResizedAlignToWorkspace(image);
	ImageResizedAlignToOtherImages(image);

}


