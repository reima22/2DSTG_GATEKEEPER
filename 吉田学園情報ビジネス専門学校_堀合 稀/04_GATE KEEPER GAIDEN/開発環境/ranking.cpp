//==============================================================================
//
// ランキングモード処理〔ranking.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "gamepad.h"
#include "bg.h" 
#include "fade.h"
#include "title.h"
#include "ranking.h"
#include "ranking_logo.h"
#include "ranking_rank.h"
#include "ranking_score.h"
#include "sound.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CRanking::CRanking()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CRanking::~CRanking()
{
}

//==============================================================================
// ランキング画面の初期化処理
//==============================================================================
HRESULT CRanking::Init(D3DXVECTOR3 pos)
{
	// タイトルから遷移したか
	m_bFromTitle = CTitle::GetAutoFade();

	// 自動遷移カウントの初期化
	m_nAutoFadeCnt = 0;
	
	// 背景生成
	CBg::Create();

	// 遷移元による分岐
	if (m_bFromTitle == true)
	{// タイトルからの遷移
		// ロゴ生成
		CRankingLogo::Create(RANK_LOGO_POS_T);

		// スコアの生成
		CRankingScore::Create(RANK_SCORE_POS_T);

		// 順位の生成
		CRankingRank::Create(RANK_POS_T);
	}
	else
	{// 通常遷移
	 // ロゴ生成
		CRankingLogo::Create(RANK_LOGO_POS);

		// スコアの生成
		CRankingScore::Create(RANK_SCORE_POS);

		// 順位の生成
		CRankingRank::Create(RANK_POS);
	}

	// BGM再生
	if (CManager::GetBGMPlay(CSound::BGMTYPE_RANKING) == true)
	{
		CSound::Play(CSound::SOUND_LABEL_BGM005);
	}
	

	return S_OK;
}

//==============================================================================
// ランキング画面の終了処理
//==============================================================================
void CRanking::Uninit(void)
{
	// BGMの停止
	CSound::Stop();

	Release();	// インスタンスのデリート
}

//==============================================================================
// ランキング画面の更新処理
//==============================================================================
void CRanking::Update(void)
{
	// フェード情報の取得 
	CFade::FADE fade = CFade::GetFade();

	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	if (keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true)
	{
		if (fade == CFade::FADE_NONE)
		{
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);	// シーン遷移
		}
		else if (fade == CFade::FADE_IN)
		{
			// フェードインのカット
			CFade::FadeInCancel();
		}
	}

	if (m_bFromTitle == true)
	{
		m_nAutoFadeCnt++;

		if (m_nAutoFadeCnt >= AUTO_FADE_CNT_T)
		{
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);	// シーン遷移
		}
	}
}

//==============================================================================
// ランキング画面の描画処理
//==============================================================================
void CRanking::Draw(void)
{

}

//==============================================================================
// ランキングモード生成処理
//==============================================================================
CRanking *CRanking::Create()
{
	// インスタンス生成
	CRanking *pRanking;
	pRanking = new CRanking;
	D3DXVECTOR3 pos;

	// 初期化
	pRanking->Init(pos);

	return pRanking;
}

//==============================================================================
// 位置の取得
//==============================================================================
D3DXVECTOR3 CRanking::GetPosition(void)
{
	D3DXVECTOR3 pos;
	return pos;
}

//==============================================================================
// サイズの取得
//==============================================================================
D3DXVECTOR3 CRanking::GetSize(void)
{
	D3DXVECTOR3 size;
	return size;
}