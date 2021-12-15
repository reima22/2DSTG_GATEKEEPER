//==============================================================================
//
// タイトルロゴ描画処理〔title_logo.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "line.h"
#include "polygon.h"
#include "sound.h"
#include "invasion.h"
#include "title_logo.h"
#include "input.h"
#include "gamepad.h"

// 静的メンバ変数宣言
bool CTitleLogo::m_bLogoMoveEnd;

//==============================================================================
// コンストラクタ
//==============================================================================
CTitleLogo::CTitleLogo(int nPriority) : CScene(nPriority)
{
	// ポリゴンの初期化
	m_pPolygon = NULL;

	m_bLogoMoveEnd = false;
	m_nMoveCnt = 0;
}

//==============================================================================
// デストラクタ
//==============================================================================
CTitleLogo::~CTitleLogo()
{
}

//==============================================================================
// タイトルロゴの初期化処理
//==============================================================================
HRESULT CTitleLogo::Init(D3DXVECTOR3 pos)
{
	m_pos = pos;

	if (m_pPolygon == NULL)
	{
		m_pPolygon = CPolygon::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f),
			T_LOGO_SIZEX, T_LOGO_SIZEY,
			CTexture::TEXTYPE_TITLE_LOGO);

		m_pPolygon->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	}

	return S_OK;
}

//==============================================================================
// タイトルロゴの終了処理
//==============================================================================
void CTitleLogo::Uninit(void)
{
	if (m_pPolygon != NULL)
	{// NULLチェック後に破棄
		m_pPolygon->Uninit();
		m_pPolygon = NULL;
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// タイトルロゴの更新処理
//==============================================================================
void CTitleLogo::Update(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();
	
	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	if (m_bLogoMoveEnd == false)
	{
		// 移動カウント
		m_nMoveCnt++;

		// ロゴ移動の省略
		if (keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true)
		{
			m_pos.y += 2 * (140 - m_nMoveCnt);
			m_nMoveCnt += 140 - m_nMoveCnt;
		}

		if (m_nMoveCnt <= 140)
		{
			// 移動処理
			m_pos.y += 2;
		}
		else
		{
			// 移動終了状態
			m_bLogoMoveEnd = true;
		}
	}

	m_pPolygon->SetPosition(m_pos);
}

//==============================================================================
// タイトルロゴの描画処理
//==============================================================================
void CTitleLogo::Draw(void)
{
	if (m_pPolygon != NULL)
	{
		m_pPolygon->Draw();
	}
}

//==============================================================================
// タイトルロゴの生成処理
//==============================================================================
CTitleLogo *CTitleLogo::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CTitleLogo *pTitleLogo;
	pTitleLogo = new CTitleLogo;

	// 初期化
	pTitleLogo->Init(pos);

	return pTitleLogo;
}

//==============================================================================
// 位置の取得
//==============================================================================
D3DXVECTOR3 CTitleLogo::GetPosition(void)
{
	D3DXVECTOR3 pos;
	return pos;
}

//==============================================================================
// サイズの取得
//==============================================================================
D3DXVECTOR3 CTitleLogo::GetSize(void)
{
	D3DXVECTOR3 size;
	return size;
}