//==============================================================================
//
// フォグ効果描画処理〔fog.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "camera.h"
#include "fog.h"
#include "input.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CFog::CFog()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CFog::~CFog()
{
}

//==============================================================================
// フォグ効果の初期化処理
//==============================================================================
HRESULT CFog::Init(void)
{
	// メンバー変数初期化
	m_fFogStart = FOG_START;
	m_fFogEnd = FOG_END;

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 黒色

	return S_OK;
}

//==============================================================================
// フォグ効果の終了処理
//==============================================================================
void CFog::Uninit(void)
{

}

//==============================================================================
// フォグ効果の更新処理
//==============================================================================
void CFog::Update(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// フォグの操作
	// 開始点
	if (keyboard->GetPress(CInput::KEYINFO_FOGSTART_PLUS) == true)
	{
		m_fFogStart += 5.0f;
	}

	if (keyboard->GetPress(CInput::KEYINFO_FOGSTART_MINUS) == true)
	{
		m_fFogStart -= 5.0f;
	}

	// 奥行終着
	if (keyboard->GetPress(CInput::KEYINFO_FOGEND_PLUS) == true)
	{
		m_fFogEnd += 5.0f;
	}

	if (keyboard->GetPress(CInput::KEYINFO_FOGEND_MINUS) == true)
	{
		m_fFogEnd -= 5.0f;
	}
}

//==============================================================================
// フォグ効果の描画処理
//==============================================================================
void CFog::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = CManager::GetRenderer()->GetDevice();

	// フォグを有効化
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	// フォグの色指定
	pDevice->SetRenderState(D3DRS_FOGCOLOR, m_col);

	// バーテックスフォグの使用(線形公式)
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);

	pDevice->SetRenderState(D3DRS_FOGSTART, *((LPDWORD)(&m_fFogStart)));	
	pDevice->SetRenderState(D3DRS_FOGEND, *((LPDWORD)(&m_fFogEnd)));		

	// フォグを無効化
	//pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
}

//==============================================================================
// フォグ効果の生成処理
//==============================================================================
CFog *CFog::Create(void)
{
	// インスタンス生成
	CFog *pFog;
	pFog = new CFog;

	// 初期化
	if (pFog != NULL)
	{
		pFog->Init();
	}

	return pFog;
}