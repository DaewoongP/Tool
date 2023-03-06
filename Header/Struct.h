#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;

	D3DXVECTOR3		vSize;

	D3DXMATRIX		matWorld;
}INFO;

typedef		struct tagTexture
{
	LPDIRECT3DTEXTURE9	pTexture;	// 객체, 이미지와 관련된 각종 기능을 제공하는 클래스

	D3DXIMAGE_INFO		tImgInfo;	// 구조체, 이미지와 관련된 실제적인 데이터를 저장하는 구조체

}TEXINFO;


typedef struct tagTile
{
	D3DXVECTOR3	vPos;
	D3DXVECTOR3	vSize;
	BYTE		byOption;
	BYTE		byDrawID;

	int			iIndex = 0;
	int			iParentIdx = 0;

}TILE;

typedef struct tagUnitData
{
	int			iType;
#ifndef _AFX
	wstring		strName;
#else
	CString		strName;
#endif // !_AFX
	int			iLayer;
	int			iAttack;
	int			iHp;
	bool		bCollision;
	wstring		wstrPath;

}UNITDATA;


static D3DXVECTOR3		Get_Mouse()
{
	POINT	Pt{};

	GetCursorPos(&Pt);
	ScreenToClient(g_hWnd, &Pt);

	return D3DXVECTOR3((float)Pt.x, (float)Pt.y, 0.f);
}


typedef	struct tagTexturePath
{
	wstring			wstrObjKey		= L"";
	wstring			wstrStateKey	= L"";
	wstring			wstrPath		= L"";
	int				iCount			= 0;

}IMGPATH;

typedef	struct tagFrame
{
	float	fFrame;	// 현재 애니메이션 재생 지점
	float	fMax;	// 최대 애니메이션 개수

}FRAME;

typedef struct tagSoundInfo
{
	TCHAR*	szName;
	CHANNELID eID;
	float	fVolume;

}SOUNDINFO;


typedef		struct tagLinePoint
{
	float	fX;
	float	fY;

	tagLinePoint(void) { ZeroMemory(this, sizeof(tagLinePoint)); }
	tagLinePoint(float _fX, float _fY) : fX(_fX), fY(_fY) {}

}MYPOINT;

typedef	struct tagLine
{
	MYPOINT		tLPoint;
	MYPOINT		tRPoint;

	tagLine(void) { ZeroMemory(this, sizeof(tagLine)); }
	tagLine(MYPOINT& tLeft, MYPOINT& tRight) : tLPoint(tLeft), tRPoint(tRight) {}

}LINE;

struct PENINFO
{
	HPEN	CurPen;
	HBRUSH	CurBrush;
	HPEN	OldPen;
	HBRUSH	OldBrush;
};
