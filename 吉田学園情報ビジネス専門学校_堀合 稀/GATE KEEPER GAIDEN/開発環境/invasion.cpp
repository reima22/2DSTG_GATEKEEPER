//==============================================================================
//
// 侵入数描画処理〔invasion.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "score.h"
#include "number.h"
#include "invasion.h"

// 静的メンバ変数宣言
CNumber *CInvasion::m_apNumber[INVASION_NUM_MAX];
int CInvasion::m_nInvasion;

//==============================================================================
// コンストラクタ
//==============================================================================
CInvasion::CInvasion(int nPriority) : CScene2D(nPriority)
{
	for (int nCnt = 0; nCnt < INVASION_NUM_MAX; nCnt++)
	{
		// 各ナンバーの初期化
		m_apNumber[nCnt] = NULL;
	}
}

//==============================================================================
// デストラクタ
//==============================================================================
CInvasion::~CInvasion()
{
}

//==============================================================================
// 侵入数描画の初期化処理
//==============================================================================
HRESULT CInvasion::Init(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < INVASION_NUM_MAX; nCnt++)
	{
		if (m_apNumber[nCnt] == NULL)
		{
			m_apNumber[nCnt] = CNumber::Create(
				D3DXVECTOR3(pos.x + (nCnt * 25.0f), pos.y, 0.0f), 10.0f, 20.0f);
		}
	}

	m_nInvasion = 0;

	return S_OK;
}

//==============================================================================
// 侵入数描画の終了処理
//==============================================================================
void CInvasion::Uninit(void)
{
	for (int nCnt = 0; nCnt < INVASION_NUM_MAX; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{// NULLチェック後に破棄
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = NULL;
		}
	}
}

//==============================================================================
// 侵入数描画の更新処理
//==============================================================================
void CInvasion::Update(void)
{
	// スコアの設定
	SetInvasion(m_nInvasion);

	for (int nCnt = 0; nCnt < INVASION_NUM_MAX; nCnt++)
	{
		D3DXVECTOR2 size = m_apNumber[nCnt]->GetSize();

		if (size.x > INV_SIZE.x)
		{
			size.x -= 0.1f;
		}
		else
		{
			m_apNumber[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		if (size.y > INV_SIZE.y)
		{
			size.y -= 0.2f;
		}

		m_apNumber[nCnt]->SetSize(size);
		m_apNumber[nCnt]->Update();
	}
}

//==============================================================================
// 侵入数描画の描画処理
//==============================================================================
void CInvasion::Draw(void)
{
	// ナンバーの描画
	for (int nCnt = 0; nCnt < INVASION_NUM_MAX; nCnt++)
	{
		if (m_apNumber != NULL)
		{
			m_apNumber[nCnt]->Draw();
		}
	}
}

//==============================================================================
// 侵入数の生成処理
//==============================================================================
CInvasion *CInvasion::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CInvasion *pInvasion;
	pInvasion = new CInvasion;

	// 初期化
	pInvasion->Init(pos);

	return pInvasion;
}

//==============================================================================
// 侵入数設定
//==============================================================================
void CInvasion::SetInvasion(int nInvasion)
{
	// スコアの設定
	m_nInvasion = nInvasion;

	int nData = (int)pow(10, (INVASION_NUM_MAX - 1));

	for (int nCnt = 0; nCnt < INVASION_NUM_MAX; nCnt++)
	{
		// 各桁の算出
		int nNumber;
		nNumber = (m_nInvasion / nData) % 10;
		nData /= 10;

		// 桁の設定
		m_apNumber[nCnt]->SetNumber(nNumber);
	}
}

//==============================================================================
// 侵入数の加算
//==============================================================================
void CInvasion::AddInvasion(void)
{
	m_nInvasion++;

	for (int nCnt = 0; nCnt < INVASION_NUM_MAX; nCnt++)
	{
		m_apNumber[nCnt]->SetSize(D3DXVECTOR2(15.0f,30.0f));
		m_apNumber[nCnt]->SetCol(D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f));
	}
}