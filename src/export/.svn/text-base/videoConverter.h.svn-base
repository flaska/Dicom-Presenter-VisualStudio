#include <QtCore/QString>

class VideoConverter
{
public:
	/**
	* @brief Check the existence of the convertor program
	* Convert the sequence of bitmaps to the video file (avi) according to the following key:
	* To the baseFileName path is appended an index (from startFrame to stopFrame) and then it is appended the ".bmp" extension
	* to form the bitmap sequence.
	* Then the external program is run on the sequence to make the output video file (.avi) at frameRate framerate
	*/
	static void ConvertBitmapsToAvi(const QString& baseFileName, int startFrame, int stopFrame, int frameRate, const QString& fileName);
};
