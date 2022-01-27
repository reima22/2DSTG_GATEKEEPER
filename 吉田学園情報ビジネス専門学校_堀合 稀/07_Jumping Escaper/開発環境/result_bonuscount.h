//=============================================================================
//
// リザルトのボーナスカウント描画処理 [result_bonuscount.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _RESULT_BONUSCOUNT_H_
#define _RESULT_BONUSCOUNT_H_

#include "main.h"
#include "scene.h"
#include "result.h"
#include "ranking.h"
#include "score.h"

// マクロ定義
#define RESULT_BONUS_POS	(D3DXVECTOR3(250.0f, 650.0f, 0.0f))	// 通常遷移時の配置
#define RESULT_BONUS_SIZE	(D3DXVECTOR2(25.0f, 40.0f))			// 一桁分のサイズ

// 前方宣言
class CBonusCount;	// ボーナスカウントクラス

//==============================================================================
// リザルトのボーナスカウント描画クラス
//==============================================================================
class CResultBonus : public CResult
{
public:
	CResultBonus(int nPriority = PRIORITY_UI);
	~CResultBonus();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CResultBonus *Create(void);

private:
	CBonusCount *m_pBonusCount;	// ボーナスカウントクラス
	int m_nBonus;				// 結果ボーナスカウント
};
#endif