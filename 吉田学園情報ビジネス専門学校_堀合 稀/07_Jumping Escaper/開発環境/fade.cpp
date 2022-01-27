//==============================================================================
//
// フェード処理〔fade.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "fade.h"
#include "gamepad.h"
#include "input.h"
#include "manager.h"
#include "polygon.h"
#include "renderer.h"
#include "scene2D.h"

// 静的メンバ変数
CPolygon *CFade::m_pPolygon = NULL;
CFade::FADE CFade::m_fade = FADE_IN;
CMode::MODE CFade::m_modeNext;
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
			0);

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
	CMode *pMode;
	pMode = CManager::GetModePoint();

	CMode::MODE mode;
	mode = pMode->GetMode();

	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	if (m_fade == FADE_IN)
	{
		// 透明度の低下
		m_col.a -= 0.01f;

		if (m_col.a <= 0.0f)
		{// フェードイン処理をキー入力で短縮
			if (mode != CMode::MODE_GAME && (keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true))
			{
				m_col.a = 0.0f;
				m_fade = FADE_NONE;
			}
			else
			{
				m_col.a = 0.0f;
				m_fade = FADE_NONE;
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
			pMode->SetMode(m_modeNext);
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
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = CManager::GetRenderer()->GetDevice();

	// フォグを無効化
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	if (m_pPolygon != NULL)
	{
		m_pPolygon->Draw();
	}

	// フォグを有効化
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
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
	if (pFade != NULL)
	{
		pFade->Init();
	}

	return pFade;
}

//==============================================================================
// フェードの設定
//==============================================================================
void CFade::SetFade(FADE fade, CMode::MODE modeNext)
{
	m_fade = fade;
	m_modeNext = modeNext;
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