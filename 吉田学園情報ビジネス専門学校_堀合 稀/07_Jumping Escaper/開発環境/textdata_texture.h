//=============================================================================
//
// テキストデータ（テクスチャ）管理 [textdata_texture.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTDATA_TEXTURE_H_
#define _TEXTDATA_TEXTURE_H_

#include "main.h"
#include "textdata.h"

//==============================================================================
// テキスト管理（テクスチャ）クラス
//==============================================================================
class CTextDataTexture : public CTextData
{
public:
	CTextDataTexture();
	~CTextDataTexture();

	void LoadData(void);
	void Unload(void);

	static CTextDataTexture *Create(void);

	int GetNumTexture(void) { return m_nNumTexture; }
	char *GetFileName(int nIdx) { return m_pFileTextureName[nIdx]; }

	char *GetTypeCheck(int nIdx){ return m_pTypeCheck[nIdx]; }

private:
	int m_nNumTexture;			// テクスチャの数
	char **m_pFileTextureName;	// ファイル名を格納するポインタ
	int *m_pIdx;				// テクスチャのインデックス

	char **m_pTypeCheck;		// 種類識別用文字列
};
#endif