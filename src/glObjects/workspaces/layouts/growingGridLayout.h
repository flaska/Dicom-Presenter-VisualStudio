#include "workspaceLayout.h"
typedef enum 
{
EGrowingColumn=0,
EGrowingRow
}TGrowingDirection;

typedef enum{
EGrowinGridLayoutHorizontal = 0,
EGrowinGridLayoutVertical = 1
}
TGrowingGridLayoutType;
/**
When object is added to layout, the layout adjusts number of rows and collumns
for images
*/
class CGrowingGridLayout:public MWorkspaceLayout
{

public:
	CGrowingGridLayout(CGLWorkspace *parentWorkspace, TGrowingGridLayoutType type);

	~CGrowingGridLayout();
public:
	void Do();
	//void AddImage(CGLImage *image);	
	bool PrepareNewImageGeometry(CGLImage *image);

	void PrepareImageToResize(CGLImage *image);
	void RemoveImage(CGLImage *image);

	void ImageResized(CGLImage *image);

	void ResizeImagesOnRowAndColumn(CGLImage* image);

	void PrepareImageToMove(CGLImage *image);
	QPoint FindGridPosition(CGLImage *image);
	QPoint GetDefaultSize(int rows, int columns);
	TLayoutType GetType();
	void FitToGrid();
	/**
	default layout aligmnment 
	1/align image to workspace borders
	2/align image to other images in workspace
	*/
	void ImageMoved(CGLImage *image);

	/**
	* Called after user releaser mouse after moving object
	*/
	void ImageMoveFinished(CGLImage *image);
private:
	TGrowingGridLayoutType iType;//type horizontal or vertical suitable
	bool iToSwap;
	CGLImage *iSwappedImage;//pointer to image which was swapped with actual image when moved
	enum {EMaximumGridLayoutRowCount = 20};
	CGLImage* iImageGrid[EMaximumGridLayoutRowCount+1][EMaximumGridLayoutRowCount+1];
	int iColumnCount;
	int iRowCount;
	int iLastUsedRow;
	int iLastUsedColumn;
	int iRowsHeights[EMaximumGridLayoutRowCount+1];
	int iColumnWidths[EMaximumGridLayoutRowCount+1];
	int iRowsTops[EMaximumGridLayoutRowCount+1];
	int iColumnLefts[EMaximumGridLayoutRowCount+1];
	//TGrowingDirection iLastDirection;
};

