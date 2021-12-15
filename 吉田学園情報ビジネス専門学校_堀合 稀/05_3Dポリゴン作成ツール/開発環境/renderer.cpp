//==============================================================================
//
// レンダリング処理〔renderer.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "debugproc.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_pFont = NULL;
}

//==============================================================================
// デストラクタ
//==============================================================================
CRenderer::~CRenderer()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	// ローカル変数宣言
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// Zバッファ24bit,ステンシルバッファ8bit
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{ // 描画処理:ハードウェア、頂点処理:ハードウェアでの処理失敗
		if (FAILED(m_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{ // 描画処理:ソフトウェア、頂点処理:ハードウェアでの処理失敗
			if (FAILED(m_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{ // 描画処理:ソフトウェア、頂点処理:ソフトウェアでの処理失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定(テクスチャ背景(ポリゴン本体の色)の透過など)
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// 縮小時　補間
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// 拡大時　補間
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// U値　繰り返し
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// V値　繰り返し

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		// アルファ値の合成	
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// フォントオブジェクトの生成
	D3DXCreateFont(
		m_pD3DDevice, 20, 0, 0, 0, FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&m_pFont);

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CRenderer::Uninit(void)
{
	// フォントの開放
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}

	// Direct3Dデバイスの開放
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの開放
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void CRenderer::Update(void)
{
	// ポリゴンの全更新
	CScene::UpdateAll();
}

//==============================================================================
// 描画処理
//==============================================================================
void CRenderer::Draw(void)
{
	// バックバッファ&Ｚバッファ&ステンシルバッファのクリア
	m_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// ポリゴンの全描画処理
		CScene::DrawAll();
		
		// デバッグモードの時
//#ifdef _DEBUG

		CDebugProc::Draw();

//#endif
		// 描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==============================================================================
// レンダラークラスの生成
//==============================================================================
CRenderer *CRenderer::Create(HWND hWnd, BOOL bWindow)
{
	// ローカル変数宣言
	CRenderer *pRenderer;
	pRenderer = new CRenderer;

	// 初期化
	if (pRenderer != NULL)
	{
		pRenderer->Init(hWnd, bWindow);
	}

	return pRenderer;
}

//==============================================================================
// FPS描画
//==============================================================================
void CRenderer::DrawFPS(void)
{
	// ローカル変数宣言
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];
	int nCountFPS = GetFPSCnt();	// FPSカウントの取得

	wsprintf(&aStr[0], "FPS:%d\n", nCountFPS);

	// テキストの描画
	m_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}