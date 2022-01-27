//==============================================================================
//
// タイトル背景ランニングマン描画処理〔title_runningman.cpp〕
// Author : Tatsuki Matsuda
//
//==============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "title_runningman.h"

// 静的メンバ変数宣言
bool CTitleRunningMan::m_bRunningManMoveEnd;

//==============================================================================
// コンストラクタ
//==============================================================================
CTitleRunningMan::CTitleRunningMan(int nPriority) : CScene2D(nPriority)
{
	// ポリゴンの初期化
	m_pScene2D = NULL;

	m_bRunningManMoveEnd = false;
}

//==============================================================================
// デストラクタ
//==============================================================================
CTitleRunningMan::~CTitleRunningMan()
{
}

//==============================================================================
// ランニングマンの初期化処理
//==============================================================================
HRESULT CTitleRunningMan::Init(D3DXVECTOR3 pos)
{
	m_pos = pos;

	if (m_pScene2D == NULL)
	{
		m_pScene2D = CScene2D::Create();
		m_pScene2D->SetPosition(pos);
		m_pScene2D->SetSize(T_RUNNINGMAN_SIZE);
		m_pScene2D->SetTex(1, 1, 0, 0, 0, 0);
		m_pScene2D->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pScene2D->BindTexture("TITLE_RUNNINGMAN");
	}

	//アニメーション用カウント
	m_nCounterAnim = 0;
	m_nPatternAnimX = 0;
	m_nPatternAnimY = 0;

	return S_OK;
}

//==============================================================================
// ランニングマンの終了処理
//==============================================================================
void CTitleRunningMan::Uninit(void)
{
	if (m_pScene2D != NULL)
	{// NULLチェック後に破棄
		m_pScene2D->Uninit();
		m_pScene2D = NULL;
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// ランニングマンの更新処理
//==============================================================================
void CTitleRunningMan::Update(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// 色
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//アニメーション用カウント
	m_nCounterAnim++;

	//アニメーション
	if (m_nCounterAnim % ANIMATION_COUNT == 0)
	{
		//アニメーション用パターン数
		m_nPatternAnimX++;
	}

	//アニメーション用テクスチャ座標
	m_pScene2D->SetTex(8, 1, m_nPatternAnimX, 0, 0, 0);

	if (m_pos.x < T_RUNNINGMAN_SETPOS.x)
	{// 位置が値より左だった場合

		// 移動値
		m_move.x = MOVE_RUNNINGMAN_X;
	}
	else
	{
		//超えたら初期位置に戻す
		m_pos.x = 0.0f - T_RUNNINGMAN_SIZE.x;
	}

	// 移動値を位置座標に足す
	m_pos += m_move;

	// 位置
	m_pScene2D->SetPosition(m_pos);
}

//==============================================================================
// ランニングマンの描画処理
//==============================================================================
void CTitleRunningMan::Draw(void)
{

}

//==============================================================================
// ランニングマンの生成処理
//==============================================================================
CTitleRunningMan *CTitleRunningMan::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CTitleRunningMan *pTitleRunningman;
	pTitleRunningman = new CTitleRunningMan;

	// 初期化
	pTitleRunningman->Init(pos);

	return pTitleRunningman;
}