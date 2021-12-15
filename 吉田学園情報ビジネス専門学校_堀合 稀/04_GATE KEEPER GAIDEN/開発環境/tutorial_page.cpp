//==============================================================================
//
// チュートリアルページ描画処理〔tutorial_page.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "polygon.h"
#include "sound.h"
#include "tutorial_page.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"

// 静的メンバ変数
int CTutorialPage::m_nPageCnt;
D3DXCOLOR CTutorialPage::m_col[TUTORIAL_PAGE_MAX];

//==============================================================================
// コンストラクタ
//==============================================================================
CTutorialPage::CTutorialPage(int nPriority) : CScene(nPriority)
{
	// ポリゴンの初期化
	for (int nCnt = 0; nCnt < TUTORIAL_PAGE_MAX; nCnt++)
	{
		m_pPolygon[nCnt] = NULL;
		m_col[nCnt] = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	}

	m_nPageCnt = TUTORIAL_PAGE_MAX;
}

//==============================================================================
// デストラクタ
//==============================================================================
CTutorialPage::~CTutorialPage()
{
}

//==============================================================================
// チュートリアルページの初期化処理
//==============================================================================
HRESULT CTutorialPage::Init(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < TUTORIAL_PAGE_MAX; nCnt++)
	{
		if (m_pPolygon[nCnt] == NULL)
		{
			m_pPolygon[nCnt] = CPolygon::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f),
				SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
				(CTexture::TEXTYPE)((int)CTexture::TEXTYPE_TUTORIAL_2 - nCnt));

			m_pPolygon[nCnt]->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
		}
	}

	return S_OK;
}

//==============================================================================
// チュートリアルページの終了処理
//==============================================================================
void CTutorialPage::Uninit(void)
{
	for (int nCnt = 0; nCnt < TUTORIAL_PAGE_MAX; nCnt++)
	{
		if (m_pPolygon[nCnt] != NULL)
		{// NULLチェック後に破棄
			m_pPolygon[nCnt]->Uninit();
			m_pPolygon[nCnt] = NULL;
		}
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// チュートリアルページの更新処理
//==============================================================================
void CTutorialPage::Update(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// フェードの取得
	CFade::FADE fade = CFade::GetFade();

	// ローカル変数宣言
	D3DXCOLOR col[TUTORIAL_PAGE_MAX];

	 // フェードの状態
	if (fade != CFade::FADE_IN)
	{
		if (keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true)
		{
			m_nPageCnt--;
			if (m_nPageCnt > 0)
			{
				m_col[m_nPageCnt].a = 0.0f;
			}			
		}
		if (m_nPageCnt < TUTORIAL_PAGE_MAX)
		{
			if (keyboard->GetTrigger(CInput::KEYINFO_TUTO_BACK) == true || gamepad->GetTrigger(CInput::KEYINFO_TUTO_BACK) == true)
			{
				m_col[m_nPageCnt].a = 1.0f;
				m_nPageCnt++;
			}
		}		
	}


	// 色の設定
	for (int nCnt = 0; nCnt < TUTORIAL_PAGE_MAX; nCnt++)
	{
		m_pPolygon[nCnt]->SetCol(m_col[nCnt]);
	}
}

//==============================================================================
// チュートリアルページの描画処理
//==============================================================================
void CTutorialPage::Draw(void)
{
	for (int nCnt = 0; nCnt < TUTORIAL_PAGE_MAX; nCnt++)
	{
		if (m_pPolygon[nCnt] != NULL)
		{
			m_pPolygon[nCnt]->Draw();
		}
	}
}

//==============================================================================
// チュートリアルページの生成処理
//==============================================================================
CTutorialPage *CTutorialPage::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CTutorialPage *pTutorialPage;
	pTutorialPage = new CTutorialPage;

	// 初期化
	pTutorialPage->Init(pos);

	return pTutorialPage;
}

//==============================================================================
// 位置の取得
//==============================================================================
D3DXVECTOR3 CTutorialPage::GetPosition(void)
{
	D3DXVECTOR3 pos;
	return pos;
}

//==============================================================================
// サイズの取得
//==============================================================================
D3DXVECTOR3 CTutorialPage::GetSize(void)
{
	D3DXVECTOR3 size;
	return size;
}