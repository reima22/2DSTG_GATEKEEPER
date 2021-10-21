
//==============================================================================
//
// ゲームモード処理〔game.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "bullet.h"
#include "game.h"
#include "player.h"
#include "sound.h"
#include "bg.h"
#include "enemy.h"
#include "score.h"
#include "life.h"
#include "UI.h"
#include "defeat.h"
#include "line.h"
#include "invasion.h"
#include "fade.h"
#include "stock.h"
#include "result.h"
#include "pause.h"
#include "bullet_UI.h"
#include "textdata_system.h"
#include "message.h"
#include "message_text.h"
#include "message_window.h"
#include "caution.h"
#include "window_effect.h"
#include "result_logo.h"
#include "title_difficulty.h"
#include "sceneX.h"

//==============================================================================
// 静的メンバ変数宣言
//==============================================================================
int CGame::m_nClearDefeat;	// クリアする撃破数
int CGame::m_nEndInvasion;	// 侵入限界数
CCharacter *CGame::m_pCharaPlayer = NULL;	// プレイヤーキャラのポインタ
bool CGame::m_bGameStart;
int CGame::m_nPhase = 0;
int CGame::m_nSetType = 0;
bool CGame::m_bSetEnemy = false;
bool CGame::m_bMotherEnemy = false;
int CGame::m_nCntDownEnd = 0;

//CSceneX *CGame::m_pSceneX = NULL;

//==============================================================================
// コンストラクタ
//==============================================================================
CGame::CGame()
{
	m_nPhase = 0;
	m_nCntDownEnd = 0;
}

//==============================================================================
// デストラクタ
//==============================================================================
CGame::~CGame()
{
}

//==============================================================================
// ゲームモードの初期化処理
//==============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos)
{
	// システム
	switch (CTitleDifficulty::GetDifficulty())
	{
	case CTitleDifficulty::DIFFICULTY_EASY:
		m_nClearDefeat = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_DEFEAT_EASY);		// クリアする撃破数
		m_nEndInvasion = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_INVASION_EASY);	// 侵入限界数
		break;

	case CTitleDifficulty::DIFFICULTY_NORMAL:
		m_nClearDefeat = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_DEFEAT_NORMAL);	// クリアする撃破数
		m_nEndInvasion = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_INVASION_NORMAL);	// 侵入限界数
		break;

	case CTitleDifficulty::DIFFICULTY_HARD:
		m_nClearDefeat = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_DEFEAT_HARD);		// クリアする撃破数
		m_nEndInvasion = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_INVASION_HARD);	// 侵入限界数
		break;
	}

	m_bGameStart = false;	// ゲーム開始のスイッチ
	m_bSetEnemy = false;	// 敵の設定間隔
	m_bMotherEnemy = false;	// 母艦戦フェーズ
	m_bSetSE = false;		// 警告画面SE
	m_bSetMother = false;	// 母艦戦BGM再生
	m_bSetLogo = false;

	// 母艦の状態リセット
	CEnemyMother::SetMotherState(CEnemyMother::MOTHER_STATE_NONE);
	CEnemyMother::SetPhase(CEnemyMother::MOTHER_PHASE_0);

	// 背景生成
	CBg::Create();

	// 警告画面の生成
	CCaution::Create(SCREEN_CENTER);

	// メインUI生成
	CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 675.0f, 0.0f));

	// ボーダーラインの生成
	CLine::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 620.0f, 0.0f));

	// プレイヤー生成
	m_pCharaPlayer = CPlayer::Create(PLAYER_POS,
		CScene::OBJTYPE_PLAYER,
		PLAYERX, PLAYERY);

	// 残機数の生成
	CStock::Create(STOCK_POS);

	// 撃破数の生成
	CDefeat::Create(DEFEAT_POS);

	//// 侵入数の生成
	CInvasion::Create(INVASION_POS);

	// スコアの生成
	CScore::Create(SCORE_POS, 10.0f, 20.0f);

	// 画面エフェクトの生成
	CWindowEffect::Create(W_EFFECT_POS);

	// ライフの生成
	CLife::Create(LIFE_POS);

	// ポーズの生成
	CPause::Create();

	// メッセージ生成
	CMessage::Create(SCREEN_CENTER);

	// デブリの配置
	CSceneX::Create(D3DXVECTOR3(100.0f, 100.0f, 300.0f), D3DXVECTOR3(0.0f, -0.6f, 0.0f), D3DXVECTOR3(0.01f, 0.009f, 0.0f), 1);
	CSceneX::Create(D3DXVECTOR3(-100.0f, 100.0f, 280.0f), D3DXVECTOR3(0.0f, -0.8f, 0.0f), D3DXVECTOR3(0.01f, 0.01f, 0.0f), 2);
	CSceneX::Create(D3DXVECTOR3(-200.0f, 200.0f, 280.0f), D3DXVECTOR3(0.0f, -0.5f, 0.0f), D3DXVECTOR3(0.01f, 0.008f, 0.0f), 1);
	CSceneX::Create(D3DXVECTOR3(200.0f, 200.0f, 300.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR3(0.01f, 0.01f, 0.0f), 2);

	// モデルの配置
	CSceneX::Create(D3DXVECTOR3(150.0f, 50.0f, 240.0f), D3DXVECTOR3(-0.4f, -0.6f, 0.0f), D3DXVECTOR3(0.03f, 0.05f, 0.0f),0);
	CSceneX::Create(D3DXVECTOR3(150.0f, 100.0f, 120.0f), D3DXVECTOR3(-0.7f, -0.6f, 0.0f), D3DXVECTOR3(0.06f, 0.04f, 0.0f),0);
	CSceneX::Create(D3DXVECTOR3(150.0f, 100.0f, 0.0f), D3DXVECTOR3(-0.5f, -0.6f, 0.0f), D3DXVECTOR3(0.04f, 0.03f, 0.0f),0);

	// BGMの生成
	if (CManager::GetBGMPlay(CSound::BGMTYPE_GAME) == true)
	{ 
		CSound::Play(CSound::SOUND_LABEL_BGM001);
	}
	
	return S_OK;
}

//==============================================================================
// ゲームモードの終了処理
//==============================================================================
void CGame::Uninit(void)
{
	// BGMの停止
	CSound::Stop();

	// メッセージの破棄
	CMessage::Uninit();

	// ポーズの終了
	CPause::Uninit();

	// 敵の消滅
	CEnemy::UninitAll();

	// プレイヤーポインタの破棄
	if (m_pCharaPlayer != NULL)
	{
		m_pCharaPlayer->Uninit();
		m_pCharaPlayer = NULL;
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// ゲームモードの更新処理
//==============================================================================
void CGame::Update(void)
{
	// メッセージの更新
	CMessage::Update();

	// 開始判定
	m_bGameStart = CMessage::GetEnd();

	// フェード情報の取得 
	CFade::FADE fade = CFade::GetFade();

	// 撃破数の取得
	int nDefeat = CDefeat::GetDefeat();

	// 侵入数の取得
	int nInvasion = CInvasion::GetInvasion();

	// プレイヤーの状態取得
	CPlayer::PLAYERSTATE state = CPlayer::GetState();

	// 母艦フェーズへ
	if (nDefeat >= m_nClearDefeat && m_bMotherEnemy == false)
	{
		m_bMotherEnemy = true;
	}

	// 敵の自動生成
	if (m_bGameStart == true && m_bMotherEnemy == false)
	{// 通常敵のフェーズ
		// ローカル変数宣言
		bool bTrue = false;	// 出現方法が武装に応じて適切か

		if (m_bSetEnemy == false)
		{
			// ローカル変数宣言
			bool aType[CBulletUI::BUITYPE_MAX];

			// 弾保有情報の取得
			for (int nCnt = 1; nCnt < CBulletUI::BUITYPE_MAX; nCnt++)
			{
				aType[nCnt] = CBulletUI::GetTakeBullet(nCnt);
			}

			// 所有アイテムの応じて敵の出現方法を指定
			while (bTrue != true)
			{
				// 出現する方法の指定
				m_nSetType = rand() % CBulletUI::BUITYPE_MAX;
				
				if (aType[m_nSetType] == true || m_nSetType <= 1)
				{// 有効の場合、ループ終了
					bTrue = true;
				}
			}
			
			// 配置情報設定完了
			m_bSetEnemy = true;
		}

		// 編隊生成
		if (CEnemyFormation::GetIntervalBool() == false)
		{
			if (m_bSetEnemy == true)
			{					
				if (CDefeat::GetDefeat() >= m_nPhase * 10 && CDefeat::GetDefeat() % 10 == 0 && CDefeat::GetDefeat() != 0)
				{// 10体ごと
					m_nPhase++;
					m_bSetEnemy = false;
				}
			}

			// 敵の配置
			SetEnemyPattern(m_nSetType);
		}
		else
		{
			CEnemyFormation::FormInterval(120);
		}
	}
	else if (m_bMotherEnemy == true)
	{// 母艦フェーズ
		if (m_bSetSE == false)
		{// 登場演出
			CSound::Stop();	// BGMの一時停止
			CSound::Play(CSound::SOUND_LABEL_SE_WARNING);
			CEnemy::UninitAll();
			CBullet::UninitAll();
			m_bSetSE = true;
		}

		if (CCaution::GetCautionState() == CCaution::CAUTION_STATE_ALERT_END)
		{
			if (m_bSetMother == false)
			{// BGMの再生と母艦の生成
				CEnemyMother::MotherSetCreate();

				if (CManager::GetBGMPlay(CSound::BGMTYPE_BOSS) == true)
				{
					CSound::Play(CSound::SOUND_LABEL_BGM005);
				}

				m_bSetMother = true;
			}		
		}	
	}

	if (CEnemyMother::GetMotherState() == CEnemyMother::MOTHER_STATE_END || nInvasion >= m_nEndInvasion || state == CPlayer::PLAYERSTATE_GAMEOVER)
	{
		// ゲーム終了演出
		Ending();
	}
}

//==============================================================================
// ゲームモードの描画処理
//==============================================================================
void CGame::Draw(void)
{
	CMessage::Draw();

	CPause::Draw();
}

//==============================================================================
// ゲームモードの生成処理
//==============================================================================
CGame *CGame::Create()
{
	// インスタンス生成
	CGame *pGame;
	pGame = new CGame;
	D3DXVECTOR3 pos;

	// 初期化
	pGame->Init(pos);

	return pGame;
}

//==============================================================================
// 位置の取得
//==============================================================================
D3DXVECTOR3 CGame::GetPosition(void)
{
	D3DXVECTOR3 pos;
	return pos;
}

//==============================================================================
// サイズの取得
//==============================================================================
D3DXVECTOR3 CGame::GetSize(void)
{
	D3DXVECTOR3 size;
	return size;
}

//==============================================================================
// 敵配置の方法指定
//==============================================================================
void CGame::SetEnemyPattern(int nType)
{
	switch (nType)
	{
	case 0:	// 単体ランダム
		CEnemy::AutoSetCreate();
		break;

	case 1:	// ランダム出現
		CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_RANDOM);
		break;

	case 2:	// 両サイド
		CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_SIDE);
		break;

	case 3:	// 画面下部
		CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_UNDER);
		break;
	}
}

//==============================================================================
// ゲーム終了までの演出
//==============================================================================
void CGame::Ending(void)
{
	// 終了状態の設定
	if (CInvasion::GetInvasion() >= m_nEndInvasion || CPlayer::GetState() == CPlayer::PLAYERSTATE_GAMEOVER)
	{
		// ゲームオーバー状態の設定
		CResult::SetEndState(CResult::ENDSTATE_GAMEOVER);
	}
	else if (CEnemyMother::GetMotherState() == CEnemyMother::MOTHER_STATE_END)
	{
		// ゲームクリア状態の設定
		CResult::SetEndState(CResult::ENDSTATE_CLEAR);
	}

	if (m_bSetLogo == false)
	{
		// ロゴの表示
		CResultLogo::Create(RESULT_GAMEEND);

		m_bSetLogo = true;
	}

	// ゲーム終了までの間隔
	CntDownEnd();
}

//==============================================================================
// ゲーム終了までのカウントダウン
//==============================================================================
void CGame::CntDownEnd(void)
{
	// フェード情報の取得 
	CFade::FADE fade = CFade::GetFade();

	if (m_nCntDownEnd == 0)
	{
		CBullet::UninitAll();
		CEnemy::UninitAll();
	}

	// カウント加算
	m_nCntDownEnd++;

	if (m_nCntDownEnd >= 180)
	{// 約3秒
		// 結果スコアの更新
		CScore::SetResultScore(CScore::GetScore());

		// フェード開始
		if (fade == CFade::FADE_NONE)
		{
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
		}
	}
}