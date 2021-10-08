//=============================================================================
//
// モデルXの設定 [sceneX.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _SCENEX_H_
#define _SCENEX_H_

#include "main.h"
#include "scene3D.h"

//==============================================================================
// SceneXクラス
//==============================================================================
class CSceneX : public CScene3D
{
public:
	typedef enum
	{
		X_TYPE_METEOR = 0,	// 隕石モデル
		X_TYPE_DEBRI_0,		// 宇宙ゴミ0
		X_TYPE_DEBRI_1,		// 宇宙ゴミ1
		X_TYPE_MAX
	}X_TYPE;

	CSceneX(int nPriority = PRIORITY_BG);
	~CSceneX();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rotMove,int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CSceneX *Create(D3DXVECTOR3 pos,D3DXVECTOR3 move,D3DXVECTOR3 rotMove,int nType);

private:
	D3DXVECTOR3 m_move;				// 移動力
	D3DXVECTOR3 m_rotMove;			// 回転力
	LPD3DXMESH m_pMesh;				// メッシュポインタ
	LPD3DXBUFFER m_pBuffMat;		// マテリアルバッファ
	DWORD m_nNumMat;				// マテリアル数
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャ情報
	X_TYPE m_type;					// 種類
};
#endif