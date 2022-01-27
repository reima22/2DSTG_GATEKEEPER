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

//==============================================================================
// ポーズベースクラス
//==============================================================================
class CPauseBase : public CScene
{
public:
	CPauseBase(int nPriority = CScene::PRIORITY_PAUSE);
	~CPauseBase();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPauseBase *Create(void);

private:
	CScene2D *m_pScene2D;	// 2Dポリゴンのポインタ
};
#endif
