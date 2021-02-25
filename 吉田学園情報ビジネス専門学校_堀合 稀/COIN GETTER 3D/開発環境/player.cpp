//==============================================================================
//
// 3Dプレイヤーの描画〔player.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "object.h"
#include "item.h"
#include "sound.h"
#include "enemy.h"
#include "gamepad.h"
#include "math.h"
#include <stdio.h>

//==============================================================================
// マクロ定義
//==============================================================================
#define PLAYER_MOVE		(1.0f)
#define PLAYER_TURN		(0.1f)
#define VTX_MINP		(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))		// 仮頂点最小値
#define	VTX_MAXP		(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))	// 仮頂点最大値
#define TEXT_LENGTH		(128)	// テキスト読み込みの最大数
#define DATA_LENGTH		(64)	// データ読み込みの最大数
#define MOVE_MIN		(0.5f)	// 移動モーション時の最小移動力

//==============================================================================
// グローバル変数
//==============================================================================

//LPD3DXMESH g_pMeshPlayer = NULL;						// メッシュ(頂点情報)へのポインタ
//LPD3DXBUFFER g_pBuffMatPlayer = NULL;					// マテリアル(材質情報)へのポインタ
//LPDIRECT3DTEXTURE9 g_apTextureModel[TEX_MAX] = {};	// テクスチャへのポインタ
//DWORD g_nNumMatPlayer = 0;							// マテリアルの数
D3DXVECTOR3 g_vtxMinPlayer, g_vtxMaxPlayer;	// モデルの各座標の最大値・最小値
Player g_player;							// プレイヤーの構造体
bool g_bJump;

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitPlayer(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	LoadMotion();
	
	// Xファイルの読み込み
	for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
	{
		D3DXLoadMeshFromX(
			g_player.aModel[nCnt].aFileName,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[nCnt].pBuffMatModel,
			NULL,
			&g_player.aModel[nCnt].nNumMatModel,
			&g_player.aModel[nCnt].pMeshModel);
	}

	// ローカル変数宣言
	int nNumVtx;			// 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = g_player.aModel[0].pMeshModel->GetNumVertices();

	// 頂点フォーマットのサイズの取得
	sizeFVF = D3DXGetFVFVertexSize(g_player.aModel[0].pMeshModel->GetFVF());

	// 変数の初期化
	g_player.pos = D3DXVECTOR3(0.0f, 200.0f, 0.0f);
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	g_player.rotDest = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.state = PLAYERSTATE_APPEAR;
	g_player.nLife = MAX_LIFE;
	g_player.motionType = MOTIONTYPE_NEUTRAL;
	g_player.bLoopMotion = false;
	g_player.bOnBlock = false;
	g_player.nNumKey = 0;
	g_player.nKey = 0;
	g_player.nCounterMotion = 0;
	g_player.nCntState = 120;
	g_player.fInRadius = 0.0f;
	g_vtxMinPlayer = VTX_MINP;
	g_vtxMaxPlayer = VTX_MAXP;
	g_bJump = true;

	// 影の設定
	g_player.nIdx = SetShadow(D3DXVECTOR3(g_player.pos.x, 0.0f, g_player.pos.z), 10.0f, 10.0f);

	// 頂点バッファをロック
	g_player.aModel[0].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// 頂点座標の比較と更新
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

		// 各座標の最大値の比較
		if (g_vtxMaxPlayer.x < vtx.x)
		{			
			g_vtxMaxPlayer.x = vtx.x;
		}			
		if (g_vtxMaxPlayer.y < vtx.y)
		{			
			g_vtxMaxPlayer.y = vtx.y;
		}			
		if (g_vtxMaxPlayer.z < vtx.z)
		{			
			g_vtxMaxPlayer.z = vtx.z;
		}

		// 各座標の最小値の比較
		if (g_vtxMinPlayer.x > vtx.x)
		{			
			g_vtxMinPlayer.x = vtx.x;
		}			
		if (g_vtxMinPlayer.y > vtx.y)
		{			
			g_vtxMinPlayer.y = vtx.y;
		}			
		if (g_vtxMinPlayer.z > vtx.z)
		{			
			g_vtxMinPlayer.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファをアンロック
	g_player.aModel[0].pMeshModel->UnlockVertexBuffer();

	// 内半径の計算
	if (g_player.fInRadius < g_vtxMaxPlayer.x)
	{// X軸最大値
		g_player.fInRadius = g_vtxMaxPlayer.x;
	}
	if (g_player.fInRadius < g_vtxMaxPlayer.z)
	{// Z軸最大値
		g_player.fInRadius = g_vtxMaxPlayer.z;
	}
	if (g_player.fInRadius < -g_vtxMinPlayer.x)
	{// X軸最小値
		g_player.fInRadius = -g_vtxMinPlayer.x;
	}
	if (g_player.fInRadius < -g_vtxMinPlayer.z)
	{// Z軸最小値
		g_player.fInRadius = -g_vtxMinPlayer.z;
	}

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
	{
		if (g_player.aModel[nCnt].pMeshModel != NULL)
		{// メッシュの破棄
			g_player.aModel[nCnt].pMeshModel->Release();
			g_player.aModel[nCnt].pMeshModel = NULL;
		}
		
		if (g_player.aModel[nCnt].pBuffMatModel != NULL)
		{// バッファの破棄
			g_player.aModel[nCnt].pBuffMatModel->Release();
			g_player.aModel[nCnt].pBuffMatModel = NULL;
		}
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdatePlayer(void)
{
	// カメラの取得
	Camera camera = *GetCamera();
	//bool bJump = true;
	StateChange();

	// 直前の位置の保存
	g_player.posOld = g_player.pos;

	// 重力の発生
	if (g_player.pos.y > 0.0f && g_player.bOnBlock == false)
	{
		if (GetKeyboardPress(KEYINFO_JUMP) != true || IsButtonPush(KEYINFO::KEYINFO_OK) != true)
		{
			g_player.move.y -= PLUS_GRAVITY;
		}
	}

	if (g_player.pos.y < 0.0f)
	{
		g_player.bOnBlock = true;
		g_player.nKey = 0;
		g_player.nCounterMotion = 0;
		g_player.motionType = MOTIONTYPE_LANDING;
	}

	if (g_player.bOnBlock == true)
	{
		if (g_player.pos.y < 0.0f)
		{
			g_player.pos.y = 0.0f;
		}
		g_bJump = false;
		g_player.move.y = 0.0f;

		//g_player.nJumpCnt = 0;
		//if (g_player.bOnBlock == false)
		//{
		//	g_player.bOnBlock = true;
		//}
	}

	// 位置のリセット
	if (GetKeyboardTrigger(KEYINFO_PRESET) == true)
	{
		g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// ジャンプ
	//if (GetKeyboardTrigger(KEYINFO_JUMP) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
	//{
	//	//SetBullet(
	//		//D3DXVECTOR3(g_player.pos.x + sinf(D3DX_PI - g_player.rot.y) * -10.0f, g_player.pos.y + 50.0f, g_player.pos.z + cosf(D3DX_PI - g_player.rot.y) * 10.0f)
	//	//	/*D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 30.0f, g_player.pos.z)*/,
	//	//	D3DXVECTOR3(sinf(D3DX_PI - g_player.rot.y) * -5.0f, 0.0f, cosf(D3DX_PI - g_player.rot.y) * 5.0f),
	//	//	100);
	//	if (g_player.motionType == MOTIONTYPE_MOVE || g_player.motionType == MOTIONTYPE_NEUTRAL)
	//	{
	//		g_player.bOnBlock = false;
	//		PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_player.move.y = 10.0f;
	//		g_player.nKey = 0;
	//		g_player.nCounterMotion = 0;
	//		g_player.motionType = MOTIONTYPE_JUMP;
	//	}
	//}

	// ジャンプ
	if (GetKeyboardTrigger(KEYINFO_JUMP) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
	{
		if (g_player.bOnBlock == true)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_player.nKey = 0;
			g_player.nCounterMotion = 0;
			
			g_player.motionType = MOTIONTYPE_JUMP;
		}
		g_player.bOnBlock = false;
	}
	if (GetKeyboardPress(KEYINFO_JUMP) == true || IsButtonPush(KEYINFO::KEYINFO_OK) == true)
	{
		if (g_bJump == false)
		{
			if (g_player.nJumpCnt == 1)
			{
				g_player.move.y = 5.0f;
			}
			if (g_player.nJumpCnt < 7)
			{
				g_player.nJumpCnt++;
				g_player.move.y += 1.0f;
			}
		}
	}
	if (GetKeyboardRelease(KEYINFO_JUMP) == true || IsButtonUp(KEYINFO::KEYINFO_OK) == true)
	{
		//if (g_player.bOnBlock == true)
		{
			g_player.nJumpCnt = 0;
			g_bJump = true;
		}
	}

	//if (GetKeyboardTrigger(KEYINFO_ATTACK) == true && g_player.motionType == MOTIONTYPE_NEUTRAL)
	//{
	//	g_player.nKey = 0;
	//	g_player.nCounterMotion = 0;
	//	g_player.motionType = MOTIONTYPE_ACTION;
	//}

	// モーションの更新
	UpdateMotion(g_player.motionType);

	// 移動量の加算
	g_player.pos += g_player.move;

	// 影の追従
	SetPositionShadow(g_player.nIdx, D3DXVECTOR3(g_player.pos.x, 0.0f, g_player.pos.z));

	// モデルの移動
	if (GetKeyboardPress(KEYINFO_UP) == true || IsButtonPush(KEYINFO::KEYINFO_UP) == true)
	{
		if (g_player.motionType != MOTIONTYPE_JUMP)
		{
			g_player.motionType = MOTIONTYPE_MOVE;
		}
		if (GetKeyboardPress(KEYINFO_LEFT) == true || IsButtonPush(KEYINFO::KEYINFO_LEFT) == true)
		{// 左上方向
			g_player.move.x -= cosf(camera.rot.y + D3DX_PI / 4) * PLAYER_MOVE;
			g_player.move.z += sinf(camera.rot.y + D3DX_PI / 4) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y + (D3DX_PI * 3 / 4);
		}
		else if (GetKeyboardPress(KEYINFO_RIGHT) == true || IsButtonPush(KEYINFO::KEYINFO_RIGHT) == true)
		{// 右上方向
			g_player.move.x += cosf(camera.rot.y - D3DX_PI / 4) * PLAYER_MOVE;
			g_player.move.z -= sinf(camera.rot.y - D3DX_PI / 4) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y - (D3DX_PI * 3 / 4);
		}
		else
		{// 上方向
			g_player.move.x += sinf(camera.rot.y) * PLAYER_MOVE;
			g_player.move.z += cosf(camera.rot.y) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y + D3DX_PI;
		}
	}
	else if (GetKeyboardPress(KEYINFO_DOWN) == true || IsButtonPush(KEYINFO::KEYINFO_DOWN) == true)
	{
		if (g_player.motionType != MOTIONTYPE_JUMP)
		{
			g_player.motionType = MOTIONTYPE_MOVE;
		}

		if (GetKeyboardPress(KEYINFO_LEFT) == true || IsButtonPush(KEYINFO::KEYINFO_LEFT) == true)
		{// 左下方向
			g_player.move.x += cosf(camera.rot.y + D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_player.move.z -= sinf(camera.rot.y + D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y + D3DX_PI / 4;
		}
		else if (GetKeyboardPress(KEYINFO_RIGHT) == true || IsButtonPush(KEYINFO::KEYINFO_RIGHT) == true)
		{// 右下方向
			g_player.move.x -= cosf(camera.rot.y - D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_player.move.z += sinf(camera.rot.y - D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y + D3DX_PI / -4;
		}
		else
		{// 下方向
			g_player.move.x += sinf(camera.rot.y - D3DX_PI) * PLAYER_MOVE;
			g_player.move.z += cosf(camera.rot.y - D3DX_PI) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y;
		}
	}
	else if (GetKeyboardPress(KEYINFO_LEFT) == true || IsButtonPush(KEYINFO::KEYINFO_LEFT) == true)
	{// 左方向
		g_player.move.x -= cosf(camera.rot.y) * PLAYER_MOVE;
		g_player.move.z += sinf(camera.rot.y) * PLAYER_MOVE;
		g_player.rotDest.y = camera.rot.y + (D3DX_PI / 2);
		if (g_player.motionType != MOTIONTYPE_JUMP)
		{
			g_player.motionType = MOTIONTYPE_MOVE;
		}
	}
	else if (GetKeyboardPress(KEYINFO_RIGHT) == true || IsButtonPush(KEYINFO::KEYINFO_RIGHT) == true)
	{// 右方向
		g_player.move.x += cosf(camera.rot.y) * PLAYER_MOVE;
		g_player.move.z -= sinf(camera.rot.y) * PLAYER_MOVE;
		g_player.rotDest.y = camera.rot.y + (D3DX_PI / -2);
		if (g_player.motionType != MOTIONTYPE_JUMP)
		{
			g_player.motionType = MOTIONTYPE_MOVE;
		}
	}

	if (GetKeyboardTrigger(KEYINFO_UP) == true || GetKeyboardTrigger(KEYINFO_DOWN) == true || GetKeyboardTrigger(KEYINFO_RIGHT) == true || GetKeyboardTrigger(KEYINFO_LEFT) == true
		|| IsButtonDown(KEYINFO::KEYINFO_UP) == true || IsButtonDown(KEYINFO::KEYINFO_LEFT) == true || IsButtonDown(KEYINFO::KEYINFO_RIGHT) == true || IsButtonDown(KEYINFO::KEYINFO_DOWN) == true)
	{// モーションカウントと位置の修正
		g_player.nCounterMotion = 0;
		g_player.aModel[0].pos.y = 20.0f;
	}

	if (GetKeyboardRelease(KEYINFO_UP) == true || GetKeyboardRelease(KEYINFO_DOWN) == true || GetKeyboardRelease(KEYINFO_RIGHT) == true || GetKeyboardRelease(KEYINFO_LEFT) == true
		|| IsButtonUp(KEYINFO::KEYINFO_UP) == true || IsButtonUp(KEYINFO::KEYINFO_LEFT) == true || IsButtonUp(KEYINFO::KEYINFO_RIGHT) == true || IsButtonUp(KEYINFO::KEYINFO_DOWN) == true)
	{// 移動終了時の位置補正
		g_player.aModel[0].pos.y = 20.0f;
	}

	// 移動モーションから待機モーションへ
	if (g_player.motionType == MOTIONTYPE_MOVE && g_player.move.x <= MOVE_MIN && g_player.move.x >= -MOVE_MIN && g_player.move.z <= MOVE_MIN && g_player.move.z >= -MOVE_MIN)
	{
		g_player.nCounterMotion = 0;
		g_player.nKey = 0;
		g_player.motionType = MOTIONTYPE_NEUTRAL;
	}

	// モデルの上昇・下降
	//if (GetKeyboardPress(KEYINFO_UP_MODEL) == true)
	//{
	//	g_player.pos.y += 2;
	//}
	//else if (GetKeyboardPress(KEYINFO_DOWN_MODEL) == true)
	//{
	//	g_player.pos.y += -2;
	//}

	// モデルの旋回
	//if (GetKeyboardPress(KEYINFO_TURN_LEFT_MODEL) == true)
	//{
	//	g_player.rotDest.y += D3DX_PI / 36;
	//}
	//if (GetKeyboardPress(KEYINFO_TURN_RIGHT_MODEL) == true)
	//{
	//	g_player.rotDest.y -= D3DX_PI / 36;
	//}

	// 目的の回転角の上限
	if (g_player.rotDest.y - g_player.rot.y < -D3DX_PI)
	{
		g_player.rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rotDest.y - g_player.rot.y > D3DX_PI)
	{
		g_player.rotDest.y -= D3DX_PI * 2.0f;
	}

	// 向きの更新
	g_player.rot.y += (g_player.rotDest.y - g_player.rot.y) * 0.1f;

	// 現在の回転角の上限
	if (g_player.rot.y < -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rot.y > D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}

	// 加速後の減速処理
	g_player.move.x += (0.0f - g_player.move.x) * SPEEDDOWN;
	g_player.move.z += (0.0f - g_player.move.z) * SPEEDDOWN;

	// 移動限界X軸
	if (g_player.pos.x >= 500.0f - g_vtxMaxPlayer.x)
	{
		g_player.pos.x = 500.0f - g_vtxMaxPlayer.x;
	}
	else if (g_player.pos.x <= -500.0f - g_vtxMinPlayer.x)
	{
		g_player.pos.x = -500.0f - g_vtxMinPlayer.x;
	}

	// 移動限界Z軸
	if (g_player.pos.z >= 500.0f - g_vtxMaxPlayer.z)
	{
		g_player.pos.z = 500.0f - g_vtxMaxPlayer.z;
	}
	else if (g_player.pos.z <= -500.0f - g_vtxMinPlayer.z)
	{
		g_player.pos.z = -500.0f - g_vtxMinPlayer.z;
	}

	// 当たり判定
	g_player.bOnBlock = CollisionVec(&g_player.pos, &g_player.posOld, &g_player.move, g_player.fInRadius,g_player.fHeight);
	TouchItem(&g_player.pos, g_player.fInRadius,g_player.fHeight);
	g_player.bOnBlock = TouchEnemy(&g_player.pos, &g_player.posOld ,&g_player.move,g_vtxMaxPlayer.x, g_vtxMinPlayer.x, g_vtxMaxPlayer.z, g_vtxMinPlayer.z, g_vtxMaxPlayer.y, g_vtxMinPlayer.y);
}

//==============================================================================
// 描画処理
//==============================================================================
void DrawPlayer(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ

												// プレイヤーのワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_player.mtxWorld);

	// プレイヤーの向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	// プレイヤーの位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	// プレイヤーのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel;	// 計算用マトリックス
		D3DXMATRIX mtxParent;					// 親のマトリックス

		// 各パーツのワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_player.aModel[nCntModel].mtxWorld);

		// 各パーツの向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.aModel[nCntModel].rot.y, g_player.aModel[nCntModel].rot.x, g_player.aModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxRotModel);

		// 各パーツの位置を反映
		D3DXMatrixTranslation(&mtxTransModel, g_player.aModel[nCntModel].pos.x, g_player.aModel[nCntModel].pos.y, g_player.aModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxTransModel);

		// 各パーツの親マトリックスを設定
		if (g_player.aModel[nCntModel].nIdxModelParent != -1)
		{
			mtxParent = g_player.aModel[g_player.aModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{
			mtxParent = g_player.mtxWorld;	// プレイヤーのマトリックスを設定
		}

		// 算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxParent);

		// 各パーツのワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.aModel[nCntModel].mtxWorld);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMatModel->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMatModel; nCntMat++)
		{
			if (g_player.state == PLAYERSTATE_APPEAR && g_player.nCntState % 2 != 0)
			{
				pMat[nCntMat].MatD3D.Diffuse.a = 0.0f;
			}
			else
			{
				pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;
			}

			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// モデル(パーツ)の描画
			g_player.aModel[nCntModel].pMeshModel->DrawSubset(nCntMat);
		}
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

}

//==============================================================================
// モデルの取得
//==============================================================================
Player *GetPlayer(void)
{
	return &g_player;
}

//==============================================================================
// モーションの更新
//==============================================================================
void UpdateMotion(MOTIONTYPE motionType)
{
	// キー数とループ情報の代入
	g_player.nNumKey = g_player.aMotionInfo[motionType].nNumKey;
	g_player.bLoopMotion = g_player.aMotionInfo[motionType].bLoop;

	// モーションカウンターの加算
	g_player.nCounterMotion++;

	// モデルパーツごとのモーションアニメーション
	for (int nCnt = 0; nCnt < MODEL_PARTS; nCnt++)
	{
		if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == true)
		{// ループして最初に戻るとき
			g_player.aModel[nCnt].pos.x += (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosX) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.y += (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosY) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.z += (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosZ) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].rot.x = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.y = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.z = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
		}
		else
		{// 通常時
			g_player.aModel[nCnt].pos.x += (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fPosX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosX) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.y += (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fPosY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosY) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.z += (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fPosZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosZ) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].rot.x = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fRotX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.y = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fRotY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.z = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fRotZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
		}

		if (g_player.nCounterMotion == g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame)
		{// キーフレーム終了時
			g_player.nCounterMotion = 0;

			if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == true)
			{// ループするとき
				g_player.nKey = 0;
			}
			else if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == false)
			{// ループせず終了するとき
				if (g_player.motionTypeOld != MOTIONTYPE_NEUTRAL)
				{// 動作直前がニュートラル以外の時
					g_player.motionType = MOTIONTYPE_NEUTRAL;	// ニュートラルへ移行
					g_player.nKey = 0;							// キーのリセット
					g_player.aModel[0].pos.y = 20.0f;			// 位置のリセット
					break;
				}
			}
			else if (g_player.nKey < g_player.nNumKey)
			{// キーのカウント
				g_player.nKey++;
			}
		}

		if (g_player.nKey == 0 && g_player.bLoopMotion == false && g_player.nCounterMotion == 1)
		{// モーション開始時の位置修正
			g_player.aModel[nCnt].pos.x += g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosX;
			g_player.aModel[nCnt].pos.y += g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosY;
			g_player.aModel[nCnt].pos.z += g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosZ;
		}
	}

	// 直前のモーション状態の保存
	g_player.motionTypeOld = g_player.motionType;
}

//==============================================================================
// .txtフォルダの読み込み
//==============================================================================
void LoadMotion(void)
{
	// ローカル変数宣言
	FILE *pFile;
	char aEqual[2] = { NULL };					// 「=」読み取り用変数
	int nCntFile = 0;							// Xファイルの数
	int nIdxParts = 0;							// パーツ数
	int nMotion = 0;							// モーションの数
	bool bComment = false;						// コメントアウトするか
	char aText[TEXT_LENGTH] = {NULL};			// テキスト読み取り用変数
	char aCharaSet[DATA_LENGTH] = { NULL };		// キャラデータ読み取り用変数
	char aPartsSet[DATA_LENGTH] = { NULL };		// パーツデータ読み取り用変数
	char aMotionSet[DATA_LENGTH] = { NULL };	// モーション読み取り用変数
	char aKeySet[DATA_LENGTH] = { NULL };		// キー設定情報読み取り用変数
	char aKey[DATA_LENGTH] = { NULL };			// キー情報読み取り用変数

	// strcmp読み取り用ポインタ
	char *pText = &aText[0];					
	char *pCharaSet = &aCharaSet[0];
	char *pPartsSet = &aPartsSet[0];
	char *pMotionSet = &aMotionSet[0];
	char *pKeySet = &aKeySet[0];
	char *pKey = &aKey[0];

	// データの読み込み
	pFile = fopen("data/TEXT/player_horiai.txt", "r");
	if (pFile != NULL)
	{ //ファイル展開可能
		while (strcmp("END_SCRIPT",pText) != 0)
		{
			aText[0] = {NULL};
			if (bComment == false)
			{// コメントアウトしていない
				fscanf(pFile, "%s", &aText[0]);

				if (aText[0] == '#')
				{// 文字列の先頭が「#」ならばコメントアウトへ
					bComment = true;
				}
				else
				{// 通常時
					if (strcmp("NUM_MODEL", pText) == 0)
					{// モデルの数
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%d", &g_player.nNumModel);
					}
					else if (strcmp("MODEL_FILENAME", pText) == 0)
					{// Xファイル名の読み込み
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%s", &g_player.aModel[nCntFile].aFileName[0]);
						nCntFile++;
					}
					else if (strcmp("CHARACTERSET", pText) == 0)
					{// キャラの情報
						while (strcmp("END_CHARACTERSET", pCharaSet) != 0)
						{// キャラ情報の読み取り
							fscanf(pFile, "%s", &aCharaSet[0]);
							if (strcmp("NUM_PARTS", pCharaSet) == 0)
							{

							}
							else if (strcmp("RADIUS", pCharaSet) == 0)
							{// 当たり判定
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%f", &g_player.fOutRadius);
							}
							else if (strcmp("HEIGHT", pCharaSet) == 0)
							{// 当たり判定
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%f", &g_player.fHeight);
							}
							else if (strcmp("PARTSSET", pCharaSet) == 0)
							{
								aPartsSet[0] = { NULL };

								while (strcmp("END_PARTSSET", pPartsSet) != 0)
								{
									fscanf(pFile, "%s", &aPartsSet[0]);
									if (strcmp("INDEX", pPartsSet) == 0)
									{// パーツ情報の読み取り
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &nIdxParts);
									}
									else if (strcmp("PARENT", pPartsSet) == 0)
									{// パーツの親情報
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &g_player.aModel[nIdxParts].nIdxModelParent);
									}
									else if (strcmp("POS", pPartsSet) == 0)
									{// パーツの位置情報
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &g_player.aModel[nIdxParts].pos.x);
										fscanf(pFile, "%f", &g_player.aModel[nIdxParts].pos.y);
										fscanf(pFile, "%f", &g_player.aModel[nIdxParts].pos.z);
									}
									else if (strcmp("ROT", pPartsSet) == 0)
									{// パーツの角度情報
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &g_player.aModel[nIdxParts].rot.x);
										fscanf(pFile, "%f", &g_player.aModel[nIdxParts].rot.y);
										fscanf(pFile, "%f", &g_player.aModel[nIdxParts].rot.z);
									}
								}
							}
						}	
					}
					else if (strcmp("MOTIONSET", pText) == 0)
					{
						// 変数の初期化
						aMotionSet[0] = { NULL };

						// ローカル変数宣言
						int nLoop = 0;
						int nKeySet = 0;

						while (strcmp("END_MOTIONSET", pMotionSet) != 0)
						{// モーションの読み込み
							fscanf(pFile, "%s", &aMotionSet[0]);
							if (strcmp("LOOP", pMotionSet) == 0)
							{// ループするか
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &nLoop);
								if (nLoop == 1)
								{// １の場合ループする
									g_player.bLoopMotion = true;
									g_player.aMotionInfo[nMotion].bLoop = true;
								}
							}
							else if (strcmp("NUM_KEY", pMotionSet) == 0)
							{// キー数の読み込み
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &g_player.nNumKey);
							}

							else if (strcmp("KEYSET",pMotionSet) == 0)
							{// キーの読み込み
								// ローカル変数宣言
								int nKey = 0;
								// 変数の初期化
								aKeySet[0] = { NULL };

								while (strcmp("END_KEYSET", pKeySet) != 0)
								{// キー情報項目の読み込み
									fscanf(pFile, "%s", &aKeySet[0]);

									if (aKeySet[0] == '#')
									{// コメントアウト
										// ローカル変数宣言
										char c = NULL;
										char d = NULL;
										fscanf(pFile, "%c", &c);
										while (c != '\n' && d != '\n')
										{
											fscanf(pFile, "%s", &aKey[0]);
											fscanf(pFile, "%c", &d);
										}
									}

									if (strcmp("KEY", pKeySet) == 0)
									{// キー情報の読み込み
										aKey[0] = { NULL };
										while (strcmp("END_KEY", pKey) != 0)
										{// 位置・角度の読み込み
											fscanf(pFile, "%s", &aKey[0]);
											if (strcmp("POS", pKey) == 0)
											{// 位置情報の読み込み
												fscanf(pFile, "%s", &aEqual[0]);
												fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosX);
												fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosY);
												fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosZ);
											}
											else if (strcmp("ROT", pKey) == 0)
											{// 角度情報の読み込み
												fscanf(pFile, "%s", &aEqual[0]);
												fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fRotX);
												fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fRotY);
												fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fRotZ);
											}	
										}
										nKey++;
									}
									else if (strcmp("FRAME", pKeySet) == 0)
									{// キーフレーム数の読み込み
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].nFrame);
									}
								}
								nKeySet++;
							}
						}
						g_player.aMotionInfo[nMotion].nNumKey = nKeySet;	// キー数の設定
						nMotion++;
					}
				}
			}
			else if (bComment == true)
			{// コメントアウト処理
				// ローカル変数宣言
				char a = NULL;
				char b = NULL;
				fscanf(pFile, "%c", &a);
				while (a != '\n' && b != '\n')
				{
					fscanf(pFile, "%s", &aText[0]);
					fscanf(pFile, "%c", &b);
				}
				bComment = false;
			}
		}
		fclose(pFile);
	}
	else
	{ // ファイル展開不可
	}
}

//==============================================================================
// プレイヤーの状態変化
//==============================================================================
void StateChange(void)
{
	// ローカル変数宣言
	Player *pPlayer = &g_player;

	// プレイヤーの状態ごとの処理
	switch (pPlayer->state)
	{
	case PLAYERSTATE_DAMAGE:	// 被ダメージ
		g_player.bOnBlock = false;					// 地面から離れる
		PlaySound(SOUND_LABEL_SE_HIT000);			// 命中音の再生
		g_player.move.x *= -1;						// 移動力を反転
		g_player.move.z *= -1;
		g_player.move.y = 5.0f;						// プレイヤーを飛ばす
		g_bJump = true;
		g_player.nKey = 0;							// キーカウントのリセット
		g_player.nCounterMotion = 0;				// モーションカウントのリセット
		pPlayer->motionType = MOTIONTYPE_DAMAGE;	// ダメージモーションの実行

		// 体力が残る場合
		if (pPlayer->nLife > 0)
		{
			pPlayer->nCntState--;					// 状態カウントの減算
			if (pPlayer->nCntState <= 0)
			{// 次の状態へ移行
				pPlayer->nCntState = 120;
				pPlayer->state = PLAYERSTATE_APPEAR;
			}
		}
		else
		{// 体力が0の時
			pPlayer->state = PLAYERSTATE_GAMEOVER;		// ゲームーオーバー
		}

		break;

	case PLAYERSTATE_APPEAR:	// 出現時
		pPlayer->nCntState--;							// 状態カウントの減算

		if (pPlayer->nCntState <= 0)
		{// 通常状態への移行
			pPlayer->state = PLAYERSTATE_NORMAL;		// 通常状態
		}
		break;
	}
}