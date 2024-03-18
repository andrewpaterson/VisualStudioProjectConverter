#ifndef __VCX_PROJ_PARSER_H__
#define __VCX_PROJ_PARSER_H__
#include "BaseLib/Chars.h"
#include "BaseLib/XMLFile.h"
#include "FilterMap.h"


class CVCxProjParser
{
protected:
	CChars		mszFilename;
	CChars		mszDirectory;
	CFilterMap*	mpmFilters;

public:
	void Init(CFilterMap* pmFilters, char* szFilename, char* szDirectory);
	void Kill(void);

	bool Parse(void);

protected:
	bool ParseProjectFile(CXMLFile* pcXMLFile);
	bool ParseItemGroup(CMarkupTag* pcItemGroupTag);
	bool ParseFilter(CMarkupTag* pcFilterTag);
	bool ParseClInclude(CMarkupTag* pcIncludeTag);
	bool ParseClCompile(CMarkupTag* pcCompileTag);
	bool ParseNone(CMarkupTag* pcNoneTag);
	bool ParseFileTag(CMarkupTag* pcFileTag);
};


#endif // !__VCX_PROJ_PARSER_H__

