//==============================================================================
//
// ランキングロゴの描画処理〔ranking_logo.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "ranking.h"
#include "ranking_logo.h"
#include "scene2D.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CRankingLogo::CRankingLogo(int nPriority) : CRanking(nPriority)
{
	// ポリゴンの初期化
	//m_pPolygon = NULL;
}

//==============================================================================
// デストラクタ
//==============================================================================
CRankingLogo::~CRankingLogo()
{
}

//==============================================================================
// ランキングロゴの初期化処理
//==============================================================================
HRESULT CRankingLogo::Init(void)
{
	//// ポリゴン初期化
	//if (m_pPolygon == NULL)
	//{
	//	m_pPolygon = CPolygon::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f),
	//		RANK_LOGO_SIZEX, RANK_LOGO_SIZEY,
	//		9);
	//	// テクスチャ設定
	//	m_pPolygon->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	//}
	//CScene2D::Init();
	//CScene2D::SetPosition(pos);
	//CScene2D::SetSize(RANK_LOGO_SIZE);
	//CScene2D::SetCol(COLOR_DEFAULT);
	//CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);

	if (m_pScene2D == NULL)
	{
		m_pScene2D = CScene2D::Create();
		m_pScene2D->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	}

	return S_OK;
}

//==============================================================================
// ランキングロゴの終了処理
//==============================================================================
void CRankingLogo::Uninit(void)
{
	//if (m_pPolygon != NULL)
	//{// NULLチェック後に破棄
	//	m_pPolygon->Uninit();
	//	m_pPolygon = NULL;
	//}
	//CScene2D::Uninit();

	// ポリゴンの破棄
	if (m_pScene2D != NULL)
	{
		m_pScene2D->Uninit();
		m_pScene2D = NULL;
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// ランキングロゴの更新処理
//==============================================================================
void CRankingLogo::Update(void)
{
	if (m_pScene2D != NULL)
	{
		m_pScene2D->Update();
	}
}

//==============================================================================
// ランキングロゴの描画処理
//==============================================================================
void CRankingLogo::Draw(void)
{
	//CScene2D::Draw();
	if (m_pScene2D != NULL)
	{
		m_pScene2D->Draw();
	}
}

//==============================================================================
// ランキングロゴの生成処理
//==============================================================================
CRankingLogo *CRankingLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, char *pTypeCheck)
{
	// インスタンス生成
	CRankingLogo *pRankingLogo;
	pRankingLogo = new CRankingLogo;

	// 初期化
	if (pRankingLogo != NULL)
	{
		pRankingLogo->Init();
		pRankingLogo->m_pScene2D->SetPosition(pos);
		pRankingLogo->m_pScene2D->SetSize(size);
		pRankingLogo->m_pScene2D->BindTexture(pTypeCheck);
	}

	return pRankingLogo;
}