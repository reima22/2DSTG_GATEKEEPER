
//==============================================================================
//
// チュートリアルモード処理〔tutorial.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "gamepad.h"
#include "tutorial.h"
#include "bg.h" 
#include "fade.h"
#include "tutorial_page.h"
#include "sound.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CTutorial::CTutorial()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CTutorial::~CTutorial()
{
}

//==============================================================================
// タイトル画面の初期化処理
//==============================================================================
HRESULT CTutorial::Init(D3DXVECTOR3 pos)
{
	// チュートリアルページの生成
	CTutorialPage::Create(SCREEN_CENTER);

	// BGM再生
	if (CManager::GetBGMPlay(CSound::BGMTYPE_TUTORIAL) == true)
	{
		CSound::Play(CSound::SOUND_LABEL_BGM006);
	}
	return S_OK;
}

//==============================================================================
// タイトル画面の終了処理
//==============================================================================
void CTutorial::Uninit(void)
{
	// BGMの停止
	CSound::Stop();

	Release();	// インスタンスのデリート
}

//==============================================================================
// タイトル画面の更新処理
//==============================================================================
void CTutorial::Update(void)
{
	// フェード情報の取得 
	CFade::FADE fade = CFade::GetFade();

	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	int nPageCnt = CTutorialPage::GetPageCnt();

	if (keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true)
	{
		if (fade == CFade::FADE_NONE && nPageCnt <= 0)
		{
			CSound::Play(CSound::SOUND_LABEL_SE_START000);			// サウンド再生
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_GAME);	// シーン遷移
		}
		else if (fade == CFade::FADE_IN)
		{
			// フェードインのカット
			CFade::FadeInCancel();
		}
	}
}

//==============================================================================
// タイトル画面の描画処理
//==============================================================================
void CTutorial::Draw(void)
{

}

//==============================================================================
// タイトルモード生成処理
//==============================================================================
CTutorial *CTutorial::Create()
{
	// インスタンス生成
	CTutorial *pTutorial;
	pTutorial = new CTutorial;
	D3DXVECTOR3 pos;

	// 初期化
	pTutorial->Init(pos);

	return pTutorial;
}

//==============================================================================
// 位置の取得
//==============================================================================
D3DXVECTOR3 CTutorial::GetPosition(void)
{
	D3DXVECTOR3 pos;
	return pos;
}

//==============================================================================
// サイズの取得
//==============================================================================
D3DXVECTOR3 CTutorial::GetSize(void)
{
	D3DXVECTOR3 size;
	return size;
}