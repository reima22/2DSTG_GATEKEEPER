//=============================================================================
//
// 2Dポリゴン描画処理 [scene2D.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"
#include "texture.h"

//==============================================================================
// 2Dポリゴンクラス
//==============================================================================
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority = PRIORITY_DEFAULT);
	~CScene2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScene2D *Create(void);	// 2Dポリゴンの生成

	// 位置の取得・設定
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }

	// サイズの取得・設定
	D3DXVECTOR2 GetSize(void) { return m_size; }
	void SetSize(D3DXVECTOR2 size);

	// 角度の取得・設定
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }

	// テクスチャの設定
	void BindTexture(int nIdx);

	void BindTextureDirect(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }	// 直接設定

	// テクスチャ座標の設定
	void SetTex(int nCutX, int nCutY, int nPatternX, int nPatternY, float fTexX, float fTexY);

	void SetTexUV(D3DXVECTOR2 Vertex_0, D3DXVECTOR2 Vertex_1, D3DXVECTOR2 Vertex_2, D3DXVECTOR2 Vertex_3);
	
	// 色情報の設定
	void SetCol(D3DXCOLOR col);

private:
	D3DXVECTOR3 m_pos;								// 位置
	D3DXVECTOR3 m_rot;								// 角度
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// バッファへのポインタ

	float m_fLength;								// 対角線の長さ
	float m_fAngle;									// 対角線の角度
	D3DXVECTOR2 m_size;								// ポリゴンのサイズ
};

#endif
