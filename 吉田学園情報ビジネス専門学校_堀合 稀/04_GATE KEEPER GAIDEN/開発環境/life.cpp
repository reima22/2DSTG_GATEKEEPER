//==============================================================================
//
// ライフ描画処理〔life.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "life.h"
#include "polygon.h"
#include "player.h"
#include "game.h"
#include "textdata_player.h"

// 静的メンバ変数宣言
CPolygon **CLife::m_apPolygon;	// ポリゴンポインタ
int CLife::m_nLife;				// ライフ数値
int CLife::m_nLifeMax;			// ライフ最大値

//==============================================================================
// コンストラクタ
//==============================================================================
CLife::CLife(int nPriority) : CScene2D(nPriority)
{
	// ライフ値設定
	m_nLife = CTextDataPlayer::SetData(CTextDataPlayer::PLAYERDATA_LIFE_START);
	m_nLifeMax = CTextDataPlayer::SetData(CTextDataPlayer::PLAYERDATA_LIFE_MAX);

	// メモリ動的確保
	m_apPolygon = new CPolygon*[m_nLifeMax];

	for (int nCnt = 0; nCnt < m_nLifeMax; nCnt++)
	{
		// 初期化
		m_apPolygon[nCnt] = NULL;
	}

	// キャラクターポインタ取得
	m_pChara = CGame::GetChara();
}

//==============================================================================
// デストラクタ
//==============================================================================
CLife::~CLife()
{
}

//==============================================================================
// ライフの初期化処理
//==============================================================================
HRESULT CLife::Init(D3DXVECTOR3 pos)
{
	// ポリゴンの設定
	for (int nCnt = 0; nCnt < m_nLifeMax; nCnt++)
	{
		if (m_apPolygon[nCnt] == NULL)
		{
			m_apPolygon[nCnt] = CPolygon::Create(
				D3DXVECTOR3(pos.x + (nCnt * 60.0f), pos.y, 0.0f), 30.0f, 30.0f, CTexture::TEXTYPE_LIFE);	

			m_apPolygon[nCnt]->SetTex(1, 1, 0,0,0.0f, 0.0f);
		}
	}

	// 開始時のライフ設定
	m_nLife = CTextDataPlayer::SetData(CTextDataPlayer::PLAYERDATA_LIFE_START);

	return S_OK;
}

//==============================================================================
// ライフの終了処理
//==============================================================================
void CLife::Uninit(void)
{
	for (int nCnt = 0; nCnt < m_nLifeMax; nCnt++)
	{
		if (m_apPolygon[nCnt] != NULL)
		{// NULLチェック後に破棄
			m_apPolygon[nCnt]->Uninit();
			delete m_apPolygon[nCnt];
			m_apPolygon[nCnt] = NULL;
		}
	}

	Release();	// CLifeインスタンスのデリート
}

//==============================================================================
// ライフの更新処理
//==============================================================================
void CLife::Update(void)
{
	// ライフ値の反映
	m_nLife = m_pChara->GetLife(CCharacter::CHARA_TYPE_PLAYER);
}

//==============================================================================
// ライフの描画処理
//==============================================================================
void CLife::Draw(void)
{
	// ナンバーの描画
	for (int nCnt = 0; nCnt < m_nLife; nCnt++)
	{
		if (m_apPolygon[nCnt] != NULL)
		{
			m_apPolygon[nCnt]->Draw();
		}
	}
}

//==============================================================================
// ライフの生成処理
//==============================================================================
CLife *CLife::Create(D3DXVECTOR3 pos)
{
	// CLifeのインスタンス生成
	CLife *pLife;
	pLife = new CLife;

	// CLifeの初期化
	pLife->Init(pos);

	return pLife;
}