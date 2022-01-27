//==============================================================================
//
// エンターサイン描画処理〔title_enter.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "fade.h"
#include "gamepad.h"
#include "input.h"
#include "main.h"
#include "manager.h"
#include "polygon.h"
#include "renderer.h"
#include "scene2D.h"
#include "title.h"
#include "title_logo.h"
#include "title_enter.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CTitleEnter::CTitleEnter(int nPriority) : CScene2D(nPriority)
{
	// ポリゴンの初期化
	m_pScene2D = NULL;
}

//==============================================================================
// デストラクタ
//==============================================================================
CTitleEnter::~CTitleEnter()
{
}

//==============================================================================
// エンターサインの初期化処理
//==============================================================================
HRESULT CTitleEnter::Init(D3DXVECTOR3 pos)
{
	// メンバー変数初期化
	m_pos = pos;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	if (m_pScene2D == NULL)
	{
		m_pScene2D = CScene2D::Create();
		m_pScene2D->SetPosition(pos);
		m_pScene2D->SetSize(T_ENTER_SIZE);
		m_pScene2D->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
		m_pScene2D->SetCol(m_col);
		m_pScene2D->BindTexture("TITLE_ENTER");
	}

	return S_OK;
}

//==============================================================================
// エンターサインの終了処理
//==============================================================================
void CTitleEnter::Uninit(void)
{
	if (m_pScene2D != NULL)
	{// NULLチェック後に破棄
		m_pScene2D->Uninit();
		m_pScene2D = NULL;
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// エンターサインの更新処理
//==============================================================================
void CTitleEnter::Update(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// フェードの取得
	CFade *pFade = CManager::GetFade();

	// タイマー
	m_nTimer++;

	// 判定がtrueの場合
	if (m_bEnter == true && (keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true) &&
		m_bNextScene == false)
	{
		// 判定をtrueに変える
		m_bNextScene = true;

		// SE
		m_pSound->Play(CSound::SOUND_LABEL_SE_TITLEENTER);
	}
	else if (m_bEnter == false)
	{// 判定がfalseの場合

	 // エンターキーを押した場合
		if (keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true)
		{
			// 判定をtrueに変える
			m_bEnter = true;

			// カウントの値を0にする
			m_nTimeColor = 0;
		}
	}

	// 判定がどちらもtrueの場合
	if (m_bEnter == true && m_bNextScene == true)
	{
		// カウント加算
		m_nTimeColor++;

		// カウントが5で割ると0になる場合
		if (m_nTimeColor % 5 == 0)
		{
			// 色
			m_col.a = 1.0f;
		}
		else
		{
			// 色
			m_col.a = 0.5f;
		}

		// カウントが50を超えた場合
		if (m_nTimeColor >= TIME_COLOR)
		{
			// フェード
			pFade->SetFade(CFade::FADE_OUT, CMode::MODE_SELECT);

			//// 判定をfalseに変える
			//m_bEnter = false;
			//// 判定をfalseに変える
			//m_bNextScene = false;
			//// タイマーを0にする
			//m_nTimer = 0;
		}
	}

	// 位置が一定よりも高い位置にあるかつ判定がfalseの場合
	if (m_pos.y > T_ENTER_SETPOS.y && m_bEnter == false)
	{
		// 移動値
		m_move.y = MOVE_START_Y;

		// 色
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		// 位置を与える
		m_pos = T_ENTER_SETPOS;

		// 移動値
		m_move.y = 0.0f;

		// 判定をtrueに変える
		m_bEnter = true;

		//待機点滅カウント
		m_nCntColor++;
	}

	// 判定がtrueではない場合
	if (m_bNextScene != true)
	{
		// カウントが指定値より低い場合場合
		if (m_nCntColor <= 50)
		{
			m_col.a -= 0.02f;

			// 点滅の折り返し
			if (m_col.a <= 0.0f)
			{
				m_col.a = 0.0f;
			}
		}

		if (m_nCntColor > 50 && m_nCntColor <= 100)
		{// カウントが指定値の範囲内の場合

		 // 色
			m_col.a += 0.02f;

			// 点滅の折り返し
			if (m_col.a >= 1.0f)
			{
				m_col.a = 1.0f;

			}
		}
		else if (m_nCntColor > 125)
		{ //カウントが指定値を超えている場合

		  //カウントを0にする
			m_nCntColor = 0;
		}
	}

	m_pos += m_move;

	// 位置設定
	m_pScene2D->SetPosition(m_pos);

	// 色の設定
	m_pScene2D->SetCol(m_col);
}

//==============================================================================
// エンターサインの描画処理
//==============================================================================
void CTitleEnter::Draw(void)
{

}

//==============================================================================
// エンターサインの生成処理
//==============================================================================
CTitleEnter *CTitleEnter::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CTitleEnter *pTitleEnter;
	pTitleEnter = new CTitleEnter;

	// 初期化
	if (pTitleEnter != NULL)
	{
		pTitleEnter->Init(pos);
	}

	return pTitleEnter;
}