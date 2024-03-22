#include "BaseLib/FileUtil.h"
#include "VCxProjParser.h"
#include "VCxProj.h"


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CVCxProj::Init(char* szProjectDirectory, char* szProjectFilterFile)
{
	mszProjectDirectory.Init(szProjectDirectory);
	mszProjectFilterFile.Init(szProjectFilterFile);

	mmFilters.Init();
	mmFilterFiles.Init();
	mmDirectoryFiles.Init();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CVCxProj::Kill(void)
{
	mmDirectoryFiles.Kill();
	mmFilterFiles.Kill();
	mmFilters.Kill();

	mszProjectFilterFile.Kill();
	mszProjectDirectory.Kill();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool CVCxProj::Parse(void)
{
	CVCxProjParser	cParser;
	bool			bResult;

	cParser.Init(&mmFilters, mszProjectFilterFile.Text(), mszProjectDirectory.Text());
	bResult = cParser.Parse();
	if (!bResult)
	{
		mmFilters.Kill();
		cParser.Kill();
		return false;
	}

	cParser.Kill();

	mmFilters.CreateInverseMap(&mmFilterFiles);

	CreateInverseDirectoryMap();

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CVCxProj::CreateInverseDirectoryMap(void)
{
	CFileUtil		cFileUtil;
	CArrayChars		aszFiles;
	int				i;
	CChars* pszFullFilename;
	CChars			szFilename;
	CChars			szPathName;
	bool			bFile;
	CFileFilters* pcFileFilters;
	int				iHomeDirectoryLength;

	iHomeDirectoryLength = mszProjectDirectory.Length();
	aszFiles.Init();
	cFileUtil.FindAllFiles(mszProjectDirectory.Text(), &aszFiles, true, true);

	for (i = 0; i < aszFiles.NumElements(); i++)
	{
		pszFullFilename = aszFiles.Get(i);
		szFilename.Init(pszFullFilename);
		cFileUtil.RemovePath(&szFilename);

		bFile = mmFilterFiles.Contains(szFilename.Text());
		if (bFile)
		{
			szPathName.Init(pszFullFilename);
			cFileUtil.RemoveLastFromPath(&szPathName);
			szPathName.RemoveFromStart(iHomeDirectoryLength);
			if (!szPathName.StartsWith("\\"))
			{
				szPathName.Insert(0, "\\");
			}
			pcFileFilters = mmDirectoryFiles.AddFile(szFilename.Text());
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
void CVCxProj::DumpDirectoryFiles(void)
{
	mmDirectoryFiles.Dump();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CVCxProj::DumpFilterFiles(void)
{
	mmFilterFiles.Dump();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CVCxProj::DumpFilters(void)
{
	mmFilters.Dump();
}

