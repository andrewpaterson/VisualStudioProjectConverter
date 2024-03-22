#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include "BaseLib/FastFunctions.h"
#include "BaseLib/NumberControl.h"
#include "BaseLib/Logger.h"
#include "VCxProj.h"


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
int __cdecl main(void)
{
	StartUp();

	bool			bResult;
	char			szHomeDirectory[] = "D:\\gameengine\\CommonLibraries\\BaseLib";

	CVCxProj	cProj;

	cProj.Init(szHomeDirectory);
	
	bResult = cProj.Parse();
	if (!bResult)
	{
		cProj.Kill();
		return Shutdown(1);
	}

	cProj.DumpDirectoryFiles();
	cProj.DumpFilterFiles();
	cProj.DumpFilters();

	cProj.Kill();

	return Shutdown(0);
}

