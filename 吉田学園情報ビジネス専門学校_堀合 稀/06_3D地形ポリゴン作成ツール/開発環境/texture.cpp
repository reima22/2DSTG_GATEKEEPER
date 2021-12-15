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
#include "textdata_texture.h"

// 静的メンバ変数宣言
//LPDIRECT3DTEXTURE9 CTexture::m_pTexture[TEXTYPE_MAX] = {};
//char *CTexture::m_pFileName[TEXTYPE_MAX] = 
//{
	//{""},										// テクスチャ無し
	//{ "data\\TEXTURE\\player_cloth00.jpg" },	// プレイヤーテクスチャ
	//{ "data\\TEXTURE\\player_cloth01.jpg" },	// プレイヤーテクスチャ
	//{ "data\\TEXTURE\\block.jpg"},				// ブロック
	//{ "data\\TEXTURE\\title_bg.png" },			// タイトル背景
	//{ "data\\TEXTURE\\title000.png" },			// タイトルロゴ
	//{ "data\\TEXTURE\\press_enter.png" },		// タイトルエンターサイン
	//{ "data\\TEXTURE\\result_bg.png" },			// リザルト背景
	//{ "data\\TEXTURE\\result_logo000.png" },	// リザルトロゴ
	//{ "data\\TEXTURE\\ranking_bg.png" },		// ランキング背景
	//{ "data\\TEXTURE\\ranking_logo.png" },		// ランキングロゴ	
	//{ "data\\TEXTURE\\ranking_rank.png" },		// ランキング順位
	//{ "data\\TEXTURE\\yourscore.png" },			// ランキング画面のリザルトスコアロゴ
	//{ "data\\TEXTURE\\number000.png" },			// ナンバー
	//{ "data\\TEXTURE\\explosion000.png" },		// 爆発
	//{ "data\\TEXTURE\\effect000.jpg" },			// エフェクト
	//{ "data\\TEXTURE\\pause_BS.png" },			// ポーズベース
	//{ "data\\TEXTURE\\continue_logo.png" },		// ポーズコンテニュー
	//{ "data\\TEXTURE\\retry_logo.png" },		// ポーズリトライ
	//{ "data\\TEXTURE\\quit_logo.png" },			// ポーズ終了 
	//{ "data\\TEXTURE\\tutorial00.jpg" },		// チュートリアル0 
	//{ "data\\TEXTURE\\tutorial01.jpg" },		// チュートリアル1
	//{ "data\\TEXTURE\\tutorial02.jpg" },		// チュートリアル2
	//{ "data\\TEXTURE\\window_effect.png" },		// 画面エフェクト
	//{ "data\\TEXTURE\\easy.png" },				// easy
	//{ "data\\TEXTURE\\normal.png" },			// normal
	//{ "data\\TEXTURE\\hard.png" },				// hard
	//{ "data\\TEXTURE\\meteor.jpg"},				// 隕石モデルのテクスチャ
	//{ "data\\TEXTURE\\panel.jpg" },				// パネルモデルのテクスチャ
//};

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

	// テクスチャデータの取得
	CTextDataTexture *pDataTexture;
	pDataTexture = CTextData::GetDataTexture();

	// テクスチャ最大数の取得
	m_nTexMax = pDataTexture->GetNumTexture();

	// インスタンス生成
	m_pTexture = (LPDIRECT3DTEXTURE9)new LPDIRECT3DTEXTURE9[m_nTexMax];

	m_pFileName = new char*[m_nTexMax];

	for (int nCnt = 0; nCnt < m_nTexMax; nCnt++)
	{
		m_pFileName[nCnt] = pDataTexture->GetFileName(nCnt);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			m_pFileName[nCnt],
			(LPDIRECT3DTEXTURE9*)&m_pTexture[nCnt]);
	}

	return S_OK;
}

//==============================================================================
// テクスチャデータの全破棄
//==============================================================================
void CTexture::UnloadAll(void)
{
	for (int nCnt = 0; nCnt < m_nTexMax; nCnt++)
	{
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

	delete[] m_pFileName;
	m_pFileName = NULL;
}

// テクスチャクラスの生成処理
CTexture *CTexture::Create(void)
{
	// ローカル変数宣言
	CTexture *pTexture;
	pTexture = new CTexture;

	if (pTexture != NULL)
	{
		pTexture->LoadAll();
	}

	return pTexture;
}

//==============================================================================
// 3Dモデルへの割り当て
//==============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress3D(char *pTexFilename)
{
	for (int nCntTex = 0; nCntTex < m_nTexMax; nCntTex++)
	{
		// 該当するテクスチャの割出し
		if (strcmp(m_pFileName[nCntTex], pTexFilename) == 0)
		{
			// テクスチャ情報を返す
			return *(LPDIRECT3DTEXTURE9*)&m_pTexture[nCntTex];
		}
	}

	return NULL;
}