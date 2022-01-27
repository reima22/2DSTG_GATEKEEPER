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
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 数値設定
	void SetNumber(int nNumber);

	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);

	// 色の設定
	void SetCol(D3DXCOLOR col);

	// 位置の取得・設定
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }

	// サイズの取得・設定
	D3DXVECTOR2 GetSize(void) { return m_size; }
	void SetSize(D3DXVECTOR2 size);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// バッファへのポインタ
	D3DXVECTOR3 m_pos;								// 位置
	D3DXVECTOR2 m_size;								// サイズ
	float m_fLength;								// 対角線の長さ
	float m_fAngle;									// 対角線の角度
};

#endif