//=============================================================================
//
// モデル管理 [model.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "scene3D.h"

// マクロ定義
#define VTX_MIN	(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))		// 仮頂点最小値
#define	VTX_MAX	(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))	// 仮頂点最大値
#define COLLISION_NUM	(4)	// 四方面の当たり判定数

//==============================================================================
// モデル管理クラス
//==============================================================================
class CModel
{
public:
	CModel();
	~CModel();
	HRESULT Init(char *pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CModel *Create(char *pFileName,D3DXVECTOR3 pos,D3DXVECTOR3 rot);

	void SetCol(D3DXCOLOR col);
	D3DXCOLOR GetCol(void) { return m_col; }

	// 当たり判定用設定
	void SetCollisionInfo(void);

	void SetParent(CModel *pModel) { m_pParent = pModel; }	// 親ポインタの設定
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }		// マトリックスの取得

	// 位置の設定
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_pos; }

	// 角度の設定
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }

	// 影のサイズ設定
	D3DXVECTOR2 SetShadowSize(void);	

	// 4辺最小値の取得
	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMin; }

	// 4辺最大値の取得
	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; }

	// 4頂点の取得
	D3DXVECTOR3 GetPosVtx(int nIdx) { return m_aPosVtx[nIdx]; }

	// 4方向ベクトルの取得
	D3DXVECTOR3 GetVecVtx(int nIdx) { return m_aVecVtx[nIdx]; }

	// 移動力の取得・設定
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }

	// 過去の位置の取得・設定
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetPosOld(D3DXVECTOR3 pos) { m_posOld = pos; }

private:
	LPD3DXMESH m_pMesh;						// メッシュポインタ
	LPD3DXBUFFER m_pBuffMat;				// マテリアルバッファ
	DWORD m_nNumMat;						// マテリアル数

	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR3 m_posOld;					// 過去の位置
	D3DXVECTOR3 m_move;						// 移動力
	CModel *m_pParent;						// 親モデルへのポインタ

	LPDIRECT3DTEXTURE9 *m_pTexture;			// テクスチャダブルポインタ

	D3DXVECTOR3 m_vtxMin;					// モデルの大きさの空間座標最小値
	D3DXVECTOR3 m_vtxMax;					// モデルの大きさの空間座標最大値

	D3DXVECTOR2 m_shadowSize;				// 影のサイズ

	float m_aLength[COLLISION_NUM];			// 対角線の距離
	float m_aAngle[COLLISION_NUM];			// 4頂点への角度
	D3DXVECTOR3 m_aPosVtx[COLLISION_NUM];	// 頂点位置
	D3DXVECTOR3 m_aVecVtx[COLLISION_NUM];	// 4辺ベクトル

	D3DXCOLOR m_col;						// オブジェクトの色
};

#endif