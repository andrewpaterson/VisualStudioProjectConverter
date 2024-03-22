#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include "BaseLib/FastFunctions.h"
#include "BaseLib/NumberControl.h"
#include "BaseLib/FileUtil.h"
#include "BaseLib/DebugOutput.h"
#include "VCxProjParser.h"


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void StartUp(void)
{
	_CrtSetBreakAlloc(0);

	gcLogger.Init();
	FastFunctionsInit();
	NumberInit();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int Shutdown(int iResult)
{
	NumberKill();
	FastFunctionsKill();
	gcLogger.Kill();

	_CrtDumpMemoryLeaks();
	return iResult;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CreateInverseDirectoryMap(char* szHomeDirectory, CFileMap* pmmDirectoryFiles, CFileMap* pmmFilterFiles)
{
	CFileUtil		cFileUtil;
	CArrayChars		aszFiles;
	int				i;
	CChars*			pszFullFilename;
	CChars			szFilename;
	CChars			szPathName;
	bool			bFile;
	CFileFilters*	pcFileFilters;
	int				iHomeDirectoryLength;

	iHomeDirectoryLength = StrLen(szHomeDirectory);
	aszFiles.Init();
	cFileUtil.FindAllFiles(szHomeDirectory, &aszFiles, true, true);

	for (i = 0; i < aszFiles.NumElements(); i++)
	{
		pszFullFilename = aszFiles.Get(i);
		szFilename.Init(pszFullFilename);
		cFileUtil.RemovePath(&szFilename);

		bFile = pmmFilterFiles->Contains(szFilename.Text());
		if (bFile)
		{
			szPathName.Init(pszFullFilename);
			cFileUtil.RemoveLastFromPath(&szPathName);
			szPathName.RemoveFromStart(iHomeDirectoryLength);
			if (!szPathName.StartsWith("\\"))
			{
				szPathName.Insert(0, "\\");
			}
			pcFileFilters = pmmDirectoryFiles->AddFile(szFilename.Text());
			pcFileFilters->AddFilter(szPathName.Text());
			szPathName.Kill();
		}
		szFilename.Kill();
	}

	aszFiles.Kill();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int __cdecl main(void)
{
	StartUp();

	CVCxProjParser	cParser;
	bool			bResult;
	CFilterMap		mmFilters;
	CFileMap		mmFilterFiles;
	CFileMap		mmDirectoryFiles;
	char			szHomeDirectory[] = "D:\\gameengine\\CommonLibraries\\BaseLib";

	mmFilters.Init();
	cParser.Init(&mmFilters, "BaseLib.vcxproj.filters", szHomeDirectory);
	bResult = cParser.Parse();
	if (!bResult)
	{
		mmFilters.Kill();
		cParser.Kill();
		return Shutdown(1);
	}

	cParser.Kill();

	mmFilterFiles.Init();
	mmFilters.CreateInverseMap(&mmFilterFiles);

	mmDirectoryFiles.Init();
	CreateInverseDirectoryMap(szHomeDirectory, &mmDirectoryFiles, &mmFilterFiles);

	mmDirectoryFiles.Dump();
	mmFilterFiles.Dump();
	mmFilters.Dump();

	mmDirectoryFiles.Kill();
	mmFilterFiles.Kill();
	mmFilters.Kill();
	return Shutdown(0);
}

