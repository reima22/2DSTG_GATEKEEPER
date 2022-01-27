//==============================================================================
//
// ƒQ[ƒ€Œ‹‰ÊƒƒSˆ—kgamesetlogo.cppl
// Author : Mare Horiai
//
//==============================================================================
#include "game.h"
#include "gamesetlogo.h"
#include "main.h"
#include "manager.h"
#include "player.h"
#include "renderer.h"
#include "scene2D.h"

//==============================================================================
// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
//==============================================================================
CGamesetLogo::CGamesetLogo(int nPriority) : CScene(nPriority)
{

}

//==============================================================================
// ƒfƒXƒgƒ‰ƒNƒ^
//==============================================================================
CGamesetLogo::~CGamesetLogo()
{

}

//==============================================================================
// ƒQ[ƒ€Œ‹‰ÊƒƒS‚Ì‰Šú‰»ˆ—
//==============================================================================
HRESULT CGamesetLogo::Init(void)
{
	// ƒvƒŒƒCƒ„[î•ñ
	CPlayer *pPlayer = CGame::GetPlayer();

	if (m_pScene2D == NULL)
	{		
		m_pScene2D = CScene2D::Create();						// 2Dƒ|ƒŠƒSƒ“‚ð¶¬	
		m_pScene2D->SetPosition(LOGO_POS);						// ˆÊ’uÝ’è
		m_pScene2D->SetSize(LOGO_SIZE);							// ƒTƒCƒYÝ’è
		m_pScene2D->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// F‚ÌÝ’è
		m_pScene2D->SetTex(1, 1, 0, 0, 0.0f, 0.0f);				// ƒeƒNƒXƒ`ƒƒ‚Ì•ªŠ„î•ñ

		// ƒeƒNƒXƒ`ƒƒÝ’è
		if (pPlayer->GetGameClear() == true)
		{
			m_pScene2D->BindTexture("G_RESULT_CLEAR");
		}
		else
		{
			m_pScene2D->BindTexture("G_RESULT_GAMEOVER");
		}
	}

	return S_OK;
}

//==============================================================================
// ƒQ[ƒ€Œ‹‰ÊƒƒS‚ÌI—¹ˆ—
//==============================================================================
void CGamesetLogo::Uninit(void)
{
	// 2Dƒ|ƒŠƒSƒ“‚Ì”jŠü
	if (m_pScene2D != NULL)
	{
		m_pScene2D->Uninit();
		m_pScene2D = NULL;
	}
}

//==============================================================================
// ƒQ[ƒ€Œ‹‰ÊƒƒS‚ÌXVˆ—
//==============================================================================
void CGamesetLogo::Update(void)
{

}

//==============================================================================
// ƒQ[ƒ€Œ‹‰ÊƒƒS‚Ì•`‰æˆ—
//==============================================================================
void CGamesetLogo::Draw(void)
{

}

//==============================================================================
// ƒQ[ƒ€Œ‹‰ÊƒƒS‚Ì¶¬ˆ—
//==============================================================================
CGamesetLogo *CGamesetLogo::Create(void)
{
	// ƒCƒ“ƒXƒ^ƒ“ƒX¶¬
	CGamesetLogo *pGamesetLogo;
	pGamesetLogo = new CGamesetLogo;

	// ƒƒS‚Ì‰Šú‰»
	if (pGamesetLogo != NULL)
	{
		pGamesetLogo->Init();
	}

	return pGamesetLogo;
}