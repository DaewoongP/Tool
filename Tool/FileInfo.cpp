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

	// PathRelativePathTo : szCurDirPath���� strFullPath�� ���� ��� ��θ� ����� szRelativePath�� �������ִ� �Լ�
	// ��, ���� ����̺� �������� ��� ���� ����

	PathRelativePathTo(szRelativePath,
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<IMGPATH*>& rPathInfoList)
{
	//CFileFind : MFC���� �����ϴ� ���� �� ��� ���� ���� Ŭ����
	CFileFind			Find;

	// ��� ���� ��� ������ ã�ƾ� �ϱ� ������ ��� ����
	wstring		wstrFilePath = wstrPath + L"\\*.*";

	// FindFile : �ش� ��ο� ������ ������ Ȯ�����ִ� �Լ�
	// ������ ������ true, ������ false

	// ���Ͽ� �����ϰ� ���� ���, FindNextFile �Լ��� ������ ȣ���ؾ� �Ѵ�.
	// FindNextFile �Լ��� ȣ���ϸ� ���ϵ� �� ���� ù ��° ���Ͽ� ������ ����

	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			// GetFilePath : ���� ã�� ��θ� ������ �Լ�
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathInfoList);
		}
		else
		{
			if (Find.IsSystem())
				continue;

			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR			szPathBuf[MAX_STR] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());

			// L"D:\\����ȯ\\135��\\Frame135\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack0.png"

			PathRemoveFileSpec(szPathBuf);

			//L"D:\\����ȯ\\135��\\Frame135\\Texture\\Stage\\Effect\\BossMultiAttack"

			pImgPath->iCount = DirFileCnt(szPathBuf);

			// GetFileTitle : ���� �̸��� ����
			wstring		wstrTextureName = Find.GetFileTitle().GetString();
			// wstrTextureName = L"BossMultiAttack0"

			// substr(����, ��) : ���ۿ��� ���� �ش��ϴ� ���ڿ��� ����
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";

			// wstrTextureName = L"BossMultiAttack%d.png"

			//PathCombine : 2, 3 �Ű� ������ �����Ͽ� 1�Ű� ������ �������
			PathCombine(szPathBuf, szPathBuf, wstrTextureName.c_str());
			// L"D:\\����ȯ\\135��\\Frame135\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			pImgPath->wstrPath = ConvertRelativePath(szPathBuf);
			// ..\\Frame135\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			PathRemoveFileSpec(szPathBuf);
			// ..\\Frame135\\Texture\\Stage\\Effect\\BossMultiAttack\\

			// PathFindFileName : ���ϸ��� ã�ų� ���� ������ �������� ã�Ƴ��� �Լ�
			pImgPath->wstrStateKey = PathFindFileName(szPathBuf);

			PathRemoveFileSpec(szPathBuf);
			// ..\\Frame135\\Texture\\Stage\\Effect\\

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
