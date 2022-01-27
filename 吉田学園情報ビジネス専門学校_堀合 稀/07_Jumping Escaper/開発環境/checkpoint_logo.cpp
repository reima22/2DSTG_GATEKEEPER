//==============================================================================
//
// チェックポイントロゴ描画処理〔checkpoint_logo.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "checkpoint_logo.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CCheckpointLogo::CCheckpointLogo(int nPriority) : CScene(nPriority)
{
}

//==============================================================================
// デストラクタ
//==============================================================================
CCheckpointLogo::~CCheckpointLogo()
{
}

//==============================================================================
// チェックポイントロゴの初期化処理
//==============================================================================
HRESULT CCheckpointLogo::Init(void)
{
	// 初期化
	if (m_pScene2D == NULL)
	{
		m_pScene2D = CScene2D::Create();				// 生成処理
		m_pScene2D->SetPosition(CHECK_POS);				// 位置の設定
		m_pScene2D->SetSize(CHECK_SIZE);				// サイズの設定
		m_pScene2D->BindTexture("CHECKPOINT_LOGO");		// テクスチャの設定
		m_pScene2D->SetTex(1, 1, 0, 0, 0.0f, 0.0f);		// テクスチャの分割設定
	}

	m_logoMove = LOGOMOVE_NONE;							// 移動方法

	return S_OK;
}

//==============================================================================
// チェックポイントロゴの終了処理
//==============================================================================
void CCheckpointLogo::Uninit(void)
{
	// ポリゴンの破棄
	if (m_pScene2D != NULL)
	{
		m_pScene2D->Uninit();
		m_pScene2D = NULL;
	}

	Release();
}

//==============================================================================
// チェックポイントロゴの更新処理
//==============================================================================
void CCheckpointLogo::Update(void)
{
	// 移動処理
	MoveLogo();
}

//==============================================================================
// チェックポイントロゴの描画処理
//==============================================================================
void CCheckpointLogo::Draw(void)
{

}

//==============================================================================
// チェックポイントロゴの生成処理
//==============================================================================
CCheckpointLogo *CCheckpointLogo::Create(void)
{
	// インスタンス生成
	CCheckpointLogo *pCheckpointLogo;
	pCheckpointLogo = new CCheckpointLogo;

	// 初期化
	if (pCheckpointLogo != NULL)
	{
		pCheckpointLogo->Init();
	}

	return pCheckpointLogo;
}

//==============================================================================
// チェックポイントロゴ移動処理
//==============================================================================
void CCheckpointLogo::MoveLogo(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;	// 位置

	// 位置の取得
	if (m_pScene2D != NULL)
	{
		pos = m_pScene2D->GetPosition();
	}

	// ロゴの移動方法
	switch (m_logoMove)
	{
	case LOGOMOVE_NONE:

		pos = CHECK_POS;		// 初期位置
		
		break;

	case LOGOMOVE_START:

		pos.x -= CHECK_MOVE;	// 移動
		
		// 中央で停止
		if (pos.x <= SCREEN_CENTER.x)
		{
			m_logoMove = LOGOMOVE_STOP;
			pos = D3DXVECTOR3(SCREEN_CENTER.x,CHECK_POS.y, 0.0f);
		}

		break;

	case LOGOMOVE_STOP:

		m_nCntStop++;	// 停止中カウント加算
		
		if (m_nCntStop > CNT_ONE_SECOND)
		{
			m_logoMove = LOGOMOVE_RESTART;	// 移動再開へ
			m_nCntStop = 0;					// カウント初期化
		}

		break;

	case LOGOMOVE_RESTART:

		pos.x -= CHECK_MOVE;			// 移動再開

		if (pos.x < -CHECK_SIZE.x)
		{
			m_logoMove = LOGOMOVE_END;	// 処理終了
		}

		break;

	case LOGOMOVE_END:

		m_logoMove = LOGOMOVE_NONE;		// 初期位置へ戻る

		break;
	}

	// 位置の設定
	if (m_pScene2D != NULL)
	{
		m_pScene2D->SetPosition(pos);
	}
}