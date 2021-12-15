//=============================================================================
//
// 3Dモデルの設定 [scene3D.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"

//==============================================================================
// Scene3Dクラス
//==============================================================================
class CScene3D : public CScene
{
public:
	CScene3D();
	~CScene3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScene3D *Create(void);



	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

private:
	D3DXVECTOR3 m_pos;								// 位置
	D3DXVECTOR3 m_rot;								// 向き
	D3DXVECTOR3 m_size;								// 大きさ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// バッファへのポインタ
	D3DXMATRIX m_mtxWorld;							// ワールドマトリックス
};
#endif