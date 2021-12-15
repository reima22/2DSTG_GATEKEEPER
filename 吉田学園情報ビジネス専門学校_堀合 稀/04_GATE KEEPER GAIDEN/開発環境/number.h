//=============================================================================
//
// 数値描画処理 [number.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"
#include "scene.h"

//==============================================================================
// ナンバークラス
//==============================================================================
class CNumber
{
public:
	CNumber();
	~CNumber();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetNumber(int nNumber);

	static CNumber *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);

	void SetCol(D3DXCOLOR col);
	void SetSize(D3DXVECTOR2 size);

	D3DXVECTOR2 GetSize(void) { return m_size; }

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// バッファへのポインタ
	D3DXVECTOR3 m_pos;								// 位置
	float m_fLength;								// 対角線の長さ
	float m_fAngle;									// 対角線の角度
	D3DXVECTOR2 m_size;								// サイズ
};

#endif