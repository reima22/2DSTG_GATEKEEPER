//==============================================================================
//
// マネージャ〔manager.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "texture.h"
#include "input.h"
#include "textdata.h"
#include "camera.h"
#include "light.h"
#include "debugproc.h"
#include "exe.h"

//==============================================================================
// 静的メンバ変数
//==============================================================================
CRenderer *CManager::m_pRenderer = NULL;			// レンダリング
CInputKeyboard *CManager::m_pInputKeyboard = NULL;	// キーボード
CCamera *CManager::m_pCamera = NULL;				// カメラポインタ
CLight *CManager::m_pLight = NULL;					// ライトのポインタ
CTextData *CManager::m_pTextData = NULL;			// テキストデータのポインタ
CTexture *CManager::m_pTexture = NULL;				// テクスチャポインタ
CExe *CManager::m_pExe = NULL;						// 実行クラス
CInputMouse *CManager::m_pInputMouse = NULL;		// マウス

//==============================================================================
// コンストラクタ
//==============================================================================
CManager::CManager()
{
}

//==============================================================================
// デストラクタ
//==============================================================================
CManager::~CManager()
{
}

//==============================================================================
// 初期化
//==============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// インスタンスと初期化
	if (m_pRenderer == NULL)
	{
		m_pRenderer = CRenderer::Create(hWnd, bWindow);
	}

	// デバッグ表示の生成
	CDebugProc::Create();

	// キーボード生成
	if (m_pInputKeyboard == NULL)
	{
		m_pInputKeyboard = CInputKeyboard::Create(hInstance, hWnd);
	}

	// テキストデータの読み込みクラスの生成
	if (m_pTextData == NULL)
	{
		m_pTextData = CTextData::Create();
	}
	
	// テクスチャの読み込み
	if (m_pTexture == NULL)
	{
		m_pTexture = CTexture::Create();
	}

	// カメラの生成
	if (m_pCamera == NULL)
	{
		m_pCamera = CCamera::Create();
	}

	// ライトの生成
	if (m_pLight == NULL)
	{
		m_pLight = CLight::Create();
	}

	// 実行クラスの生成
	if (m_pExe == NULL)
	{
		m_pExe = CExe::Create();
	}
	
	// マウス生成
	if (m_pInputMouse == NULL)
	{
		m_pInputMouse = CInputMouse::Create(hInstance, hWnd);
	}

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CManager::Uninit(void)
{
	// キーボードの破棄
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		m_pInputKeyboard = NULL;
	}

	// マウスの破棄
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();
		m_pInputMouse = NULL;
	}
	
	// テキストデータの破棄
	if (m_pTextData != NULL)
	{
		m_pTextData->UnloadText();
		m_pTextData = NULL;
	}
	
	// テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->UnloadAll();
		m_pTexture = NULL;
	}

	// カメラの開放
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		m_pCamera = NULL;
	}

	// ライトの開放
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		m_pLight = NULL;
	}

	// 実行クラスの開放
	if (m_pExe != NULL)
	{
		m_pExe->Uninit();
		m_pExe = NULL;
	}

	// 全破棄
	CScene::ReleaseAll();

	// ポインタの開放
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		m_pRenderer = NULL;
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void CManager::Update(void)
{
	// キーボードの更新
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// マウスの更新
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Update();
	}

	// レンダラーの更新
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	// カメラの更新
	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}

	// ライトの更新
	if (m_pLight != NULL)
	{
		m_pLight->Update();
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void CManager::Draw(void)
{
	// レンダラーの描画処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}