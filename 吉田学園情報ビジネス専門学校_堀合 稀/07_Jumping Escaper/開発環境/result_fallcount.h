//=============================================================================
//
// リザルトの落下カウント描画処理 [result_fallcount.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _RESULT_FALLCOUNT_H_
#define _RESULT_FALLCOUNT_H_

#include "main.h"
#include "scene.h"
#include "result.h"
#include "ranking.h"
#include "score.h"

// マクロ定義
#define RESULT_FC_POS	(D3DXVECTOR3(250.0f, 525.0f, 0.0f))	// 通常遷移時の配置
#define RESULT_FC_SIZE	(D3DXVECTOR2(25.0f, 40.0f))			// 一桁分のサイズ

// 前方宣言
class CFallCount;	// 落下カウントクラス

//==============================================================================
// リザルトの落下カウント描画クラス
//==============================================================================
class CResultFallCount : public CResult
{
public:
	CResultFallCount(int nPriority = PRIORITY_UI);
	~CResultFallCount();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CResultFallCount *Create();

private:
	CFallCount *m_pFallCount;	// 落下カウントクラス
	int m_nFall;				// 結果落下カウント
};
#endif