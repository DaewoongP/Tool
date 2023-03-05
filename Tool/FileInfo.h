#pragma once

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString		ConvertRelativePath(CString strFullPath);
	static void			DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathInfoList);
	static int			DirFileCnt(const wstring& wstrPath);
	static int			DirCnt(const wstring & wstrPath);
};