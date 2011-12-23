#ifndef SAVESEQUENCEDIALOG_H
#define SAVESEQUENCEDIALOG_H
#include <GL/glew.h>
#include <QtGui/QMainWindow>
#include "ui_sequencesavedialog.h"
class SaveSequenceDialog : public QDialog
{
	Q_OBJECT
public:
	SaveSequenceDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SaveSequenceDialog();
public slots:
	void accept();
	void WidthChanged(int);
	void HeightChanged(int);
private:
	bool iDoDimensionUpdate;
	float iAspectRatio;
	Ui::DialogSaveSequence ui;
};

#endif // SAVESEQUENCEDIALOG_H
