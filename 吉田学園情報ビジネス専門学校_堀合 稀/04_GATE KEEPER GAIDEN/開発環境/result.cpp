
//==============================================================================
//
// リザルトモード処理〔result.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "gamepad.h"
#include "result.h"
#include "bg.h"
#include "fade.h"
#include "result_logo.h"
#include "result_score.h"
#include "sound.h"

// 静的メンバ変数宣言
CResult::ENDSTATE CResult::m_endState;	// ゲーム終了時の状態

//==============================================================================
// コンストラクタ
//==============================================================================
CResult::CResult()
{
	
}

//==============================================================================
// デストラクタ
//==============================================================================
CResult::~CResult()
{

}

//==============================================================================
// リザルト画面の初期化処理
//==============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos)
{
	// 背景の生成
	CBg::Create();

	// ロゴの生成
	CResultLogo::Create(R_LOGO_POS);

	// リザルトスコアの生成
	CResultScore::Create(RESULT_SCORE_POS);

	// BGMの再生
	if (CManager::GetBGMPlay(CSound::BGMTYPE_RESULT) == true)
	{
		CSound::Play(CSound::SOUND_LABEL_BGM002);
	}

	return S_OK;
}

//==============================================================================
// リザルト画面の終了処理
//==============================================================================
void CResult::Uninit(void)
{
	// BGMの停止
	CSound::Stop();

	Release();	// インスタンスのデリート
}

//==============================================================================
// リザルト画面の更新処理
//==============================================================================
void CResult::Update(void)
{
	// フェード情報の取得 
	CFade::FADE fade = CFade::GetFade();

	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// シーン遷移
	if (keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true)
	{
		if (fade == CFade::FADE_NONE && CResultScore::GetShuffleCnt() > 150)
		{
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RANKING);
		}
		else if (fade == CFade::FADE_IN)
		{// フェードインのカット
			CFade::FadeInCancel();
		}

		if (CResultScore::GetShuffleCnt() < 150 && fade == CFade::FADE_NONE)
		{// カウントの短縮
			CResultScore::SetShuffleCnt(150);
		}
	}
}

//==============================================================================
// リザルト画面の描画処理
//==============================================================================
void CResult::Draw(void)
{

}


//==============================================================================
// リザルトモードの生成処理
//==============================================================================
CResult *CResult::Create()
{
	// インスタンス生成
	CResult *pResult;
	pResult = new CResult;
	D3DXVECTOR3 pos;

	// 初期化
	pResult->Init(pos);

	return pResult;
}

//==============================================================================
// ゲーム終了時の状態設定
//==============================================================================
void CResult::SetEndState(ENDSTATE endState)
{
	m_endState = endState;
}

//==============================================================================
// 位置の取得
//==============================================================================
D3DXVECTOR3 CResult::GetPosition(void)
{
	D3DXVECTOR3 pos;
	return pos;
}

//==============================================================================
// サイズの取得
//==============================================================================
D3DXVECTOR3 CResult::GetSize(void)
{
	D3DXVECTOR3 size;
	return size;
}