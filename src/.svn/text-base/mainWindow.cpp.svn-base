#include <GL/glew.h>
#include <QtGui/QFileDialog>

#include <mainWindow.h>
#include <infoPanel.h>
#include <glObjects/workspaces/workspaceManager.h>
#include <animation/animationManager.h>
#include <dialogs/saveSequenceDialog.h>

MainWindow* MainWindow::iSelfS=NULL;
MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
{

	iSelfS= this;
	iOpenGLInitialized = false;
	ui.setupUi(this);

	QObject::connect(ui.actionExit , SIGNAL(triggered()), 
		this, SLOT(Exit())); 
	//
	QObject::connect(ui.actionOtev_t , SIGNAL(triggered()), 
		this, SLOT(OpenFile())); 

	QObject::connect(ui.actionSaveSnapshot , SIGNAL(triggered()), 
		this, SLOT(SaveSnapshot())); 
	QObject::connect(ui.actionSaveSequence , SIGNAL(triggered()), 
		this, SLOT(SaveSequence())); 
	iGLWidget  = CGLWidget::InitInstance(this);
	iAnimationManager = CAnimationManager::InitInstance();
	setCentralWidget(iGLWidget);
	//continues in openglinitialized function
	iDock = new QDockWidget(QString("Properties"),this);
	iDock->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
	iDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea,iDock);
}

MainWindow::~MainWindow()
{
	delete iWorkspaceManager;
	delete iInfoPanel;
	delete iGLWidget;
	delete iAnimationManager;
	
}
void MainWindow::Exit()
{
	QApplication::exit();
}
//FInish initialization
void MainWindow::OpenGLInitialized()
{
	iWorkspaceManager = CWorkspaceManager::InitInstance(iGLWidget);
	CGLWorkspace * newWorkspace = new CGLWorkspace(iGLWidget,iGLWidget->GetDefaultWorkspacePosition(),iGLWidget->GetDefaultWorkspaceSize());
	CWorkspaceManager::GetInstance()->AddWorkspace(newWorkspace);
	newWorkspace->SetGeometry(iGLWidget->GetDefaultWorkspacePosition().x(),iGLWidget->GetDefaultWorkspacePosition().y(),iGLWidget->GetDefaultWorkspaceSize().x(),iGLWidget->GetDefaultWorkspaceSize().y());;
	iGLWidget->show();
	iInfoPanel = CInfoPanel::InitInstance(iDock);
	CInfoPanel::GetInstance()->SetImageExplorerInfoView();
	iGLWidget->setGeometry(15,50,800,600);
	iOpenGLInitialized = true;
}
void MainWindow::resizeEvent(QResizeEvent* event)
{
}

/**
* @brief Save the actual workspace animation
*/
void MainWindow::SaveSequence()
{
	SaveSequenceDialog dial;

	int ret = dial.exec();
	if(ret)
	{
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save sequence"),
			"",
			tr("video file (*.avi)"));
		CAnimationManager::GetInstance()->SaveSequence(fileName);
	}
}

/**
* @brief Save the actual workspace as image file
*/
void MainWindow::SaveSnapshot()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save snapshot"),
		"",
		tr("picture (*.png)"));
	CWorkspaceManager::GetInstance()->GetActiveWorkspace()->SaveSnapshot(fileName);
}


void MainWindow::OpenFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"", tr("DICOM Images (*.dcm *.ima)"));
	CGLImageExplorer::GetInstance()->OpenImage(fileName);
}
void MainWindow::mousePressEvent ( QMouseEvent * e )
{
	//iGLWidget->SetOpenGLInterpolation(true);
}


