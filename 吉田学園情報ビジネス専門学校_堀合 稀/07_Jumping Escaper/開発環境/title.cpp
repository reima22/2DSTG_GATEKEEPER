//=============================================================================
//
// メイン処理 [title.cpp]
// Author : Tatsuki Matsuda 
//
//=============================================================================
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "sound.h"
#include "title.h"
#include "title_enter.h"
#include "title_logo.h"
#include "title_runningman.h"

//*****************************************************************************
// 静的メンバ変数の初期化
//*****************************************************************************
CBg*CTitle::m_pBg = NULL;								// 背景変数
CTitleLogo *CTitle::m_pTitleLogo = NULL;				// ロゴポインタ
CTitleEnter *CTitle::m_pTitleEnter = NULL;				// エンターサインポインタ
CTitleRunningMan *CTitle::m_pTitleRunningMan = NULL;	// 背景アニメーションポインタ

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTitle::CTitle()
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTitle::~CTitle()
{

}

//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT CTitle::Init(void)
{
	// 背景クラスの生成
	m_pBg = CBg::Create();

	// ランニングマン
	m_pTitleRunningMan = CTitleRunningMan::Create(T_RUNNINGMAN_POS);

	// タイトルロゴ
	m_pTitleLogo = CTitleLogo::Create(T_LOGO_POS);

	// エンターサイン
	m_pTitleEnter = CTitleEnter::Create(T_ENTER_POS);


	// BGMの再生
	if (m_pSound != NULL && CManager::GetBGMPlay(CSound::BGMTYPE_TITLE) == true)
	{
		m_pSound->Play(CSound::SOUND_LABEL_BGM000);
	}

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CTitle::Uninit(void)
{
	// BGMの停止
	if (m_pSound != NULL && CManager::GetBGMPlay(CSound::BGMTYPE_TITLE) == true)
	{
		m_pSound->Stop();
	}

	// タイトルロゴ
	if (m_pTitleLogo != NULL)
	{
		m_pTitleLogo->Uninit();
		m_pTitleLogo = NULL;
	}

	// エンターサイン
	if (m_pTitleEnter != NULL)
	{
		m_pTitleEnter->Uninit();
		m_pTitleEnter = NULL;
	}

	// エンターサイン
	if (m_pTitleRunningMan != NULL)
	{
		m_pTitleRunningMan->Uninit();
		m_pTitleRunningMan = NULL;
	}

	Release();
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CTitle::Update(void)
{

}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CTitle::Draw(void)
{

}

//*****************************************************************************
// 生成処理
//*****************************************************************************
CTitle *CTitle::Create(void)
{
	CTitle *pTitle;
	pTitle = new CTitle;

	if (pTitle != NULL)
	{
		pTitle->Init();
	}

	return pTitle;
}

//*****************************************************************************
// 背景情報処理
//*****************************************************************************
CBg*CTitle::GetBg(void)
{
	return m_pBg;
}