#include "FilterMap.h"


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
void CFilterMap::Init(void)
{
	CMapStringBlock::Init();
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
void CFilterMap::Kill(void)
{
	SMapIterator	sIter;
	char*			szKey;
	CFilterFiles*	pcFiles;
	bool			bExists;

	bExists = StartIteration(&sIter, (void**)&szKey, NULL, (void**)&pcFiles, NULL);
	while (bExists)
	{
		pcFiles->Kill();
		bExists = Iterate(&sIter, (void**)&szKey, NULL, (void**)&pcFiles, NULL);
	}

	CMapStringBlock::Kill();
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
CFilterFiles* CFilterMap::AddFilter(char* szFilter)
{
	CFilterFiles*	pcFiles;

	pcFiles = (CFilterFiles*)Get(szFilter);
	if (pcFiles == NULL)
	{
		pcFiles = (CFilterFiles*)Put(szFilter,  sizeof(CFilterFiles));
		pcFiles->Init();
	}
	return pcFiles;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
void CFilterMap::Print(CChars* psz)
{
	SMapIterator	sIter;
	char*			szKey;
	CFilterFiles*	pcFiles;
	bool			bExists;
	CArrayChars*	paszFiles;
	int				i;
	int				iNumElements;
	char*			szFilename;

	bExists = StartIteration(&sIter, (void**)&szKey, NULL, (void**)&pcFiles, NULL);
	while (bExists)
	{
		paszFiles = pcFiles->GetFiles();
		iNumElements = paszFiles->NumElements();
		for (i = 0; i < iNumElements; i++)
		{
			szFilename = paszFiles->Get(i)->Text();
			if (!StrEmpty(szKey))
			{
				psz->Append(szKey);
				psz->Append("\\");
			}
			psz->Append(szFilename);
			psz->AppendNewLine();

		}

		bExists = Iterate(&sIter, (void**)&szKey, NULL, (void**)&pcFiles, NULL);
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
void CFilterMap::Dump(void)
{
	CChars	sz;

	sz.Init();
	Print(&sz);
	sz.DumpKill();
}


