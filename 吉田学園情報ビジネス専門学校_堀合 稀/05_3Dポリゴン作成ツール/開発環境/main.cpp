//==============================================================================
//
// メイン処理〔main.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "manager.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define CLASS_NAME		"WindowClass"	// ウィンドウクラスの名前
#define WINDOW_NAME		"Mesh Tool"	// ウィンドウの名前

//==============================================================================
// プロトタイプ宣言
//==============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ウィンドウプロシージャ

//==============================================================================
// グローバル変数
//==============================================================================
int g_nCountFPS;						// FPSカウンタ

//==============================================================================
// メイン関数
//==============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ランダムの現在時刻除外
	srand((unsigned int)time(0));

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

	// インスタンス生成
	CManager *pManager = new CManager;	// マネージャクラス

	// 初期化処理
	if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while (1)
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
				pManager->Update();

				// 描画処理
				pManager->Draw();

				dwFrameCount++;				
			}
		}
	}

	// マネージャ終了処理
	if (pManager != NULL)
	{
		pManager->Uninit();
		delete pManager;
		pManager = NULL;
	}

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
// FPSカウンタの取得
//==============================================================================
int GetFPSCnt(void)
{
	//if (g_nCountFPS <= 30 && g_nCountFPS > 0)
	//{
	//	int nSet = 0;
	//}

	return g_nCountFPS;
}