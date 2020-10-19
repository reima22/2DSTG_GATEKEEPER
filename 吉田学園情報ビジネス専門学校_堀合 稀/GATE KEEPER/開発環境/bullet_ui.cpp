//==============================================================================
//
// 弾ユーザーインターフェースの描画〔bullet_ui.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "bullet_ui.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define BULLET_UI_POLYGON	(4)			// UIのポリゴン数
#define UI_BULLET_SIZEX		(32)		// 弾UIの幅
#define UI_BULLET_SIZEY		(34)		// 弾UIの高さ
#define UI_BULLET_POSX		(48.0f)		// 弾のUIのX座標
#define UI_BULLET_POSY		(676.0f)	// 弾のUIのX座標
#define CUT_UI_TEXTURE		(3.0f)		// 弾(BCD)UIテクスチャの分割数

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureBulletUi[BULLET_UI_POLYGON] = {};	// ユーザーインターフェーステクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBulletUi = NULL;				// バッファへのポインタ
D3DXVECTOR3 g_posBulletUi;										// 弾ユーザーインターフェース(メイン)の中心の位置
float g_fBullet;												// 通常弾のテクスチャ分割
float g_fBulletPlus[BULLET_PLUS];								// 追加弾のテクスチャ分割

//==============================================================================
// 弾ユーザーインターフェースの初期化処理
//==============================================================================
HRESULT InitBulletUI(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 背景テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bullet_UI000.png",
		&g_pTextureBulletUi[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bullet_UI001.png",
		&g_pTextureBulletUi[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bullet_UI002.png",
		&g_pTextureBulletUi[2]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bullet_UI003.png",
		&g_pTextureBulletUi[3]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * BULLET_UI_POLYGON,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBulletUi,
		NULL)))
	{
		return E_FAIL;
	}

	g_fBullet = 0.0f;
	for (int nCnt = 0; nCnt < BULLET_PLUS; nCnt++)
	{
		g_fBulletPlus[nCnt] = 0.0f;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBulletUi->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < BULLET_UI_POLYGON; nCnt++, pVtx += 4)
	{
		// 弾ユーザーインターフェースの中心座標
		g_posBulletUi = D3DXVECTOR3(UI_BULLET_POSX + ((float)nCnt * 80.0f), UI_BULLET_POSY, 0.0f);

		// ポリゴンの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_posBulletUi.x - UI_BULLET_SIZEX, g_posBulletUi.y + UI_BULLET_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posBulletUi.x - UI_BULLET_SIZEX, g_posBulletUi.y - UI_BULLET_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posBulletUi.x + UI_BULLET_SIZEX, g_posBulletUi.y + UI_BULLET_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posBulletUi.x + UI_BULLET_SIZEX, g_posBulletUi.y - UI_BULLET_SIZEY, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBulletUi->Unlock();

	return S_OK;
}

//==============================================================================
// 弾ユーザーインターフェースの終了処理
//==============================================================================
void UninitBulletUI(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffBulletUi != NULL)
	{
		g_pVtxBuffBulletUi->Release();
		g_pVtxBuffBulletUi = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < BULLET_UI_POLYGON; nCnt++)
	{
		if (g_pTextureBulletUi[nCnt] != NULL)
		{
			g_pTextureBulletUi[nCnt]->Release();
			g_pTextureBulletUi[nCnt] = NULL;
		}
	}
}

//==============================================================================
// 弾ユーザーインターフェースの更新処理
//==============================================================================
void UpdateBulletUI(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	PLAYER *Player;
	SETBULLET state;

	// プレイヤーの取得
	Player = GetPlayer();

	// 弾の装填状態の取得
	state = GetState();

	// アイテム取得によるUIテクスチャの切り替え
	for (int nCnt = 0; nCnt < BULLET_PLUS; nCnt++)
	{
		if (Player->aBullet[nCnt] == true && g_fBulletPlus[nCnt] == 0.0f)
		{
			g_fBulletPlus[nCnt] += 1.0f / CUT_UI_TEXTURE;
		}
	}

	// アイテム切り替えによるテクスチャの切り替え
	if (GetKeyboardTrigger(KEYINFO_CHANGE) == true && state == SETBULLET_NORMAL && Player->aBullet[0] == true)
	{ // 通常弾からウェーブへ
		g_fBullet += 0.5f;
		g_fBulletPlus[0] += 1.0f / CUT_UI_TEXTURE;
	}
	else if (GetKeyboardTrigger(KEYINFO_CHANGE) == true && state == SETBULLET_NORMAL && Player->aBullet[0] == false && Player->aBullet[1] == true)
	{ // 通常弾からミサイルへ
		g_fBullet += 0.5f;
		g_fBulletPlus[1] += 1.0f / CUT_UI_TEXTURE;
	}
	else if (GetKeyboardTrigger(KEYINFO_CHANGE) == true && state == SETBULLET_NORMAL && Player->aBullet[0] == false && Player->aBullet[1] == false && Player->aBullet[2] == true)
	{ // 通常弾からリバースウェーブへ
		g_fBullet += 0.5f;
		g_fBulletPlus[2] += 1.0f / CUT_UI_TEXTURE;
	}
	else if (GetKeyboardTrigger(KEYINFO_CHANGE) == true && state == SETBULLET_WAVE && Player->aBullet[1] == true)
	{ // ウェーブからミサイルへ
		g_fBulletPlus[0] -= 1.0f / CUT_UI_TEXTURE;
		g_fBulletPlus[1] += 1.0f / CUT_UI_TEXTURE;
	}
	else if (GetKeyboardTrigger(KEYINFO_CHANGE) == true && state == SETBULLET_WAVE && Player->aBullet[1] == false && Player->aBullet[2] == true)
	{ // ウェーブからリバースウェーブへ
		g_fBulletPlus[0] -= 1.0f / CUT_UI_TEXTURE;
		g_fBulletPlus[2] += 1.0f / CUT_UI_TEXTURE;
	}
	else if (GetKeyboardTrigger(KEYINFO_CHANGE) == true && state == SETBULLET_WAVE && Player->aBullet[1] == false && Player->aBullet[2] == false)
	{ // ウェーブから通常弾へ
		g_fBullet -= 0.5f;
		g_fBulletPlus[0] -= 1.0f / CUT_UI_TEXTURE;
	}
	else if (GetKeyboardTrigger(KEYINFO_CHANGE) == true && state == SETBULLET_MISSILE && Player->aBullet[2] == true)
	{ // ミサイルからリバースウェーブへ
		g_fBulletPlus[1] -= 1.0f / CUT_UI_TEXTURE;
		g_fBulletPlus[2] += 1.0f / CUT_UI_TEXTURE;
	}
	else if (GetKeyboardTrigger(KEYINFO_CHANGE) == true && state == SETBULLET_MISSILE && Player->aBullet[2] == false)
	{ // ミサイルから通常弾へ
		g_fBullet -= 0.5f;
		g_fBulletPlus[1] -= 1.0f / CUT_UI_TEXTURE;
	}
	else if (GetKeyboardTrigger(KEYINFO_CHANGE) == true && state == SETBULLET_REVERSEWAVE)
	{ // リバースウェーブから通常弾へ
		g_fBullet -= 0.5f;
		g_fBulletPlus[2] -= 1.0f / CUT_UI_TEXTURE;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBulletUi->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の更新
	pVtx[0].tex = D3DXVECTOR2(0.0f + g_fBullet, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + g_fBullet, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.5f + g_fBullet, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f + g_fBullet, 0.0f);

	for (int nCnt = 0; nCnt < BULLET_PLUS; nCnt++,pVtx += 4)
	{
		pVtx[4].tex = D3DXVECTOR2(0.0f + g_fBulletPlus[nCnt], 1.0f);
		pVtx[5].tex = D3DXVECTOR2(0.0f + g_fBulletPlus[nCnt], 0.0f);
		pVtx[6].tex = D3DXVECTOR2(1.0f / 3.0f + g_fBulletPlus[nCnt], 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f / 3.0f + g_fBulletPlus[nCnt], 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBulletUi->Unlock();
}

//==============================================================================
// 弾ユーザーインターフェースの描画処理
//==============================================================================
void DrawBulletUI(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffBulletUi,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < BULLET_UI_POLYGON; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureBulletUi[nCnt]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}
}