//==============================================================================
//
// ゲームモード管理〔mode.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "mode.h"
#include "title.h"
#include "select.h"
#include "game.h"
#include "result.h"
#include "ranking.h"

//==============================================================================
// 静的メンバ変数
//==============================================================================
CTitle *CMode::m_pTitle = NULL;					// タイトルモード
CGame *CMode::m_pGame = NULL;					// ゲームモード
CResult *CMode::m_pResult = NULL;				// リザルトモード
CRanking *CMode::m_pRanking = NULL;				// ランキングモード
CStageSelect *CMode::m_pStageSelect = NULL;		// ステージセレクトモード
CMode::MODE CMode::m_mode = MODE_TITLE;			// 現在のゲームモード

//==============================================================================
// コンストラクタ
//==============================================================================
CMode::CMode(int nPriority) : CScene(nPriority)
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CMode::~CMode()
{

}

//==============================================================================
// 初期化
//==============================================================================
HRESULT CMode::Init(void)
{
	return S_OK;
}

//==============================================================================
// 終了
//==============================================================================
void CMode::Uninit(void)
{

}

//==============================================================================
// 更新
//==============================================================================
void CMode::Update(void)
{

}

//==============================================================================
// 描画
//==============================================================================
void CMode::Draw(void)
{

}

//==============================================================================
// 生成処理
//==============================================================================
CMode *CMode::Create(void)
{
	// ローカル変数宣言
	CMode *pMode;
	pMode = new CMode;

	// モードの設定
	if (pMode != NULL)
	{
		pMode->SetMode(m_mode);
	}

	return pMode;
}

//==============================================================================
// 現在のシーンモードの設定
//==============================================================================
void CMode::SetMode(MODE mode)
{
	// モードの破棄
	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}
		break;

	case MODE_SELECT:
		if (m_pStageSelect != NULL)
		{
			m_pStageSelect->Uninit();
			m_pStageSelect = NULL;
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();
			m_pResult = NULL;
		}
		break;

	case MODE_RANKING:
		if (m_pRanking != NULL)
		{
			m_pRanking->Uninit();
			m_pRanking = NULL;
		}
		break;

	default:
		break;
	}

	// オブジェクト破棄
	CScene::ReleaseAll();

	// 死亡フラグ更新
	CScene::UpdateAll();

	// モードの設定
	m_mode = mode;

	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle == NULL)
		{
			m_pTitle = CTitle::Create();
		}
		break;

	case MODE_SELECT:
		if (m_pStageSelect == NULL)
		{
			m_pStageSelect = CStageSelect::Create();
		}
		break;

	case MODE_GAME:
		if (m_pGame == NULL)
		{
			m_pGame = CGame::Create();
		}
		break;

	case MODE_RESULT:
		if (m_pResult == NULL)
		{
			m_pResult = CResult::Create();
		}
		break;

	case MODE_RANKING:
		if (m_pRanking == NULL)
		{
			m_pRanking = CRanking::Create();
		}
		break;

	default:
		break;
	}
}