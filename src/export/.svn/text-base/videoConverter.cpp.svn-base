#include <export/videoConverter.h>
#include <QtGui/QMessageBox>

void VideoConverter::ConvertBitmapsToAvi( const QString& baseFileName, int startFrame, int stopFrame, int frameRate, const QString& outFileName)
{
	int i = system("EasyBMPtoAVI.exe -help");
	if(i)
	{
		
	QString strCommand("EasyBMPtoAVI.exe -filebase ");
	strCommand.append(baseFileName);
	strCommand.append(" -start ");
	strCommand.append(QString::number(startFrame));
	strCommand.append(" -end ");
	strCommand.append(QString::number(stopFrame));
	strCommand.append(" -framerate ");
	strCommand.append(QString::number(frameRate));
	strCommand.append(" -output ");
	strCommand.append(outFileName);
	//i=system("dir");
	i = system(strCommand.toAscii().data());
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Video file not created.\nEasyBMPtoAVI.exe not found in the app directory.");
		msgBox.exec();
	}
}
