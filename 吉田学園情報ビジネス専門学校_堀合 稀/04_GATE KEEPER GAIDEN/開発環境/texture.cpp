//==============================================================================
//
// テクスチャ管理〔texture.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

// 静的メンバ変数宣言
char *CTexture::m_pFileName[TEXTYPE_MAX] = 
{
	{""},									// テクスチャ無し
	{ "data\\TEXTURE\\player000.png" },		// プレイヤー
	{ "data\\TEXTURE\\enemy000.png" },		// 敵0
	{ "data\\TEXTURE\\enemy001.png" },		// 敵1
	{ "data\\TEXTURE\\enemy002.png" },		// 敵2
	{ "data\\TEXTURE\\enemy003.png" },		// 敵3
	{ "data\\TEXTURE\\enemyMother.png" },	// 敵4(母艦)
	{ "data\\TEXTURE\\mother_parts.jpg" },	// 母艦のパーツ
	{ "data\\TEXTURE\\bullet000.png" },		// 弾0
	{ "data\\TEXTURE\\bullet001.png" },		// 弾1
	{ "data\\TEXTURE\\bullet002.png" },		// 弾2
	{ "data\\TEXTURE\\bullet003.png" },		// 弾3
	{ "data\\TEXTURE\\bullet004.png" },		// 敵弾
	{ "data\\TEXTURE\\bg100.png" },			// 背景0
	{ "data\\TEXTURE\\bg101.png" },			// 背景1
	{ "data\\TEXTURE\\bg102.png" },			// 背景2
	{ "data\\TEXTURE\\bg201.jpg" },			// 背景3
	{ "data\\TEXTURE\\bg202.jpg" },			// 背景4
	{ "data\\TEXTURE\\bg203.jpg" },			// 背景5
	{ "data\\TEXTURE\\title_bg.png" },		// タイトル背景
	{ "data\\TEXTURE\\title000.png" },		// タイトルロゴ
	{ "data\\TEXTURE\\press_enter.png" },		// タイトルエンターサイン
	{ "data\\TEXTURE\\result_bg.png" },		// リザルト背景
	{ "data\\TEXTURE\\result_logo000.png" },	// リザルトロゴ
	{ "data\\TEXTURE\\ranking_bg.png" },		// ランキング背景
	{ "data\\TEXTURE\\ranking_logo.png" },	// ランキングロゴ	
	{ "data\\TEXTURE\\ranking_rank.png" },	// ランキング順位
	{ "data\\TEXTURE\\yourscore.png" },		// ランキング画面のリザルトスコアロゴ
	{ "data\\TEXTURE\\life000.png" },			// ライフ
	{ "data\\TEXTURE\\MUI_000.png" },			// メインUI
	{ "data\\TEXTURE\\bullet_UI000.png" },	// 弾UI0 
	{ "data\\TEXTURE\\bullet_UI001.png" },	// 弾UI1
	{ "data\\TEXTURE\\bullet_UI002.png" },	// 弾UI2
	{ "data\\TEXTURE\\bullet_UI003.png" },	// 弾UI3
	{ "data\\TEXTURE\\item000.png" },			// アイテム0(波形弾)
	{ "data\\TEXTURE\\item001.png" },			// アイテム1(ミサイル)
	{ "data\\TEXTURE\\item002.png" },			// アイテム2(逆波形弾)
	{ "data\\TEXTURE\\item003.png" },			// アイテム3(回復)
	{ "data\\TEXTURE\\number000.png" },		// ナンバー
	{ "data\\TEXTURE\\explosion000.png" },	// 爆発
	{ "data\\TEXTURE\\effect000.jpg" },		// エフェクト
	{ "data\\TEXTURE\\safetyline000.png" },	// ボーダーライン
	{ "data\\TEXTURE\\pause_BS.png" },		// ポーズベース
	{ "data\\TEXTURE\\continue_logo.png" },	// ポーズコンテニュー
	{ "data\\TEXTURE\\retry_logo.png" },		// ポーズリトライ
	{ "data\\TEXTURE\\quit_logo.png" },		// ポーズ終了 
	{ "data\\TEXTURE\\tutorial00.jpg" },		// チュートリアル0 
	{ "data\\TEXTURE\\tutorial01.jpg" },		// チュートリアル1
	{ "data\\TEXTURE\\tutorial02.jpg" },		// チュートリアル2
	{ "data\\TEXTURE\\message_window.png" },	// メッセージウィンドウ
	{ "data\\TEXTURE\\message_text.png" },		// メッセージテキスト
	{ "data\\TEXTURE\\warning.png" },			// 警告画面
	{ "data\\TEXTURE\\window_effect.png" },		// 画面エフェクト
	{ "data\\TEXTURE\\easy.png" },				// easy
	{ "data\\TEXTURE\\normal.png" },			// normal
	{ "data\\TEXTURE\\hard.png" },				// hard
	{ "data\\TEXTURE\\meteor.jpg"},				// 隕石モデルのテクスチャ
	{ "data\\TEXTURE\\panel.jpg" },				// パネルモデルのテクスチャ
	{ "data\\TEXTURE\\Lava.png" },
	{ "data\\TEXTURE\\Tex000.jpg" },
	{ "data\\TEXTURE\\Tex001.jpg" },
	{ "data\\TEXTURE\\Tex002.jpg" },
	{ "data\\TEXTURE\\Tex003.jpg" },
	{ "data\\TEXTURE\\Tex004.jpg" },
};	   

//==============================================================================
// コンストラクタ
//==============================================================================
CTexture::CTexture()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CTexture::~CTexture()
{

}

//==============================================================================
// テクスチャデータの全読み込み
//==============================================================================
HRESULT CTexture::LoadAll(void)
{	
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// インスタンス生成
	m_pTexture = (LPDIRECT3DTEXTURE9)new LPDIRECT3DTEXTURE9[TEXTYPE_MAX];

	// 先頭の無効化
	*(LPDIRECT3DTEXTURE9*)&m_pTexture[0] = NULL;

	for (int nCnt = 0; nCnt < TEXTYPE_MAX; nCnt++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			m_pFileName[nCnt],
			(LPDIRECT3DTEXTURE9*)&m_pTexture[nCnt]);
	}

	//// テクスチャ無し
	//m_apTexture[0] = NULL;

	//for (int nCnt = 1; nCnt < TEXTYPE_MAX; nCnt++)
	//{
	//	// テクスチャの読み込み
	//	D3DXCreateTextureFromFile(
	//		pDevice,
	//		m_pFileName[nCnt],
	//		&m_apTexture[nCnt]);
	//}

	return S_OK;
}

//==============================================================================
// テクスチャデータの全破棄
//==============================================================================
void CTexture::UnloadAll(void)
{
	for (int nCnt = 0; nCnt < TEXTYPE_MAX; nCnt++)
	{
		//if (m_apTexture[nCnt] != NULL)
		//{
		//	m_apTexture[nCnt]->Release();
		//	m_apTexture[nCnt] = NULL;
		//}

		// 全てのテクスチャの破棄
		if (*(LPDIRECT3DTEXTURE9*)&m_pTexture[nCnt] != NULL)
		{
			(*(LPDIRECT3DTEXTURE9*)&m_pTexture[nCnt])->Release();
			*(LPDIRECT3DTEXTURE9*)&m_pTexture[nCnt] = NULL;
		}

		// ファイル名の破棄
		if (m_pFileName[nCnt] != NULL)
		{
			m_pFileName[nCnt] = NULL;
		}
	}

	// メモリの破棄
	delete[] m_pTexture;
	m_pTexture = NULL;
}

//==============================================================================
// 3Dモデルへの割り当て
//==============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress3D(char *pTexFilename)
{
	for (int nCntTex = 0; nCntTex < TEXTYPE_MAX; nCntTex++)
	{
		// 該当するテクスチャの割出し
		if (strcmp(m_pFileName[nCntTex] ,pTexFilename) == 0)
		{
			// テクスチャ情報を返す
			return *(LPDIRECT3DTEXTURE9*)&m_pTexture[nCntTex];
		}
	}

	return NULL;
}