#ifndef CWORKSPACEMANGER_H_asdasd456
#define CWORKSPACEMANGER_H_asdasd456
#include <GL/glew.h>
#include <glObjects/glImage.h>
#include <glObjects/workspaces/glWorkspace.h>
#include <glObjects/workspaces/glPlanarWorkspace.h>



/**
* manages all open dicom studies to provide that each study is loaded only once
*/
class CWorkspaceManager
{
public:
		~CWorkspaceManager();
private:
	CWorkspaceManager(CGLObject *parent);

	
public:
	static CWorkspaceManager* InitInstance(CGLObject *parent = NULL);
	static CWorkspaceManager* GetInstance(CGLObject *parent = NULL);
private:
	static CWorkspaceManager *instance;	
public:
	void AddImageToActiveWorkspace(CGLImage *image);

	CGLWorkspace *GetActiveWorkspace();
	CGLPlanarWorkspace *GetPlanarWorkspace();
	void SetActiveWorkspace(CGLWorkspace *workspace);


	void AddWorkspace(CGLWorkspace *workspace);
	void CreatePlanarWorkspace(CGLPlanarWorkspace *workspace);
	void RemoveWorkspace(CGLWorkspace*workspace);
	QList<CGLWorkspace*> &GetWorkspaces();
	bool PlanarWorkspaceExists();
	/*void ReleaseTexture(CDicom3DTexture *texture);
	CDicom3DTexture *LoadTexture(QString &fileName);*/
private:
	CGLWorkspace *iActiveWorkspace;
	QList<CGLWorkspace*> iWorkspaces;
	static CGLObject *iParent;
	CGLPlanarWorkspace *iPlanarWorkspace;
};
#endif
