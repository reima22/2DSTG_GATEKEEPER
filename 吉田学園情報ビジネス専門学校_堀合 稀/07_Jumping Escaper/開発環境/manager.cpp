//==============================================================================
//
// マネージャ〔manager.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "camera.h"
#include "collision.h"
#include "debugproc.h"
#include "gamepad.h"
#include "fade.h"
#include "input.h"
#include "light.h"
#include "main.h"
#include "manager.h"
#include "mode.h"
#include "renderer.h"
#include "sound.h"
#include "textdata.h"
#include "texture.h"

//==============================================================================
// 静的メンバ変数
//==============================================================================
CRenderer *CManager::m_pRenderer = NULL;			// レンダリング
CInputKeyboard *CManager::m_pInputKeyboard = NULL;	// キーボード
CGamepad *CManager::m_pGamepad = NULL;				// ゲームパッド
CFade *CManager::m_pFade = NULL;					// フェードポインタ
CMode *CManager::m_pMode = NULL;					// ゲームモードポインタ
CSound *CManager::m_pSound = NULL;					// サウンドのポインタ
CCamera *CManager::m_pCamera = NULL;				// カメラポインタ
CLight *CManager::m_pLight = NULL;					// ライトのポインタ
CTextData *CManager::m_pTextData = NULL;			// テキストデータのポインタ
CTexture *CManager::m_pTexture = NULL;				// テクスチャポインタ
CCollision *CManager::m_pCollision = NULL;			// 当たり判定のポインタ
bool CManager::m_bBGM[CSound::BGMTYPE_MAX] =
{
	true,		// タイトル
	true,		// ステージセレクト
	true,		// ゲーム
	true,		// リザルト
	true		// ランキング
};		

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

	// フェードの初期化
	if (m_pFade == NULL)
	{
		m_pFade = CFade::Create();
	}

	// サウンド生成
	if (m_pSound == NULL)
	{
		m_pSound = CSound::Create(hWnd);
	}

	// キーボード生成
	if (m_pInputKeyboard == NULL)
	{
		m_pInputKeyboard = CInputKeyboard::Create(hInstance, hWnd);
	}

	// ゲームパッド生成
	if (m_pGamepad == NULL)
	{
		m_pGamepad = CGamepad::Create(hInstance, hWnd);
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

	// ゲームモード生成
	if (m_pMode == NULL)
	{
		m_pMode = CMode::Create();
	}

	// 当たり判定の生成
	if (m_pCollision == NULL)
	{
		m_pCollision = CCollision::Create();
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

	// ゲームパッドの破棄
	if (m_pGamepad != NULL)
	{
		m_pGamepad->Uninit();
		m_pGamepad = NULL;
	}

	// サウンド終了
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		m_pSound = NULL;
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

	// 当たり判定の破棄
	if(m_pCollision != NULL)
	{
		m_pCollision = NULL;
	}

	// 全破棄
	CScene::ReleaseAll();

	// フェードの終了
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		m_pFade = NULL;
	}

	// ゲームモードの開放
	if (m_pMode != NULL)
	{
		m_pMode = NULL;
	}

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

	// ゲームパッドの更新
	if (m_pGamepad != NULL)
	{
		m_pGamepad->Update();
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

	// フェードの更新
	if (m_pFade != NULL)
	{
		m_pFade->Update();
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