//==============================================================================
//
// ボーナスカウント描画処理〔bonus_count.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "bonus_count.h"
#include "main.h"
#include "manager.h"
#include "mode.h"
#include "number.h"
#include "renderer.h"
#include "texture.h"

// 静的メンバ変数
int CBonusCount::m_nSaveBonus = 0;
int CBonusCount::m_nBonusResult = 0;

//==============================================================================
// コンストラクタ
//==============================================================================
CBonusCount::CBonusCount()
{
	for (int nCnt = 0; nCnt < BONUS_DIGIT_NUM; nCnt++)
	{
		// 各ナンバーの初期化
		m_apNumber[nCnt] = NULL;
	}
}

//==============================================================================
// デストラクタ
//==============================================================================
CBonusCount::~CBonusCount()
{

}

//==============================================================================
// 落下数カウントの初期化処理
//==============================================================================
HRESULT CBonusCount::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// 落下数設定
	m_nBonus = 0;

	for (int nCntDigit = 0; nCntDigit < BONUS_DIGIT_NUM; nCntDigit++)
	{
		m_apNumber[nCntDigit] = CNumber::Create(
			D3DXVECTOR3(pos.x + (nCntDigit * (size.x * 2)), pos.y, 0.0f), size);

		m_apNumber[nCntDigit]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	return S_OK;
}

//==============================================================================
// 落下数カウントの終了処理
//==============================================================================
void CBonusCount::Uninit(void)
{
	for (int nCntDigit = 0; nCntDigit < BONUS_DIGIT_NUM; nCntDigit++)
	{
		if (m_apNumber[nCntDigit] != NULL)
		{// NULLチェック後に破棄
			m_apNumber[nCntDigit]->Uninit();
			m_apNumber[nCntDigit] = NULL;
		}
	}

	m_nBonusResult = m_nBonus;
}

//==============================================================================
// 落下数カウントの更新処理
//==============================================================================
void CBonusCount::Update(void)
{
	SetNumber(m_nBonus);

	// ゲームモードに限定
	if (CManager::GetModePoint()->GetMode() == CMode::MODE_GAME)
	{
		SizeReturn();
	}

	for (int nCntNum = 0; nCntNum < BONUS_DIGIT_NUM; nCntNum++)
	{
		m_apNumber[nCntNum]->Update();
	}
}

//==============================================================================
// 落下数カウントの描画処理
//==============================================================================
void CBonusCount::Draw(void)
{
	for (int nCntDigit = 0; nCntDigit < BONUS_DIGIT_NUM; nCntDigit++)
	{
		if (m_apNumber[nCntDigit] != NULL)
		{// NULLチェック後に描画
			m_apNumber[nCntDigit]->Draw();
		}
	}
}

//==============================================================================
// 落下数カウントの生成処理
//==============================================================================
CBonusCount *CBonusCount::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ローカル変数宣言
	CBonusCount *pBonusCount;
	pBonusCount = new CBonusCount;

	// 初期化
	if (pBonusCount != NULL)
	{
		pBonusCount->Init(pos, size);
	}

	return pBonusCount;
}

//==============================================================================
// 数値設定
//==============================================================================
void CBonusCount::SetNumber(int nBonus)
{
	// 数値設定
	m_nBonus = nBonus;

	int nData = (int)pow(10, (BONUS_DIGIT_NUM - 1));

	for (int nCnt = 0; nCnt < BONUS_DIGIT_NUM; nCnt++)
	{
		// 各桁の算出
		int nNumber;
		nNumber = (m_nBonus / nData) % 10;
		nData /= 10;

		// 桁の設定
		m_apNumber[nCnt]->SetNumber(nNumber);
	}
}

//==============================================================================
// ボーナスカウント増加
//==============================================================================
void CBonusCount::AddBonus(int nAdd)
{
	// 加算
	m_nBonus += nAdd;

	// サイズ拡大
	for (int nCnt = 0; nCnt < BONUS_DIGIT_NUM; nCnt++)
	{
		D3DXVECTOR2 size = m_apNumber[nCnt]->GetSize();

		size = BONUS_COUNT_SIZE + D3DXVECTOR2(20.0f, 20.0f);

		m_apNumber[nCnt]->SetSize(size);
		m_apNumber[nCnt]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	}
}

//==============================================================================
// 数字サイズを戻す
//==============================================================================
void CBonusCount::SizeReturn(void)
{
	for (int nCnt = 0; nCnt < BONUS_DIGIT_NUM; nCnt++)
	{
		D3DXVECTOR2 size = m_apNumber[nCnt]->GetSize();

		if (size.x > BONUS_COUNT_SIZE.x)
		{
			size -= D3DXVECTOR2(0.5f, 0.5f);

			m_apNumber[nCnt]->SetSize(size);
		}
		else
		{
			m_apNumber[nCnt]->SetSize(BONUS_COUNT_SIZE);
			m_apNumber[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0, 1.0f));
		}
	}
}