#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include "BaseLib/FastFunctions.h"
#include "BaseLib/NumberControl.h"
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
int __cdecl main(void)
{
	StartUp();

	CVCxProjParser	cParser;
	bool			bResult;
	CFilterMap		mmFilters;

	mmFilters.Init();
	cParser.Init(&mmFilters, "BaseLib.vcxproj.filters", "D:/gameengine/CommonLibraries/BaseLib");
	bResult = cParser.Parse();
	if (!bResult)
	{
		mmFilters.Kill();
		cParser.Kill();
		return Shutdown(1);
	}

	cParser.Kill();

	mmFilters.Dump();

	mmFilters.Kill();
	return Shutdown(0);
}

