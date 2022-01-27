//=============================================================================
//
// ゲームモード管理処理 [mode.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _MODE_H_
#define _MODE_H_

#include "main.h"
#include "scene.h"

// 前方宣言
class CTitle;			// タイトルクラス
class CGame;			// ゲームクラス
class CResult;			// リザルトクラス
class CRanking;			// ランキングクラス	
class CStageSelect;		// ステージ選択クラス

//==============================================================================
// ゲームモード管理クラス
//==============================================================================
class CMode : public CScene
{
public:
	// ゲームモードの列挙
	typedef enum
	{
		MODE_TITLE = 0,	// タイトル
		MODE_SELECT,	// ステージ選択
		MODE_GAME,		// ゲーム
		MODE_RESULT,	// リザルト
		MODE_RANKING,	// ランキング
		MODE_MAX
	}MODE;

	CMode(int nPriority = PRIORITY_DEFAULT);
	~CMode();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMode *Create(void);

	// モード取得・設定
	MODE GetMode(void) { return m_mode; }
	void SetMode(MODE mode);

private:
	static CTitle *m_pTitle;				// タイトルポインタ
	static CStageSelect *m_pStageSelect;	// ステージセレクトポインタ
	static CGame *m_pGame;					// ゲームポインタ
	static CResult *m_pResult;				// リザルトポインタ
	static CRanking *m_pRanking;			// ランキングポインタ
	static MODE m_mode;						// シーンモード
};

#endif
