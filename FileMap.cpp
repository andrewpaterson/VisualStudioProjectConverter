#include "FileMap.h"


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
void CFileMap::Init(void)
{
	CMapStringBlock::Init();
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
void CFileMap::Kill(void)
{
	SMapIterator	sIter;
	char*			szKey;
	CFileFilters*	pcFilters;
	bool			bExists;

	bExists = StartIteration(&sIter, (void**)&szKey, NULL, (void**)&pcFilters, NULL);
	while (bExists)
	{
		pcFilters->Kill();
		bExists = Iterate(&sIter, (void**)&szKey, NULL, (void**)&pcFilters, NULL);
	}

	CMapStringBlock::Kill();
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
CFileFilters* CFileMap::AddFile(char* szFilename)
{
	CFileFilters* pcFilters;

	pcFilters = (CFileFilters*)Get(szFilename);
	if (pcFilters == NULL)
	{
		pcFilters = (CFileFilters*)Put(szFilename, sizeof(CFileFilters));
		pcFilters->Init(szFilename);
	}
	return pcFilters;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
bool CFileMap::Contains(char* szFilename)
{
	CFileFilters* pcFilters;

	pcFilters = (CFileFilters*)Get(szFilename);
	return pcFilters != NULL;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
void CFileMap::Print(CChars* psz)
{
	SMapIterator	sIter;
	char*			szKey;
	CFileFilters*	pcFilters;
	bool			bExists;
	CArrayChars*	paszFilters;
	int				i;
	int				iNumElements;
	char*			szFilter;

	bExists = StartIteration(&sIter, (void**)&szKey, NULL, (void**)&pcFilters, NULL);
	while (bExists)
	{
		paszFilters = pcFilters->GetFilters();
		iNumElements = paszFilters->NumElements();
		for (i = 0; i < iNumElements; i++)
		{
			szFilter = paszFilters->Get(i)->Text();
			psz->Append(szKey);
			psz->Append(" -> ");
			psz->Append(szFilter);
			psz->AppendNewLine();
		}

		bExists = Iterate(&sIter, (void**)&szKey, NULL, (void**)&pcFilters, NULL);
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
void CFileMap::Dump(void)
{
	CChars	sz;

	sz.Init();
	Print(&sz);
	sz.DumpKill();
}


