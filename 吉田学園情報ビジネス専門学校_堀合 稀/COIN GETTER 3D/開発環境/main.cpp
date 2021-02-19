//==============================================================================
//
// ウィンドウ表示の処理〔main.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include "polygon.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "shadow.h"
#include "wall.h"
#include "billboard.h"
#include "meshfield.h"
#include "meshwall.h"
#include "bullet.h"
#include "object.h"
#include "player.h"
#include "score.h"
#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "ranking.h"
#include "result.h"
#include "gamepad.h"
#include "sound.h"
#include <stdio.h>
#include <string.h>

//#include "bg.h"
//#include "block.h"
//#include "enemy.h"
//#include "item.h"
//#include "score.h"
//#include "fade.h"
//#include "title.h"
//#include "tutorial.h"
//#include "game.h"
//#include "result.h"
//#include "ranking.h"
//#include "sound.h"
//#include "gamepad.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define CLASS_NAME		"WindowClass"		// ウィンドウクラスの名前
#define WINDOW_NAME		"Coin Getter 3D"	// ウィンドウの名前

//==============================================================================
// プロトタイプ宣言
//==============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ウィンドウプロシージャ
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);							// 初期化処理
void Uninit(void);		// 終了処理
void Update(void);		// 更新処理
void Draw(void);		// 描画処理
void DrawFPS(void);		// FPSの描画処理

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3D9 g_pD3D = NULL;				// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	// Direct3Dデバイスへのポインタ(描画処理に必要)
LPD3DXFONT g_pFont = NULL;				// フォントへのポインタ
int g_nCountFPS;						// FPSカウンタ
MODE g_mode = MODE_TITLE;				// ゲームモード

//==============================================================================
// メイン関数
//==============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 変数宣言
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		("%s",CLASS_NAME),					// ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)
	};
	HWND hWnd;
	MSG msg;
	DWORD dwExecLastTime;	// 処理実行最終時刻
	DWORD dwFPSLastTime;	// FPS計測最終時刻
	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwFrameCount;		// フレームカウント

	dwExecLastTime =
	dwFPSLastTime = 
	dwCurrentTime = timeGetTime();
	dwFrameCount = 0;

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);	// ウィンドウの大きさを調整する

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindowEx(
		0,
		("%s", CLASS_NAME),		// ウィンドウクラスの名前
		("%s", WINDOW_NAME),	// ウィンドウの名前(キャプション)
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,			// ウィンドウの左上X座標
		CW_USEDEFAULT,			// ウィンドウの左上Y座標
		rect.right,				// スクリーンの幅
		rect.bottom,			// スクリーンの高さ
		NULL,
		NULL,
		hInstance,
		NULL);

	// 初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while(1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)	// メッセージを取得しなかった場合"0"を返す
		{ // windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{ // DirectXの処理
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{ // 0.5秒経過
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;		// 値の更新
				dwFrameCount = 0;					// カウントクリア
			}

			if ((dwCurrentTime - dwExecLastTime) > (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}

	// 終了処理
	Uninit();

	// ウィンドウクラスの登録解除
	UnregisterClass(
		("%s", CLASS_NAME),		// ウィンドウクラスの名前
		wcex.hInstance);

	return (int)msg.wParam;
}

//==============================================================================
// ウィンドウプロシージャ
//==============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// ローカル変数宣言
	int nID;		// メッセージへの回答

	// ウィンドウの内容
	switch (uMsg)
	{
	case WM_CLOSE:	// ウィンドウを閉じる
		// メッセージボックスの表示と選択
		nID = MessageBox(NULL, "終了しますか？", "確認", MB_YESNO | MB_ICONQUESTION);
		if (nID == IDYES)
		{ // メッセージ[はい]を選択
			DestroyWindow(hWnd);	// WM_DESTROYメッセージを返す
		}
		else
		{ // メッセージ[いいえ]を選択
			return 0;				// 処理を継続
		}
		break;

	case WM_DESTROY:			// メッセージの破棄
		PostQuitMessage(0);		// WM_QUITメッセージを返す
		break;

	case WM_KEYDOWN:			// キー入力のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:			// ESCキー入力
			// メッセージボックスの表示と選択
			nID = MessageBox(NULL, "終了しますか？", "確認", MB_YESNO | MB_ICONQUESTION | MB_TOPMOST);
			if (nID == IDYES)
			{ // メッセージ[はい]を選択
				DestroyWindow(hWnd);	// WM_DESTROYメッセージを返す
			}
			else
			{ // メッセージ[いいえ]を選択
				return 0;				// 処理を継続
			}
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// ローカル変数宣言
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
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
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{ // 描画処理:ハードウェア、頂点処理:ハードウェアでの処理失敗
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{ // 描画処理:ソフトウェア、頂点処理:ハードウェアでの処理失敗
			if (FAILED(g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{ // 描画処理:ソフトウェア、頂点処理:ソフトウェアでの処理失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定(テクスチャ背景(ポリゴン本体の色)の透過など)
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);		// 縮小時　補間
	g_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);		// 拡大時　補間
	g_pD3DDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);		// U値　繰り返し
	g_pD3DDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);		// V値　繰り返し

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);	// アルファ値の合成	
	g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_CURRENT);

	// フォントオブジェクトの生成
	D3DXCreateFont(
		g_pD3DDevice,20,0,0,0,FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&g_pFont);

	// 入力の初期化処理
	InitKeyboard(hInstance, hWnd);

	// パッド入力の初期化処理
	InitGamepad(hInstance, hWnd);

	// サウンド再生の初期化処理
	InitSound(hWnd);

	// モードの設定
	SetMode(g_mode);

	// フェードの初期化処理
	InitFade(g_mode);
	
	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void Uninit(void)
{
	// サウンド再生の終了処理
	UninitSound();

	// フェードの終了処理
	UninitFade();

	// フォントの開放
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3Dデバイスの開放
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの開放
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// 入力の終了処理
	UninitKeyboard();

	// パッド入力の終了処理
	UninitGamepad();
}

//==============================================================================
// 更新処理
//==============================================================================
void Update(void)
{
	// 入力の更新処理
	UpdateKeyboard();

	// パッド入力の更新処理
	UpdateGamepad();

	// モードの設定
	switch (g_mode)
	{
	case MODE_TITLE:
		// タイトル画面の更新処理
		UpdateTitle();
		break;

	case MODE_TUTORIAL:
		// チュートリアル画面の更新処理
		UpdateTutorial();
		break;

	case MODE_GAME:
		// ゲーム画面の更新処理
		UpdateGame();
		break;

	case MODE_RESULT:
		// リザルト画面の更新処理
		UpdateResult();
		break;

	case MODE_RANKING:
		// ランキング画面の更新処理
		UpdateRanking();
		break;

	default:
		break;
	}

	// フェードの更新処理
	UpdateFade();
}

//==============================================================================
// 描画処理
//==============================================================================
void Draw(void)
{
	// バックバッファ＆Zバッファのクリア(画面のクリア)
	g_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	// 描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (g_mode)
		{
		case MODE_TITLE:
			// タイトル画面の描画処理
			DrawTitle();
			break;

		case MODE_TUTORIAL:
			// チュートリアル画面の描画処理
			DrawTutorial();
			break;

		case MODE_GAME:
			// ローカル変数宣言
			D3DVIEWPORT9 viewportDef;

			// 現在のビューポートを取得
			g_pD3DDevice->GetViewport(&viewportDef);

			// カメラの設定
			for (int nCntCamera = 0; nCntCamera < 2; nCntCamera++)
			{
				SetCamera(nCntCamera);

				// ゲーム画面の描画処理
				DrawGame();
			}

			// ビューポートを戻す
			g_pD3DDevice->SetViewport(&viewportDef);

			break;

		case MODE_RESULT:
			// リザルト画面の描画処理
			DrawResult();
			break;

		case MODE_RANKING:
			// ランキング画面の描画処理
			DrawRanking();
			break;

		default:
			break;
		}

		// フェードの描画処理
		DrawFade();

		// デバッグモードの時
		#ifdef _DEBUG

		// FPSの描画処理
		DrawFPS();

		DrawT();

		#endif

		// 描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==============================================================================
// FPSの描画処理
//==============================================================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[1024];
	int nNum;
	Camera camera = *GetCamera();
	Player player = *GetPlayer();
	//Model model = *GetModel();

	nNum = sprintf(&aStr[0], "FPS:%d\n",g_nCountFPS);
	nNum += sprintf(&aStr[nNum], " (視点)  X:%.2f Y:%.2f Z:%.2f\n", camera.posV.x, camera.posV.y, camera.posV.z);
	nNum += sprintf(&aStr[nNum], " (注視点) X:%.2f Y:%.2f Z:%.2f\n", camera.posR.x, camera.posR.y, camera.posR.z);
	//nNum += sprintf(&aStr[nNum], "(モデルの位置) X:%.2f Y:%.2f Z:%.2f\n", model.pos.x, model.pos.y, model.pos.z);
	//nNum += sprintf(&aStr[nNum], "(モデルの向き) X:%.3f Y:%.3f Z:%.3f\n", model.rot.x, model.rot.y, model.rot.z);
	nNum += sprintf(&aStr[nNum], "(モデルのキー):%d\n",player.nKey);
	nNum += sprintf(&aStr[nNum], "(モーションカウンター):%d\n",player.nCounterMotion);
	nNum += sprintf(&aStr[nNum], "(モデルとカメラの距離) %.1f\n", camera.fLength);
	nNum += sprintf(&aStr[nNum], "(モデルの前方の距離) 　%.1f\n", camera.fFront);
	nNum += sprintf(&aStr[nNum], "モデルの移動　　　：[W:上 A:左 S:下 D:右]\n");
	nNum += sprintf(&aStr[nNum], "モデルの上昇下降　：[U:上 M:下]\n");
	nNum += sprintf(&aStr[nNum], "視点の旋回　　　　：[Z:左 C:右]\n");
	nNum += sprintf(&aStr[nNum], "注視点の上下移動　：[T:上 B:下]\n");
	nNum += sprintf(&aStr[nNum], "視点の上下移動　　：[Y:上 N:下]\n");
	nNum += sprintf(&aStr[nNum], "ズームイン・アウト：[U:イン M:アウト]\n");
	nNum += sprintf(&aStr[nNum], "ジャンプ　　　　　：[SPACE]\n");
	nNum += sprintf(&aStr[nNum], "画面遷移：[ENTER]\nプレイヤー位置リセット：[1]\nカメラ位置リセット：[2]\n");




	// テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//==============================================================================
// デバイスの取得
//==============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//==============================================================================
// モードの設定
//==============================================================================
void SetMode(MODE mode)
{
	switch (g_mode)
	{
	case MODE_TITLE:
		// タイトル画面の終了処理
		UninitTitle();
		break;

	case MODE_TUTORIAL:
		// チュートリアル画面の終了処理
		UninitTutorial();
		break;

	case MODE_GAME:
		// ゲーム画面の終了処理
		UninitGame();
		break;

	case MODE_RESULT:
		// リザルト画面の終了処理
		UninitResult();
		break;

	case MODE_RANKING:
		// ランキング画面の終了処理
		UninitRanking();
		break;

	default:
		break;
	}
	 
	switch (mode)
	{
	case MODE_TITLE:
		// タイトル画面の初期化処理
		InitTitle();

		// ステージカウントの初期化
		//InitGameCnt();

		// スコアの初期化処理
		InitScore();

		break;

	case MODE_TUTORIAL:
		// チュートリアル画面の初期化処理
		InitTutorial();
		break;

	case MODE_GAME:
		// ゲーム画面の初期化処理
		InitGame();
		break;

	case MODE_RESULT:
		// リザルト画面の初期化処理
		InitResult();
		break;

	case MODE_RANKING:
		// ランキング画面の初期化処理
		InitRanking();
		break;

	default:
		break;
	}

	g_mode = mode;
}

//==============================================================================
// モードの取得
//==============================================================================
MODE GetMode(void)
{
	return g_mode;
}

// テキスト描写
void DrawT(void)
{
	//RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	//char aStr[512];

	//Camera camera = GetCamera();

	////int nNum;

	//sprintf(&aStr[0], "FPS:%d\n",g_nCountFPS);

	//// テキストの描画
	//g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 255, 255, 255));
}