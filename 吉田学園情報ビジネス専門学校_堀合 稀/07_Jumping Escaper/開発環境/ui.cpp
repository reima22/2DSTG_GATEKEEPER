//==============================================================================
//
// UIの描画処理〔ui.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "bonus.h"
#include "checkpoint_logo.h"
#include "fall.h"
#include "main.h"
#include "start_cntdown.h"
#include "timer.h"
#include "timeralert_logo.h"
#include "ui.h"

//==============================================================================
// 静的メンバ変数
//==============================================================================
CTimer			*CUi::m_pTimer = NULL;			// タイマーポインタ
CFall			*CUi::m_pFall = NULL;			// 落下数ポインタ
CBonus			*CUi::m_pBonus = NULL;			// ボーナスポインタ
CStartCntdown	*CUi::m_pStartCntdown = NULL;	// スタートカウントダウンポインタ
CCheckpointLogo	*CUi::m_pCheckPointLogo = NULL;	// チェックポイントポインタ
CAlertLogo		*CUi::m_pAlertLogo = NULL;		// アラートポインタ

//==============================================================================
// コンストラクタ
//==============================================================================
CUi::CUi(int nPriority) : CScene(nPriority)
{
}

//==============================================================================
// デストラクタ
//==============================================================================
CUi::~CUi()
{
}

//==============================================================================
// タイマーの初期化処理
//==============================================================================
HRESULT CUi::Init(void)
{
	// タイマーの配置
	if (m_pTimer == NULL)
	{
		m_pTimer = CTimer::Create();
	}

	// 落下数カウントの配置
	if (m_pFall == NULL)
	{
		m_pFall = CFall::Create();
	}

	// ボーナスカウントの配置
	if (m_pBonus == NULL)
	{
		m_pBonus = CBonus::Create();
	}

	// 開始時カウントダウンの生成
	if (m_pStartCntdown == NULL)
	{
		m_pStartCntdown = CStartCntdown::Create();
	}

	// チェックポイントロゴ
	if (m_pCheckPointLogo == NULL)
	{
		m_pCheckPointLogo = CCheckpointLogo::Create();
	}

	// タイマーアラートロゴ
	if (m_pAlertLogo == NULL)
	{
		m_pAlertLogo = CAlertLogo::Create();
	}

	return S_OK;
}

//==============================================================================
// UIの終了処理
//==============================================================================
void CUi::Uninit(void)
{
	// タイマーカウント
	if (m_pTimer != NULL)
	{
		m_pTimer->Uninit();
		m_pTimer = NULL;
	}

	// 落下数カウント
	if (m_pFall != NULL)
	{
		m_pFall->Uninit();
		m_pFall = NULL;
	}

	// ボーナスカウント
	if (m_pBonus != NULL)
	{
		m_pBonus->Uninit();
		m_pBonus = NULL;
	}

	// スタート時カウントダウン
	if (m_pStartCntdown != NULL)
	{
		m_pStartCntdown->Uninit();
		m_pStartCntdown = NULL;
	}

	// チェックポイントロゴ
	if (m_pCheckPointLogo != NULL)
	{
		m_pCheckPointLogo->Uninit();
		m_pCheckPointLogo = NULL;
	}

	// タイマーアラートロゴ
	if (m_pAlertLogo != NULL)
	{
		m_pAlertLogo->Uninit();
		m_pAlertLogo = NULL;
	}

	Release();
}

//==============================================================================
// UIの更新処理
//==============================================================================
void CUi::Update(void)
{

}

//==============================================================================
// UIの描画処理
//==============================================================================
void CUi::Draw(void)
{

}

//==============================================================================
// UIの生成処理
//==============================================================================
CUi *CUi::Create(void)
{
	// インスタンス生成
	CUi *pUi;
	pUi = new CUi;

	// 初期化
	if (pUi != NULL)
	{
		pUi->Init();
	}

	return pUi;
}