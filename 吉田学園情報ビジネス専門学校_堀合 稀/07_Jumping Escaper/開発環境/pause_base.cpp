//==============================================================================
//
// ポーズベース処理〔pause_base.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "pause_base.h"
#include "scene2D.h"
#include "textdata.h"
#include "textdata_pause.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CPauseBase::CPauseBase(int nPriority) : CScene(nPriority)
{
}

//==============================================================================
// デストラクタ
//==============================================================================
CPauseBase::~CPauseBase()
{
}

//==============================================================================
// ポーズベースの初期化処理
//==============================================================================
HRESULT CPauseBase::Init(void)
{
	// テキストデータの取得
	CTextDataPause *pDataPause;
	pDataPause = CTextData::GetDataPause();

	D3DXVECTOR3 pos = pDataPause->GetPosition(CTextDataPause::PAUSEPOLY_BASE);
	D3DXVECTOR2 size = pDataPause->GetSize(CTextDataPause::PAUSEPOLY_BASE);
	int nTexIdx = pDataPause->GetTexIdx(CTextDataPause::PAUSEPOLY_BASE);

	// 2Dポリゴンの生成
	m_pScene2D = CScene2D::Create();

	// 初期化
	m_pScene2D->SetPosition(pos);
	m_pScene2D->SetSize(size);
	m_pScene2D->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	m_pScene2D->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	m_pScene2D->BindTextureNum(nTexIdx);

	return S_OK;
}

//==============================================================================
// ポーズベースの終了処理
//==============================================================================
void CPauseBase::Uninit(void)
{
	if (m_pScene2D != NULL)
	{
		m_pScene2D->Uninit();
		m_pScene2D = NULL;
	}
}

//==============================================================================
// ポーズベースの更新処理
//==============================================================================
void CPauseBase::Update(void)
{
	bool bPause = CPause::GetPause();

	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	switch (bPause)
	{
	case true:
		col.a = 1.0f;
		break;

	case false:
		col.a = 0.0f;
		break;
	}

	// 色情報の設定
	m_pScene2D->SetCol(col);
	
	// 更新
	//m_pScene2D->Update();
}

//==============================================================================
// ポーズベースの描画処理
//==============================================================================
void CPauseBase::Draw(void)
{
	if (m_pScene2D != NULL)
	{
		m_pScene2D->Draw();
	}
}

//==============================================================================
// ポーズベースの生成処理
//==============================================================================
CPauseBase *CPauseBase::Create(void)
{
	// インスタンス生成
	CPauseBase *pPauseBase;
	pPauseBase = new CPauseBase;

	// 初期化
	if (pPauseBase != NULL)
	{
		pPauseBase->Init();
	}

	return pPauseBase;
}