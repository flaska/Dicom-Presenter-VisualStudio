#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <GL/glew.h>
#include <QtGui/QMainWindow>
#include <ui_mainwindow.h>
#include <glObjects/glWidget.h>
#include <animation/animationManager.h>
class CInfoPanel;
class CWorkspaceManager;
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();
	void mousePressEvent ( QMouseEvent * e );
	void OpenGLInitialized();

public slots:
	void Exit();
	void OpenFile();
	void SaveSnapshot();
	void SaveSequence();
public:
void resizeEvent(QResizeEvent* event);
	static MainWindow* iSelfS;
private:
	Ui::DicomPresenter ui;
	CInfoPanel *iInfoPanel;
	CGLWidget *iGLWidget;
	CWorkspaceManager* iWorkspaceManager;
	QDockWidget *iDock;
	bool iOpenGLInitialized;
	CAnimationManager * iAnimationManager;
};

#endif // MAINWINDOW_H
