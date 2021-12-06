//=============================================================================
//
// 3Dキャラの設定 [chara3D.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _CHARA3D_H_
#define _CHARA3D_H_

#include "main.h"
#include "scene3D.h"

//==============================================================================
// キャラクター3Dクラス
//==============================================================================
class CChara3D : public CScene3D
{
public:
	typedef enum
	{
		CHARA3DTYPE_MAIN = 0,	// メイン
		CHARA3DTYPE_SUB,		// サブパーツ
		CHARA3DTYPE_MAX
	}CHARA3DTYPE;

	CChara3D(int nPriority = PRIORITY_CHARA);
	~CChara3D();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CChara3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType);

private:
	static char *m_pFileName[CHARA3DTYPE_MAX];

	D3DXVECTOR3 m_pos;				// 位置
	D3DXVECTOR3 m_rot;				// 角度

	D3DXVECTOR3 m_move;				// 移動力
	D3DXVECTOR3 m_rotMove;			// 回転力
	LPD3DXMESH m_pMesh;				// メッシュポインタ
	LPD3DXBUFFER m_pBuffMat;		// マテリアルバッファ
	DWORD m_nNumMat;				// マテリアル数
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	LPDIRECT3DTEXTURE9 *m_pTexture;	// テクスチャダブルポインタ

	CHARA3DTYPE m_type;				// 種類
};
#endif
