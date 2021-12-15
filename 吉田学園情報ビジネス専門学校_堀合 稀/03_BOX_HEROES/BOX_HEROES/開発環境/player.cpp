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
#include "gamepad.h"
#include "select.h"
#include "hp.h"
#include "light.h"
#include "particle.h"
#include "math.h"
#include <stdio.h>

//==============================================================================
// マクロ定義
//==============================================================================
#define PLAYER_MOVE		(1.0f)	// 移動力
#define PLAYER_TURN		(0.1f)	// 旋回
#define TEXT_LENGTH		(128)	// テキスト読み込みの最大数
#define DATA_LENGTH		(64)	// データ読み込みの最大数
#define MOVE_MIN		(0.5f)	// 移動モーション時の最小移動力
#define MOVECNT_MAX		(20)	// 移動モーション以降カウント最大値
#define SPHERE_SLICE	(10)	// 球体スライス数
#define SPHERE_STACK	(7)		// 球体スタック数

//==============================================================================
// グローバル変数
//==============================================================================
Player g_player[PLAYERNUM_MAX];	// プレイヤーの構造体
int g_nSMG[PLAYERNUM_MAX];

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitPlayer(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得
	Select *pSelect = GetSelect();

	// 第1プレイヤーの状態
	g_player[0].p_type = P_TYPE_P1;

	// 第2プレイヤーがCPUか
	if (pSelect->mode == PLAYMODE_0)
	{
		g_player[1].p_type = P_TYPE_CPU;
	}
	else
	{
		g_player[1].p_type = P_TYPE_P2;
	}

	for (int nCnt = 0; nCnt < PLAYERNUM_MAX; nCnt++)
	{
		if (pSelect->chara[nCnt] == CHARA_0)
		{
			g_player[nCnt].modelType = MODELTYPE_0;
		}
		else if (pSelect->chara[nCnt] == CHARA_1)
		{
			g_player[nCnt].modelType = MODELTYPE_1;
		}
		else
		{
			g_player[nCnt].modelType = MODELTYPE_2;
		}
	}

	LoadMotion();

	g_player[0].pos = D3DXVECTOR3(-850.0f, 20.0f, -550.0f);

	g_player[1].pos = D3DXVECTOR3(850.0f, 20.0f, 550.0f);
	
	for (int nCntPlayer = 0; nCntPlayer < PLAYERNUM_MAX; nCntPlayer++)
	{
		// Xファイルの読み込み
		for (int nCnt = 0; nCnt < (g_player[nCntPlayer].nNumModel); nCnt++)
		{
			D3DXLoadMeshFromX(
				g_player[nCntPlayer].aModel[nCnt].aFileName,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_player[nCntPlayer].aModel[nCnt].pBuffMatModel,
				NULL,
				&g_player[nCntPlayer].aModel[nCnt].nNumMatModel,
				&g_player[nCntPlayer].aModel[nCnt].pMeshModel);

			g_player[nCntPlayer].aModel[nCnt].bUse = true;

			g_player[nCntPlayer].aModel[nCnt].pBuffMatShadow = g_player[nCntPlayer].aModel[nCnt].pBuffMatModel;
		}

		for (int nShort = 0; nShort < SHORT_MAX; nShort++)
		{
			// 攻撃判定球体の生成
			D3DXCreateSphere(
				pDevice,
				g_player[nCntPlayer].as[nShort].fRadius,
				SPHERE_SLICE,
				SPHERE_STACK,
				&g_player[nCntPlayer].as[nShort].pMeshSphere,
				&g_player[nCntPlayer].as[nShort].pBuffMatSphere);

			g_player[nCntPlayer].as[nShort].bUse = false;
			g_player[nCntPlayer].as[nShort].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
		}


		// ローカル変数宣言
		int nNumVtx;			// 頂点数
		DWORD sizeFVF;			// 頂点フォーマットのサイズ
		BYTE *pVtxBuff;			// 頂点バッファへのポインタ

		D3DXMATERIAL *pMat;		// マテリアルへのポインタ

		// マテリアル情報に対するポインタを取得
		for (int nCnt = 0; nCnt < g_player[nCntPlayer].nNumModel; nCnt++,pMat++)
		{
			pMat = (D3DXMATERIAL*)g_player[nCntPlayer].aModel[nCnt].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_player[nCntPlayer].aModel[nCnt].nNumMatModel; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// ファイル名を使用してテクスチャを読み込む
					D3DXCreateTextureFromFile(
						pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_player[nCntPlayer].aModel[nCnt].pTextureModel[nCntMat]);
				}
			}
		}

		// 頂点数を取得
		nNumVtx = g_player[nCntPlayer].aModel[0].pMeshModel->GetNumVertices();

		// 頂点フォーマットのサイズの取得
		sizeFVF = D3DXGetFVFVertexSize(g_player[nCntPlayer].aModel[0].pMeshModel->GetFVF());

		// 変数の初期化
		g_player[nCntPlayer].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player[nCntPlayer].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		g_player[nCntPlayer].rotDest = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		g_player[nCntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player[nCntPlayer].state = PLAYERSTATE_APPEAR;
		g_player[nCntPlayer].motionType = MOTIONTYPE_NEUTRAL;
		g_player[nCntPlayer].bLoopMotion = false;
		g_player[nCntPlayer].bOnBlock = true;
		g_player[nCntPlayer].nNumKey = 0;
		g_player[nCntPlayer].nKey = 0;
		g_player[nCntPlayer].nCounterMotion = 0;
		g_player[nCntPlayer].nCntState = 120;
		g_player[nCntPlayer].fInRadius = 0.0f;
		g_player[nCntPlayer].bJump = true;
		g_player[nCntPlayer].nMoveCnt = 0;
		g_player[nCntPlayer].nBrendFrame = 5;
		g_player[nCntPlayer].bMotionChange = false;
		g_player[nCntPlayer].vtxMaxPlayer = VTX_MAXP;
		g_player[nCntPlayer].vtxMinPlayer = VTX_MINP;
		g_player[nCntPlayer].weapon = WEAPON_NONE;
		g_player[nCntPlayer].sr = SHORT_NONE;
		g_player[nCntPlayer].nBullet = 0;
		g_nSMG[nCntPlayer] = 0;

		// 影の設定
		//g_player[nCntPlayer].nIdx = SetShadow(D3DXVECTOR3(g_player[nCntPlayer].pos.x, 0.0f, g_player[nCntPlayer].pos.z), 10.0f, 10.0f);

		// 頂点バッファをロック
		g_player[nCntPlayer].aModel[0].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		// 頂点座標の比較と更新
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

			// 各座標の最大値の比較
			if (g_player[nCntPlayer].vtxMaxPlayer.x < vtx.x)
			{			
				g_player[nCntPlayer].vtxMaxPlayer.x = vtx.x;
			}			
			if (g_player[nCntPlayer].vtxMaxPlayer.y < vtx.y)
			{			
				g_player[nCntPlayer].vtxMaxPlayer.y = vtx.y;
			}			
			if (g_player[nCntPlayer].vtxMaxPlayer.z < vtx.z)
			{			
				g_player[nCntPlayer].vtxMaxPlayer.z = vtx.z;
			}

			// 各座標の最小値の比較
			if (g_player[nCntPlayer].vtxMinPlayer.x > vtx.x)
			{			
				g_player[nCntPlayer].vtxMinPlayer.x = vtx.x;
			}			
			if (g_player[nCntPlayer].vtxMinPlayer.y > vtx.y)
			{			
				g_player[nCntPlayer].vtxMinPlayer.y = vtx.y;
			}			
			if (g_player[nCntPlayer].vtxMinPlayer.z > vtx.z)
			{			
				g_player[nCntPlayer].vtxMinPlayer.z = vtx.z;
			}

			pVtxBuff += sizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
		}

		// 内半径の計算
		if (g_player[nCntPlayer].fInRadius < g_player[nCntPlayer].vtxMaxPlayer.x)
		{// X軸最大値
			g_player[nCntPlayer].fInRadius = g_player[nCntPlayer].vtxMaxPlayer.x;
		}
		if (g_player[nCntPlayer].fInRadius < g_player[nCntPlayer].vtxMaxPlayer.z)
		{// Z軸最大値
			g_player[nCntPlayer].fInRadius = g_player[nCntPlayer].vtxMaxPlayer.z;
		}
		if (g_player[nCntPlayer].fInRadius < -g_player[nCntPlayer].vtxMinPlayer.x)
		{// X軸最小値
			g_player[nCntPlayer].fInRadius = -g_player[nCntPlayer].vtxMinPlayer.x;
		}
		if (g_player[nCntPlayer].fInRadius < -g_player[nCntPlayer].vtxMinPlayer.z)
		{// Z軸最小値
			g_player[nCntPlayer].fInRadius = -g_player[nCntPlayer].vtxMinPlayer.z;
		}

		// プレイヤーの高さの代入
		g_player[nCntPlayer].vtxMaxPlayer.y = g_player[nCntPlayer].fHeight;
	}
	

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < PLAYERNUM_MAX; nCntPlayer++)
	{
		for (int nCnt = 0; nCnt < g_player[nCntPlayer].nNumModel; nCnt++)
		{
			if (g_player[nCntPlayer].aModel[nCnt].pMeshModel != NULL)
			{// メッシュの破棄
				g_player[nCntPlayer].aModel[nCnt].pMeshModel->Release();
				g_player[nCntPlayer].aModel[nCnt].pMeshModel = NULL;
			}

			if (g_player[nCntPlayer].aModel[nCnt].pBuffMatModel != NULL)
			{// バッファの破棄
				g_player[nCntPlayer].aModel[nCnt].pBuffMatModel->Release();
				g_player[nCntPlayer].aModel[nCnt].pBuffMatModel = NULL;
			}
		}

		for (int nShort = 0; nShort < SHORT_MAX; nShort++)
		{
			if (g_player[nCntPlayer].as[nShort].pMeshSphere != NULL)
			{
				g_player[nCntPlayer].as[nShort].pMeshSphere->Release();
				g_player[nCntPlayer].as[nShort].pMeshSphere = NULL;
			}						   
									   
			if (g_player[nCntPlayer].as[nShort].pBuffMatSphere != NULL)
			{						   
				g_player[nCntPlayer].as[nShort].pBuffMatSphere->Release();
				g_player[nCntPlayer].as[nShort].pBuffMatSphere = NULL;
			}
		}
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdatePlayer(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	for (int nCntPlayer = 0; nCntPlayer < PLAYERNUM_MAX; nCntPlayer++)
	{
		// 外側判定の外積計算
		g_player[nCntPlayer].aPoint[0] = D3DXVECTOR3(g_player[nCntPlayer].pos.x - g_player[nCntPlayer].fOutRadius, 0.0f, g_player[nCntPlayer].pos.z - g_player[nCntPlayer].fOutRadius);
		g_player[nCntPlayer].aPoint[1] = D3DXVECTOR3(g_player[nCntPlayer].pos.x - g_player[nCntPlayer].fOutRadius, 0.0f, g_player[nCntPlayer].pos.z + g_player[nCntPlayer].fOutRadius);
		g_player[nCntPlayer].aPoint[2] = D3DXVECTOR3(g_player[nCntPlayer].pos.x + g_player[nCntPlayer].fOutRadius, 0.0f, g_player[nCntPlayer].pos.z + g_player[nCntPlayer].fOutRadius);
		g_player[nCntPlayer].aPoint[3] = D3DXVECTOR3(g_player[nCntPlayer].pos.x + g_player[nCntPlayer].fOutRadius, 0.0f, g_player[nCntPlayer].pos.z - g_player[nCntPlayer].fOutRadius);
		
		g_player[nCntPlayer].aVec[0] = g_player[nCntPlayer].aPoint[1] - g_player[nCntPlayer].aPoint[0];
		g_player[nCntPlayer].aVec[1] = g_player[nCntPlayer].aPoint[2] - g_player[nCntPlayer].aPoint[1];
		g_player[nCntPlayer].aVec[2] = g_player[nCntPlayer].aPoint[3] - g_player[nCntPlayer].aPoint[2];
		g_player[nCntPlayer].aVec[3] = g_player[nCntPlayer].aPoint[0] - g_player[nCntPlayer].aPoint[3];

		// モーションの更新
		UpdateMotion(g_player[nCntPlayer].motionType, nCntPlayer);

		// プレイヤーの操作
		MovePlayer(nCntPlayer);

		// プレイヤーの状態変化
		StateChange(nCntPlayer);

		WeaponSet(nCntPlayer, g_player[nCntPlayer].weapon);

		if (g_player[nCntPlayer].nBullet <= 0 && g_player[nCntPlayer].weapon != WEAPON_KNIFE)
		{
			g_player[nCntPlayer].weapon = WEAPON_NONE;
		}

		// 当たり判定
		if(nCntPlayer == PLAYERNUM_0)
		{
			CollisionDamage(nCntPlayer, PLAYERNUM_1, D3DXVECTOR3(g_player[PLAYERNUM_0].as[g_player[PLAYERNUM_0].sr].mtxWorld._41, g_player[PLAYERNUM_0].as[g_player[PLAYERNUM_0].sr].mtxWorld._42, g_player[PLAYERNUM_0].as[g_player[PLAYERNUM_0].sr].mtxWorld._43));
		}
		else if (nCntPlayer == PLAYERNUM_1)
		{
			CollisionDamage(nCntPlayer, PLAYERNUM_0, D3DXVECTOR3(g_player[PLAYERNUM_1].as[g_player[PLAYERNUM_1].sr].mtxWorld._41, g_player[PLAYERNUM_1].as[g_player[PLAYERNUM_1].sr].mtxWorld._42, g_player[PLAYERNUM_1].as[g_player[PLAYERNUM_1].sr].mtxWorld._43));
		}

		g_player[nCntPlayer].bOnBlock = CollisionVec(&g_player[nCntPlayer].pos, &g_player[nCntPlayer].posOld, &g_player[nCntPlayer].move, g_player[nCntPlayer].fInRadius, g_player[nCntPlayer].fHeight, nCntPlayer);
		TouchItem(&g_player[nCntPlayer].pos, g_player[nCntPlayer].fInRadius, g_player[nCntPlayer].fHeight,nCntPlayer);
	}
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
	D3DLIGHT9 *pLight = GetLight();

	D3DXMATRIX mtxShadow;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 pos, normal;
	D3DXPLANE plane;
	D3DMATERIAL9 matShadow;

	for (int nCntPlayer = 0; nCntPlayer < PLAYERNUM_MAX; nCntPlayer++)
	{
		// プレイヤーのワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_player[nCntPlayer].mtxWorld);

		// プレイヤーの向きの反映
		D3DXMatrixRotationYawPitchRoll(
			&mtxRot, 
			g_player[nCntPlayer].rot.y, 
			g_player[nCntPlayer].rot.x, 
			g_player[nCntPlayer].rot.z);
		D3DXMatrixMultiply(
			&g_player[nCntPlayer].mtxWorld,
			&g_player[nCntPlayer].mtxWorld, 
			&mtxRot);

		// プレイヤーの位置を反映
		D3DXMatrixTranslation(
			&mtxTrans, 
			g_player[nCntPlayer].pos.x,
			g_player[nCntPlayer].pos.y, 
			g_player[nCntPlayer].pos.z);
		D3DXMatrixMultiply(
			&g_player[nCntPlayer].mtxWorld,
			&g_player[nCntPlayer].mtxWorld, 
			&mtxTrans);

		// プレイヤーのワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player[nCntPlayer].mtxWorld);



		// シャドウマトリックスの初期化
		D3DXMatrixIdentity(&mtxShadow);

		// ライトベクトルの設定
		vecLight = D3DXVECTOR4(pLight->Direction.x, pLight->Direction.y, pLight->Direction.z, 0.0f);

		// 平面の生成
		pos = g_player[nCntPlayer].posShadow;
		normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		D3DXPlaneFromPointNormal(&plane, &pos, &normal);


		// シャドウマトリックスの設定
		//pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matShadow);

		for (int nCntModel = 0; nCntModel < g_player[nCntPlayer].nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransModel;	// 計算用マトリックス
			D3DXMATRIX mtxParent;					// 親のマトリックス

			// 各パーツのワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_player[nCntPlayer].aModel[nCntModel].mtxWorld);

			// 各パーツの向きを反映
			D3DXMatrixRotationYawPitchRoll(
				&mtxRotModel,
				g_player[nCntPlayer].aModel[nCntModel].rot.y,
				g_player[nCntPlayer].aModel[nCntModel].rot.x,
				g_player[nCntPlayer].aModel[nCntModel].rot.z);

			D3DXMatrixMultiply(
				&g_player[nCntPlayer].aModel[nCntModel].mtxWorld,
				&g_player[nCntPlayer].aModel[nCntModel].mtxWorld,
				&mtxRotModel);

			// 各パーツの位置を反映
			D3DXMatrixTranslation(
				&mtxTransModel,
				g_player[nCntPlayer].aModel[nCntModel].pos.x,
				g_player[nCntPlayer].aModel[nCntModel].pos.y,
				g_player[nCntPlayer].aModel[nCntModel].pos.z);

			D3DXMatrixMultiply(
				&g_player[nCntPlayer].aModel[nCntModel].mtxWorld,
				&g_player[nCntPlayer].aModel[nCntModel].mtxWorld,
				&mtxTransModel);

			// 各パーツの親マトリックスを設定
			if (g_player[nCntPlayer].aModel[nCntModel].nIdxModelParent != -1)
			{
				mtxParent = g_player[nCntPlayer].aModel[g_player[nCntPlayer].aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{
				mtxParent = g_player[nCntPlayer].mtxWorld;	// プレイヤーのマトリックスを設定
			}

			// 算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
			D3DXMatrixMultiply(
				&g_player[nCntPlayer].aModel[nCntModel].mtxWorld,
				&g_player[nCntPlayer].aModel[nCntModel].mtxWorld,
				&mtxParent);

			// 減算合成の生成
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			for (int nCntMat = 0; nCntMat < (int)g_player[nCntPlayer].aModel[nCntModel].nNumMatModel; nCntMat++)
			{
				matShadow.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

				// マテリアルの設定
				pDevice->SetMaterial(&matShadow);

				// テクスチャの設定
				pDevice->SetTexture(0, NULL);

				// シャドウマトリックスの生成
				D3DXMatrixShadow(&mtxShadow, &vecLight, &plane);
				D3DXMatrixMultiply(&mtxShadow, &g_player[nCntPlayer].aModel[nCntModel].mtxWorld, &mtxShadow);

				// シャドウマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

				if (g_player[nCntPlayer].aModel[nCntModel].bUse == true)
				{
					// モデル(パーツ)の描画
					g_player[nCntPlayer].aModel[nCntModel].pMeshModel->DrawSubset(nCntMat);
				}
			}

			// 通常の合成に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matShadow);




		

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		for (int nCntModel = 0; nCntModel < g_player[nCntPlayer].nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransModel;	// 計算用マトリックス
			D3DXMATRIX mtxParent;					// 親のマトリックス

			// 各パーツのワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_player[nCntPlayer].aModel[nCntModel].mtxWorld);

			// 各パーツの向きを反映
			D3DXMatrixRotationYawPitchRoll(
				&mtxRotModel, 
				g_player[nCntPlayer].aModel[nCntModel].rot.y, 
				g_player[nCntPlayer].aModel[nCntModel].rot.x, 
				g_player[nCntPlayer].aModel[nCntModel].rot.z);

			D3DXMatrixMultiply(
				&g_player[nCntPlayer].aModel[nCntModel].mtxWorld, 
				&g_player[nCntPlayer].aModel[nCntModel].mtxWorld, 
				&mtxRotModel);

			// 各パーツの位置を反映
			D3DXMatrixTranslation(
				&mtxTransModel, 
				g_player[nCntPlayer].aModel[nCntModel].pos.x, 
				g_player[nCntPlayer].aModel[nCntModel].pos.y, 
				g_player[nCntPlayer].aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(
				&g_player[nCntPlayer].aModel[nCntModel].mtxWorld,
				&g_player[nCntPlayer].aModel[nCntModel].mtxWorld, 
				&mtxTransModel);

			// 各パーツの親マトリックスを設定
			if (g_player[nCntPlayer].aModel[nCntModel].nIdxModelParent != -1)
			{
				mtxParent = g_player[nCntPlayer].aModel[g_player[nCntPlayer].aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{
				mtxParent = g_player[nCntPlayer].mtxWorld;	// プレイヤーのマトリックスを設定
			}

			// 算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
			D3DXMatrixMultiply(
				&g_player[nCntPlayer].aModel[nCntModel].mtxWorld,
				&g_player[nCntPlayer].aModel[nCntModel].mtxWorld,
				&mtxParent);

			// 各パーツのワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_player[nCntPlayer].aModel[nCntModel].mtxWorld);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_player[nCntPlayer].aModel[nCntModel].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_player[nCntPlayer].aModel[nCntModel].nNumMatModel; nCntMat++)
			{
				if (g_player[nCntPlayer].state == PLAYERSTATE_APPEAR && g_player[nCntPlayer].nCntState % 2 != 0)
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
				pDevice->SetTexture(0, *g_player[nCntPlayer].aModel[nCntModel].pTextureModel);

				if (g_player[nCntPlayer].aModel[nCntModel].bUse == true)
				{
					// モデル(パーツ)の描画
					g_player[nCntPlayer].aModel[nCntModel].pMeshModel->DrawSubset(nCntMat);
				}
			}
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);

		AttackParts(nCntPlayer);
	}
}

//==============================================================================
// プレイヤーの取得
//==============================================================================
Player *GetPlayer(void)
{
	return &g_player[0];
}

//==============================================================================
// モーションの更新
//==============================================================================
void UpdateMotion(MOTIONTYPE motionType, int nCntPlayer)
{
	// キー数とループ情報の代入
	g_player[nCntPlayer].nNumKey = g_player[nCntPlayer].aMotionInfo[motionType].nNumKey;
	g_player[nCntPlayer].bLoopMotion = g_player[nCntPlayer].aMotionInfo[motionType].bLoop;

	// モーションカウンターの加算
	if (g_player[nCntPlayer].bMotionChange == true)
	{// モーションブレンド実行時
		if (g_player[nCntPlayer].nCounterMotion < g_player[nCntPlayer].nBrendFrame)
		{// カウント上限
			g_player[nCntPlayer].nCounterMotion++;
		}
	}
	else
	{// 通常時
		if (g_player[nCntPlayer].nCounterMotion < g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].nFrame)
		{// カウント上限
			g_player[nCntPlayer].nCounterMotion++;
		}
	}


	if (g_player[nCntPlayer].as[g_player[nCntPlayer].sr].nCntFrameTotal < g_player[nCntPlayer].nFrameTotal[motionType])
	{
		g_player[nCntPlayer].as[g_player[nCntPlayer].sr].nCntFrameTotal++;
	}

	// モデルパーツごとのモーションアニメーション
	for (int nCnt = 0; nCnt < MODEL_PARTS; nCnt++)
	{
		if (g_player[nCntPlayer].nKey == g_player[nCntPlayer].nNumKey - 1 && g_player[nCntPlayer].bLoopMotion == true)
		{// ループして最初に戻るとき
			g_player[nCntPlayer].aModel[nCnt].pos.x = g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fPosX + (g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosX - g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fPosX) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].nFrame);
			g_player[nCntPlayer].aModel[nCnt].pos.y = g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fPosY + (g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosY - g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fPosY) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].nFrame);
			g_player[nCntPlayer].aModel[nCnt].pos.z = g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fPosZ + (g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosZ - g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fPosZ) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].nFrame);
			g_player[nCntPlayer].aModel[nCnt].rot.x = g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fRotX + (g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotX - g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fRotX) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].nFrame);
			g_player[nCntPlayer].aModel[nCnt].rot.y = g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fRotY + (g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotY - g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fRotY) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].nFrame);
			g_player[nCntPlayer].aModel[nCnt].rot.z = g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fRotZ + (g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotZ - g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fRotZ) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].nFrame);
			g_player[nCntPlayer].aModel[nCnt].posMemo = g_player[nCntPlayer].aModel[nCnt].pos;
			g_player[nCntPlayer].aModel[nCnt].rotMemo = g_player[nCntPlayer].aModel[nCnt].rot;
		}
		else if (g_player[nCntPlayer].bMotionChange == true)
		{// モーションブレンド
			g_player[nCntPlayer].aModel[nCnt].pos.x = g_player[nCntPlayer].aModel[nCnt].posMemo.x + (g_player[nCntPlayer].aMotionInfo[g_player[nCntPlayer].motionType].aKeySet[0].aKey[nCnt].fPosX - g_player[nCntPlayer].aModel[nCnt].posMemo.x) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].nBrendFrame);
			g_player[nCntPlayer].aModel[nCnt].pos.y = g_player[nCntPlayer].aModel[nCnt].posMemo.y + (g_player[nCntPlayer].aMotionInfo[g_player[nCntPlayer].motionType].aKeySet[0].aKey[nCnt].fPosY - g_player[nCntPlayer].aModel[nCnt].posMemo.y) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].nBrendFrame);
			g_player[nCntPlayer].aModel[nCnt].pos.z = g_player[nCntPlayer].aModel[nCnt].posMemo.z + (g_player[nCntPlayer].aMotionInfo[g_player[nCntPlayer].motionType].aKeySet[0].aKey[nCnt].fPosZ - g_player[nCntPlayer].aModel[nCnt].posMemo.z) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].nBrendFrame);
			g_player[nCntPlayer].aModel[nCnt].rot.x = g_player[nCntPlayer].aModel[nCnt].rotMemo.x + (g_player[nCntPlayer].aMotionInfo[g_player[nCntPlayer].motionType].aKeySet[0].aKey[nCnt].fRotX - g_player[nCntPlayer].aModel[nCnt].rotMemo.x) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].nBrendFrame);
			g_player[nCntPlayer].aModel[nCnt].rot.y = g_player[nCntPlayer].aModel[nCnt].rotMemo.y + (g_player[nCntPlayer].aMotionInfo[g_player[nCntPlayer].motionType].aKeySet[0].aKey[nCnt].fRotY - g_player[nCntPlayer].aModel[nCnt].rotMemo.y) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].nBrendFrame);
			g_player[nCntPlayer].aModel[nCnt].rot.z = g_player[nCntPlayer].aModel[nCnt].rotMemo.z + (g_player[nCntPlayer].aMotionInfo[g_player[nCntPlayer].motionType].aKeySet[0].aKey[nCnt].fRotZ - g_player[nCntPlayer].aModel[nCnt].rotMemo.z) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].nBrendFrame);
			g_player[nCntPlayer].aModel[nCnt].posMemo = g_player[nCntPlayer].aModel[nCnt].pos;
			g_player[nCntPlayer].aModel[nCnt].rotMemo = g_player[nCntPlayer].aModel[nCnt].rot;
		}
		else
		{// 通常時
			g_player[nCntPlayer].aModel[nCnt].pos.x = g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fPosX + (g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey + 1].aKey[nCnt].fPosX - g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fPosX) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].nFrame);
			g_player[nCntPlayer].aModel[nCnt].pos.y = g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fPosY + (g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey + 1].aKey[nCnt].fPosY - g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fPosY) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].nFrame);
			g_player[nCntPlayer].aModel[nCnt].pos.z = g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fPosZ + (g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey + 1].aKey[nCnt].fPosZ - g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fPosZ) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].nFrame);
			g_player[nCntPlayer].aModel[nCnt].rot.x = g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fRotX + (g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey + 1].aKey[nCnt].fRotX - g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fRotX) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].nFrame);
			g_player[nCntPlayer].aModel[nCnt].rot.y = g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fRotY + (g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey + 1].aKey[nCnt].fRotY - g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fRotY) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].nFrame);
			g_player[nCntPlayer].aModel[nCnt].rot.z = g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fRotZ + (g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey + 1].aKey[nCnt].fRotZ - g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].aKey[nCnt].fRotZ) * ((float)g_player[nCntPlayer].nCounterMotion / (float)g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].nFrame);
			g_player[nCntPlayer].aModel[nCnt].posMemo = g_player[nCntPlayer].aModel[nCnt].pos;
			g_player[nCntPlayer].aModel[nCnt].rotMemo = g_player[nCntPlayer].aModel[nCnt].rot;
		}

		if (g_player[nCntPlayer].bMotionChange == false)
		{// モーション変化なし
			if (g_player[nCntPlayer].nCounterMotion == g_player[nCntPlayer].aMotionInfo[motionType].aKeySet[g_player[nCntPlayer].nKey].nFrame)
			{// キーフレーム終了時
				if (g_player[nCntPlayer].motionType != MOTIONTYPE_JUMP || g_player[nCntPlayer].nKey != g_player[nCntPlayer].nNumKey - 2)
				{// ジャンプ以外でキー終了時リセット
					g_player[nCntPlayer].nCounterMotion = 0;
				}

				if (g_player[nCntPlayer].nKey == g_player[nCntPlayer].nNumKey - 1 && g_player[nCntPlayer].bLoopMotion == true)
				{// ループするとき
					g_player[nCntPlayer].nKey = 0;
					g_player[nCntPlayer].as[g_player[nCntPlayer].sr].nCntFrameTotal = 0;
				}
				else if (g_player[nCntPlayer].nKey == g_player[nCntPlayer].nNumKey - 2 && g_player[nCntPlayer].bLoopMotion == false)
				{// ループせず終了するとき
					if (g_player[nCntPlayer].motionTypeOld != MOTIONTYPE_JUMP)
					{// 直前がジャンプ以外の時
						g_player[nCntPlayer].motionType = MOTIONTYPE_NEUTRAL;	// ニュートラルへ移行
						g_player[nCntPlayer].as[g_player[nCntPlayer].sr].nCntFrameTotal = 0;
					}
				}
				else if (g_player[nCntPlayer].nKey < g_player[nCntPlayer].nNumKey)
				{// キーのカウント
					g_player[nCntPlayer].nKey++;
				}
			}
			else if (g_player[nCntPlayer].motionTypeOld != g_player[nCntPlayer].motionType)
			{// モーション変化時	
				if (g_player[nCntPlayer].motionType != MOTIONTYPE_JUMP &&
					g_player[nCntPlayer].motionType != MOTIONTYPE_LANDING)
				{
					g_player[nCntPlayer].bMotionChange = true;
				}

				g_player[nCntPlayer].nKey = 0;
				g_player[nCntPlayer].nCounterMotion = 0;
				g_player[nCntPlayer].attack.nCntFrameTotal = 0;

				if (g_player[nCntPlayer].motionTypeOld == MOTIONTYPE_NEUTRAL && g_player[nCntPlayer].motionType == MOTIONTYPE_MOVE)
				{// ニュートラルから移動時
					g_player[nCntPlayer].nBrendFrame = MBF_NEU_MOVE;
				}
				else if (g_player[nCntPlayer].motionTypeOld == MOTIONTYPE_MOVE && g_player[nCntPlayer].motionType == MOTIONTYPE_NEUTRAL)
				{// 移動からニュートラル時
					g_player[nCntPlayer].nBrendFrame = MBF_MOVE_NEU;
				}
				else if (g_player[nCntPlayer].motionTypeOld == MOTIONTYPE_NEUTRAL && g_player[nCntPlayer].motionType == MOTIONTYPE_ACTION)
				{// ニュートラルからアクション時
					g_player[nCntPlayer].nBrendFrame = g_player[nCntPlayer].aMotionInfo[MOTIONTYPE_ACTION].aKeySet[0].nFrame;
				}
				else if (g_player[nCntPlayer].motionTypeOld == MOTIONTYPE_ACTION && g_player[nCntPlayer].motionType == MOTIONTYPE_NEUTRAL)
				{// アクションからニュートラル時
					g_player[nCntPlayer].nBrendFrame = MBF_ACT_NEU;
				}
				else if (g_player[nCntPlayer].motionTypeOld == MOTIONTYPE_LANDING && g_player[nCntPlayer].motionType == MOTIONTYPE_NEUTRAL)
				{// 着地からニュートラル
					g_player[nCntPlayer].nBrendFrame = MBF_LND_NEU;
				}
				break;
			}
		}
		else if (g_player[nCntPlayer].bMotionChange == true)
		{// モーションブレンド時のカウントリセット
			if (g_player[nCntPlayer].nCounterMotion == g_player[nCntPlayer].nBrendFrame || g_player[nCntPlayer].motionTypeOld != g_player[nCntPlayer].motionType)
			{
				g_player[nCntPlayer].nKey = 0;
				g_player[nCntPlayer].nCounterMotion = 0;
				g_player[nCntPlayer].as[g_player[nCntPlayer].sr].nCntFrameTotal = 0;
				g_player[nCntPlayer].bMotionChange = false;
			}
		}

		// 打撃の当たり判定の有効化
		if (g_player[nCntPlayer].motionType == MOTIONTYPE_ACTION || g_player[nCntPlayer].motionType == MOTIONTYPE_SLASH)
		{
			if (g_player[nCntPlayer].as[g_player[nCntPlayer].sr].nCntFrameTotal >= g_player[nCntPlayer].as[g_player[nCntPlayer].sr].nStartFrame &&
			 g_player[nCntPlayer].as[g_player[nCntPlayer].sr].nCntFrameTotal <= g_player[nCntPlayer].as[g_player[nCntPlayer].sr].nEndFrame)
			{// カウント範囲内で有効
				g_player[nCntPlayer].as[g_player[nCntPlayer].sr].bUse = true;
				if (g_player[nCntPlayer].motionType == MOTIONTYPE_ACTION)
				{
					if (g_player[nCntPlayer].as[0].nCntFrameTotal == g_player[nCntPlayer].as[0].nStartFrame)
					{
						PlaySound(SOUND_LABEL_SE_SMASH);
					}
				}
				else
				{
					if (g_player[nCntPlayer].as[1].nCntFrameTotal == g_player[nCntPlayer].as[1].nStartFrame)
					{
						PlaySound(SOUND_LABEL_SE_SLASH);
					}
				}
			}
		}
		else
		{// それ以外
			g_player[nCntPlayer].as[g_player[nCntPlayer].sr].bUse = false;
		}

		// 当たり判定の表示
		if (g_player[nCntPlayer].as[g_player[nCntPlayer].sr].bUse == true)
		{
			g_player[nCntPlayer].as[g_player[nCntPlayer].sr].col.a = 1.0f;
		}
		else
		{
			g_player[nCntPlayer].as[g_player[nCntPlayer].sr].col.a = 0.0f;
		}
	}

	// 直前のモーション状態の保存
	g_player[nCntPlayer].motionTypeOld = g_player[nCntPlayer].motionType;
}

//==============================================================================
// .txtフォルダの読み込み
//==============================================================================
void LoadMotion(void)
{
	// ローカル変数宣言
	FILE *pFile = NULL;
	char aEqual[2] = { NULL };					// 「=」読み取り用変数
	bool bComment = false;						// コメントアウトするか
	char aText[TEXT_LENGTH] = {NULL};			// テキスト読み取り用変数
	char aCharaSet[DATA_LENGTH] = { NULL };		// キャラデータ読み取り用変数
	char aPartsSet[DATA_LENGTH] = { NULL };		// パーツデータ読み取り用変数
	char aMotionSet[DATA_LENGTH] = { NULL };	// モーション読み取り用変数
	char aKeySet[DATA_LENGTH] = { NULL };		// キー設定情報読み取り用変数
	char aKey[DATA_LENGTH] = { NULL };			// キー情報読み取り用変数

	Select *pSelect = GetSelect();

	//g_player[0].modelType = (ModelType)pSelect->chara;

	// strcmp読み取り用ポインタ
	char *pText = &aText[0];					
	char *pCharaSet = &aCharaSet[0];
	char *pPartsSet = &aPartsSet[0];
	char *pMotionSet = &aMotionSet[0];
	char *pKeySet = &aKeySet[0];
	char *pKey = &aKey[0];

	for (int nCnt = 0; nCnt < PLAYERNUM_MAX; nCnt++)
	{
		switch (g_player[nCnt].modelType)
		{
		case 0:
			// データの読み込み
			pFile = fopen("data/TEXT/motion_player00.txt", "r");

			break;

		case 1:
			// データの読み込み
			pFile = fopen("data/TEXT/motion_player01.txt", "r");

			break;

		case 2:
			// データの読み込み
			pFile = fopen("data/TEXT/motion_player02.txt", "r");

			break;

		default:
			break;
		}

		if (pFile != NULL)
		{ //ファイル展開可能
			aText[0] = { NULL };
			aCharaSet[0] = { NULL };
			aPartsSet[0] = { NULL };
			aMotionSet[0] = { NULL };
			aKeySet[0] = { NULL };	
			aKey[0] = { NULL };
			int nIdxParts = 0;			// パーツ数
			int nMotion = 0;			// モーションの数
			int nCntFile = 0;			// Xデータの数
			int nAs = 0;
			while (strcmp("END_SCRIPT", pText) != 0)
			{
				aText[0] = { NULL };
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
							fscanf(pFile, "%d", &g_player[nCnt].nNumModel);
						}
						else if (strcmp("NUM_WEAPON", pText) == 0)
						{
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &g_player[nCnt].nNumWeapon);
						}
						else if (strcmp("MODEL_FILENAME", pText) == 0)
						{// Xファイル名の読み込み
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%s", &g_player[nCnt].aModel[nCntFile].aFileName[0]);
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
								else if (strcmp("MOVE", pCharaSet) == 0)
								{// 当たり判定
									fscanf(pFile, "%s", &aEqual[0]);
									fscanf(pFile, "%f", &g_player[nCnt].fMove);
								}
								else if (strcmp("JUMP", pCharaSet) == 0)
								{// 当たり判定
									fscanf(pFile, "%s", &aEqual[0]);
									fscanf(pFile, "%f", &g_player[nCnt].fJump);
								}
								else if (strcmp("RADIUS", pCharaSet) == 0)
								{// 当たり判定
									fscanf(pFile, "%s", &aEqual[0]);
									fscanf(pFile, "%f", &g_player[nCnt].fOutRadius);
								}
								else if (strcmp("HEIGHT", pCharaSet) == 0)
								{// 当たり判定
									fscanf(pFile, "%s", &aEqual[0]);
									fscanf(pFile, "%f", &g_player[nCnt].fHeight);
								}
								else if (strcmp("PARTSSET", pCharaSet) == 0)
								{// パーツ情報
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
											fscanf(pFile, "%d", &g_player[nCnt].aModel[nIdxParts].nIdxModelParent);
										}
										else if (strcmp("POS", pPartsSet) == 0)
										{// パーツの位置情報
											fscanf(pFile, "%s", &aEqual[0]);
											fscanf(pFile, "%f", &g_player[nCnt].aModel[nIdxParts].pos.x);
											fscanf(pFile, "%f", &g_player[nCnt].aModel[nIdxParts].pos.y);
											fscanf(pFile, "%f", &g_player[nCnt].aModel[nIdxParts].pos.z);
											g_player[nCnt].aModel[nIdxParts].posReset = g_player[nCnt].aModel[nIdxParts].pos;
										}
										else if (strcmp("ROT", pPartsSet) == 0)
										{// パーツの角度情報
											fscanf(pFile, "%s", &aEqual[0]);
											fscanf(pFile, "%f", &g_player[nCnt].aModel[nIdxParts].rot.x);
											fscanf(pFile, "%f", &g_player[nCnt].aModel[nIdxParts].rot.y);
											fscanf(pFile, "%f", &g_player[nCnt].aModel[nIdxParts].rot.z);
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
										g_player[nCnt].bLoopMotion = true;
										g_player[nCnt].aMotionInfo[nMotion].bLoop = true;
									}
								}
								else if (strcmp("NUM_KEY", pMotionSet) == 0)
								{// キー数の読み込み
									fscanf(pFile, "%s", &aEqual[0]);
									fscanf(pFile, "%d", &g_player[nCnt].nNumKey);
								}
								else if (strcmp("COLLISION", pMotionSet) == 0)
								{
									fscanf(pFile, "%s", &aEqual[0]);
									fscanf(pFile, "%d", &g_player[nCnt].as[nAs].nParts);
									fscanf(pFile, "%f", &g_player[nCnt].as[nAs].pos.x);
									fscanf(pFile, "%f", &g_player[nCnt].as[nAs].pos.y);
									fscanf(pFile, "%f", &g_player[nCnt].as[nAs].pos.z);
									fscanf(pFile, "%f", &g_player[nCnt].as[nAs].fRadius);
									fscanf(pFile, "%d", &g_player[nCnt].as[nAs].nStartFrame);
									fscanf(pFile, "%d", &g_player[nCnt].as[nAs].nEndFrame);
									nAs++;
								}
								else if (strcmp("KEYSET", pMotionSet) == 0)
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
													fscanf(pFile, "%f", &g_player[nCnt].aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosX);
													fscanf(pFile, "%f", &g_player[nCnt].aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosY);
													fscanf(pFile, "%f", &g_player[nCnt].aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosZ);
													g_player[nCnt].aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosX += g_player[nCnt].aModel[nKey].pos.x;
													g_player[nCnt].aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosY += g_player[nCnt].aModel[nKey].pos.y;
													g_player[nCnt].aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosZ += g_player[nCnt].aModel[nKey].pos.z;
												}
												else if (strcmp("ROT", pKey) == 0)
												{// 角度情報の読み込み
													fscanf(pFile, "%s", &aEqual[0]);
													fscanf(pFile, "%f", &g_player[nCnt].aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fRotX);
													fscanf(pFile, "%f", &g_player[nCnt].aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fRotY);
													fscanf(pFile, "%f", &g_player[nCnt].aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fRotZ);
												}
											}
											nKey++;
										}
										else if (strcmp("FRAME", pKeySet) == 0)
										{// キーフレーム数の読み込み
											fscanf(pFile, "%s", &aEqual[0]);
											fscanf(pFile, "%d", &g_player[nCnt].aMotionInfo[nMotion].aKeySet[nKeySet].nFrame);
											g_player[nCnt].nFrameTotal[nMotion] += g_player[nCnt].aMotionInfo[nMotion].aKeySet[nKeySet].nFrame;
											if (g_player[nCnt].aMotionInfo[nMotion].bLoop == false && nKeySet == (g_player[nCnt].nNumKey - 1))
											{
												g_player[nCnt].nFrameTotal[nMotion] -= g_player[nCnt].aMotionInfo[nMotion].aKeySet[nKeySet].nFrame;
											}
										}
									}
									nKeySet++;
								}
							}
							g_player[nCnt].aMotionInfo[nMotion].nNumKey = nKeySet;	// キー数の設定
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
}

//==============================================================================
// プレイヤーの状態変化
//==============================================================================
void StateChange(int nCnt)
{
	// ローカル変数宣言
	Player *pPlayer = &g_player[0];

	pPlayer += nCnt;

	// プレイヤーの状態ごとの処理
	switch (pPlayer->state)
	{
	case PLAYERSTATE_DAMAGE:	// 被ダメージ
		pPlayer->bOnBlock = false;					// 地面から離れる
		pPlayer->move.x *= -1;						// 移動力を反転
		pPlayer->move.z *= -1;
		pPlayer->move.y = 5.0f;						// プレイヤーを飛ばす
		pPlayer->bJump = true;
		pPlayer->nKey = 0;							// キーカウントのリセット
		pPlayer->nCounterMotion = 0;				// モーションカウントのリセット
		pPlayer->motionType = MOTIONTYPE_DAMAGE;	// ダメージモーションの実行

		pPlayer->nCntState--;					// 状態カウントの減算
		if (pPlayer->nCntState <= 0)
		{// 次の状態へ移行
			pPlayer->nCntState = 50;
			pPlayer->state = PLAYERSTATE_APPEAR;
		}
		//else
		//{// 体力が0の時
		//	pPlayer->state = PLAYERSTATE_GAMEOVER;		// ゲームーオーバー
		//}

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

//==============================================================================
// プレイヤーの操作
//==============================================================================
void MovePlayer(int nCnt)
{
	// カメラの取得
	Camera *pCamera = GetCamera();

	pCamera += nCnt;

	// 直前の位置の保存
	g_player[nCnt].posOld = g_player[nCnt].pos;

	// 重力の発生
	if (g_player[nCnt].pos.y > 0.0f && g_player[nCnt].bOnBlock == false)
	{
		if (GetKeyboardPress(KEYINFO_JUMP) != true || BuPu(KEYINFO::KEYINFO_OK,nCnt) != true)
		{
			g_player[nCnt].move.y -= PLUS_GRAVITY;
		}
	}

	// プレイヤーの着地判定
	if (g_player[nCnt].pos.y < 0.0f)
	{
		g_player[nCnt].bOnBlock = true;
		g_player[nCnt].nKey = 0;
		g_player[nCnt].nCounterMotion = 0;
		g_player[nCnt].motionType = MOTIONTYPE_LANDING;
		g_player[nCnt].posShadow = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
	}
	if (g_player[nCnt].bOnBlock == true)
	{
		if (g_player[nCnt].pos.y < 0.0f)
		{
			g_player[nCnt].pos.y = 0.0f;
		}
		g_player[nCnt].bJump = false;
		g_player[nCnt].move.y = 0.0f;
	}

	// 位置のリセット
	if (GetKeyboardTrigger(KEYINFO_PRESET) == true)
	{
		g_player[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player[nCnt].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		g_player[nCnt].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// ジャンプ
	if (GetKeyboardTrigger(KEYINFO_JUMP) == true || BuDo(KEYINFO::KEYINFO_OK,nCnt) == true)
	{
		if (g_player[nCnt].bOnBlock == true)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_player[nCnt].nKey = 0;
			g_player[nCnt].nCounterMotion = 0;
			g_player[nCnt].motionType = MOTIONTYPE_JUMP;
		}
		g_player[nCnt].bOnBlock = false;
	}
	if (GetKeyboardPress(KEYINFO_JUMP) == true || BuPu(KEYINFO::KEYINFO_OK,nCnt) == true)
	{
		if (g_player[nCnt].bJump == false)
		{
			if (g_player[nCnt].nJumpCnt == 1)
			{
				g_player[nCnt].move.y = g_player[nCnt].fJump;
			}
			if (g_player[nCnt].nJumpCnt < 7)
			{
				g_player[nCnt].nJumpCnt++;
				g_player[nCnt].move.y += 1.0f;
			}
		}
	}
	if (GetKeyboardRelease(KEYINFO_JUMP) == true || BuUp(KEYINFO::KEYINFO_OK,nCnt) == true)
	{
		if (g_player[nCnt].bJump == false)
		{
			g_player[nCnt].nJumpCnt = 0;
			g_player[nCnt].bJump = true;
		}
	}

	// 攻撃行動
	if (g_player[nCnt].weapon == WEAPON_MACHINEGUN)
	{// マシンガン
		if (GetKeyboardPress(KEYINFO_ATTACK) == true || BuPu(KEYINFO::KEYINFO_ATTACK, nCnt) == true)
		{
			// カメラをプレイヤーの背後へ
			if (GetKeyboardTrigger(KEYINFO_CAMERA_FRONT) == true || BuDo(KEYINFO::KEYINFO_CAMERA_FRONT, nCnt) == true)
			{
				pCamera->rot.y = g_player[nCnt].rot.y - D3DX_PI;
			}

			g_player[nCnt].nKey = 0;
			g_player[nCnt].nCounterMotion = 0;
			WeaponAction(nCnt, g_player[nCnt].weapon);
			g_nSMG[nCnt]++;
			if (g_nSMG[nCnt] % 8 == 0)
			{
				SetBullet(D3DXVECTOR3(
					g_player[nCnt].aModel[17].mtxWorld._41 + sinf(D3DX_PI - g_player[nCnt].rot.y) * -10.0f,
					g_player[nCnt].aModel[17].mtxWorld._42 + 25.0f,
					g_player[nCnt].aModel[17].mtxWorld._43 + cosf(D3DX_PI - g_player[nCnt].rot.y) * 10.0f),
					D3DXVECTOR3(sinf(D3DX_PI - g_player[nCnt].rot.y) * -15.0f, 0.0f, cosf(D3DX_PI - g_player[nCnt].rot.y) * 15.0f), 100, 1, nCnt);
				g_player[nCnt].nBullet--;
				PlaySound(SOUND_LABEL_SE_SHOT);
			}


		}
		if (GetKeyboardRelease(KEYINFO_ATTACK) == true || BuUp(KEYINFO::KEYINFO_ATTACK, nCnt) == true)
		{
			g_nSMG[nCnt] = 0;
		}
	}
	else if (g_player[nCnt].weapon == WEAPON_HANDGUN)
	{// ハンドガン
		if (GetKeyboardTrigger(KEYINFO_ATTACK) == true || BuDo(KEYINFO::KEYINFO_ATTACK, nCnt) == true)
		{
			g_player[nCnt].nKey = 0;
			g_player[nCnt].nCounterMotion = 0;
			WeaponAction(nCnt, g_player[nCnt].weapon);
			SetBullet(D3DXVECTOR3(
				g_player[nCnt].aModel[16].mtxWorld._41 + sinf(D3DX_PI - g_player[nCnt].rot.y) * -10.0f,
				g_player[nCnt].aModel[16].mtxWorld._42 + 25.0f,
				g_player[nCnt].aModel[16].mtxWorld._43 + cosf(D3DX_PI - g_player[nCnt].rot.y) * 10.0f),
				D3DXVECTOR3(sinf(D3DX_PI - g_player[nCnt].rot.y) * -20.0f, 0.0f, cosf(D3DX_PI - g_player[nCnt].rot.y) * 20.0f), 100, 0, nCnt);
			g_player[nCnt].nBullet--;
			PlaySound(SOUND_LABEL_SE_SHOT);
		}
	}
	else if (g_player[nCnt].weapon == WEAPON_RUNCHER && g_player[nCnt].motionType != MOTIONTYPE_MOVE)
	{// ランチャー
		if (GetKeyboardTrigger(KEYINFO_ATTACK) == true || BuDo(KEYINFO::KEYINFO_ATTACK, nCnt) == true)
		{
			g_player[nCnt].nKey = 0;
			g_player[nCnt].nCounterMotion = 0;
			WeaponAction(nCnt, g_player[nCnt].weapon);
			SetBullet(D3DXVECTOR3(
				g_player[nCnt].aModel[18].mtxWorld._41 + sinf(D3DX_PI - g_player[nCnt].rot.y) * -10.0f,
				g_player[nCnt].aModel[18].mtxWorld._42 + 25.0f,
				g_player[nCnt].aModel[18].mtxWorld._43 + cosf(D3DX_PI - g_player[nCnt].rot.y) * 10.0f),
				D3DXVECTOR3(sinf(D3DX_PI - g_player[nCnt].rot.y) * -20.0f, 0.0f, cosf(D3DX_PI - g_player[nCnt].rot.y) * 20.0f), 100, 2, nCnt);
			g_player[nCnt].nBullet--;
			PlaySound(SOUND_LABEL_SE_EXPLOSION);
		}
	}
	else
	{
		if (GetKeyboardTrigger(KEYINFO_ATTACK) == true || BuDo(KEYINFO::KEYINFO_ATTACK, nCnt) == true)
		{// 近接
			if (g_player[nCnt].motionType == MOTIONTYPE_NEUTRAL)
			{
				g_player[nCnt].nKey = 0;
				g_player[nCnt].nCounterMotion = 0;
				WeaponAction(nCnt, g_player[nCnt].weapon);
				//g_player[nCnt].motionType = MOTIONTYPE_ACTION;
			}
		}
	}

	if (g_player[nCnt].weapon != WEAPON_NONE && BuDo(KEYINFO::KEYINFO_WEAPON_DROP, nCnt) == true)
	{
		g_player[nCnt].weapon = WEAPON_NONE;
		g_player[nCnt].sr = SHORT_NONE;
		g_player[nCnt].nBullet = 0;
	}

	// 移動量の加算
	g_player[nCnt].pos += g_player[nCnt].move;

	// モデルの移動
	if (GetKeyboardPress(KEYINFO_UP) == true || BuPu(KEYINFO::KEYINFO_UP,nCnt) == true)
	{
		if (g_player[nCnt].nMoveCnt < 20)
		{
			g_player[nCnt].nMoveCnt++;
		}

		if (g_player[nCnt].motionType != MOTIONTYPE_JUMP)
		{
			g_player[nCnt].motionType = MOTIONTYPE_MOVE;
		}
		if (GetKeyboardPress(KEYINFO_LEFT) == true || BuPu(KEYINFO::KEYINFO_LEFT,nCnt) == true)
		{// 左上方向
			g_player[nCnt].move.x -= cosf(pCamera->rot.y + D3DX_PI / 4) * g_player[nCnt].fMove;
			g_player[nCnt].move.z += sinf(pCamera->rot.y + D3DX_PI / 4) * g_player[nCnt].fMove;
			g_player[nCnt].rotDest.y = pCamera->rot.y + (D3DX_PI * 3 / 4);
		}
		else if (GetKeyboardPress(KEYINFO_RIGHT) == true || BuPu(KEYINFO::KEYINFO_RIGHT,nCnt) == true)
		{// 右上方向
			g_player[nCnt].move.x += cosf(pCamera->rot.y - D3DX_PI / 4) * g_player[nCnt].fMove;
			g_player[nCnt].move.z -= sinf(pCamera->rot.y - D3DX_PI / 4) * g_player[nCnt].fMove;
			g_player[nCnt].rotDest.y = pCamera->rot.y - (D3DX_PI * 3 / 4);
		}
		else
		{// 上方向
			g_player[nCnt].move.x += sinf(pCamera->rot.y) * g_player[nCnt].fMove;
			g_player[nCnt].move.z += cosf(pCamera->rot.y) * g_player[nCnt].fMove;
			g_player[nCnt].rotDest.y = pCamera->rot.y + D3DX_PI;
		}
	}
	else if (GetKeyboardPress(KEYINFO_DOWN) == true || BuPu(KEYINFO::KEYINFO_DOWN,nCnt) == true)
	{
		if (g_player[nCnt].nMoveCnt < MOVECNT_MAX)
		{
			g_player[nCnt].nMoveCnt++;
		}

		if (g_player[nCnt].motionType != MOTIONTYPE_JUMP)
		{
			g_player[nCnt].motionType = MOTIONTYPE_MOVE;
		}

		if (GetKeyboardPress(KEYINFO_LEFT) == true || BuPu(KEYINFO::KEYINFO_LEFT, nCnt) == true)
		{// 左下方向
			g_player[nCnt].move.x += cosf(pCamera->rot.y + D3DX_PI * 3 / 4) * g_player[nCnt].fMove;
			g_player[nCnt].move.z -= sinf(pCamera->rot.y + D3DX_PI * 3 / 4) * g_player[nCnt].fMove;
			g_player[nCnt].rotDest.y = pCamera->rot.y + D3DX_PI / 4;
		}
		else if (GetKeyboardPress(KEYINFO_RIGHT) == true || BuPu(KEYINFO::KEYINFO_RIGHT, nCnt) == true)
		{// 右下方向
			g_player[nCnt].move.x -= cosf(pCamera->rot.y - D3DX_PI * 3 / 4) * g_player[nCnt].fMove;
			g_player[nCnt].move.z += sinf(pCamera->rot.y - D3DX_PI * 3 / 4) * g_player[nCnt].fMove;
			g_player[nCnt].rotDest.y = pCamera->rot.y + D3DX_PI / -4;
		}
		else
		{// 下方向
			g_player[nCnt].move.x += sinf(pCamera->rot.y - D3DX_PI) * g_player[nCnt].fMove;
			g_player[nCnt].move.z += cosf(pCamera->rot.y - D3DX_PI) * g_player[nCnt].fMove;
			g_player[nCnt].rotDest.y = pCamera->rot.y;
		}
	}
	else if (GetKeyboardPress(KEYINFO_LEFT) == true || BuPu(KEYINFO::KEYINFO_LEFT, nCnt) == true)
	{// 左方向
		if (g_player[nCnt].nMoveCnt < MOVECNT_MAX)
		{
			g_player[nCnt].nMoveCnt++;
		}

		g_player[nCnt].move.x -= cosf(pCamera->rot.y) * g_player[nCnt].fMove;
		g_player[nCnt].move.z += sinf(pCamera->rot.y) * g_player[nCnt].fMove;
		g_player[nCnt].rotDest.y = pCamera->rot.y + (D3DX_PI / 2);

		if (g_player[nCnt].motionType != MOTIONTYPE_JUMP)
		{
			g_player[nCnt].motionType = MOTIONTYPE_MOVE;
		}
	}
	else if (GetKeyboardPress(KEYINFO_RIGHT) == true || BuPu(KEYINFO::KEYINFO_RIGHT, nCnt) == true)
	{// 右方向
		if (g_player[nCnt].nMoveCnt < MOVECNT_MAX)
		{
			g_player[nCnt].nMoveCnt++;
		}

		g_player[nCnt].move.x += cosf(pCamera->rot.y) * g_player[nCnt].fMove;
		g_player[nCnt].move.z -= sinf(pCamera->rot.y) * g_player[nCnt].fMove;
		g_player[nCnt].rotDest.y = pCamera->rot.y + (D3DX_PI / -2);
		if (g_player[nCnt].motionType != MOTIONTYPE_JUMP)
		{
			g_player[nCnt].motionType = MOTIONTYPE_MOVE;
		}
	}
	else
	{
		if (g_player[nCnt].nMoveCnt > 0)
		{
			g_player[nCnt].nMoveCnt--;
		}
	}

	// 移動モーションから待機モーションへ
	if (g_player[nCnt].motionType == MOTIONTYPE_MOVE && g_player[nCnt].nMoveCnt == 0)
	{
		g_player[nCnt].nCounterMotion = 0;
		g_player[nCnt].nKey = 0;
		g_player[nCnt].motionType = MOTIONTYPE_NEUTRAL;
	}

	// 目的の回転角の上限
	if (g_player[nCnt].rotDest.y - g_player[nCnt].rot.y < -D3DX_PI)
	{
		g_player[nCnt].rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_player[nCnt].rotDest.y - g_player[nCnt].rot.y > D3DX_PI)
	{
		g_player[nCnt].rotDest.y -= D3DX_PI * 2.0f;
	}

	// 向きの更新
	g_player[nCnt].rot.y += (g_player[nCnt].rotDest.y - g_player[nCnt].rot.y) * 0.1f;

	// 現在の回転角の上限
	if (g_player[nCnt].rot.y < -D3DX_PI)
	{
		g_player[nCnt].rot.y += D3DX_PI * 2.0f;
	}
	else if (g_player[nCnt].rot.y > D3DX_PI)
	{
		g_player[nCnt].rot.y -= D3DX_PI * 2.0f;
	}

	// 加速後の減速処理
	g_player[nCnt].move.x += (0.0f - g_player[nCnt].move.x) * SPEEDDOWN;
	g_player[nCnt].move.z += (0.0f - g_player[nCnt].move.z) * SPEEDDOWN;

	// 移動限界X軸
	if (g_player[nCnt].pos.x >= 1500.0f - g_player[nCnt].vtxMaxPlayer.x)
	{
		g_player[nCnt].pos.x = 1500.0f - g_player[nCnt].vtxMaxPlayer.x;
	}
	else if (g_player[nCnt].pos.x <= -1500.0f - g_player[nCnt].vtxMinPlayer.x)
	{
		g_player[nCnt].pos.x = -1500.0f - g_player[nCnt].vtxMinPlayer.x;
	}

	// 移動限界Z軸
	if (g_player[nCnt].pos.z >= 1500.0f - g_player[nCnt].vtxMaxPlayer.z)
	{
		g_player[nCnt].pos.z = 1500.0f - g_player[nCnt].vtxMaxPlayer.z;
	}
	else if (g_player[nCnt].pos.z <= -1500.0f - g_player[nCnt].vtxMinPlayer.z)
	{
		g_player[nCnt].pos.z = -1500.0f - g_player[nCnt].vtxMinPlayer.z;
	}
}

//==============================================================================
// プレイヤーの攻撃位置反映
//==============================================================================
void AttackParts(int nCnt)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRotAttack, mtxTransAttack;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATRIX mtxParent;						// 親のマトリックス
	D3DXMATERIAL *pMat;

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 攻撃判定マトリックスの初期化
	D3DXMatrixIdentity(&g_player[nCnt].as[g_player[nCnt].sr].mtxWorld);

	// 攻撃判定の向きを反映
	D3DXMatrixRotationYawPitchRoll(
		&mtxRotAttack,
		g_player[nCnt].as[g_player[nCnt].sr].rot.y,
		g_player[nCnt].as[g_player[nCnt].sr].rot.x,
		g_player[nCnt].as[g_player[nCnt].sr].rot.z);
	D3DXMatrixMultiply(
		&g_player[nCnt].as[g_player[nCnt].sr].mtxWorld,
		&g_player[nCnt].as[g_player[nCnt].sr].mtxWorld,
		&mtxRotAttack);

	// 各パーツの位置を反映
	D3DXMatrixTranslation(
		&mtxTransAttack,
		g_player[nCnt].as[g_player[nCnt].sr].pos.x,
		g_player[nCnt].as[g_player[nCnt].sr].pos.y,
		g_player[nCnt].as[g_player[nCnt].sr].pos.z);
	D3DXMatrixMultiply(
		&g_player[nCnt].as[g_player[nCnt].sr].mtxWorld,
		&g_player[nCnt].as[g_player[nCnt].sr].mtxWorld,
		&mtxTransAttack);

	mtxParent = g_player[nCnt].aModel[g_player[nCnt].as[g_player[nCnt].sr].nParts].mtxWorld;

	// 算出した各パーツのワールドマトリックスと判定発生パーツのマトリックスを掛け合わせる
	D3DXMatrixMultiply(
		&g_player[nCnt].as[g_player[nCnt].sr].mtxWorld,
		&g_player[nCnt].as[g_player[nCnt].sr].mtxWorld,
		&mtxParent);

	// 判定のワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player[nCnt].as[g_player[nCnt].sr].mtxWorld);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_player[nCnt].as[g_player[nCnt].sr].pBuffMatSphere->GetBufferPointer();

	pMat[0].MatD3D.Diffuse = g_player[nCnt].as[g_player[nCnt].sr].col;

	// マテリアルの設定
	pDevice->SetMaterial(&pMat[0].MatD3D);

	if (g_player[nCnt].as[g_player[nCnt].sr].bUse == true)
	{
		// モデル(パーツ)の描画
		//g_player[nCnt].as[g_player[nCnt].sr].pMeshSphere->DrawSubset(nCnt);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==============================================================================
// 攻撃の当たり判定
//==============================================================================
void CollisionDamage(int nCnt,int nTarget,D3DXVECTOR3 pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 aVec[COLLISION_PARTS];	// 矩形頂点から判定対象へのベクトル

	for (int nCntVec = 0; nCntVec < COLLISION_PARTS; nCntVec++)
	{
		if (nCntVec == 0)
		{// 左面
			aVec[nCntVec] = pos - g_player[nTarget].aPoint[nCntVec];
		}					    
		else if (nCntVec == 1)  
		{// 奥側				  
			aVec[nCntVec] = pos - g_player[nTarget].aPoint[nCntVec];
		}					    
		else if (nCntVec == 2)  
		{// 右面				  
			aVec[nCntVec] = pos - g_player[nTarget].aPoint[nCntVec];
		}					    
		else if (nCntVec == 3)  
		{// 手前				  
			aVec[nCntVec] = pos - g_player[nTarget].aPoint[nCntVec];
		}

		g_player[nTarget].attack.fGetDamage[nCntVec] = (g_player[nTarget].aVec[nCntVec].z * aVec[nCntVec].x) - (g_player[nTarget].aVec[nCntVec].x * aVec[nCntVec].z);
	}

	if (g_player[nCnt].as[g_player[nCnt].sr].bUse == true && g_player[nTarget].state != PLAYERSTATE_DAMAGE && g_player[nTarget].state != PLAYERSTATE_APPEAR)
	{
		if (g_player[nTarget].attack.fGetDamage[0] > 0.0f &&
			g_player[nTarget].attack.fGetDamage[1] > 0.0f &&
			g_player[nTarget].attack.fGetDamage[2] > 0.0f &&
			g_player[nTarget].attack.fGetDamage[3] > 0.0f)
		{
			g_player[nTarget].nCntState = 1;
			if (g_player[nTarget].sr == 0)
			{
				CalcuHp(nTarget, 40.0f, false);
				PlaySound(SOUND_LABEL_SE_SMASH_HIT);
			}
			else
			{
				CalcuHp(nTarget, 70.0f, false);
				PlaySound(SOUND_LABEL_SE_SLASH_HIT);
			}
			
			g_player[nTarget].state = PLAYERSTATE_DAMAGE;
		}
	}
}

//==============================================================================
// 影の追従
//==============================================================================
void SetPosShadow(int nPlayer,D3DXVECTOR3 pos)
{
	g_player[nPlayer].posShadow = pos;
}

//==============================================================================
// 武器の保有状態
//==============================================================================
void WeaponSet(int nCntPlayer,Weapon weapon)
{
	// ローカル変数宣言
	Player *pPlayer = &g_player[nCntPlayer];
	int nCnt = (pPlayer->nNumModel - pPlayer->nNumWeapon);

	for (int nCntWeapon = nCnt; nCntWeapon < pPlayer->nNumModel; nCntWeapon++)
	{
		if ((int)weapon != 0 && (int)weapon == nCntWeapon - nCnt + 1)
		{
			pPlayer->aModel[nCntWeapon].bUse = true;
		}
		else
		{
			pPlayer->aModel[nCntWeapon].bUse = false;
		}

		//switch (weapon)
		//{
		//case WEAPON_NONE:
		//	break;
		//case WEAPON_KNIFE:
		//	break;
		//case WEAPON_HANDGUN:
		//	break;
		//case WEAPON_MACHINEGUN:
		//	break;
		//case WEAPON_RUNCHER:
		//	break;
		//default: 
		//	break;
		//}
	}
}

//==============================================================================
// 武器ごとのアクション管理
//==============================================================================
void WeaponAction(int nCntPlayer,Weapon weapon)
{
	Player *pPlayer = &g_player[nCntPlayer];

	switch(weapon)
	{
	case WEAPON_NONE:
		pPlayer->motionType = MOTIONTYPE_ACTION;
		break;

	case WEAPON_KNIFE:
		pPlayer->motionType = MOTIONTYPE_SLASH;
		break;

	case WEAPON_HANDGUN:
		pPlayer->motionType = MOTIONTYPE_SHOT;
		break;

	case WEAPON_MACHINEGUN:
		pPlayer->motionType = MOTIONTYPE_SHOT;
		break;

	case WEAPON_RUNCHER:
		pPlayer->motionType = MOTIONTYPE_RANCHER;
		break;

	default:
		break;
	}
}

//==============================================================================
// 弾の当たり判定
//==============================================================================
bool CollisionBullet(D3DXVECTOR3 pos,int nType,int nPlayer)
{
	// ローカル変数宣言
	D3DXVECTOR3 aVec[COLLISION_PARTS];	// 矩形頂点から判定対象へのベクトル
	bool bBullet = true;
	
	for (int nCntVec = 0; nCntVec < COLLISION_PARTS; nCntVec++)
	{
		if (nCntVec == 0)
		{// 左面
			aVec[nCntVec] = pos - g_player[nPlayer].aPoint[nCntVec];
		}
		else if (nCntVec == 1)
		{// 奥側				  
			aVec[nCntVec] = pos - g_player[nPlayer].aPoint[nCntVec];
		}
		else if (nCntVec == 2)
		{// 右面				  
			aVec[nCntVec] = pos - g_player[nPlayer].aPoint[nCntVec];
		}
		else if (nCntVec == 3)
		{// 手前				  
			aVec[nCntVec] = pos - g_player[nPlayer].aPoint[nCntVec];
		}

		g_player[nPlayer].attack.fGetDamage[nCntVec] = (g_player[nPlayer].aVec[nCntVec].z * aVec[nCntVec].x) - (g_player[nPlayer].aVec[nCntVec].x * aVec[nCntVec].z);
	}

	if (g_player[nPlayer].state != PLAYERSTATE_DAMAGE && g_player[nPlayer].state != PLAYERSTATE_APPEAR)
	{
		if (g_player[nPlayer].attack.fGetDamage[0] > 0.0f &&
			g_player[nPlayer].attack.fGetDamage[1] > 0.0f &&
			g_player[nPlayer].attack.fGetDamage[2] > 0.0f &&
			g_player[nPlayer].attack.fGetDamage[3] > 0.0f && 
			g_player[nPlayer].fHeight > pos.y &&
			g_player[nPlayer].pos.y < pos.y)
		{
			g_player[nPlayer].state = PLAYERSTATE_DAMAGE;
			bBullet = false;
			switch (nType)
			{
			case 0:
				CalcuHp(nPlayer, 50.0f, false);
				PlaySound(SOUND_LABEL_SE_SHOT_HIT);		// 命中音の再生
				break;

			case 1:
				CalcuHp(nPlayer, 10.0f, false);
				PlaySound(SOUND_LABEL_SE_SHOT_HIT);
				break;

			case 2:
				CalcuHp(nPlayer, 200.0f, false);
				SetEffect(pos, 0.05f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 10.0f, 0.05f, 100);
				PlaySound(SOUND_LABEL_SE_EXPLOSION_HIT);
				break;

			default:
				break;
			}
		}
	}

	return bBullet;
}