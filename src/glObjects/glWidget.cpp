#include <GL/glew.h>
#include <glObjects/glWidget.h>
#include <QtCore/QString>
#include <QtCore/QList>
#include <plib/fnt.h>

//TODO remove
#include <glObjects/glObject.h>
#include <glObjects/workspaces/workspaceManager.h>
#include <glObjects/workspaces/glWorkspace.h>
#include <mainWindow.h>

#include <../config.h>

#include "dpDebug/dpDebug.h"
debug::Log WidLog;

const char *CGLWidget::iFragmentShaderBiasScale3DProgramName = "main";
const char *CGLWidget::iFragmentShaderBiasScale2DProgramName = "main";
const char *CGLWidget::iFragmentShaderBiasScale3DFileName = FRAGMENT_SHADER_BIAS_SCALE3D_FILE;
const char *CGLWidget::iFragmentShaderBiasScale2DFileName = FRAGMENT_SHADER_BIAS_SCALE2D_FILE;

const char *CGLWidget::iMoveIconFileName = MOVE_ICON_FILE;
const char *CGLWidget::iResizeIconFileName = RESIZE_ICON_FILE;
const char *CGLWidget::iCloseIconFileName = CLOSE_ICON_FILE;
GLenum glGetErr()
{
	GLenum err = glGetError();
	if(err!=GL_NO_ERROR)
	{
		int err=1;
	}
	return err;
}
CGLWidget *CGLWidget::instance = NULL;

CGLWidget* CGLWidget::InitInstance(QWidget *parent)
{
	WidLog.write("Begin: *CGLWidget::InitInstance");
	if(!instance)
	{

		instance = new CGLWidget(parent);
	}
	WidLog.write("End: *CGLWidget::InitInstance");
	return instance;
}

CGLWidget *CGLWidget::GetInstance(){
	return instance;
}

const int CGLWidget::height()
{	
	return QWidget::height();
}
const int CGLWidget::width()
{
	return QWidget::width();
}

CGLWidget::CGLWidget ( QWidget *parent) : QGLWidget (QGLFormat(QGL::DirectRendering | QGL::DoubleBuffer  /* QGL::NoDepthBuffer |*//* QGL::IndirectRendering */), parent )
{

	iActiveObject=NULL;

}
CGLWidget::~CGLWidget ()
{
	instance = NULL;
	if(iImageExplorer)
	{
		delete iImageExplorer;
		iImageExplorer = NULL;
	}
	if(iWorkspaceExplorer)
	{
		delete iWorkspaceExplorer;
		iWorkspaceExplorer = NULL;
	}
	/*
	if(iWorkspace)
	{
	delete iWorkspace;
	iWorkspace = NULL;
	}*/
}

void CGLWidget::CheckForCgError(const char *request)
{
	WidLog.write("Begin: CGLWidget::CheckForCgError");
	CGerror error;
	const char *string = cgGetLastErrorString(&error);

	if (CG_NO_ERROR != error) {
		cout << "Shader error - Cg: " << request << ":" << string << endl;
		if (error == CG_COMPILER_ERROR) {
			cout << cgGetLastListing(iCgContext) << endl;
		}
		exit(1);
	}
	WidLog.write("End: CGLWidget::CheckForCgError");
}

CGprofile &CGLWidget::GetCgFragmentProfile()
{
	return iCgFragmentProfile;
}
CGprogram  &CGLWidget::GetFragmentShaderBiasScaleProgram3D()
{
	return iFragmentShaderBiasScale3DProgram ;
}
CGparameter &CGLWidget::GetFragmentShaderBiasScale3DBiasParam()
{
	return iFragmentShaderBiasScale3DParamBias;
}
CGparameter &CGLWidget::GetFragmentShaderBiasScale3DScaleParam()
{
	return iFragmentShaderBiasScale3DParamScale;
}

CGprogram  &CGLWidget::GetFragmentShaderBiasScaleProgram2D()
{
	return iFragmentShaderBiasScale2DProgram ;
}
CGparameter &CGLWidget::GetFragmentShaderBiasScale2DBiasParam()
{
	return iFragmentShaderBiasScale2DParamBias;
}
CGparameter &CGLWidget::GetFragmentShaderBiasScale2DScaleParam()
{
	return iFragmentShaderBiasScale2DParamScale;
}

GLuint CGLWidget::GetMoveIconTexture()
{
	return iGLTextureMoveIcon;
}
GLuint CGLWidget::GetResizeIconTexture()
{
	return iGLTextureResizeIcon;
}
GLuint CGLWidget::GetCloseIconTexture()
{
	return iGLTextureCloseIcon;
}
void CGLWidget::InitIconTextures()
{
	WidLog.write("Begin: CGLWidget::InitIconTextures");
	QImage iImageMoveIcon = QImage(iMoveIconFileName);
	QImage iImageResizeIcon = QImage(iResizeIconFileName);
	QImage iImageCloseIcon = QImage(iCloseIconFileName);
	
	if (!iImageMoveIcon.isNull()){
		iGLTextureMoveIcon = bindTexture(iImageMoveIcon);
	}
	else cout << "No such file: moveicon.bmp\n";

	if (!iImageResizeIcon.isNull()){
		iGLTextureResizeIcon = bindTexture(iImageResizeIcon);
	}
	else cout << "No such file: resizeicon.bmp\n";

	if (!iImageCloseIcon.isNull()){
		iGLTextureCloseIcon = bindTexture(iImageCloseIcon);
	}
	else cout << "No such file: closeicon.bmp\n";
	WidLog.write("End: CGLWidget::InitIconTextures");
}


void CGLWidget::InitializeShaders()
{

	WidLog.write("Begin: CGLWidget::InitializeShaders");

	iCgContext = cgCreateContext();
	CheckForCgError("cgCreateContext");
	cgGLSetDebugMode(CG_FALSE);
	cgSetParameterSettingMode(iCgContext, CG_DEFERRED_PARAMETER_SETTING);

	iCgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	cgGLSetOptimalOptions(iCgFragmentProfile);
	CheckForCgError("cgGLSetOptimalOptions - selecting fragment profile");

	iFragmentShaderBiasScale3DProgram = 
		cgCreateProgramFromFile(iCgContext,
		CG_SOURCE, //program in human readable form
		iFragmentShaderBiasScale3DFileName,
		iCgFragmentProfile,
		iFragmentShaderBiasScale3DProgramName ,
		NULL);//no extra compiler options
	CheckForCgError ("cgCreateProgramFromFile");

	cgGLLoadProgram(iFragmentShaderBiasScale3DProgram );
	CheckForCgError("cgGLLoadProgram");


	iFragmentShaderBiasScale2DProgram = 
		cgCreateProgramFromFile(iCgContext,
		CG_SOURCE, //program in human readable form
		iFragmentShaderBiasScale2DFileName,
		iCgFragmentProfile,
		iFragmentShaderBiasScale2DProgramName ,
		NULL);//no extra compiler options
	CheckForCgError ("cgCreateProgramFromFile");


	cgGLLoadProgram(iFragmentShaderBiasScale2DProgram );
	CheckForCgError("cgGLLoadProgram");

	iFragmentShaderBiasScale3DParamBias =
		cgGetNamedParameter (iFragmentShaderBiasScale3DProgram , "bias");
	CheckForCgError("cgGetNamedParameter-bias");
	iFragmentShaderBiasScale3DParamScale =
		cgGetNamedParameter (iFragmentShaderBiasScale3DProgram , "scale");
	CheckForCgError("cgGetNamedParameter-scale");

	iFragmentShaderBiasScale2DParamBias =
		cgGetNamedParameter (iFragmentShaderBiasScale2DProgram , "bias");
	CheckForCgError("cgGetNamedParameter-bias");
	iFragmentShaderBiasScale2DParamScale =
		cgGetNamedParameter (iFragmentShaderBiasScale2DProgram , "scale");
	CheckForCgError("cgGetNamedParameter-scale");

	WidLog.write("End: CGLWidget::InitializeShaders");
}

void CGLWidget::initializeGL()
{
	WidLog.write("Begin: CGLWidget::initializeGL");
	/*
 if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "Nemohu inicializovat SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);
	*/

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	}
	cerr << "Status: Using GLEW" << glewGetString(GLEW_VERSION) << endl;

	if (GLEW_ARB_fragment_program)
	{
		/* It is safe to use the ARB_vertex_program extension here. */
		cerr << "arb supported"<<endl;
		InitializeShaders();
	}
	else
	{
		cerr << "arb NOT supported"<<endl;
	}

	InitIconTextures();


	//dpt=0.0;
	glClearColor(0,0,0,0);
	glClearDepth(20.0f); 
	glDepthFunc(GL_LEQUAL  );
	// Ensure correct display of polygons
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	fntInit();
	//	iActiveObject = NULL;
	QPointF imageExplorerPos(this->width()*4/5,0);
	QPointF imageExplorerSize(this->width()*1/5,this->height()*4/5);
	CGLImageExplorer::InitInstance(this, imageExplorerPos,imageExplorerSize);
	iImageExplorer = CGLImageExplorer::GetInstance();;

	iImageExplorer->SetGeometry(imageExplorerPos.x(),imageExplorerPos.y(),imageExplorerSize.x(),imageExplorerSize.y());
	QPointF workspaceExplorerPos(0,this->height()*4/5);
	QPointF workspaceExplorerSize(this->width(),this->height()*1/5);
	CGLWorkspaceExplorer::InitInstance(this, workspaceExplorerPos,workspaceExplorerSize);
	iWorkspaceExplorer = CGLWorkspaceExplorer::GetInstance();
	MainWindow::iSelfS->OpenGLInitialized();

	WidLog.write("End: CGLWidget::initializeGL");
}
QPointF CGLWidget::GetDefaultWorkspacePosition()
{
	return QPointF(0,0);
}
QPointF CGLWidget::GetDefaultWorkspaceSize()
{
	return QPointF(this->width()*4/5,this->height()*4/5);
}
void CGLWidget::resizeGL(int w, int h)
{
	SetGeometry(0,0,w,h);

}
void CGLWidget::SetGeometry(int x, int y, int w, int h)
{
	WidLog.write("Begin: CGLWidget::SetGeometry");
	if(!CWorkspaceManager::GetInstance()) {
		WidLog.write("End: CGLWidget::SetGeometry, return1");
		return;
	}
	iSize.setX(w);
	iSize.setY(h);
	int height = h;
	int width =w;
	if (height==0)								// Prevent A Divide By Zero By
	{
		height=1;
	}						// Making Height Equal One
	glViewport(0, 0, width, height);					// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();							// Reset The Projection Matrix
	glOrtho(0, width ,0, height, 0.1, 90);  

	QPoint imageExplorerPos(this->width()*4/5,0);
	QPoint imageExplorerSize(this->width()-imageExplorerPos.x(),this->height()*4/5);
	iImageExplorer->SetGeometry(imageExplorerPos.x(),imageExplorerPos.y(),imageExplorerSize.x(),imageExplorerSize.y());

	QPoint workspaceExplorerPos(0,this->height()*4/5);
	QPoint workspaceExplorerSize(this->width(),this->height()-workspaceExplorerPos.y());

	iWorkspaceExplorer->SetGeometry(workspaceExplorerPos.x(),workspaceExplorerPos.y(),workspaceExplorerSize.x(),workspaceExplorerSize.y());;

	QPoint workspacePos(0,0);
	QPoint workspaceSize(this->width()*4/5,this->height()*4/5);

	QListIterator<CGLWorkspace*> workspaces((CWorkspaceManager::GetInstance()->GetWorkspaces()));
	workspaces.toFront();
	while(workspaces.hasNext())
	{
		CGLWorkspace* ws = workspaces.next();

		//	QPoint size=GetDefaultWorkspaceSnapshotSize();
		//QPoint pos = GetDefaultWorkspaceSnapshotPos();

		ws->SetGeometry(workspacePos.x(),workspacePos.y(),
			workspaceSize.x(),workspaceSize.y());
		//posx+=size.x();

	}


	CGLObject::resizeGL();
	WidLog.write("End: CGLWidget::SetGeometry");
}

void CGLWidget::resetGLView()
{
	WidLog.write("Begin: CGLWidget::resetGLView");
	int height = this->height();
	int width =this->width();
	if (height==0)								// Prevent A Divide By Zero By
	{
		height=1;
	}						// Making Height Equal One
	glViewport(0, 0, width, height);					// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();							// Reset The Projection Matrix
	glOrtho(0, width ,0, height, 0.1, 90);  

	glMatrixMode(GL_MODELVIEW);                 // bude se menit modelova matice
	glLoadIdentity();   
	//glTranslatef(0,iSize.y(),-50.0f);
	//iParentWindow->height();
	glTranslatef(0,0, -50.0f);  
	WidLog.write("End: CGLWidget::resetGLView");
}
/*
my overload of rendering function

*/
void CGLWidget::renderText(int x, int y, const QString &str, int size)
{
	WidLog.write("Begin: CGLWidget::renderText");

	static bool fntInit=false;
	static   fntRenderer texout ;
	static  fntTexFont  TimesRoman ( "Times-Roman.txf" ) ;
	if(!fntInit)
	{
		texout.setFont      ( & TimesRoman ) ;

		fntInit=true;
	}
	texout.setPointSize (size) ;
	//  glColor3f(1,0,0);
	float scale = 1.2;
	float bias = 0.3;
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glDisable(GL_BLEND);
	//Set up Shader for computing scale and bias influence
	/*cgGLBindProgram(CGLWidget::GetInstance()->GetFragmentShaderBiasScaleProgram2D ());
	cgGLEnableProfile(CGLWidget::GetInstance()->GetCgFragmentProfile());
	cgSetParameter1f(CGLWidget::GetInstance()->GetFragmentShaderBiasScale2DBiasParam(), bias);
	cgSetParameter1f(CGLWidget::GetInstance()->GetFragmentShaderBiasScale2DScaleParam(), scale);
	cgUpdateProgramParameters (CGLWidget::GetInstance()->GetFragmentShaderBiasScaleProgram2D ());
*/

	texout.begin () ;
	texout.start2f ( x, y) ;
	texout.puts ( str.toAscii().data() ) ;

	texout.end () ;
	//text end does not release the texture
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//	glPopMatrix();

	/*cgGLDisableProgramProfiles (CGLWidget::GetInstance()->GetFragmentShaderBiasScaleProgram2D ());
	cgGLDisableProfile (CGLWidget::GetInstance()->GetCgFragmentProfile ());
	cgGLUnbindProgram(CGLWidget::GetInstance()->GetCgFragmentProfile());*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	WidLog.write("End: CGLWidget::renderText");

}
void CGLWidget::paintGL()
{

	WidLog.write("Begin: CGLWidget::paintGL");

	if(!CWorkspaceManager::GetInstance()){
		WidLog.write("End: CGLWidget::paintGL");
		return;
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	
	if(CWorkspaceManager::GetInstance()->GetActiveWorkspace())
	{
		CWorkspaceManager::GetInstance()->GetActiveWorkspace()->paintGL();
	}else if(CWorkspaceManager::GetInstance()->PlanarWorkspaceExists())
	{
		CWorkspaceManager::GetInstance()->GetPlanarWorkspace()->paintGL();
	}


	if(CGLImageExplorer::GetInstance())
	{
		CGLImageExplorer::GetInstance()->paintGL();
	}

	
	if(CGLWorkspaceExplorer::GetInstance())
	{
		CGLWorkspaceExplorer::GetInstance()->paintGL();
	}
	
	if(iActiveObject)
	{
		iActiveObject->Translate();
		iActiveObject->DrawSelection();
		if(dynamic_cast<CGLWorkspaceExplorer*>(iActiveObject))
		{
		dynamic_cast<CGLWorkspaceExplorer*>(iActiveObject)->DrawTexts();
		}
		if(dynamic_cast<CGLImageExplorer*>(iActiveObject))
		{
		dynamic_cast<CGLImageExplorer*>(iActiveObject)->DrawTexts();
		}
	}
	WidLog.write("End: CGLWidget::paintGL");
	return;
	//QImage im = grabFrameBuffer();
}

void CGLWidget::wheelEvent(QWheelEvent *event)
{

	//what object is mouse on
	if(iActiveObject)
	{
		iActiveObject->wheelEvent(event);
	}
}
void CGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if(iActiveObject)
	{
		iActiveObject->mouseReleaseEvent(event);
	}
}
void CGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(iActiveObject)
	{
		iActiveObject->mouseMoveEvent(event);
	}
}


void CGLWidget::mousePressEvent(QMouseEvent *event)
{

	iActiveObject = NULL;
	if(CGLImageExplorer::GetInstance())
	{
		if(CGLImageExplorer::GetInstance()->IsPointOnObject(event->x(),event->y()))
		{
			iActiveObject  = iImageExplorer;
			iImageExplorer->mousePressEvent(event);
			updateGL ();
			return;
		}}
	if(CGLWorkspaceExplorer::GetInstance())
	{
		if(CGLWorkspaceExplorer::GetInstance()->IsPointOnObject(event->x(),event->y()))
		{
			iActiveObject  = iWorkspaceExplorer;
			iWorkspaceExplorer->mousePressEvent(event);
			updateGL ();
			return;
		}
	}
	if(CWorkspaceManager::GetInstance())
	{
		if(CWorkspaceManager::GetInstance()->GetActiveWorkspace())
		{
			if(CWorkspaceManager::GetInstance()->GetActiveWorkspace()->IsPointOnObject(event->x(),event->y()))
			{
				iActiveObject  = CWorkspaceManager::GetInstance()->GetActiveWorkspace();
				iActiveObject->mousePressEvent(event);
				updateGL ();
				return;
			}
		}
		if(CWorkspaceManager::GetInstance()->GetPlanarWorkspace()->IsPointOnObject(event->x(),event->y())){
			iActiveObject  = CWorkspaceManager::GetInstance()->GetPlanarWorkspace();
			iActiveObject->mousePressEvent(event);
			updateGL ();
			return;
		}
	}
}

void CGLWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	iActiveObject = NULL;
	if(iImageExplorer->IsPointOnObject(event->x(),event->y()))
	{
		iActiveObject  = iImageExplorer;
		iImageExplorer->mouseDoubleClickEvent(event);
		updateGL ();
		
		return;
	}
	if(iWorkspaceExplorer->IsPointOnObject(event->x(),event->y()))
	{
		iActiveObject  = iWorkspaceExplorer;
		iWorkspaceExplorer->mouseDoubleClickEvent(event);
		updateGL ();
		return;
	}
	if(CWorkspaceManager::GetInstance()->GetActiveWorkspace())
	{
		if(CWorkspaceManager::GetInstance()->GetActiveWorkspace()->IsPointOnObject(event->x(),event->y()))
		{
			iActiveObject  = CWorkspaceManager::GetInstance()->GetActiveWorkspace();
			iActiveObject->mouseDoubleClickEvent(event);
			updateGL ();
			return;
		}
	}
}


/*
void CGLWidget::AddNewDicomObject(QString *file)
{
try
{
QPoint p1(50,50);
QPoint p2(200,200);
iObjects.append (new CGLObject(this,file,p1,p2));
}
catch(DicomFramesException &e)
{
if(!iObjects.isEmpty() )
{
iObjects.removeLast ();
}
cerr << e.what ();
return;
}
updateGL();

}*/
