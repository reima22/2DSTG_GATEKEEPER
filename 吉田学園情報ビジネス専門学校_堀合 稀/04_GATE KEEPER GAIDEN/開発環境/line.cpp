//==============================================================================
//
// ボーダーライン描画処理〔line.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "line.h"
#include "polygon.h"
#include "sound.h"
#include "invasion.h"
#include "effect.h"
#include "game.h"
#include "window_effect.h"

// 静的メンバ変数宣言
CLine::LINE_STATE CLine::m_lineState = LINE_STATE_NONE;		// ボーダーラインの状態
D3DXCOLOR CLine::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ボーダーラインの色

//==============================================================================
// コンストラクタ
//==============================================================================
CLine::CLine(int nPriority) : CScene2D(nPriority)
{
	// 状態変化カウント
	m_nCntState = 0;
	m_nCautionCnt = 0;
	m_nTutoCnt = 0;

	m_lineState = LINE_STATE_NONE;
}

//==============================================================================
// デストラクタ
//==============================================================================
CLine::~CLine()
{
}

//==============================================================================
// ボーダーラインの初期化処理
//==============================================================================
HRESULT CLine::Init(D3DXVECTOR3 pos)
{
	// 初期化
	CScene2D::Init(pos);
	CScene2D::SetSize(LINE_SIZEX, LINE_SIZEY);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	CScene2D::SetObjType(OBJTYPE_UI);
	CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return S_OK;
}

//==============================================================================
// ボーダーラインの終了処理
//==============================================================================
void CLine::Uninit(void)
{
	// 終了処理
	CScene2D::Uninit();
}

//==============================================================================
// ボーダーラインの更新処理
//==============================================================================
void CLine::Update(void)
{
	// ローカル変数宣言
	CScene *pScene;
	pScene = CScene::GetScene(PRIORITY_CHARA);
	CScene *pSceneNext = NULL;
	CRenderer *pRenderer = CManager::GetRenderer();	// レンダラーの取得

	// ラインの位置
	D3DXVECTOR3 pos = CScene2D::GetPosition();

	// ラインの当たり判定
	while (pScene)
	{		
		// 次のポインタ
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene != NULL)
		{
			// サイズの取得
			float fSizeX = pScene->GetSize().x;
			float fSizeY = pScene->GetSize().y;

			// オブジェクトの種類
			CScene::OBJTYPE objType;
			objType = pScene->GetObjType();

			if (objType == OBJTYPE_ENEMY)
			{// 敵の侵入判定
				D3DXVECTOR3 posEnemy;
				posEnemy = pScene->GetPosition();

				if (pos.x + LINE_SIZEX >= posEnemy.x - fSizeX &&
					pos.x - LINE_SIZEX <= posEnemy.x + fSizeX &&
					pos.y + LINE_SIZEY >= posEnemy.y - fSizeY &&
					pos.y - LINE_SIZEY <= posEnemy.y + fSizeY)
				{
					// サウンド再生
					CSound::Play(CSound::SOUND_LABEL_SE_LINEOVER);

					// 侵入数の加算
					CInvasion::AddInvasion();

					// エフェクトの発生
					for (int nCnt = 0; nCnt < 100; nCnt++)
					{
						CEffect::Create(posEnemy, 0.05f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 10.0f, 0.02f, 1);
					}

					// 状態変化カウント設定
					m_nCntState = 9;

					// 状態変化
					m_lineState = LINE_STATE_INVASION;

					// フィードバックエフェクトの発生 
					pRenderer->SetFeedbackEffect();

					// 画面エフェクトの実行
					CWindowEffect::SetState(CWindowEffect::EFFECT_STATE_INVASION);
					CWindowEffect::SetInvationEvent();

					pScene->Uninit();

					return;
				}
			}
		}

		// 次のポインタの代入
		pScene = pSceneNext;
	}

	// ラインの状態
	SetLineState();

	// 更新
	CScene2D::Update();
}

//==============================================================================
// ボーダーラインの描画処理
//==============================================================================
void CLine::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// ボーダーラインの生成処理
//==============================================================================
CLine *CLine::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CLine *pLine;
	pLine = new CLine;

	// 初期化
	pLine->Init(pos);

	pLine->BindTexture(CTexture::TEXTYPE_LINE);

	return pLine;
}

//==============================================================================
// ボーダーラインの設定
//==============================================================================
void CLine::SetLineState(void)
{
	// ラインの状態ごとの処理
	switch (m_lineState)
	{
	case LINE_STATE_NONE:
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// カウンター減算
		if (m_nCntState >= 0)
		{
			m_nCntState--;
		}

		// 次ステータスへ移行
		if (m_nCntState <= 0)
		{
			// 状態変化
			if (CInvasion::GetInvasion() >= (float)CGame::GetEndInvasion() / 10 * 8)
			{
				m_lineState = LINE_STATE_CAUTION;
			}
			else
			{
				m_lineState = LINE_STATE_NONE;
			}			
		}

		break;

	case LINE_STATE_INVASION:

		// 侵入時のライン色変化
		m_col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);

		// カウンター減算
		if (m_nCntState >= 0)
		{
			m_nCntState--;
		}

		// 次ステータスへ移行
		if (m_nCntState <= 0)
		{
			// 状態変化
			if (CInvasion::GetInvasion() >= (float)CGame::GetEndInvasion() / 10 * 8)
			{
				m_lineState = LINE_STATE_CAUTION;
			}
			else
			{
				m_lineState = LINE_STATE_NONE;
			}
		}

		break;

	case LINE_STATE_CAUTION:

		// カウント加算
		m_nCautionCnt++;

		// 色の点滅変化
		if (m_nCautionCnt % 60 == 0)
		{
			m_col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);

			// サウンド再生
			CSound::Play(CSound::SOUND_LABEL_SE_LINE_CAUTION);
		}
		else if (m_nCautionCnt % 30 == 0 && m_nCautionCnt % 60 != 0)
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		break;

	case LINE_STATE_TUTORIAL:
		
		// カウント加算
		m_nTutoCnt++;

		// 色の点滅変化
		if (m_nTutoCnt % 60 == 0)
		{
			m_col = D3DXCOLOR(1.0f, 0.0f, 0.6f, 1.0f);
		}
		else if (m_nTutoCnt % 30 == 0 && m_nTutoCnt % 60 != 0)
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		break;
	}



	CScene2D::SetCol(m_col);
}