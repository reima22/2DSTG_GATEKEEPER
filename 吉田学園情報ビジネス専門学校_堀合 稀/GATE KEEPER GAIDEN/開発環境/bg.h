//=============================================================================
//
// 背景描画処理 [bg.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _BG_H_
#define _BG_H_

#include "main.h"
#include "scene.h"

// マクロ定義
#define BG_MAX	(3)

// 前方宣言
class CScene2D;

//==============================================================================
// 背景クラス
//==============================================================================
class CBg : public CScene
{
public:
	CBg(int nPriority = PRIORITY_BG);
	~CBg();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void);

	static CBg *Create(void);

private:
	CScene2D *m_apScene2D[BG_MAX];	// CScene2Dのポインタ
	float m_fTexX[BG_MAX];			// テクスチャX軸
	float m_fTexY[BG_MAX];			// テクスチャY軸
	static D3DXVECTOR3 m_pos;		// 背景の中心位置
};

#endif