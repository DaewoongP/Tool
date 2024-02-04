#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR	szRelativePath[MAX_PATH] = L"";
	TCHAR	szCurDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	PathRelativePathTo(szRelativePath,
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<IMGPATH*>& rPathInfoList)
{
	CFileFind			Find;
	wstring		wstrFilePath = wstrPath + L"\\*.*";
	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathInfoList);
		}
		else
		{
			if (Find.IsSystem())
				continue;

			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR			szPathBuf[MAX_STR] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());
			PathRemoveFileSpec(szPathBuf);
			pImgPath->iCount = DirFileCnt(szPathBuf);

			// �߷�

			wstring		wstrTextureName = Find.GetFileTitle().GetString();
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
			PathCombine(szPathBuf, szPathBuf, wstrTextureName.c_str());
			pImgPath->wstrPath = ConvertRelativePath(szPathBuf);
			PathRemoveFileSpec(szPathBuf);
			pImgPath->wstrStateKey = PathFindFileName(szPathBuf);
			PathRemoveFileSpec(szPathBuf);
			pImgPath->wstrObjKey = PathFindFileName(szPathBuf);

			rPathInfoList.push_back(pImgPath);

			bContinue = false;
		}
	}
}

int CFileInfo::DirFileCnt(const wstring & wstrPath)
{
	wstring		wstrFilePath = wstrPath + L"\\*.*";

	CFileFind			Find;
	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	int	iFileCnt = 0;

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if (Find.IsSystem())
			continue;

		++iFileCnt;
	}


	return iFileCnt;
}

int CFileInfo::DirCnt(const wstring & wstrPath)
{
	wstring		wstrFilePath = wstrPath + L"\\*.*";

	CFileFind			Find;
	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	int	iDirCnt = 0;

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if (Find.IsSystem())
			continue;

		if (Find.IsDirectory())
			++iDirCnt;
	}

	return iDirCnt;
}

int CFileInfo::ExtractIntOfBackFromCString(CString & cstr)
{
	int i = 0;
	for (; i < cstr.GetLength(); ++i)
	{
		// isdigit : �Ű� ������ �Ѱܹ��� ���ڰ� ������ �������� ������ �����α� �������ִ� �Լ�
		// ���� �� ���ڶ� �Ǵ��� ��� 0�� �ƴ� ���� ��ȯ
		if (0 != isdigit(cstr[i]))
			break;
	}
	cstr.Delete(0, i);
	
	return _tstoi(cstr);
}
