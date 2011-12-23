#ifndef GLIMAGE_H_AWDS
#define GLIMAGE_H_AWDS
#include <QtCore/QString>
#include <QtCore/QPoint>
#include <glObjects/glObject.h>
#include <dicom/dicomSerieData.h>
#include <3dTexture/dicom3DTexture.h>

//forward
class CGLObject;
class CGLWorkspace;
class CAnimation;

typedef struct
{
	float x;
	float y;
	float z;
}TPlanarCrossPosition;


typedef enum{
	EManipNone,
	EManipAllowed,
	EManipDissalowed
}
TManipulationState;
typedef enum
{
	EImageOrientationAxial = 0, //shora dolu
	EImageOrientationSagittal = 1, //zezadu dopredu
	EImageOrientationCoronal = 2//zleva doprava


}TImageAxisOrientation;

typedef enum
{
	EDisplayTextZoom = 1,
	EDisplayTextOrientation = 2,
	EDisplayTextWindow = 4,
	EDisplayTextFrameData = 8,
	EDisplayTextAll = 1073741823
}TDiplayText;

/**
* @class CGLImage
* @brief Basic object for derived textured objects
*/
class CGLImage:public CGLObject
{
public:
	CGLImage(CGLObject *parentWindow,QString &file, QPointF &position, QPointF &size);
	CGLImage(CGLObject *parentWindow,CDicom3DTexture *texture, QPointF& position, QPointF &size );
	virtual ~CGLImage();
	CGLImage(CGLObject *parentWindow, QPointF &position, QPointF &size);
	void SetAnimation(CAnimation *animation);
	CAnimation* GetAnimation();
	void initializeGL();
	void Init(CGLObject *parentWindow, QPointF &position, QPointF &size);
	void DrawManipulation();
	void DrawSlider();
	bool InitSliceTexture();
	void PrepareActualTexture();
	void SetParentWorkspace(CGLWorkspace *workspace);
	CGLWorkspace * GetParentWorkspace();
	void DrawImage();
	void DrawMultiplanarSlice();

	void PreparePlanarSlice(char type, TPlanarCrossPosition);

	void DrawToTextureSliceZ(TPlanarCrossPosition);
	void DrawToTextureSliceX(TPlanarCrossPosition);
	void DrawToTextureSliceY(TPlanarCrossPosition);

	void DrawManipulationSliceZ(TPlanarCrossPosition);
	void DrawManipulationSliceX(TPlanarCrossPosition);
	void DrawManipulationSliceY(TPlanarCrossPosition);

	void DrawSliceZ();
	void DrawSliceX();
	void DrawSliceY();


	void DrawTexts();
	QString& GetName();
	void SetName( const QString& name);
	/**
	* @brief Returns the actual image window center and width settings
	*/
	TImageWindow GetImageWindow();
	/**
	* @brief Sets the image window center and width settings
	*/
	void SetImageWindow(TImageWindow window);
	TImageAxisOrientation GetOrientation();
	void SetOrientation(TImageAxisOrientation orientation);
	void paintGL();
	void CloseMe();//delete this
	void SetOpenGLInterpolation(bool on);
	void SetGeometry(float x, float y, float width, float height);
	//void SetSize(const QPoint &size);
	//	void SetPosition(const QPoint &position);
	float GetZoom();
	void SetZoom(float zoom);
	void mouseMoveEvent(QMouseEvent *event);
	void MoveToFrame(int frame);
	void MoveToDepth(float inDepthPosition);//move in texture from front to back
	bool IsOnFrameSliderIcon(int x, int y);;
	void wheelEvent(QWheelEvent *event);	
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	/*
	* Set that image is being moved or something else - than when painted, DrawManipulation is called
	* Used in layouts to signalize image swap,...
	*/
	void SetManipulated(TManipulationState manipulated);
	CDicom3DTexture *GetTexture();
	/**
	Get image from ImageExplorer
	*/
	CGLImage *GetOwner();
	void SetOwner(CGLImage *owner);

	/**
	create image in active workspace from selected (this) texture
	*/
	CGLImage* CreateDerivedImage(TImageAxisOrientation orientation);
	void RemoveDerivedImage(CGLImage* image);
	QList<CGLImage*>&  GetDerivedImages();
	float GetActualTextureDepth();
	bool GetTextDisplay(TDiplayText displayText);
	void SetTextDisplay(TDiplayText displayText, bool on);
	/**
	* @brief Returns the actual frame nr
	*/
	int GetActualFrameNr();
	int GetActualSliceWidth();
	int GetActualSliceHeight();
private:
	float SwitchY(float ypos);
	TManipulationState iManipulated;
	float ToOpenGLCoords(float y);
	QList<CGLImage *> iDerivedImages;
private:
	unsigned int iDisplayTextSettings;

	int iLastAxialPosition;
	int  iLastSagittalPosition;
	int iLastCoronalPosition;
	TImageAxisOrientation iImageAxisOrientation;
	QString *iName;
	CGLWorkspace *iParentWorkspace;
	/**
	image from image explorer from which was the image derived
	*/
	CGLImage *iOwner;
	//properties	

	bool iOpenGLInterpol;
	/**
	* Draw slice of the texture in the real ratio of width and height
	*/
	void DrawSliceToTexture();//called from prepareactual texture;
	GLuint iFBO;
	GLuint iFBOZ;
	GLuint iFBOX;
	GLuint iFBOY;
	CAnimation* iAnimation;
	TTextureCoordinates iActualTextureCoords;
	GLuint iActualSliceTexture;
	GLuint iSliceZ;
	GLuint iSliceX;
	GLuint iSliceY;
	TSliceInfo iActualSliceTextureInfo;
	int iSum;
	float iScale;
	float iBias;
	//image
	float iZoomFactor;
	QPointF iImageCenter; //parts of image border
	TImageWindow iImageWindow;
	QPointF iLockedImageCenter;
	float iLockedZoom;
	//
	QPoint iPreviousMousePosition;
	QPoint iLockedGlobalMousePosition;
	QPointF iLockedSize ;
	QPointF iLockedPosition ;
	//CGLWidget* iParentWindow;
	//QPoint iPosition;
	//QPoint iSize;
	//CDicomFrames* iFrames;
	CDicom3DTexture *iTexture;

	TMouseState iMouseState;

	TGLIcon iFrameSlider;
	//bool iSkipMouseMovement;
};
#endif
