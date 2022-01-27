//==============================================================================
//
// ランキング選択肢処理〔ranking_select.cpp〕
// Author : Tatsuki Matsuda
//
//==============================================================================
#include "fade.h"
#include "gamepad.h"
#include "input.h"
#include "ranking_select.h"
#include "scene2D.h"
#include "textdata_rankingselect.h"

//==============================================================================
// 静的メンバ変数宣言
//==============================================================================
CScene2D *CRankingSelect::m_apScene2D[RANKING_MAX] = {};	// 2Dポリゴンポインタ
CRankingSelect::RANKING CRankingSelect::m_ranking;			// 選択中のカーソル
D3DXCOLOR CRankingSelect::m_col;							// 選択中カーソルの色

//==============================================================================
// コンストラクタ
//==============================================================================
CRankingSelect::CRankingSelect(int nPriority) : CRanking(nPriority)
{
}

//==============================================================================
// デストラクタ
//==============================================================================
CRankingSelect::~CRankingSelect()
{
}

//==============================================================================
//ランキング選択の初期化処理
//==============================================================================
HRESULT CRankingSelect::Init(void)
{
	// テキストデータの取得
	CTextDataRankingSelect *pDataRankingSelect;
	pDataRankingSelect = CTextData::GetDataSelectRanking();

	for (int nCnt = 0; nCnt < RANKING_MAX; nCnt++)
	{
		// データの取得
		D3DXVECTOR3 pos = pDataRankingSelect->GetPosition(CTextDataRankingSelect::RANKINGPOLY_RETRY + nCnt);
		D3DXVECTOR2 size = pDataRankingSelect->GetSize(CTextDataRankingSelect::RANKINGPOLY_RETRY + nCnt);
		int nTexIdx = pDataRankingSelect->GetTexIdx(CTextDataRankingSelect::RANKINGPOLY_RETRY) + nCnt;

		// 2Dポリゴンの生成
		m_apScene2D[nCnt] = CScene2D::Create();
		m_apScene2D[nCnt]->SetPosition(pos);
		m_apScene2D[nCnt]->SetSize(size);
		m_apScene2D[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_apScene2D[nCnt]->BindTextureNum(nTexIdx);
	}

	// 初期化
	m_ranking = RANKING_RETRY;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bAlphaDown = true;
	m_bSelect = false;
	m_bOK = false;
	m_nCntTime = 0;

	return S_OK;
}

//==============================================================================
// ランキング選択の終了処理
//==============================================================================
void CRankingSelect::Uninit(void)
{
	for (int nCnt = 0; nCnt < RANKING_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			delete m_apScene2D[nCnt];
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt] = NULL;
		}
	}
}

//==============================================================================
// ランキング選択の更新処理
//==============================================================================
void CRankingSelect::Update(void)
{
	// 選択モードの設定
	SetMode();

	for (int nCnt = 0; nCnt < RANKING_MAX; nCnt++)
	{
		// 色状態の設定
	//	m_apScene2D[nCnt]->SetCol(m_col);
		m_apScene2D[nCnt]->Update();
	}
}

//==============================================================================
// ランキング選択の描画処理
//==============================================================================
void CRankingSelect::Draw(void)
{
	for (int nCnt = 0; nCnt < RANKING_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			m_apScene2D[nCnt]->Draw();
		}
	}
}

//==============================================================================
// ランキング選択の生成処理
//==============================================================================
CRankingSelect *CRankingSelect::Create(void)
{
	// インスタンス生成
	CRankingSelect *pRankingSelect;
	pRankingSelect = new CRankingSelect;

	// 初期化
	if(pRankingSelect != NULL)
	{ 
		pRankingSelect->Init();
	}

	return pRankingSelect;
}

//==============================================================================
// 選択状態ごとの設定
//==============================================================================
void CRankingSelect::SetMode(void)
{
	// フェード情報の取得
	CFade *pFade;
	pFade = CManager::GetFade();
	CFade::FADE fade = pFade->GetFade();

	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// カーソルの明示
	for (int nCnt = 0; nCnt < RANKING_MAX; nCnt++)
	{
		if (nCnt == m_ranking)
		{// 選択中
			if (m_bAlphaDown == true)
			{
				m_col.a -= 0.03f;
	
				// 点滅の折り返し
				if (m_col.a <= 0.0f)
				{
					m_col.a = 0.0f;
					m_bAlphaDown = false;
				}
			}
			else
			{
				m_col.a += 0.03f;

				// 点滅の折り返し
				if (m_col.a >= 1.0f)
				{
					m_col.a = 1.0f;


					m_bAlphaDown = true;
				}
			}

			m_apScene2D[nCnt]->SetTex(1, 2, 0, 0, 0.0f, 0.0f);
			m_apScene2D[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_col.a));

		}
		else
		{// それ以外
			m_apScene2D[nCnt]->SetTex(1, 2, 0, 1, 0.0f, 0.0f);
			m_apScene2D[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}

	//	選択肢ごとの処理
	if (m_bOK == false)
	{
		// カーソルの移動
		if (keyboard->GetTrigger(CInput::KEYINFO_DOWN) == true || gamepad->GetTrigger(CInput::KEYINFO_DOWN) == true)
		{
			m_pSound->Play(CSound::SOUND_LABEL_SE_SELECT);
			m_ranking = (RANKING)(m_ranking + 1);
		}
		else if (keyboard->GetTrigger(CInput::KEYINFO_UP) == true || gamepad->GetTrigger(CInput::KEYINFO_UP) == true)
		{
			m_pSound->Play(CSound::SOUND_LABEL_SE_SELECT);
			m_ranking = (RANKING)(m_ranking - 1);
		}
	}

	// カーソルの補正
	if (m_ranking < 0)
	{
		m_ranking = (RANKING)(RANKING_MAX - 1);

	}
	else if (m_ranking >(RANKING_MAX - 1))
	{
		m_ranking = (RANKING)0;

	}

	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		// 判定処理
		if (pFade->GetFade() == CFade::FADE_NONE && (keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true))
		{
			// 効果音の再生
			m_pSound->Play(CSound::SOUND_LABEL_SE_DECIDE);

			m_bOK = true;
		}
	}

	//	選択肢ごとの処理
	if (m_bOK == true)
	{
		// カウント加算
		m_nCntTime++;

		if (m_ranking == RANKING_RETRY)
		{
			m_bSelect = true;

			if (m_bSelect == true)
			{
				// カウントが5で割ると0になる場合
				if (m_nCntTime % 5 == 0)
				{
					// 色
					m_col.a = 1.0f;
				}
				else
				{

					// 色
					m_col.a = 0.5f;
				}

				// カウントが指定値を超えた場合
				if (m_nCntTime >= 50)
				{
					// ゲームのやり直し
					pFade->SetFade(CFade::FADE_OUT, CMode::MODE_GAME);
				}
			}
		}

		if (m_ranking == RANKING_STAGE)
		{
			if (m_bSelect == false)
			{
				// カウントが5で割ると0になる場合
				if (m_nCntTime % 5 == 0)
				{
					// 色
					m_col.a = 1.0f;
				}
				else
				{
					// 色
					m_col.a = 0.5f;
				}

				// カウントが指定値を超えた場合
				if (m_nCntTime >= 50)
				{
					// タイトルへ戻る
					pFade->SetFade(CFade::FADE_OUT, CMode::MODE_SELECT);
				}
			}
		}

		if (m_ranking == RANKING_QUIT)
		{
			if (m_bSelect == false)
			{
				// カウントが5で割ると0になる場合
				if (m_nCntTime % 5 == 0)
				{
					// 色
					m_col.a = 1.0f;
				}
				else
				{
					// 色
					m_col.a = 0.5f;
				}

				// カウントが指定値を超えた場合
				if (m_nCntTime >= 50)
				{
					// タイトルへ戻る
					pFade->SetFade(CFade::FADE_OUT, CMode::MODE_TITLE);
				}
			}
		}

	}

	if (fade == CFade::FADE_IN && (keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true))
	{
		// フェードインのカット
		pFade->FadeInCancel();
	}
}