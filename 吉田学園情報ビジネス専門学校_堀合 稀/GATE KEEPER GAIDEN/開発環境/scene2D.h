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
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosition(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPosition(void) { return m_pos; }

	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_fSizeX, m_fSizeY, 0.0f); }
	
	void BindTexture(CTexture::TEXTYPE texType);

	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }

	void SetSize(float fSizeX, float fSizeY);

	void SetTex(int nCutX, int nCutY, int nPatternX, int nPatternY, float fTexX, float fTexY);
	
	void SetCol(D3DXCOLOR col);

private:
	D3DXVECTOR3 m_pos;								// 位置
	D3DXVECTOR3 m_rot;								// 角度
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// バッファへのポインタ

	float m_fLength;								// 対角線の長さ
	float m_fAngle;									// 対角線の角度
	float m_fSizeX;									// 幅
	float m_fSizeY;									// 高さ
};

#endif
