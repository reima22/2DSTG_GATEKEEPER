//==============================================================================
//
// タイトルロゴ描画処理〔title_logo.cpp〕
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
#include "title_logo.h"

// 静的メンバ変数宣言
bool CTitleLogo::m_bLogoMoveEnd;

//==============================================================================
// コンストラクタ
//==============================================================================
CTitleLogo::CTitleLogo(int nPriority) : CScene2D(nPriority)
{
	// ポリゴンの初期化
	m_pScene2D = NULL;

	m_bLogoMoveEnd = false;
}

//==============================================================================
// デストラクタ
//==============================================================================
CTitleLogo::~CTitleLogo()
{
}

//==============================================================================
// タイトルロゴの初期化処理
//==============================================================================
HRESULT CTitleLogo::Init(D3DXVECTOR3 pos)
{
	m_pos = pos;

	if (m_pScene2D == NULL)
	{
		m_pScene2D = CScene2D::Create();
		m_pScene2D->SetPosition(pos);
		m_pScene2D->SetSize(T_LOGO_SIZE);
		m_pScene2D->SetTex(1, 1, 0, 0, 0, 0);
		m_pScene2D->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pScene2D->BindTexture("TITLE_LOGO");
	}

	return S_OK;
}

//==============================================================================
// タイトルロゴの終了処理
//==============================================================================
void CTitleLogo::Uninit(void)
{
	if (m_pScene2D != NULL)
	{// NULLチェック後に破棄
		m_pScene2D->Uninit();
		m_pScene2D = NULL;
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// タイトルロゴの更新処理
//==============================================================================
void CTitleLogo::Update(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();
	
	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// 色
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// Enterを押すか、位置が指定値より右だった場合
	if ((keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) || m_pos.x > T_LOGO_SETPOS.x) &&
		m_bLogoMoveEnd == false)
	{
		// 移動値
		m_move = VECTOR3_NULL;

		// 位置を与える
		m_pos = T_LOGO_SETPOS;

		// 着地音
		m_pSound->Play(CSound::SOUND_LABEL_SE_SAVEPOINT);

		// 移動完了
		m_bLogoMoveEnd = true;
	}
	else  if (m_pos.x < T_LOGO_SETPOS.x)
	{// 位置が指定値より左だった場合

	 // 重力
		float fGravity = GRAVITY;

		// カウント値が一定値を下回っている場合
		if (m_nJumpCnt <= 0)
		{
			// 速度
			m_fSpead = MOVE_SPEED;
		}

		// 速度に重力を代入
		m_fSpead += fGravity;

		// 位置が指定値よりも高い場合
		if (m_pos.y > T_LOGO_SETPOS.y)
		{
			// 速度に代入
			m_fSpead = -m_fSpead;

			// カウント加算
			m_nJumpCnt++;

			// バウンド音
			m_pSound->Play(CSound::SOUND_LABEL_SE_JUMP);
		}

		// 移動値
		m_move.x += MOVE_TITLE_X;
		m_move.y = m_fSpead + (m_nJumpCnt / 1.2f);
	}

	// 移動値を位置座標に足す
	m_pos += m_move;

	m_pScene2D->SetPosition(m_pos);
}

//==============================================================================
// タイトルロゴの描画処理
//==============================================================================
void CTitleLogo::Draw(void)
{

}

//==============================================================================
// タイトルロゴの生成処理
//==============================================================================
CTitleLogo *CTitleLogo::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CTitleLogo *pTitleLogo;
	pTitleLogo = new CTitleLogo;

	// 初期化
	pTitleLogo->Init(pos);

	return pTitleLogo;
}