//==============================================================================
//
// 球の描画〔sphere.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "main.h"
#include "player.h"

// メッシュフィールドクラス
class CSphere : public CPlayer
{
public:
	CSphere();
	~CSphere();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CSphere *Create(void);

	// 各パラメータの設定
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	void SetSlice(int nSlice) { m_nSlice = nSlice; }
	void SetStack(int nStack) { m_nStack = nStack; }

private:
	LPD3DXMESH m_pMeshSphere = NULL;			// 球体用メッシュ
	LPD3DXBUFFER m_pBuffMatSphere = NULL;		// 球体マテリアルバッファ
	D3DXMATRIX m_sphereMtxWorld;						// ワールドマトリックス

	float m_fRadius;							// 球体半径
	int m_nSlice;								// 球体のスライス
	int m_nStack;								// 球体のスタック
	
	//D3DXVECTOR3 m_posSphire;					// 球体位置
};

#endif