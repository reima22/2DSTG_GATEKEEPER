//==============================================================================
//
// オブジェクト描画処理〔scene.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "pause.h"
#include "camera.h"

// 静的メンバ変数宣言
int CScene::m_nNumAll = 0;
CScene *CScene::m_pTop[PRIORITY_MAX] = {};			// 先頭のオブジェクトポインタ
CScene *CScene::m_pCur[PRIORITY_MAX] = {};			// 現在のオブジェクトのポインタ

//==============================================================================
// コンストラクタ
//==============================================================================
CScene::CScene(int nPriority)
{
	// リストへの追加
	if (m_pTop[nPriority] == NULL)
	{// 先頭が空白
		// 前後のポインタを空白
		this->m_pNext = NULL;
		this->m_pPrev = NULL;

		// 先頭ポインタへ代入
		m_pTop[nPriority] = this;
	}	
	else
	{// 次以降が空白
		// 最後尾を次のポインタへ移行
		m_pCur[nPriority]->m_pNext = this;

		// 1つ前のオブジェクトのポインタを最後尾に指定
		this->m_pPrev = m_pCur[nPriority];

		// 次のポインタを空白
		this->m_pNext = NULL;
	}

	// 最後尾の代入
	m_pCur[nPriority] = this;

	m_bDeath = false;			// 死亡フラグ
	m_objType = OBJTYPE_NONE;	// オブジェクトタイプ
	m_nNumAll++;				// オブジェクトの総数
}

//==============================================================================
// デストラクタ
//==============================================================================
CScene::~CScene()
{
}

//==============================================================================
// 全開放
//==============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CScene *pScene = m_pTop[nCntPriority];

		// ループ
		while (pScene)
		{// NULLで終了
			// 次へのポインタ
			CScene *pSceneNext = pScene->m_pNext;

			// 死亡フラグの成立
			pScene->m_bDeath = true;

			// 次のポインタ
			pScene = pSceneNext;
		}
	}
}

//==============================================================================
// 全更新
//==============================================================================
void CScene::UpdateAll(void)
{
	// 更新処理
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CScene *pScene = m_pTop[nCntPriority];

		// 次の情報(m_pNext)がある限り続ける
		while (pScene)
		{
			if (nCntPriority < PRIORITY_PAUSE && CPause::GetPause() == true)
			{// ポーズ時の停止
				break;
			}

			// 次の情報代入
			CScene *pSceneNext = pScene->m_pNext;

			// 更新処理
			pScene->Update();

			// 次の情報
			pScene = pSceneNext;
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CScene *pScene = m_pTop[nCntPriority];

		// 死亡フラグが成立ならば破棄
		while (pScene)
		{
			// 次の情報代入
			CScene *pSceneNext = pScene->m_pNext;

			// 死亡フラグが立っているとき
			if (pScene->m_bDeath == true)
			{
				// リストの再形成
				if (pScene->m_pPrev != NULL)
				{// 直前のポインタが空白
					pScene->m_pPrev->m_pNext = pScene->m_pNext;
				}

				if (pScene->m_pNext != NULL)
				{// 直後のポインタが空白
					pScene->m_pNext->m_pPrev = pScene->m_pPrev;
				}

				if (pScene == m_pTop[nCntPriority])
				{// リスト先頭
					m_pTop[nCntPriority] = pScene->m_pNext;
				}

				if(pScene == m_pCur[nCntPriority])
				{// リスト最後尾
					m_pCur[nCntPriority] = pScene->m_pPrev;
				}

				// 破棄
				delete pScene;
				pScene = NULL;

				m_nNumAll--;
			}

			// 次の情報
			pScene = pSceneNext;
		}	
	}
}

//==============================================================================
// 全描画
//==============================================================================
void CScene::DrawAll(void)
{
	// カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	// カメラの取得
	pCamera->SetCamera();

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CScene *pScene = m_pTop[nCntPriority];

		// 次の情報(m_pNext)がある限り続ける
		while (pScene)
		{
			// 次の情報代入
			CScene *pSceneNext = pScene->m_pNext;

			// 更新処理
			pScene->Draw();

			// 次の情報
			pScene = pSceneNext;
		}
	}
}

//==============================================================================
// 種類の設定
//==============================================================================
void CScene::SetObjType(OBJTYPE objType)
{
	m_objType = objType;
}

//==============================================================================
// 種類の取得
//==============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}

//==============================================================================
// 先頭シーンの取得
//==============================================================================
CScene *CScene::GetScene(int nPriority)
{
	return m_pTop[nPriority];
}

//==============================================================================
// 次シーンの取得
//==============================================================================
CScene *CScene::GetSceneNext(CScene *pScene)
{
	return pScene->m_pNext;
}

//==============================================================================
// 部分開放
//==============================================================================
void CScene::Release(void)
{	
	// 死亡フラグの成立
	this->m_bDeath = true;
}