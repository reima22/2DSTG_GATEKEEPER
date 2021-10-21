//==============================================================================
//
// 難易度選択処理〔title_difficulty.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "title.h"
#include "title_difficulty.h"
#include "polygon.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

// 静的メンバ変数宣言
CScene2D *CTitleDifficulty::m_apScene2D[DIFFICULTY_MAX] = {};	// Scene2Dクラスのポインタ
CTitleDifficulty::DIFFICULTY CTitleDifficulty::m_difficulty;	// 選択中の難易度
D3DXCOLOR CTitleDifficulty::m_col[DIFFICULTY_MAX];				// 選択中カーソルの色
bool CTitleDifficulty::m_bSelectDifficulty;						// 選択開始

//==============================================================================
// コンストラクタ
//==============================================================================
CTitleDifficulty::CTitleDifficulty(int nPriority) : CScene2D(nPriority)
{
	m_bSelectDifficulty = false;
	m_nBlinkCnt = 0;
}

//==============================================================================
// デストラクタ
//==============================================================================
CTitleDifficulty::~CTitleDifficulty()
{
}

//==============================================================================
// 難易度選択の初期化処理
//==============================================================================
HRESULT CTitleDifficulty::Init(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < DIFFICULTY_MAX; nCnt++)
	{
		m_apScene2D[nCnt] = new CScene2D(PRIORITY_PAUSE);
		m_apScene2D[nCnt]->Init(D3DXVECTOR3(pos.x, pos.y - 100.0f + (nCnt * 110.0f), 0.0f));
		m_apScene2D[nCnt]->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
		m_apScene2D[nCnt]->SetSize(T_DIFFICULT_X, T_DIFFICULT_Y);
		m_apScene2D[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_apScene2D[nCnt]->BindTexture((CTexture::TEXTYPE)(CTexture::TEXTYPE_EASY + nCnt));
	}

	// 初期化
	m_difficulty = DIFFICULTY_EASY;

	// 色の設定
	m_col[DIFFICULTY_EASY] = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	m_col[DIFFICULTY_NORMAL] = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f);
	m_col[DIFFICULTY_HARD] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
	m_bAlphaDown = true;

	return S_OK;
}

//==============================================================================
// 難易度選択の終了処理
//==============================================================================
void CTitleDifficulty::Uninit(void)
{
	for (int nCnt = 0; nCnt < DIFFICULTY_MAX; nCnt++)
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
// 難易度選択の更新処理
//==============================================================================
void CTitleDifficulty::Update(void)
{
	SetMode();
	
	for (int nCnt = 0; nCnt < DIFFICULTY_MAX; nCnt++)
	{
		// 更新
		m_apScene2D[nCnt]->Update();
	}
}

//==============================================================================
// 難易度選択の描画処理
//==============================================================================
void CTitleDifficulty::Draw(void)
{
	for (int nCnt = 0; nCnt < DIFFICULTY_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			m_apScene2D[nCnt]->Draw();
		}
	}
}

//==============================================================================
// 難易度選択の生成処理
//==============================================================================
CTitleDifficulty *CTitleDifficulty::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CTitleDifficulty *pTitleDifficulty;
	pTitleDifficulty = new CTitleDifficulty;

	// 初期化
	pTitleDifficulty->Init(pos);

	return pTitleDifficulty;
}

//==============================================================================
// 難易度ごとの設定
//==============================================================================
void CTitleDifficulty::SetMode(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// カーソルの明示
	if (m_bSelectDifficulty == false)
	{// 表示していない状態
		for (int nCnt = 0; nCnt < DIFFICULTY_MAX; nCnt++)
		{// α値ゼロ
			m_col[nCnt].a = 0.0f;
		}
	}
	else
	{
		if (CTitle::GetStart() == false)
		{
			for (int nCnt = 0; nCnt < DIFFICULTY_MAX; nCnt++)
			{
				if (nCnt == m_difficulty)
				{// 選択中
					if (m_bAlphaDown == true)
					{
						m_col[m_difficulty].a -= 0.01f;

						// 点滅の折り返し
						if (m_col[m_difficulty].a <= 0.5f)
						{
							m_col[m_difficulty].a = 0.5f;
							m_bAlphaDown = false;
						}
					}
					else
					{
						m_col[m_difficulty].a += 0.01f;

						// 点滅の折り返し
						if (m_col[m_difficulty].a >= 1.0f)
						{
							m_col[m_difficulty].a = 1.0f;

							m_bAlphaDown = true;
						}
					}

					m_apScene2D[nCnt]->SetCol(m_col[m_difficulty]);

				}
				else
				{// それ以外
					m_col[nCnt].a = 0.1f;
					m_apScene2D[nCnt]->SetCol(m_col[nCnt]);
				}
			}
		}
		else
		{
			m_nBlinkCnt++;

			if (m_nBlinkCnt % 2 == 0)
			{
				m_col[m_difficulty].a = 0.0f;
			}
			else
			{
				m_col[m_difficulty].a = 1.0f;
			}

			// 色の設定
			m_apScene2D[m_difficulty]->SetCol(m_col[m_difficulty]);
		}
		// カーソルの移動
		if (keyboard->GetTrigger(CInput::KEYINFO_DOWN) == true || gamepad->GetTrigger(CInput::KEYINFO_DOWN) == true)
		{
			CSound::Play(CSound::SOUND_LABEL_SE_ITEM001);
			m_difficulty = (DIFFICULTY)(m_difficulty + 1);
		}
		else if (keyboard->GetTrigger(CInput::KEYINFO_UP) == true || gamepad->GetTrigger(CInput::KEYINFO_UP) == true)
		{
			CSound::Play(CSound::SOUND_LABEL_SE_ITEM001);
			m_difficulty = (DIFFICULTY)(m_difficulty - 1);
		}

		// カーソルの補正
		if (m_difficulty < 0)
		{
			m_difficulty = (DIFFICULTY)(DIFFICULTY_MAX - 1);
		}
		else if (m_difficulty >(DIFFICULTY_MAX - 1))
		{
			m_difficulty = DIFFICULTY_EASY;
		}

		// 選択肢ごとの処理
		if (m_bSelectDifficulty == true && CTitle::GetStart() == false)
		{
			if (keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true)
			{
				// ゲームの開始
				CTitle::SetStart(true);

				// 効果音の再生
				CSound::Play(CSound::SOUND_LABEL_SE_START000);

				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TUTORIAL);	// シーン遷移
			}
		}
	}	
}