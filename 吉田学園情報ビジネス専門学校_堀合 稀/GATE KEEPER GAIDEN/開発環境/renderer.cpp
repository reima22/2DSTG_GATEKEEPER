//==============================================================================
//
// レンダリング処理〔renderer.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "fade.h"
#include "game.h"
#include "debugproc.h"
#include "polygon.h"
#include "camera.h"
#include "input.h"

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
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
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
		m_pD3DDevice, 18, 0, 0, 0, FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&m_pFont);

	// マルチターゲットレンダリング
	// ローカル変数宣言
	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;	// 保存用変数

	for (int nCnt = 0; nCnt < FEEDBACK_NUM; nCnt++)
	{
		// レンダリングターゲット用テクスチャの生成
		m_pD3DDevice->CreateTexture(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_apTextureMT[nCnt],
			NULL);

		// テクスチャレンダリング用インターフェースの生成
		m_apTextureMT[nCnt]->GetSurfaceLevel(0, &m_apRenderMT[nCnt]);

		// テクスチャレンダリング用Zバッファの生成
		m_pD3DDevice->CreateDepthStencilSurface(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			D3DFMT_D16,
			D3DMULTISAMPLE_NONE,
			0,
			TRUE,
			&m_apBuffMT[nCnt],
			NULL);

		// 現在のレンダリングターゲットを取得(保存)
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		// 現在のZバッファを取得(保存)
		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);

		// レンダリングターゲットを設定し生成したテクスチャに設定
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[nCnt]);

		// Zバッファを設定
		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[nCnt]);

		// レンダリングターゲット用テクスチャのクリア
		m_pD3DDevice->Clear(
			0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		// レンダリングターゲットを元に戻す
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		// Zバッファを元に戻す
		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);
	}

	// レンダリング用ビューポートの設定
	m_viewportMT.X = 0;
	m_viewportMT.Y = 0;
	m_viewportMT.Width = SCREEN_WIDTH;
	m_viewportMT.Height = SCREEN_HEIGHT;
	m_viewportMT.MinZ = 0.0f;
	m_viewportMT.MaxZ = 1.0f;

	// フィードバック用ポリゴンの生成
	VERTEX_2D *pVtx;

	// 頂点情報を設定
	// 頂点バッファの生成
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuffMT,
		NULL)))
	{
		return E_FAIL;
	}

	D3DXVECTOR3 pos = SCREEN_CENTER;
	m_fLength = sqrtf(SCREEN_WIDTH / 2.0f * SCREEN_WIDTH / 2.0f + SCREEN_HEIGHT / 2.0f * SCREEN_HEIGHT / 2.0f);
	m_fAngle = atan2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
	m_fAlpha = DEFAULT_ALPHA;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	// 数値の各頂点座標
	pVtx[0].pos = D3DXVECTOR3(
		pos.x + sinf(-m_fAngle) * m_fLength - 0.5f,
		pos.y + cosf(-m_fAngle) * m_fLength - 0.5f,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(
		pos.x + sinf(m_fAngle - D3DX_PI) * m_fLength - 0.5f,
		pos.y + cosf(m_fAngle - D3DX_PI) * m_fLength - 0.5f,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(
		pos.x + sinf(m_fAngle) * m_fLength - 0.5f,
		pos.y + cosf(m_fAngle) * m_fLength - 0.5f,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(
		pos.x + sinf(-m_fAngle + D3DX_PI) * m_fLength - 0.5f,
		pos.y + cosf(-m_fAngle + D3DX_PI) * m_fLength - 0.5f,
		0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);

	// テクスチャ頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuffMT->Unlock();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CRenderer::Uninit(void)
{
	for (int nCnt = 0; nCnt < FEEDBACK_NUM; nCnt++)
	{
		// テクスチャの開放
		if (m_apTextureMT[nCnt] != NULL)
		{
			m_apTextureMT[nCnt]->Release();
			m_apTextureMT[nCnt] = NULL;
		}

		// テクスチャレンダリング用インターフェースの開放
		if (m_apRenderMT[nCnt] != NULL)
		{
			m_apRenderMT[nCnt]->Release();
			m_apRenderMT[nCnt] = NULL;
		}

		// テクスチャレンダリング用Zバッファの開放
		if (m_apBuffMT[nCnt] != NULL)
		{
			m_apBuffMT[nCnt]->Release();
			m_apBuffMT[nCnt] = NULL;
		}
	}

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
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	D3DXVECTOR3 pos = SCREEN_CENTER;

	// フェード情報の取得 
	CFade::FADE fade = CFade::GetFade();

	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME && fade == CFade::FADE_OUT)
	{
		return;
	}
	else
	{
		// ポリゴンの全更新
		CScene::UpdateAll();
	}

	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// デバッグモードの時
#ifdef _DEBUG

	if (keyboard->GetPress(CInput::KEYINFO_ALPHA_PLUS) == true)
	{
		m_fAlpha += 0.01f;
	}

	if (keyboard->GetPress(CInput::KEYINFO_ALPHA_MINUS) == true)
	{
		m_fAlpha -= 0.01f;
	}

	if (keyboard->GetPress(CInput::KEYINFO_FE_BIG) == true)
	{
		m_fLength += 1.0f;
	}

	if (keyboard->GetPress(CInput::KEYINFO_FE_SMALL) == true)
	{
		m_fLength -= 1.0f;
	}

#endif

	// エフェクト発生中の処理
	if (m_bAppeal == true)
	{
		m_nAppealCnt++;

		if (m_nAppealCnt >= FEEDBACK_FRAMECNT)
		{
			m_fLength -= FEEDBACK_LENGTH;
			m_nAppealCnt = 0;
			m_fAlpha = DEFAULT_ALPHA;
			m_bAppeal = false;
		}
	}

	// α値の上限・下限
	if (m_fAlpha >= 1.0f)
	{
		m_fAlpha = 0.99f;
	}

	if (m_fAlpha < 0.0f)
	{
		m_fAlpha = 0.0f;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	// 数値の各頂点座標
	pVtx[0].pos = D3DXVECTOR3(
		pos.x + sinf(-m_fAngle) * m_fLength,
		pos.y + cosf(-m_fAngle) * m_fLength,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(
		pos.x + sinf(m_fAngle - D3DX_PI) * m_fLength,
		pos.y + cosf(m_fAngle - D3DX_PI) * m_fLength,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(
		pos.x + sinf(m_fAngle) * m_fLength,
		pos.y + cosf(m_fAngle) * m_fLength - 0.5f,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(
		pos.x + sinf(-m_fAngle + D3DX_PI) * m_fLength,
		pos.y + cosf(-m_fAngle + D3DX_PI) * m_fLength,
		0.0f);

	// 各頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);

	// 頂点バッファをアンロックする
	m_pVtxBuffMT->Unlock();
}

//==============================================================================
// 描画処理
//==============================================================================
void CRenderer::Draw(void)
{
	// ローカル変数宣言
	LPDIRECT3DSURFACE9 pRenderWk;
	LPDIRECT3DTEXTURE9 pTextureWk;
	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;	// 保存用変数

	// バックバッファ＆Ｚバッファのクリア
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
		// 現在のレンダリングターゲットを取得(保存)
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		// 現在のZバッファを取得(保存)
		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);

		// レンダリングターゲットをテクスチャ[0]設定
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[0]);

		// Zバッファの設定
		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[0]);

		// テクスチャ[0]のクリア
		m_pD3DDevice->Clear(
			0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0);

		// ポリゴンの全描画処理
		CScene::DrawAll();

		// 頂点バッファをデータストリームに設定
		m_pD3DDevice->SetStreamSource(
			0,
			m_pVtxBuffMT,
			0,
			sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		// フィードバック用ポリゴンへテクスチャ[1]を貼り付け描画
		m_pD3DDevice->SetTexture(0, m_apTextureMT[1]);

		// ポリゴンの描画
		m_pD3DDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,							// 描画を開始する頂点インデックス
			2);							// 描画するプリミティブ数

		// レンダリングターゲットを元に戻す
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		// Zバッファを元に戻す
		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);

		// 不透明を有効
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

		// 頂点バッファをデータストリームに設定
		m_pD3DDevice->SetStreamSource(
			0,
			m_pVtxBuffMT,
			0,
			sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		// フィードバック用ポリゴンにテクスチャ[0]を貼り付け描画
		m_pD3DDevice->SetTexture(0, m_apTextureMT[0]);

		// ポリゴンの描画
		m_pD3DDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,							// 描画を開始する頂点インデックス
			2);							// 描画するプリミティブ数

		// 元に戻す
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

		// テクスチャ[0]とテクスチャ[1]を入れ替える
		pRenderWk = m_apRenderMT[0];
		m_apRenderMT[0] = m_apRenderMT[1];
		m_apRenderMT[1] = pRenderWk;

		pTextureWk = m_apTextureMT[0];
		m_apTextureMT[0] = m_apTextureMT[1];
		m_apTextureMT[1] = pTextureWk;

		// フェードの描画
		CFade::Draw();

		// デバッグモードの時
#ifdef _DEBUG

		// FPSの描画処理
		//DrawFPS();

		CDebugProc::Draw();

#endif

		// 描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
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

//==============================================================================
// フィードバックエフェクトの発生
//==============================================================================
void CRenderer::SetFeedbackEffect(void)
{
	if (m_bAppeal == false)
	{
		m_fLength += FEEDBACK_LENGTH;
		m_fAlpha += FEEDBACK_ALPHA;
		m_bAppeal = true;
	}
}