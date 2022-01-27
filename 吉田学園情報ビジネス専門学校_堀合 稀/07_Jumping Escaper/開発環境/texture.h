//=============================================================================
//
// テクスチャ管理 [texture.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

class CTextDataTexture;

//==============================================================================
// テクスチャ管理クラス
//==============================================================================
class CTexture
{
public:
	CTexture();
	~CTexture();

	HRESULT LoadAll(void);			// 全読み込み
	void UnloadAll(void);			// 全破棄
	static CTexture *Create(void);	// 生成処理

	// テクスチャの割り当て(文字列)
	LPDIRECT3DTEXTURE9 GetAddress(char *pTypeCheck);

	// テクスチャの割り当て(数値)
	LPDIRECT3DTEXTURE9 GetAddressNum(int nTexNum);

	// 3Dモデルへのテクスチャ割り当て
	LPDIRECT3DTEXTURE9 GetAddress3D(char *pTexFilename);

	// テクスチャテキストのデータ取得
	CTextDataTexture *GetData(void) { return m_pDataTexture; }

private:
	CTextDataTexture *m_pDataTexture;	// テクスチャデータポインタ
	char **m_pFileName;					// 保管用ポインタ
	LPDIRECT3DTEXTURE9 m_pTexture;		// ポインタ配列格納アドレス
	int m_nTexMax;						// テクスチャの最大数
};
#endif
