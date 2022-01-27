//==============================================================================
//
// リザルトに描画する図の処理〔result_picture.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "result.h"
#include "result_picture.h"
#include "scene2D.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CResultPicture::CResultPicture(int nPriority) : CResult(nPriority)
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CResultPicture::~CResultPicture()
{
}

//==============================================================================
// リザルト表示図の初期化処理
//==============================================================================
HRESULT CResultPicture::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, char *pTypeCheck)
{
	// ポリゴン生成
	if (m_pScene2D == NULL)
	{
		m_pScene2D = CScene2D::Create();
		m_pScene2D->SetPosition(pos);
		m_pScene2D->SetSize(size);
		m_pScene2D->SetCol(COLOR_DEFAULT);
		m_pScene2D->SetTex(1, 1, 0, 0, 0, 0);
		m_pScene2D->BindTexture(pTypeCheck);
	}

	return S_OK;
}

//==============================================================================
// リザルト表示図の終了処理
//==============================================================================
void CResultPicture::Uninit(void)
{
	if (m_pScene2D != NULL)
	{// NULLチェック後に破棄
		m_pScene2D->Uninit();
		m_pScene2D = NULL;
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// リザルト表示図の更新処理
//==============================================================================
void CResultPicture::Update(void)
{

}

//==============================================================================
// リザルト表示図の描画処理
//==============================================================================
void CResultPicture::Draw(void)
{
}

//==============================================================================
// リザルト表示図の生成処理
//==============================================================================
CResultPicture *CResultPicture::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, char *pTypeCheck)
{
	// インスタンス生成
	CResultPicture *pResultPicture;
	pResultPicture = new CResultPicture;

	// 初期化
	if (pResultPicture != NULL)
	{
		pResultPicture->Init(pos, rot, size, pTypeCheck);
	}

	return pResultPicture;
}