//=============================================================================
//
// テクスチャ管理 [texture.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

//==============================================================================
// テクスチャ管理クラス
//==============================================================================
class CTexture
{
public:
	CTexture();
	~CTexture();

	HRESULT LoadAll(void);
	void UnloadAll(void);

	static CTexture *Create(void);

	LPDIRECT3DTEXTURE9 GetAddress(int nIdx) { return *(LPDIRECT3DTEXTURE9*)&m_pTexture[nIdx]; }

	// 3Dモデルへのテクスチャ割り当て
	LPDIRECT3DTEXTURE9 GetAddress3D(char *pTexFilename);

	int GetTexNum(void) { return m_nTexMax; }

private:
	char **m_pFileName;				// 保管用ポインタ
	LPDIRECT3DTEXTURE9 m_pTexture;	// ポインタ配列格納アドレス
	int m_nTexMax;					// テクスチャの最大数
};

#endif
