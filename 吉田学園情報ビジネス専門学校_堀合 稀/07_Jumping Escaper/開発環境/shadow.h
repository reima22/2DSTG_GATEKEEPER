//==============================================================================
//
// 影の描画〔shadow.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _SHADOW_H_
#define _SHADOW_H_
#include "main.h"
#include "scene3D.h"

//==============================================================================
// 影クラス
//==============================================================================
class CShadow : public CScene
{
public:
	CShadow(int nPriority = PRIORITY_SHADOW);
	~CShadow();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CShadow *Create(D3DXVECTOR2 size);

	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }

	D3DXVECTOR2 GetSize(void) { return m_size; }
	void SetSize(D3DXVECTOR2 size) { m_size = size; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// バッファへのポインタ
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;		// テクスチャへのポインタ
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_rot;							// 角度
	D3DXVECTOR2 m_size;							// 大きさ
};

#endif
