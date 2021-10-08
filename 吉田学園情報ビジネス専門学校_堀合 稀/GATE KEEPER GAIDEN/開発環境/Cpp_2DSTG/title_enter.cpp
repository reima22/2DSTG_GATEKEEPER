//==============================================================================
//
// エンターサイン描画処理〔title_enter.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "polygon.h"
#include "sound.h"
#include "title.h"
#include "title_logo.h"
#include "title_enter.h"
#include "title_difficulty.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CTitleEnter::CTitleEnter(int nPriority) : CScene(nPriority)
{
	// ポリゴンの初期化
	m_pPolygon = NULL;

	m_nAnimCnt = 0;
	m_nBlinkCnt = 0;
}

//==============================================================================
// デストラクタ
//==============================================================================
CTitleEnter::~CTitleEnter()
{
}

//==============================================================================
// エンターサインの初期化処理
//==============================================================================
HRESULT CTitleEnter::Init(D3DXVECTOR3 pos)
{
	// メンバー変数初期化
	m_pos = pos;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	if (m_pPolygon == NULL)
	{
		m_pPolygon = CPolygon::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f),
			T_ENTER_SIZEX, T_ENTER_SIZEY,
			CTexture::TEXTYPE_TITLE_ENTER);

		m_pPolygon->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	}

	return S_OK;
}

//==============================================================================
// エンターサインの終了処理
//==============================================================================
void CTitleEnter::Uninit(void)
{
	if (m_pPolygon != NULL)
	{// NULLチェック後に破棄
		m_pPolygon->Uninit();
		m_pPolygon = NULL;
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// エンターサインの更新処理
//==============================================================================
void CTitleEnter::Update(void)
{
	// ローカル変数宣言
	bool bEndLogo = CTitleLogo::GetLogoState();
	bool bDifficult = CTitle::GetDifficultStart();
	
	// エンターサイン点滅
	if (bEndLogo == true)
	{
		m_nAnimCnt++;

		if (bDifficult == false)
		{// スタート判定
			if (m_nAnimCnt % 60 == 0)
			{
				m_col.a = 0.0f;
			}
			else if (m_nAnimCnt % 30 == 0 && m_nAnimCnt % 60 != 0)
			{
				m_col.a = 1.0f;
			}
		}
		else
		{// 開始時
			if (CTitleDifficulty::GetSelect() == false)
			{
				m_nBlinkCnt++;

				if (m_nAnimCnt % 2 == 0)
				{
					m_col.a = 0.0f;
				}
				else
				{
					m_col.a = 1.0f;
				}

				if (m_nBlinkCnt >= 90)
				{
					CTitleDifficulty::SetSelect(true);
				}
			}
			else
			{
				m_col.a = 0.0f;
			}
		}
	}

	// 色の設定
	m_pPolygon->SetCol(m_col);
}

//==============================================================================
// エンターサインの描画処理
//==============================================================================
void CTitleEnter::Draw(void)
{
	if (m_pPolygon != NULL)
	{
		m_pPolygon->Draw();
	}
}

//==============================================================================
// エンターサインの生成処理
//==============================================================================
CTitleEnter *CTitleEnter::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CTitleEnter *pTitleEnter;
	pTitleEnter = new CTitleEnter;

	// 初期化
	pTitleEnter->Init(pos);

	return pTitleEnter;
}

//==============================================================================
// 位置の取得
//==============================================================================
D3DXVECTOR3 CTitleEnter::GetPosition(void)
{
	D3DXVECTOR3 pos;
	return pos;
}

//==============================================================================
// サイズの取得
//==============================================================================
D3DXVECTOR3 CTitleEnter::GetSize(void)
{
	D3DXVECTOR3 size;
	return size;
}