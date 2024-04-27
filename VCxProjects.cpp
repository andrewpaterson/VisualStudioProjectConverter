#include "VCxProjects.h"


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
void CVCxProjects::Init(char* szCommonLibraryDirectory)
{
	mszCommonLibraryDirectory.Init(szCommonLibraryDirectory);
	mlProjects.Init();
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
void CVCxProjects::Kill(void)
{
	CVCxProj* pcProj;

	pcProj = mlProjects.GetHead();
	while (pcProj != NULL)
	{
		pcProj->Kill();
		pcProj = mlProjects.GetNext(pcProj);
	}

	mlProjects.Kill();
	mszCommonLibraryDirectory.Kill();
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
bool CVCxProjects::Add(char* szProjectDirectory, char* szProjectFilterFile)
{
	CVCxProj*	pcProj;
	bool		bResult;

	pcProj = mlProjects.Add();

	pcProj->Init(szProjectDirectory, szProjectFilterFile);
	bResult = pcProj->Parse();
	if (!bResult)
	{
		pcProj->Kill();
		mlProjects.Remove(pcProj);
		return false;
	}
	return true;
}


