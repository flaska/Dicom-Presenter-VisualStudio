#ifndef CGLWidget_H_asasd456
#define CGLWidget_H_asasd456
#include <GL/glew.h>
#include <QtOpenGL/QtOpenGL>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include <3dTexture/dicom3DTexture.h>
#include <glObjects/explorers/glImageExplorer.h>
#include <glObjects/explorers/glWorkspaceExplorer.h>
#include <glObjects/workspaces/glWorkspace.h>
//forward declaration
class CGLObject;
GLenum glGetErr();

class CGLWidget : public QGLWidget, public CGLObject
{
	Q_OBJECT        // must include this if you use Qt signals/slots

private:
	CGLWidget ( QWidget *parent);
	static CGLWidget *instance;
public:
	static CGLWidget* InitInstance(QWidget *parent);
	static CGLWidget *GetInstance();
	~CGLWidget ();
	
public:
		const int height();
const int width();
protected:
	void InitIconTextures();
	void initializeGL();
	void InitializeShaders();
	//void resizeGL (  );
	void resizeGL ( int w, int h );
	void paintGL();

public:
	QPointF GetDefaultWorkspacePosition();
	QPointF GetDefaultWorkspaceSize();
	void renderText(int x, int y, const QString &str, int size);
//	void SetOpenGLInterpolation(bool on);
	void resetGLView();
	void SetGeometry(int x, int y, int w, int h);
	/*void AddNewDicomObject(QString *file);*/
public:
	void wheelEvent(QWheelEvent *e);
	void mouseMoveEvent ( QMouseEvent * e );
	void mousePressEvent ( QMouseEvent * e );
	void mouseReleaseEvent(QMouseEvent *e);
	 void mouseDoubleClickEvent(QMouseEvent *event);
public: //access to shared icon textures
	GLuint GetMoveIconTexture();
	GLuint GetResizeIconTexture();
	GLuint GetCloseIconTexture();
public: //access to shared shaders

inline void CheckForCgError(const char *request);
CGprofile &GetCgFragmentProfile();
CGprogram  &GetFragmentShaderBiasScaleProgram2D();
CGparameter &GetFragmentShaderBiasScale2DBiasParam();
CGparameter &GetFragmentShaderBiasScale2DScaleParam();
CGprogram  &GetFragmentShaderBiasScaleProgram3D();
CGparameter &GetFragmentShaderBiasScale3DBiasParam();
CGparameter &GetFragmentShaderBiasScale3DScaleParam();
public:
	//CGLWorkspace *iWorkspace;
private:
	CGLObject *iActiveObject;
	CGLImageExplorer *iImageExplorer;
	CGLWorkspaceExplorer *iWorkspaceExplorer;

	GLuint iGLTextureMoveIcon;
	GLuint iGLTextureResizeIcon;
	GLuint iGLTextureCloseIcon;
	CGcontext iCgContext;
	CGprofile iCgFragmentProfile;
	CGparameter iFragmentShaderBiasScale2DParamBias;
	CGparameter iFragmentShaderBiasScale2DParamScale;
	CGparameter iFragmentShaderBiasScale3DParamBias;
	CGparameter iFragmentShaderBiasScale3DParamScale;
	static const char *iFragmentShaderBiasScale3DFileName;
	static const char *iFragmentShaderBiasScale2DFileName;
	static const char *iFragmentShaderBiasScale3DProgramName;
	static const char *iFragmentShaderBiasScale2DProgramName;
	CGprogram iFragmentShaderBiasScale3DProgram;
	CGprogram iFragmentShaderBiasScale2DProgram;
	static const char *iMoveIconFileName;
	static const char *iResizeIconFileName;
	static const char *iCloseIconFileName;
};
#endif
