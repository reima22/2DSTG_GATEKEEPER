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
#include "textdata.h"

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
	m_pData = CManager::GetTextData();

	// テクスチャ最大数の取得
	m_nTexMax = m_pData->GetNumTexture();

	// インスタンス生成
	m_pTexture = (LPDIRECT3DTEXTURE9)new LPDIRECT3DTEXTURE9[m_nTexMax];

	// パスの動的確保
	m_pFileName = new char*[m_nTexMax];

	for (int nCnt = 0; nCnt < m_nTexMax; nCnt++)
	{
		m_pFileName[nCnt] = m_pData->GetFileName(nCnt);

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

//==============================================================================
// テクスチャクラスの生成処理
//==============================================================================
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
// テクスチャの割り当て
//==============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	return *(LPDIRECT3DTEXTURE9*)&m_pTexture[nIdx];
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