//==============================================================================
//
// ポーズ処理〔pause.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "fade.h"
#include "game.h"
#include "gamepad.h"
#include "input.h"
#include "main.h"
#include "manager.h"
#include "pause.h"
#include "pause_base.h"
#include "pause_bg.h"
#include "pause_select.h"
#include "renderer.h"
#include "scene2D.h"
#include "sound.h"

// 静的メンバ変数
bool CPause::m_bPause;

//==============================================================================
// コンストラクタ
//==============================================================================
CPause::CPause(int nPriority) : CScene(nPriority)
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CPause::~CPause()
{

}

//==============================================================================
// ポーズの初期化処理
//==============================================================================
HRESULT CPause::Init(void)
{
	// ポーズの初期化
	m_bPause = false;

	// 背景の生成
	CPauseBg::Create();

	// ベースの生成
	CPauseBase::Create();

	// 選択肢の生成
	CPauseSelect::Create();

	return S_OK;
}

//==============================================================================
// ポーズの終了処理
//==============================================================================
void CPause::Uninit(void)
{
	
}

//==============================================================================
// ポーズの更新処理
//==============================================================================
void CPause::Update(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// フェード情報の取得
	CFade::FADE fade = CManager::GetFade()->GetFade();

	// フェード中、ゲーム終了演出を除き、処理する
	if (fade == CFade::FADE_NONE && CGame::GetGameEnd() == false)
	{
		if (keyboard->GetTrigger(CInput::KEYINFO_PAUSE) == true || gamepad->GetTrigger(CInput::KEYINFO_PAUSE) == true)
		{
			m_pSound->Play(CSound::SOUND_LABEL_SE_DECIDE);
			SetPause();
		}
	}
}

//==============================================================================
// ポーズの描画処理
//==============================================================================
void CPause::Draw(void)
{
	
}

//==============================================================================
// ポーズの生成処理
//==============================================================================
CPause *CPause::Create()
{
	// インスタンス生成
	CPause *pPause;
	pPause = new CPause;

	// 初期化
	pPause->Init();

	return pPause;
}

//==============================================================================
// ポーズの切り替え
//==============================================================================
void CPause::SetPause(void)
{
	// 選択状態の初期化
	CPauseSelect::SetPause(CPauseSelect::PAUSE_CONTINUE);

	// ポーズのオンオフ
	m_bPause = !m_bPause;
}