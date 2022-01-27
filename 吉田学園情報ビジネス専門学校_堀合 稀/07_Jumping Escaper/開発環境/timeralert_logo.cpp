//==============================================================================
//
// タイマーアラートロゴ描画処理〔timeralert_logo.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "timeralert_logo.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CAlertLogo::CAlertLogo(int nPriority) : CScene2D(nPriority)
{
}

//==============================================================================
// デストラクタ
//==============================================================================
CAlertLogo::~CAlertLogo()
{
}

//==============================================================================
// タイマーの初期化処理
//==============================================================================
HRESULT CAlertLogo::Init(void)
{
	// 初期化
	if (m_pScene2D == NULL)
	{
		m_pScene2D = new CScene2D;
		m_pScene2D->Init();
		m_pos = ALERT_POS;
		m_pScene2D->SetPosition(m_pos);
		m_pScene2D->SetSize(ALERT_SIZE);
		m_pScene2D->BindTexture("TIMERALERT_LOGO");
		m_pScene2D->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	}

	m_alertLogoMove = ALERTLOGOMOVE_NONE;

	return S_OK;
}

//==============================================================================
//タイマーの終了処理
//==============================================================================
void CAlertLogo::Uninit(void)
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
// タイマーの更新処理
//==============================================================================
void CAlertLogo::Update(void)
{
	// 移動処理
	MoveLogo();

	if (m_pScene2D != NULL)
	{
		m_pScene2D->SetPosition(m_pos);
	}
}

//==============================================================================
// タイマーの描画処理
//==============================================================================
void CAlertLogo::Draw(void)
{

}

//==============================================================================
// タイマーの生成処理
//==============================================================================
CAlertLogo *CAlertLogo::Create(void)
{
	// インスタンス生成
	CAlertLogo *pAlertLogo;
	pAlertLogo = new CAlertLogo;

	// 初期化
	if (pAlertLogo != NULL)
	{
		pAlertLogo->Init();
	}

	return pAlertLogo;
}

//==============================================================================
// チェックポイントロゴ移動処理
//==============================================================================
void CAlertLogo::MoveLogo(void)
{
	switch (m_alertLogoMove)
	{
	case ALERTLOGOMOVE_NONE:
		m_pos = ALERT_POS;
		break;

	case ALERTLOGOMOVE_START:
		m_pos.x -= ALERT_MOVE;
		if (m_pos.x <= SCREEN_CENTER.x)
		{
			m_alertLogoMove = ALERTLOGOMOVE_STOP;
			m_pos = D3DXVECTOR3(SCREEN_CENTER.x, ALERT_POS.y, 0.0f);
		}
		break;

	case ALERTLOGOMOVE_STOP:
		m_nCntStop++;

		if (m_nCntStop > CNT_ONE_SECOND)
		{
			m_alertLogoMove = ALERTLOGOMOVE_RESTART;
			m_nCntStop = 0;
		}

		break;

	case ALERTLOGOMOVE_RESTART:
		m_pos.x -= ALERT_MOVE;

		if (m_pos.x < -ALERT_SIZE.x)
		{
			m_alertLogoMove = ALERTLOGOMOVE_END;
		}

		break;

	case ALERTLOGOMOVE_END:

		m_alertLogoMove = ALERTLOGOMOVE_NONE;

		break;
	}
}