//==============================================================================
//
// ライトの設定〔light.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "light.h"

//==============================================================================
// グローバル変数
//==============================================================================
//D3DLIGHT9 g_light;
D3DLIGHT9 g_aLight[3];

//==============================================================================
// 初期化処理
//==============================================================================
void InitLight(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	D3DXVECTOR3 vecDir;

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		// ライトをクリアする
		ZeroMemory(&g_aLight[nCnt], sizeof(D3DLIGHT9));

		// ライトの種類を設定
		g_aLight[nCnt].Type = D3DLIGHT_DIRECTIONAL;

		// ライトの拡散光を設定
		g_aLight[nCnt].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ライトの方向を設定
		if (nCnt == 0)
		{
			vecDir = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
		}
		else if (nCnt == 1)
		{
			vecDir = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
		}
		else if (nCnt == 2)
		{
			vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		}
		D3DXVec3Normalize(&vecDir, &vecDir);		// 正規化する
		g_aLight[nCnt].Direction = vecDir;

		// ライトを設定する
		pDevice->SetLight(nCnt, &g_aLight[nCnt]);

		// ライトを有効にする
		pDevice->LightEnable(nCnt, TRUE);
	}



	//// ライトをクリアする
	//ZeroMemory(&g_light, sizeof(D3DLIGHT9));

	//// ライトの種類を設定
	//g_light.Type = D3DLIGHT_DIRECTIONAL;

	//// ライトの拡散光を設定
	//g_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ライトの方向を設定
	//vecDir = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
	//D3DXVec3Normalize(&vecDir, &vecDir);		// 正規化する
	//g_light.Direction = vecDir;

	//// ライトを設定する
	//pDevice->SetLight(0, &g_light);

	//// ライトを有効にする
	//pDevice->LightEnable(0, TRUE);


	//// ライトをクリアする
	//ZeroMemory(&g_light, sizeof(D3DLIGHT9));

	//// ライトの種類を設定
	//g_light.Type = D3DLIGHT_DIRECTIONAL;

	//// ライトの拡散光を設定
	//g_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ライトの方向を設定
	//vecDir = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
	//D3DXVec3Normalize(&vecDir, &vecDir);		// 正規化する
	//g_light.Direction = vecDir;

	//// ライトを設定する
	//pDevice->SetLight(0, &g_light);

	//// ライトを有効にする
	//pDevice->LightEnable(0, TRUE);

	//// ライトをクリアする
	//ZeroMemory(&g_light, sizeof(D3DLIGHT9));

	//// ライトの種類を設定
	//g_light.Type = D3DLIGHT_DIRECTIONAL;

	//// ライトの拡散光を設定
	//g_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ライトの方向を設定
	//vecDir = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
	//D3DXVec3Normalize(&vecDir, &vecDir);		// 正規化する
	//g_light.Direction = vecDir;

	//// ライトを設定する
	//pDevice->SetLight(0, &g_light);

	//// ライトを有効にする
	//pDevice->LightEnable(0, TRUE);
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitLight(void)
{

}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateLight(void)
{

}