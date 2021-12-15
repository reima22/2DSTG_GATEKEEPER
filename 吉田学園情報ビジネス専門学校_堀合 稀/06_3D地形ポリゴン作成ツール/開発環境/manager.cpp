//==============================================================================
//
// マネージャ〔manager.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "gamepad.h"
#include "textdata_player.h"
#include "camera.h"
#include "light.h"
#include "debugproc.h"
#include "collision.h"
#include "exe.h"

//==============================================================================
// 静的メンバ変数
//==============================================================================
CRenderer *CManager::m_pRenderer = NULL;			// レンダリング
CInputKeyboard *CManager::m_pInputKeyboard = NULL;	// キーボード
CGamepad *CManager::m_pGamepad = NULL;				// ゲームパッド
CCamera *CManager::m_pCamera = NULL;				// カメラポインタ
CLight *CManager::m_pLight;							// ライトのポインタ
CTextData *CManager::m_pTextData = NULL;			// テキストデータのポインタ
CTexture *CManager::m_pTexture = NULL;				// テクスチャポインタ
CCollision *CManager::m_pCollision = NULL;			// 当たり判定のポインタ
CExe *CManager::m_pExe = NULL;						// 実行クラスポインタ
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
		m_pRenderer = new CRenderer;
		m_pRenderer->Init(hWnd, bWindow);
	}



	// デバッグ表示の生成
	CDebugProc::Create();

	// カメラの生成
	m_pCamera = CCamera::Create();

	// ライトの生成
	m_pLight = CLight::Create();

	// キーボード生成
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	// ゲームパッド生成
	m_pGamepad = new CGamepad;
	m_pGamepad->Init(hInstance, hWnd);

	// マウス生成
	m_pInputMouse = CInputMouse::Create(hInstance, hWnd);

	// テキストデータの読み込みクラスの生成
	m_pTextData = CTextData::Create();

	// テクスチャの読み込み
	m_pTexture = CTexture::Create();

	// 当たり判定の生成
	m_pCollision = CCollision::Create();
	
	// 実行クラス
	m_pExe = CExe::Create();

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
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// ゲームパッドの破棄
	if (m_pGamepad != NULL)
	{
		m_pGamepad->Uninit();
		delete m_pGamepad;
		m_pGamepad = NULL;
	}
	
	// テキストデータの破棄
	if (m_pTextData != NULL)
	{
		m_pTextData->UninitAll();
		m_pTextData = NULL;
	}
	
	// テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->UnloadAll();
		//delete m_pTexture;
		m_pTexture = NULL;
	}

	// カメラの開放
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		//delete m_pCamera;
		m_pCamera = NULL;
	}

	// ライトの開放
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		//delete m_pLight;
		m_pLight = NULL;
	}

	// 当たり判定の破棄
	if(m_pCollision != NULL)
	{
		m_pCollision = NULL;
	}

	// 全破棄
	CScene::ReleaseAll();

	// ポインタの開放
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
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

	// ゲームパッドの更新
	if (m_pGamepad != NULL)
	{
		m_pGamepad->Update();
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
}

//==============================================================================
// 描画処理
//==============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}