//==============================================================================
//
// ポーズ選択肢処理〔pause_select.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "pause_select.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

// 静的メンバ変数宣言
CScene2D *CPauseSelect::m_apScene2D[PAUSE_MAX] = {};
CPauseSelect::PAUSE CPauseSelect::m_pause;				// 選択中のカーソル
D3DXCOLOR CPauseSelect::m_col;							// 選択中カーソルの色

//==============================================================================
// コンストラクタ
//==============================================================================
CPauseSelect::CPauseSelect(int nPriority) : CScene2D(nPriority)
{
}

//==============================================================================
// デストラクタ
//==============================================================================
CPauseSelect::~CPauseSelect()
{
}

//==============================================================================
// ポーズベースの初期化処理
//==============================================================================
HRESULT CPauseSelect::Init(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < PAUSE_MAX; nCnt++)
	{
		m_apScene2D[nCnt] = new CScene2D(PRIORITY_PAUSE);
		m_apScene2D[nCnt]->Init(D3DXVECTOR3(pos.x, pos.y - 100.0f + (nCnt * 100.0f), 0.0f));
		m_apScene2D[nCnt]->SetSize(140.0f, 40.0f);
		m_apScene2D[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_apScene2D[nCnt]->BindTexture((CTexture::TEXTYPE)(CTexture::TEXTYPE_PAUSE_CONTINUE + nCnt));
	}

	// 初期化
	m_pause = PAUSE_CONTINUE;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bAlphaDown = true;

	return S_OK;
}

//==============================================================================
// ポーズベースの終了処理
//==============================================================================
void CPauseSelect::Uninit(void)
{
	for (int nCnt = 0; nCnt < PAUSE_MAX; nCnt++)
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
// ポーズベースの更新処理
//==============================================================================
void CPauseSelect::Update(void)
{
	// ローカル変数宣言
	bool bPause = CPause::GetPause();	// ポーズの取得
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// 色の設定
	
	// ポーズ状態に応じて変化
	switch (bPause)
	{
	case true:
		col.a = 1.0f;

		// 選択モードの設定
		SetMode();

		break;

	case false:
		col.a = 0.0f;
		break;
	}

	for (int nCnt = 0; nCnt < PAUSE_MAX; nCnt++)
	{
		// 色状態の設定
		m_apScene2D[nCnt]->SetCol(col);
		m_apScene2D[nCnt]->Update();
	}
}

//==============================================================================
// ポーズベースの描画処理
//==============================================================================
void CPauseSelect::Draw(void)
{
	for (int nCnt = 0; nCnt < PAUSE_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			m_apScene2D[nCnt]->Draw();
		}
	}
}

//==============================================================================
// ポーズベースの生成処理
//==============================================================================
CPauseSelect *CPauseSelect::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CPauseSelect *pPauseSelect;
	pPauseSelect = new CPauseSelect;

	// 初期化
	pPauseSelect->Init(pos);

	return pPauseSelect;
}

//==============================================================================
// ポーズ状態ごとの設定
//==============================================================================
void CPauseSelect::SetMode(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// カーソルの明示
	for (int nCnt = 0; nCnt < PAUSE_MAX; nCnt++)
	{
		if (nCnt == m_pause)
		{// 選択中
			if (m_bAlphaDown == true)
			{
				m_col.a -= 0.01f;
				
				// 点滅の折り返し
				if (m_col.a <= 0.5f)
				{
					m_col.a = 0.5f;
					m_bAlphaDown = false;
				}
			}
			else
			{
				m_col.a += 0.01f;

				// 点滅の折り返し
				if (m_col.a >= 1.0f)
				{
					m_col.a = 1.0f;

					m_bAlphaDown = true;
				}
			}

			m_apScene2D[nCnt]->SetTex(1, 2, 0, 0, 0.0f, 0.0f);
			m_apScene2D[nCnt]->SetCol(m_col);
			
		}
		else
		{// それ以外
			m_apScene2D[nCnt]->SetTex(1, 2, 0, 1, 0.0f, 0.0f);
			m_apScene2D[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	// カーソルの移動
	if (keyboard->GetTrigger(CInput::KEYINFO_DOWN) == true || gamepad->GetTrigger(CInput::KEYINFO_DOWN) == true)
	{
		CSound::Play(CSound::SOUND_LABEL_SE_ITEM001);
		m_pause = (PAUSE)(m_pause + 1);
	}
	else if (keyboard->GetTrigger(CInput::KEYINFO_UP) == true || gamepad->GetTrigger(CInput::KEYINFO_UP) == true)
	{
		CSound::Play(CSound::SOUND_LABEL_SE_ITEM001);
		m_pause = (PAUSE)(m_pause - 1);
	}

	// カーソルの補正
	if (m_pause < 0)
	{
		m_pause = (PAUSE)(PAUSE_MAX - 1);
	}
	else if (m_pause > (PAUSE_MAX - 1))
	{
		m_pause = PAUSE_CONTINUE;
	}

	// 選択肢ごとの処理
	if (keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true)
	{
		// 効果音の再生
		CSound::Play(CSound::SOUND_LABEL_SE_START000);

		switch (m_pause)
		{
		case PAUSE_CONTINUE:
			// ポーズの解除
			CPause::SetPause();
			break;

		case PAUSE_RETRY:
			// ゲームのやり直し
			CFade::SetFade(CFade::FADE_OUT,CManager::MODE_GAME);
			break;

		case PAUSE_QUIT:
			// タイトルへ戻る
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
			CPause::SetPause();
			break;
		}	
	}
}