//=============================================================================
//
// 影モデルの設定 [shadowS.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _SHADOWS_H_
#define _SHADOWS_H_

#include "main.h"
#include "scene3D.h"
#include "sceneX.h"

//==============================================================================
// 影モデルクラス
//==============================================================================
class CShadowS : public CSceneX
{
public:
	CShadowS();
	~CShadowS();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CShadowS *Create(D3DXVECTOR3 pos);

	void SetPosition(D3DXVECTOR3 pos);

private:
	//D3DXVECTOR3 m_pos;				// 位置
	//D3DXVECTOR3 m_rot;				// 角度
	LPD3DXMESH m_pMesh;				// メッシュポインタ
	LPD3DXBUFFER m_pBuffMat;		// マテリアルバッファ
	DWORD m_nNumMat;				// マテリアル数
	//D3DXMATRIX m_mtxWorld;			// ワールドマトリックス

	// 2Dポリゴン
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// バッファへのポインタ
	D3DXVECTOR3 m_posPolygon;						// ポリゴンの位置
	D3DXVECTOR2 m_sizePolygon;						// ポリゴンの大きさ
};	
#endif