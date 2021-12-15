//=============================================================================
//
// ポーズ背景処理 [pause_bg.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _PAUSE_BG_H_
#define _PAUSE_BG_H_

#include "main.h"
#include "pause.h"

// マクロ定義
#define P_BG_MAX	(2)	// 背景数

// 前方宣言
class CPolygon;

//==============================================================================
// ポーズクラス
//==============================================================================
class CPauseBg : public CScene2D
{
public:
	CPauseBg(int nPriority = CScene::PRIORITY_PAUSE);
	~CPauseBg();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPauseBg *Create(D3DXVECTOR3 pos);
};
#endif
