//==============================================================================
//
// セーフティラインの描画〔line.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "line.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define LINE_POSX		(640)	// セーフティラインのX座標
#define LINE_POSY		(620)	// セーフティラインのY座標
#define LINE_SIZEX		(640)	// セーフティラインの幅
#define LINE_SIZEY		(12)	// セーフティラインの高さ
#define LINEOVER		(30)	// ライン越えのテクスチャ更新カウント
#define ALERT_COUNTA	(20)	// 警告ラインのカウントA
#define ALERT_COUNTB	(40)	// 警告ラインのカウントB
#define LINE_ALERT		(7)		// ライン越えの警告ライン

//==============================================================================
// セーフティラインの状態
//==============================================================================
typedef enum
{
	LINESTATE_NORMAL = 0,	// 通常状態
	LINESTATE_OVER,			// 通過状態
	LINESTATE_ALERT,		// 警告状態
	LINESTATE_MAX
} LINESTATE;

//==============================================================================
// セーフティラインの構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色・透明度
	LINESTATE linestate;	// セーフティラインの状態
	int nLineCnt;			// ライン越えの変化中のカウント
	int nAlertLine;			// 警告中のカウント
} LINE;

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureLine = NULL;		// ユーザーインターフェーステクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLine = NULL;	// バッファへのポインタ
LINE line;										// セーフティラインの構造体

//==============================================================================
// セーフティラインの初期化処理
//==============================================================================
HRESULT InitLine(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/safetyline000.png",
		&g_pTextureLine);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffLine,
		NULL)))
	{
		return E_FAIL;
	}

	//　変数の初期化
	line.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	line.linestate = LINESTATE_NORMAL;
	line.nLineCnt = 0;
	line.nAlertLine = 0;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);

	// ユーザーインターフェースの中心座標
	line.pos = D3DXVECTOR3(LINE_POSX, LINE_POSY, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(line.pos.x - LINE_SIZEX, line.pos.y + LINE_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(line.pos.x - LINE_SIZEX, line.pos.y - LINE_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(line.pos.x + LINE_SIZEX, line.pos.y + LINE_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(line.pos.x + LINE_SIZEX, line.pos.y - LINE_SIZEY, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	// 頂点バッファをアンロックする
	g_pVtxBuffLine->Unlock();

	return S_OK;
}

//==============================================================================
// セーフティラインの終了処理
//==============================================================================
void UninitLine(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffLine != NULL)
	{
		g_pVtxBuffLine->Release();
		g_pVtxBuffLine = NULL;
	}

	// テクスチャの開放
	if (g_pTextureLine != NULL)
	{
		g_pTextureLine->Release();
		g_pTextureLine = NULL;
	}
}

//==============================================================================
// セーフティラインの更新処理
//==============================================================================
void UpdateLine(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	PLAYER *Player;
	ENEMY *pEnemy;

	// プレイヤーの取得
	Player = GetPlayer();

	// 敵の取得
	pEnemy = GetEnemy();

	// セーフティラインの状態
	switch (line.linestate)
	{
	case LINESTATE_NORMAL:
		// カウント・配色の初期化
		line.nLineCnt = 0;
		line.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		break;

	case LINESTATE_OVER:
		// 配色の変化と更新カウント増加
		line.col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		line.nLineCnt++;

		// ステータスの変化
		if (line.nLineCnt >= LINEOVER && Player->nLineOver < LINE_ALERT)
		{
			line.linestate = LINESTATE_NORMAL;
		}
		else if (line.nLineCnt >= LINEOVER && Player->nLineOver >= LINE_ALERT)
		{
			line.linestate = LINESTATE_ALERT;
		}

		break;

	case LINESTATE_ALERT:
		line.nLineCnt = 0;	// 更新カウントの初期化
		line.nAlertLine++;	// アラートカウントの増加

		// アラート時の配色の変化
		if (line.nAlertLine % ALERT_COUNTA == 0 && line.nAlertLine % ALERT_COUNTB != 0)
		{
			line.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (line.nAlertLine % ALERT_COUNTB == 0)
		{
			line.col = D3DXCOLOR(1.0f, 0.3f, 0.9f, 1.0f);
		}

		break;

	default:
		break;
	}

	// ライン越えのステータス変化
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{ // 敵が有効状態
			if (pEnemy->pos.y >= WINDOW_LOWEST)
			{ // ライン越え
				line.linestate = LINESTATE_OVER;
			}
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の更新
	pVtx[0].col = line.col;
	pVtx[1].col = line.col;
	pVtx[2].col = line.col;
	pVtx[3].col = line.col;

	// 頂点バッファをアンロックする
	g_pVtxBuffLine->Unlock();
}

//==============================================================================
// セーフティラインの描画処理
//==============================================================================
void DrawLine(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffLine,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureLine);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,						// 描画を開始する頂点インデックス
		2);						// 描画するプリミティブ数
}