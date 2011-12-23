#include <glObjects/workspaces/workspaceManager.h>
#include <glObjects/glWidget.h>
#include <glObjects/workspaces/glWorkspaceSnapshot.h>
#include "dpDebug/dpDebug.h"
debug::Log WSMLog;

CGLObject* CWorkspaceManager::iParent = NULL;
CWorkspaceManager* CWorkspaceManager::instance = NULL;
CWorkspaceManager::CWorkspaceManager(CGLObject *parent)
{
	iActiveWorkspace = NULL;
	iPlanarWorkspace = NULL;
}
CWorkspaceManager::~CWorkspaceManager()
{
	instance = NULL;
	//TODO remove
	//	delete iActiveWorkspace;
}


/*
get singleton instance
*/
CWorkspaceManager* CWorkspaceManager::InitInstance(CGLObject *parent)
{
	if(!instance)
	{
		if(parent)
		{
			iParent = parent;
		}
		else
		{
			return NULL;
		}
		instance = new CWorkspaceManager(parent);
	}
	return instance;
}
CWorkspaceManager* CWorkspaceManager::GetInstance(CGLObject *parent)
{
	return instance;
}



void CWorkspaceManager::AddImageToActiveWorkspace(CGLImage *image)
{
	iActiveWorkspace->AddImage(image);
}

CGLWorkspace *CWorkspaceManager::GetActiveWorkspace()
{
	return iActiveWorkspace;
}

CGLPlanarWorkspace *CWorkspaceManager::GetPlanarWorkspace()
{
	return iPlanarWorkspace;
}

void CWorkspaceManager::SetActiveWorkspace(CGLWorkspace *workspace)
{
	iActiveWorkspace = workspace;
}

QList<CGLWorkspace*>& CWorkspaceManager::GetWorkspaces()
{
	return iWorkspaces;
}
void CWorkspaceManager::RemoveWorkspace(CGLWorkspace*workspace)
{
	if (iWorkspaces.contains(workspace))
	{

		iWorkspaces.removeOne(workspace);
		if(iActiveWorkspace==workspace)
		{
			if(iWorkspaces.count()>0)
			{
			iActiveWorkspace=iWorkspaces.at(0);
			}
			else
			{
				iActiveWorkspace=NULL;
			}
		}
		delete workspace;
		workspace=NULL;
	}
	//rearange workspace snapshots
	QListIterator<CGLWorkspace*> workspaces(iWorkspaces);
	workspaces.toFront();
	

	QPoint pos=CGLWorkspaceExplorer::GetInstance()->GetDefaultWorkspaceSnapshotPos();
	QPoint size=CGLWorkspaceExplorer::GetInstance()->GetDefaultWorkspaceSnapshotSize();
	int lastx = pos.x();
	while(workspaces.hasNext())
	{
		
		CGLWorkspace* ws = workspaces.next();
		pos.setX(lastx);
		lastx=pos.x()+size.x();
		ws->GetSnapshot().SetSize(size);
		ws->GetSnapshot().SetPosition(pos);
	}
	
}

void CWorkspaceManager::AddWorkspace(CGLWorkspace *workspace){
	if(!CGLWorkspaceExplorer::GetInstance())
	return;
	QString jk;
	QListIterator<CGLWorkspace*> workspaces(iWorkspaces);
	workspaces.toBack();
	CGLWorkspace *lastWorkspace = NULL;
	if(workspaces.hasPrevious())
	{
		lastWorkspace= workspaces.previous();
	}

	QPointF pos=CGLWorkspaceExplorer::GetInstance()->GetDefaultWorkspaceSnapshotPos();
	QPointF size=CGLWorkspaceExplorer::GetInstance()->GetDefaultWorkspaceSnapshotSize();
	if(lastWorkspace)
	{
		pos=lastWorkspace->GetSnapshot().GetPosition();
		pos.setX(pos.x()+lastWorkspace->GetSnapshot().GetSize().x());
		size=lastWorkspace->GetSnapshot().GetSize();
	}
	iWorkspaces.append(workspace);
	workspace->GetSnapshot().SetSize(size);  //fail
	workspace->GetSnapshot().SetPosition(pos); //fail

	if(iActiveWorkspace==NULL)
	{
		iActiveWorkspace = workspace;
	}
}

void CWorkspaceManager::CreatePlanarWorkspace(CGLPlanarWorkspace *workspace){
	if(!CGLWorkspaceExplorer::GetInstance())
	return;
	iPlanarWorkspace = workspace;
}


bool CWorkspaceManager::PlanarWorkspaceExists(){
	if(iPlanarWorkspace==NULL)
		return false;
	if(iPlanarWorkspace!=NULL)
		return true;
}

