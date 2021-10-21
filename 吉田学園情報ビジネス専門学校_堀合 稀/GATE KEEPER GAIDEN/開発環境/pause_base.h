//=============================================================================
//
// ポーズベース処理 [pause_base.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _PAUSE_BASE_H_
#define _PAUSE_BASE_H_

#include "main.h"
#include "pause.h"

// 前方宣言
class CPolygon;

//==============================================================================
// ポーズベースクラス
//==============================================================================
class CPauseBase : public CScene2D
{
public:
	CPauseBase(int nPriority = CScene::PRIORITY_PAUSE);
	~CPauseBase();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPauseBase *Create(D3DXVECTOR3 pos);
};
#endif
