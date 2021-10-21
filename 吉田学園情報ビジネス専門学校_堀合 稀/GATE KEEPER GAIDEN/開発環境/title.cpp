
//==============================================================================
//
// タイトルモード処理〔title.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "gamepad.h"
#include "title.h"
#include "bg.h" 
#include "fade.h"
#include "title_logo.h"
#include "title_enter.h"
#include "title_difficulty.h"
#include "sound.h"
#include "sceneX.h"

// 静的メンバ変数宣言
bool CTitle::m_bDifficult;	// 難易度選択開始判定
bool CTitle::m_bStart;		// ゲーム開始判定
bool CTitle::m_bAutoFade;	// 自動遷移判定

//==============================================================================
// コンストラクタ
//==============================================================================
CTitle::CTitle()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CTitle::~CTitle()
{
}

//==============================================================================
// タイトル画面の初期化処理
//==============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos)
{
	// 背景生成
	CBg::Create();

	// タイトルロゴの生成
	CTitleLogo::Create(T_LOGO_POS);

	// タイトルエンターサインの生成
	CTitleEnter::Create(T_ENTER_POS);

	// 難易度選択の生成
	CTitleDifficulty::Create(T_DIFFICULT_POS);

	// スタート判定
	m_bDifficult = false;
	m_bStart = false;

	// 自動遷移判定
	m_bAutoFade = false;
	m_nAutoFadeCnt = 0;

	// BGM再生
	if (CManager::GetBGMPlay(CSound::BGMTYPE_TITLE) == true)
	{
		CSound::Play(CSound::SOUND_LABEL_BGM000);
	}
	return S_OK;
}

//==============================================================================
// タイトル画面の終了処理
//==============================================================================
void CTitle::Uninit(void)
{
	// BGMの停止
	CSound::Stop();

	Release();	// インスタンスのデリート
}

//==============================================================================
// タイトル画面の更新処理
//==============================================================================
void CTitle::Update(void)
{
	// フェード情報の取得 
	CFade::FADE fade = CFade::GetFade();

	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// 自動遷移のカウント
	if (m_bDifficult == false)
	{
		m_nAutoFadeCnt++;
	}

	// 自動遷移への移行
	if (m_nAutoFadeCnt >= AUTO_FADE_CNT_R)
	{
		m_bAutoFade = true;
	}

	if (m_bAutoFade == false)
	{
		if (m_bDifficult == false)
		{
			if (keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true)
			{
				if (fade == CFade::FADE_NONE)
				{
					m_bDifficult = true;										// ゲームを開始
					CSound::Play(CSound::SOUND_LABEL_SE_START000);				// サウンド再生
				}
				else if (fade == CFade::FADE_IN)
				{
					// フェードインのカット
					CFade::FadeInCancel();
				}
			}
		}	
	}
	else if (m_bAutoFade == true)
	{// 自動遷移でランキング画面へ
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RANKING);
	}
}

//==============================================================================
// タイトル画面の描画処理
//==============================================================================
void CTitle::Draw(void)
{

}

//==============================================================================
// タイトルモード生成処理
//==============================================================================
CTitle *CTitle::Create()
{
	// インスタンス生成
	CTitle *pTitle;
	pTitle = new CTitle;
	D3DXVECTOR3 pos;

	// 初期化
	pTitle->Init(pos);

	return pTitle;
}

//==============================================================================
// 位置の取得
//==============================================================================
D3DXVECTOR3 CTitle::GetPosition(void)
{
	D3DXVECTOR3 pos;
	return pos;
}

//==============================================================================
// サイズの取得
//==============================================================================
D3DXVECTOR3 CTitle::GetSize(void)
{
	D3DXVECTOR3 size;
	return size;
}