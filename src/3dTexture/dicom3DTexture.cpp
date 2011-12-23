#include <GL/glew.h>
#include <3dTexture/dicom3DTexture.h>
#include <dicom/dicomSerieData.h>
#include <glObjects/workspaces/workspaceManager.h>
#include <glObjects/glWidget.h>

#include "dpDebug/dpDebug.h"
debug::Log d3dLog;

int CDicom3DTexture::GetDepth()
{
	return iDepth;
}

int CDicom3DTexture::GetHeight()
{
	return iHeight;
}

int CDicom3DTexture::GetWidth()
{
	return iWidth;
}


CDicom3DTexture::~CDicom3DTexture()
{
	if(iFrames)
	{
		delete iFrames;
		iFrames = NULL;
	}
	if(iFileName)
	{
		delete iFileName;
		iFileName = NULL;
	}
}

QString& CDicom3DTexture::GetFileName()
{
	return *iFileName;
}
CDicom3DTexture::CDicom3DTexture(QString &fileName)
{
	iFrames = NULL;
	GLenum err = glGetErr();//To throw away the last Open GL Error
	iFileName = new QString();
	iFileName->append(fileName);
	
	iFrames= new CDicomFrames(fileName.toAscii().data());
	glGenTextures(1,&iTextureID);            // Allocate space for texture
	
	glBindTexture(GL_TEXTURE_3D,iTextureID); // Set our Tex handle as current

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);          // zpusob ulozeni bytu v texture
	//generate texture
	switch (iFrames->GetColorCount())
	{
	case 1:
		{
			int maxsize[3] ;
			glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE,&maxsize[0] );

			//	int j=sizeof(GLushort );
			/*
			on't use GL_PROXY_TEXTURE_3D as its isn't very reliable as you have
already seen, it might not be well implemented in the driver. A better
thing is to actually use glTexImage3D(GL_TEXTURE_3D...) and check for
GL errror (using glGetErr ). Repeat this with different texture sizes
until you succeed (no GL error) with the call
glTexImage3D(GL_TEXTURE_3D...).
			*/
			//test for succesfull texture load
			/*glTexImage3D(GL_PROXY_TEXTURE_3D,0,GL_RGB32F_ARB    ,8200, 8200 ,8248,0,
				GL_LUMINANCE ,GL_UNSIGNED_SHORT  ,frames.GetImageData());

			GLint format;
			glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0,
				GL_TEXTURE_INTERNAL_FORMAT, &format);
				*/
			
			//GL_RGB32F_ARB - for color resolution
			GLenum err = glGetErr();
			if(err!=GL_NO_ERROR )
			{
				int errt=1;
				d3dLog.write("glGetErr()(CDicom3DTexture::CDicom3DTexture genTextures,PixelStorei) not succesful");
				throw TextureNotCreatedException();
			}

			glTexImage3DEXT=(PFNGLTEXIMAGE3DEXTPROC)wglGetProcAddress("glTexImage3DEXT");
			glTexSubImage3DEXT=(PFNGLTEXSUBIMAGE3DEXTPROC)wglGetProcAddress("glTexSubImage3DEXT");

			glTexImage3D(GL_TEXTURE_3D,0,GL_LUMINANCE16  ,iFrames->GetWidth(), iFrames->GetHeight() ,iFrames->GetFramesCount(),0,
				GL_LUMINANCE ,GL_UNSIGNED_SHORT  ,iFrames->GetImageData());  //tento prikaz neprobehne spravne

			err = glGetErr();

			if(err!=GL_NO_ERROR )
			{
				int errt=1;
				d3dLog.write("glGetErr()(CDicom3DTexture::CDicom3DTexture glTexImage3D) not succesful");
				throw TextureNotCreatedException();
			}
		
			
		}
		break;
	default:
		assert(1==0); //TODO other color depths
	}

	iWidth = iFrames->GetWidth();
	iHeight = iFrames->GetHeight();
	iDepth = iFrames->GetFramesCount ();
	iColorsCount = iFrames->GetColorCount ();
	iBitsPerPixel = iFrames->GetBitsPerSampleCount ()*iFrames->GetColorCount ();
	iWindowDefaults = iFrames->GetWindowDefaults();
	iMaxColorValue= iFrames->GetMinColorValue();
	iMinColorValue=iFrames->GetMaxColorValue();

	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);	
	// Specify filtering and edge actions
	//glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_NEAREST );
	//nasledujici parametry znehodnoti texturu - nevim proc
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	// same as above for R axis
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// this is a 3d texture, level 0 (max detail), GL should store it in RGB8 format, its WIDTHxHEIGHTxDEPTH in size, 
	// it doesnt have a border, we're giving it to GL in RGB format as a series of unsigned bytes, and texels is where the texel data is.
 err = glGetErr();
 if(err!=GL_NO_ERROR )
			{
				int errt=1;
				throw TextureNotCreatedException();
			}
	iFrames->FreeData();
	
}

void CDicom3DTexture::SetIdentificationString(QString &str)
{
	iIdentificationString = str;
}
QString &CDicom3DTexture::GetIdentificationString()
{
	return iIdentificationString;
}
CDicomHeader& CDicom3DTexture::GetDicomHeader(int frame)
{
	return iFrames->GetHeader(frame);
}
GLuint CDicom3DTexture::GetTextureID()
{
	return iTextureID;
}

TImageWindow CDicom3DTexture::GetWindowDefaults()
{
	return iWindowDefaults;
}



