
//=============================================================================
//
// リザルトモード処理 [result.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "scene.h"
#include "mode.h"

// 前方宣言
class CResultPicture;		// リザルトに表示する図のクラス
class CResultScore;			// リザルトスコアのクラス
class CResultTimer;			// リザルトタイマークラス
class CResultBonus;			// リザルトボーナスクラス
class CResultFallCount;		// リザルト落下カウントクラス
class CResultScoreEx;		// 他リザルトスコアクラス
class CBg;					// 背景クラス

//==============================================================================
// リザルトモードクラス
//==============================================================================
class CResult : public CMode
{
public:
	// ゲーム終了時のクリア状態
	typedef enum
	{
		ENDSTATE_NONE = 0,		// 結果が出ていない
		ENDSTATE_CLEAR,			// クリア
		ENDSTATE_GAMEOVER,		// ゲームオーバー
		ENDSTATE_MAX
	}ENDSTATE;

	// 表示する図の種類
	typedef enum
	{
		PICTYPE_MAIN = 0,		// メインの表示
		PICTYPE_YOURSCORE,		// スコア表示ロゴ
		PICTYPE_TIMERLOGO,		// タイマーロゴ
		PICTYPE_COUNTLOGO,		// カウントロゴ
		PICTYPE_BONUSLOGO,		// ボーナスロゴ
		PICTYPE_TIMERMARK_0,	// タイマー算用記号0
		PICTYPE_TIMERMARK_1,	// タイマー算用記号1
		PICTYPE_TIMERMARK_2,	// タイマー算用記号2
		PICTYPE_COUNTMARK_0,	// カウント算用記号0
		PICTYPE_COUNTMARK_1,	// カウント算用記号1
		PICTYPE_COUNTMARK_2,	// カウント算用記号2
		PICTYPE_BONUSMARK_0,	// ボーナス算用記号0
		PICTYPE_BONUSMARK_1,	// ボーナス算用記号1
		PICTYPE_BONUSMARK_2,	// ボーナス算用記号2
		PICTYPE_MAX
	}PICTYPE;

	// 他スコアの種類
	typedef enum
	{
		EXSCORETYPE_TIMER = 0,		// タイマーのよるスコア
		EXSCORETYPE_TIMER_CALCU,	// タイマースコアの計算結果
		EXSCORETYPE_COUNT,			// 落下カウントによるスコア
		EXSCORETYPE_COUNT_CALCU,	// カウントスコアの計算結果
		EXSCORETYPE_BONUS,			// ボーナスカウントによるスコア
		EXSCORETYPE_BONUS_CALCU,	// ボーナススコアの計算結果
		EXSCORETYPE_MAX
	}EXSCORETYPE;

	CResult(int nPriority = PRIORITY_DEFAULT);
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CResult *Create(void);

	// スコア表記の更新
	void UpdateScore(void);

	// 背景更新
	void UpdateBg(void);

	static void SetEndState(ENDSTATE endState) { m_endState = endState; }
	static ENDSTATE GetEndState(void) { return m_endState; }

	// スコアの取得
	CResultScoreEx *GetScoreExPoint(EXSCORETYPE type) { return m_pResultScoreEx[type]; }

private:
	static CResultPicture *m_pResultPicture[PICTYPE_MAX];			// 画像ポインタ
	static CResultScore *m_pResultScore;							// リザルトスコアポインタ
	static CResultTimer *m_pResultTimer;							// リザルトタイマーポインタ
	static CResultFallCount *m_pResultFallCount;					// リザルト落下数ポインタ
	static CResultScoreEx *m_pResultScoreEx[EXSCORETYPE_MAX];		// 他リザルトスコアポインタ
	static CResultBonus * m_pResultBonus;							// リザルトボーナスポインタ

	CBg *m_pBg;					// 背景ポインタ
	static ENDSTATE m_endState;	// ゲーム終了時の状態
};
#endif
