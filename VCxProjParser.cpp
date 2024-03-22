#include "BaseLib/FileUtil.h"
#include "VCxProjParser.h"


#define PROJECT_TAG				"Project"
#define ITEM_GROUP_TAG			"ItemGroup"
#define FILTER_TAG				"Filter"
#define CL_INCLUDE_TAG			"ClInclude"
#define CL_COMPILE_TAG			"ClCompile"
#define NONE_TAG				"None"
#define UNIQUE_IDENTIFIER_TAG	"UniqueIdentifier"


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CVCxProjParser::Init(CFilterMap* pmFilters, char* szFilename, char* szDirectory)
{
	mszFilename.Init(szFilename);
	mszDirectory.Init(szDirectory);
	mpmFilters = pmFilters;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CVCxProjParser::Kill(void)
{
	mszFilename.Kill();
	mszDirectory.Kill();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool CVCxProjParser::Parse(void)
{
	CXMLFile	cXMLFile;
	bool		bResult;
	
	cXMLFile.Init();
	bResult = cXMLFile.Read(mszFilename.Text(), mszDirectory.Text());
	if (!bResult)
	{
		cXMLFile.Kill();
		return false;
	}

	bResult = ParseProjectFile(&cXMLFile);
	if (!bResult)
	{
		cXMLFile.Kill();
		return false;
	}

	cXMLFile.Kill();
	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool CVCxProjParser::ParseFilter(CMarkupTag* pcFilterTag)
{
	char*			szInclude;
	CMarkupTag*		pcIdentifier;
	CChars			szGuid;
	CFilterFiles*	pcFiles;
	TRISTATE		tResult;

	szInclude = pcFilterTag->GetAttribute("Include");
	if (StrEmpty(szInclude))
	{
		return gcLogger.Error2(__METHOD__, " Filter Tag [%s] in file [%s] has empty [%s] argument.", pcFilterTag->GetName(), mszFilename.Text(), "Include", NULL);
	}
	pcFiles = mpmFilters->AddFilter(szInclude);

	pcIdentifier = pcFilterTag->GetTag(UNIQUE_IDENTIFIER_TAG);
	if (pcIdentifier != NULL)
	{
		szGuid.Init();
		pcIdentifier->GetText(&szGuid);
		tResult = pcFiles->SetGuid(szGuid.Text());
		
		if (tResult != TRITRUE)
		{
			gcLogger.Error2(__METHOD__, " Filter Tag [%s] in file [%s] for parameter [%s] has bad GUID [%s].", pcFilterTag->GetName(), mszFilename.Text(), "Include", szGuid.Text(), NULL);
			szGuid.Kill();
			return false;
		}
		szGuid.Kill();
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool CVCxProjParser::ParseFileTag(CMarkupTag* pcFileTag)
{
	char*			szFilename;
	CMarkupTag*		pcFilter;
	CChars			szFilter;
	CFilterFiles*	pcFiles;
	CChars			sz;
	CFileUtil		cFileUtil;

	szFilename = pcFileTag->GetAttribute("Include");
	if (StrEmpty(szFilename))
	{
		return gcLogger.Error2(__METHOD__, " ClInclude Tag [%s] in file [%s] has empty [%s] argument.", pcFileTag->GetName(), mszFilename.Text(), "Include", NULL);
	}

	pcFilter = pcFileTag->GetTag(FILTER_TAG);
	if (pcFilter != NULL)
	{
		szFilter.Init("\\");
		pcFilter->GetText(&szFilter);
		pcFiles = mpmFilters->AddFilter(szFilter.Text());
		szFilter.Kill();
	}
	else
	{
		pcFiles = mpmFilters->AddFilter("\\");
	}

	sz.Init(szFilename);
	cFileUtil.RemovePath(&sz);
	pcFiles->AddFile(sz.Text());
	sz.Kill();

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool CVCxProjParser::ParseClInclude(CMarkupTag* pcClIncludeTag)
{
	return ParseFileTag(pcClIncludeTag);
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool CVCxProjParser::ParseClCompile(CMarkupTag* pcCompileTag)
{
	return ParseFileTag(pcCompileTag);
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool CVCxProjParser::ParseNone(CMarkupTag* pcNoneTag)
{
	return ParseFileTag(pcNoneTag);
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool CVCxProjParser::ParseItemGroup(CMarkupTag* pcItemGroupTag)
{
	CMarkupTag*		pcTag;
	STagIterator	sIter;
	char*			szTagName;
	bool			bResult;

	pcTag = pcItemGroupTag->GetTag(&sIter);
	while (pcTag != NULL)
	{
		szTagName = pcTag->GetName();
		if (strcmp(FILTER_TAG, szTagName) == 0)
		{
			bResult = ParseFilter(pcTag);
			if (!bResult)
			{
				return false;
			}
		}
		else if (strcmp(CL_INCLUDE_TAG, szTagName) == 0)
		{
			bResult = ParseClInclude(pcTag);
			if (!bResult)
			{
				return false;
			}
		}
		else if (strcmp(CL_COMPILE_TAG, szTagName) == 0)
		{
			bResult = ParseClCompile(pcTag);
			if (!bResult)
			{
				return false;
			}
		}
		else if (strcmp(NONE_TAG, szTagName) == 0)
		{
			bResult = ParseNone(pcTag);
			if (!bResult)
			{
				return false;
			}
		}
		else
		{
			return gcLogger.Error2(__METHOD__, " Unknown second level tag [%s] in file [%s].", szTagName, mszFilename.Text(), NULL);
		}

		pcTag = pcItemGroupTag->GetNextTag(&sIter);
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
bool CVCxProjParser::ParseProjectFile(CXMLFile* pcXMLFile)
{
	CMarkupTag*		pcRoot;
	CMarkupTag*		pcTag;
	STagIterator	sIter;
	char*			szTagName;
	bool			bResult;

	pcRoot = pcXMLFile->mcMarkup.mpcDoc->GetRootTag();
	if (pcRoot == NULL)
	{
		return gcLogger.Error2(__METHOD__, " Root Tag not present in file [%s].", mszFilename.Text(), NULL);
	}

	szTagName = pcRoot->GetName();
	if (strcmp(PROJECT_TAG, szTagName) != 0)
	{
		return gcLogger.Error2(__METHOD__, " Root tag [%s] in file [%s] must be name %s.", szTagName, mszFilename.Text(), PROJECT_TAG, NULL);
	}

	pcTag = pcRoot->GetTag(&sIter);
	while (pcTag != NULL)
	{
		szTagName = pcTag->GetName();
		if (strcmp(ITEM_GROUP_TAG, szTagName) != 0)
		{
			return gcLogger.Error2(__METHOD__, " First level tag [%s] in file [%s] must be name %s.", szTagName, mszFilename.Text(), ITEM_GROUP_TAG, NULL);
		}

		bResult = ParseItemGroup(pcTag);
		if (!bResult)
		{
			return false;
		}

		pcTag = pcRoot->GetNextTag(&sIter);
	}

	return true;
}

