//=============================================================================
//
// ポリゴン描画処理 [polygon.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"
#include "scene.h"

//==============================================================================
// ポリゴンクラス
//==============================================================================
class CPolygon
{
public:
	CPolygon();
	~CPolygon();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, CTexture::TEXTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPolygon *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, CTexture::TEXTYPE type);

	void SetPosition(D3DXVECTOR3 pos);
	void SetTex(int nCutX, int nCutY, int nPatternX, int nPatternY, float fTexX, float fTexY);
	void SetCol(D3DXCOLOR col);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// バッファへのポインタ
	D3DXVECTOR3 m_pos;							// 位置
	float m_fLength;							// 対角線の長さ
	float m_fAngle;								// 対角線の角度
	CTexture::TEXTYPE m_Type;					// テクスチャの種類
};

#endif