//==============================================================================
//
// カメラの設定〔camera.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "camera.h"
#include "player.h"
#include "gamepad.h"
#include "input.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define CAMERA_MOVE		(5.0f)				// カメラの移動量
#define CAMERA_TURN		(D3DX_PI / 100.0f)	// カメラの旋回速度
#define CAMERA_RANGE	(400.0f)			// 視点・注視点間の距離
#define CAMERA_HEIGHT	(100.0f)			// カメラ(視点)の高さ
#define CAMERA_SPDOWN	(0.1f)				// 減衰係数
#define CAMERA_LIMIT_TOP	(150.0f)		// カメラ上げの最大値
#define CAMERA_LIMIT_BOTTOM	(-50.0f)		// カメラ降ろしの最小値

//==============================================================================
// グローバル変数
//==============================================================================
Camera g_camera[CAMERA_MAX];

//==============================================================================
// 初期化処理
//==============================================================================
void InitCamera(void)
{
	
	for (int nCnt = 0; nCnt < CAMERA_MAX; nCnt++)
	{
		CameraInfo(nCnt);
		//InitPlayer();
	}
}	

//==============================================================================
// 終了処理
//==============================================================================
void UninitCamera(void)
{

}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateCamera(void)
{
	for (int nCntPlayer = 0; nCntPlayer < CAMERA_MAX; nCntPlayer++)
	{
		MoveCamera(nCntPlayer);
	}
}

//==============================================================================
// カメラの設定処理
//==============================================================================
void SetCamera(int nIdx)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// ビューポートの設定
	pDevice->SetViewport(&g_camera[nIdx].viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera[nIdx].mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(
		&g_camera[nIdx].mtxProjection,
		D3DXToRadian(45.0f),							// 画角の設定
		((float)SCREEN_WIDTH / 2.0f) / (float)SCREEN_HEIGHT,		// 画面比率の設定
		10.0f,											// 手前限界の描画距離
		3000.0f);										// 奥限界の描画距離

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera[nIdx].mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera[nIdx].mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(
		&g_camera[nIdx].mtxView,
		&g_camera[nIdx].posV,
		&g_camera[nIdx].posR,
		&g_camera[nIdx].vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera[nIdx].mtxView);
}

//==============================================================================
// カメラの取得
//==============================================================================
Camera *GetCamera(void)
{
	return &g_camera[0];
}

//==============================================================================
// カメラの情報
//==============================================================================
void CameraInfo(int nIdx)
{
	// ローカル変数宣言
	Player *player = GetPlayer();

	player += nIdx;

	switch (nIdx)
	{
	case 0:
		// 構造体の初期化
		g_camera[nIdx].posV = D3DXVECTOR3(
			player->pos.x,
			player->pos.y + 100.0f,
			player->pos.z - 200.0f);							// カメラ視点の位置
		g_camera[nIdx].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 注視点位置
		g_camera[nIdx].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 上方向ベクトル
		g_camera[nIdx].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_camera[nIdx].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// カメラの角度
		g_camera[nIdx].fLength = CAMERA_RANGE;					// 視点・注視点間の距離
		g_camera[nIdx].fHeightV = -60.0f;						// 視点の高さ
		g_camera[nIdx].fHeightR = player->pos.y + 100.0f;				// 注視点の高さ
		g_camera[nIdx].fFront = 15.0f;							// モデルの前方の距離
		g_camera[nIdx].bAutoTurn = false;						// カメラの回り込みON・OFF

		g_camera[nIdx].viewport.X = (DWORD)0.0f;				// 描画する画面左上のＸ座標
		g_camera[nIdx].viewport.Y = (DWORD)0.0f;				// 描画する画面左上のＹ座標
		g_camera[nIdx].viewport.Width = (DWORD)640.0f;			// 描画する画面の幅
		g_camera[nIdx].viewport.Height = (DWORD)720.0f;			// 描画する画面の高さ
		g_camera[nIdx].viewport.MinZ = 0.0f;
		g_camera[nIdx].viewport.MaxZ = 1.0f;
		break;

	case 1:
		// 構造体の初期化
		g_camera[nIdx].posV = D3DXVECTOR3(
			player->pos.x,
			player->pos.y + 100.0f,
			player->pos.z - 200.0f);							// カメラ視点の位置
		g_camera[nIdx].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 注視点位置
		g_camera[nIdx].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 上方向ベクトル
		g_camera[nIdx].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_camera[nIdx].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// カメラの角度
		g_camera[nIdx].fLength = CAMERA_RANGE;					// 視点・注視点間の距離
		g_camera[nIdx].fHeightV = -60.0f;						// 視点の高さ
		g_camera[nIdx].fHeightR = player->pos.y + 100.0f;		// 注視点の高さ
		g_camera[nIdx].fFront = 15.0f;							// モデルの前方の距離
		g_camera[nIdx].bAutoTurn = false;						// カメラの回り込みON・OFF
		g_camera[nIdx].viewport.X = (DWORD)640.0f;				// 描画する画面左上のＸ座標
		g_camera[nIdx].viewport.Y = (DWORD)0.0f;				// 描画する画面左上のＹ座標 
		g_camera[nIdx].viewport.Width = (DWORD)640.0f;			// 描画する画面の幅
		g_camera[nIdx].viewport.Height = (DWORD)720.0f;			// 描画する画面の高さ
		g_camera[nIdx].viewport.MinZ = 0.0f;
		g_camera[nIdx].viewport.MaxZ = 1.0f;

		break;
	default:
		break;
	}
}

// カメラの操作
void MoveCamera(int nCntPlayer)
{
	// ローカル変数宣言
	Player *player = GetPlayer();

	player += nCntPlayer;

	// カメラをプレイヤーの背後へ
	if (GetKeyboardTrigger(KEYINFO_CAMERA_FRONT) == true || BuDo(KEYINFO::KEYINFO_CAMERA_FRONT,nCntPlayer) == true)
	{
		g_camera[nCntPlayer].rot.y = player->rot.y - D3DX_PI;
	}

	//// 注視点の移動(上下)(T,Bキー)
	//if (GetKeyboardPress(KEYINFO_UP_CENTER) == true)
	//{
	//	g_camera[nCntPlayer].fHeightR += CAMERA_MOVE;
	//}
	//if (GetKeyboardPress(KEYINFO_DOWN_CENTER) == true)
	//{
	//	g_camera[nCntPlayer].fHeightR -= CAMERA_MOVE;
	//}

	//視点の移動(左右)(Z,Cキー)
	if (GetKeyboardPress(KEYINFO_LEFT_TURN) == true || BuPu(KEYINFO::KEYINFO_LEFT_TURN,nCntPlayer) == true)
	{
		g_camera[nCntPlayer].rot.y += CAMERA_TURN;

		if (g_camera[nCntPlayer].rot.y > D3DX_PI)
		{
			g_camera[nCntPlayer].rot.y -= D3DX_PI * 2.0f;
		}

		g_camera[nCntPlayer].posV.x = g_camera[nCntPlayer].posR.x - sinf(g_camera[nCntPlayer].rot.y) * g_camera[nCntPlayer].fLength;
		g_camera[nCntPlayer].posV.z = g_camera[nCntPlayer].posR.z - cosf(g_camera[nCntPlayer].rot.y) * g_camera[nCntPlayer].fLength;
	}
	if (GetKeyboardPress(KEYINFO_RIGHT_TURN) == true || BuPu(KEYINFO::KEYINFO_RIGHT_TURN,nCntPlayer) == true)
	{
		g_camera[nCntPlayer].rot.y -= CAMERA_TURN;

		if (g_camera[nCntPlayer].rot.y < -D3DX_PI)
		{
			g_camera[nCntPlayer].rot.y += D3DX_PI * 2.0f;
		}

		g_camera[nCntPlayer].posV.x = g_camera[nCntPlayer].posR.x - sinf(g_camera[nCntPlayer].rot.y) * g_camera[nCntPlayer].fLength;
		g_camera[nCntPlayer].posV.z = g_camera[nCntPlayer].posR.z - cosf(g_camera[nCntPlayer].rot.y) * g_camera[nCntPlayer].fLength;
	}


	// 視点の移動(上下)(Y,Nキー)
	if (GetKeyboardPress(KEYINFO_UP_SIGHT) == true || BuPu(KEYINFO::KEYINFO_UP_SIGHT,nCntPlayer) == true)
	{
		if (g_camera[nCntPlayer].fHeightR < CAMERA_LIMIT_TOP)
		{
			g_camera[nCntPlayer].fHeightR += 2.5f;
		}
	}
	if (GetKeyboardPress(KEYINFO_DOWN_SIGHT) == true || BuPu(KEYINFO::KEYINFO_DOWN_SIGHT,nCntPlayer) == true)
	{
		if (g_camera[nCntPlayer].fHeightR > CAMERA_LIMIT_BOTTOM)
		{
			g_camera[nCntPlayer].fHeightR -= 2.5f;
		}
	}

	// 1周した角度の修正
	if (g_camera[nCntPlayer].rot.y < -D3DX_PI)
	{
		g_camera[nCntPlayer].rot.y += D3DX_PI * 2.0f;
	}
	else if (g_camera[nCntPlayer].rot.y > D3DX_PI)
	{
		g_camera[nCntPlayer].rot.y -= D3DX_PI * 2.0f;
	}

	// 目的の注視点
	g_camera[nCntPlayer].posRDest.x = player->pos.x - g_camera[nCntPlayer].fFront * sinf(player->rotDest.y);
	g_camera[nCntPlayer].posRDest.z = player->pos.z - g_camera[nCntPlayer].fFront * cosf(player->rotDest.y);
	g_camera[nCntPlayer].posRDest.y = player->pos.y + g_camera[nCntPlayer].fHeightR;

	// 目的の視点
	g_camera[nCntPlayer].posVDest.x = player->pos.x - sinf(g_camera[nCntPlayer].rot.y) * g_camera[nCntPlayer].fLength;
	g_camera[nCntPlayer].posVDest.z = player->pos.z - cosf(g_camera[nCntPlayer].rot.y) * g_camera[nCntPlayer].fLength;
	g_camera[nCntPlayer].posVDest.y = player->pos.y + cosf(g_camera[nCntPlayer].rot.x) * g_camera[nCntPlayer].fLength + g_camera[nCntPlayer].fHeightV;

	// 注視点の位置更新
	g_camera[nCntPlayer].posR.x += (g_camera[nCntPlayer].posRDest.x - g_camera[nCntPlayer].posR.x) * CAMERA_SPDOWN;
	g_camera[nCntPlayer].posR.z += (g_camera[nCntPlayer].posRDest.z - g_camera[nCntPlayer].posR.z) * CAMERA_SPDOWN;
	g_camera[nCntPlayer].posR.y += (g_camera[nCntPlayer].posRDest.y - g_camera[nCntPlayer].posR.y) * CAMERA_SPDOWN;
																			   
	// 視点の位置更新														
	g_camera[nCntPlayer].posV.x += (g_camera[nCntPlayer].posVDest.x - g_camera[nCntPlayer].posV.x) * CAMERA_SPDOWN;
	g_camera[nCntPlayer].posV.z += (g_camera[nCntPlayer].posVDest.z - g_camera[nCntPlayer].posV.z) * CAMERA_SPDOWN;
	g_camera[nCntPlayer].posV.y += (g_camera[nCntPlayer].posVDest.y - g_camera[nCntPlayer].posV.y) * CAMERA_SPDOWN;
}