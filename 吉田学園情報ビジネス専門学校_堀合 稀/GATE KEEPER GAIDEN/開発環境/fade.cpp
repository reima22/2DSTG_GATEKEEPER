//==============================================================================
//
// フェード処理〔fade.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h"
#include "polygon.h"
#include "input.h"
#include "gamepad.h"

// 静的メンバ変数
CPolygon *CFade::m_pPolygon = NULL;
CFade::FADE CFade::m_fade = FADE_IN;
CManager::MODE CFade::m_modeNext;
D3DXCOLOR CFade::m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

//==============================================================================
// コンストラクタ
//==============================================================================
CFade::CFade()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CFade::~CFade()
{

}

//==============================================================================
// フェードの初期化処理
//==============================================================================
HRESULT CFade::Init(void)
{
	if (m_pPolygon == NULL)
	{
		m_pPolygon = 
			CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
			CTexture::TEXTYPE_NONE);

		m_pPolygon->SetCol(m_col);
	}

	return S_OK;
}

//==============================================================================
// フェードの終了処理
//==============================================================================
void CFade::Uninit(void)
{
	if (m_pPolygon != NULL)
	{// NULLチェック後に破棄
		m_pPolygon->Uninit();
		m_pPolygon = NULL;
	}
}

//==============================================================================
// フェードの更新処理
//==============================================================================
void CFade::Update(void)
{
	// ローカル変数宣言
	CManager::MODE mode;
	mode = CManager::GetMode();

	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	if (m_fade == FADE_IN)
	{
		// 透明度の低下
		m_col.a -= 0.01f;

		if (m_col.a <= 0.0f || keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true)
		{// フェードイン処理をキー入力で短縮
			if (mode != CManager::MODE_GAME)
			{
				m_col.a = 0.0f;
				m_fade = FADE_NONE;
			}
			else
			{
				if (m_col.a <= 0.0f)
				{// 自動でモード切替
					m_fade = FADE_NONE;
				}
			}
		}
	}
	else if (m_fade == FADE_OUT)
	{
		m_col.a += 0.01f;

		if (m_col.a >= 1.0f)
		{
			m_col.a = 1.0f;
			m_fade = FADE_IN;
			CManager::SetMode(m_modeNext);
		}
	}

	// 色の設定
	m_pPolygon->SetCol(m_col);
}

//==============================================================================
// フェードの描画処理
//==============================================================================
void CFade::Draw(void)
{
	if (m_pPolygon != NULL)
	{
		m_pPolygon->Draw();
	}
}

//==============================================================================
// フェードの生成処理
//==============================================================================
CFade *CFade::Create()
{
	// インスタンス生成
	CFade *pFade;
	pFade = new CFade;

	// 初期化
	pFade->Init();

	return pFade;
}

//==============================================================================
// フェードの設定
//==============================================================================
void CFade::SetFade(FADE fade, CManager::MODE modeNext)
{
	m_fade = fade;
	m_modeNext = modeNext;
}

//==============================================================================
// フェードの取得
//==============================================================================
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}

//==============================================================================
// フェードインのカット
//==============================================================================
void CFade::FadeInCancel(void)
{
	// 透明度の変更
	m_col.a = 0.0f;

	// フェードモードの切り替え
	m_fade = FADE_NONE;
}