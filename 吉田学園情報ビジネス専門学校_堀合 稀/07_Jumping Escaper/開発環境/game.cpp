//==============================================================================
//
// ゲームモード処理〔game.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "camera.h"
#include "effect.h"
#include "fade.h"
#include "fog.h"
#include "game.h"
#include "gamesetlogo.h"
#include "main.h"
#include "manager.h"
#include "meshfield.h"
#include "motion_player.h"
#include "object.h"
#include "pause.h"
#include "player.h"
#include "renderer.h"
#include "result.h"
#include "scene2D.h"
#include "select.h"
#include "Terrain.h"
#include "textdata_object.h"
#include "textdata_meshfield.h"
#include "timer_count.h"
#include "ui.h"

//==============================================================================
// 静的メンバ変数宣言
//==============================================================================
bool					CGame::m_bGameEnd = false;						// ゲーム終了のフラグ
int						CGame::m_nClearDirectingCnt = 0;				// ゲーム終了演出カウント
CGame::CLEARPHASE		CGame::m_clearPhase = CLEARPHASE_NONE;			// クリアフェイズ
CGame::GAMEOVERPHASE	CGame::m_gameoverPhase = GAMEOVERPHASE_NONE;	// ゲームオーバーフェイズ
CGamesetLogo			*CGame::m_pGamesetLogo = NULL;					// ゲーム終了ロゴポインタ
CFog					*CGame::m_pFog = NULL;							// フォグ効果ポインタ
CPause					*CGame::m_pPause = NULL;						// ポーズポインタ
CPlayer					*CGame::m_pPlayer = NULL;						// プレイヤーポインタ
CTerrain				*CGame::m_pTerrain = NULL;						// メッシュポインタ
CUi						*CGame::m_pUi = NULL;							// UIポインタ
CMeshfield				**CGame::m_ppMeshField = {};					// メッシュフィールドポインタ
CObject					**CGame::m_ppObject = {};						// オブジェクトポインタ

//==============================================================================
// コンストラクタ
//==============================================================================
CGame::CGame(int nPriority) : CMode(nPriority)
{
	m_bGameEnd = false;
	m_nClearDirectingCnt = 0;
	m_clearPhase = CLEARPHASE_NONE;
	m_gameoverPhase = GAMEOVERPHASE_NONE;
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
HRESULT CGame::Init(void)
{
	// カメラの位置初期化
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != NULL)
	{
		pCamera->SetInit();
	}

	// ステージ設定
	SetStageObject();
	
	// 3Dプレイヤーの生成
	if (m_pPlayer == NULL)
	{
		m_pPlayer = CPlayer::Create();
	}

	// UIの生成
	if (m_pUi == NULL)
	{
		m_pUi = CUi::Create();
	}

	// ポーズの生成
	if (m_pPause == NULL)
	{
		m_pPause = CPause::Create();
	}
	
	return S_OK;
}

//==============================================================================
// ゲームモードの終了処理
//==============================================================================
void CGame::Uninit(void)
{
	// BGMの停止
	if (m_pSound != NULL && CManager::GetBGMPlay(CSound::BGMTYPE_GAME) == true)
	{
		m_pSound->Stop();
	}

	// ポーズの終了
	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		m_pPause = NULL;
	}

	// UIの終了
	if (m_pUi != NULL)
	{
		m_pUi->Uninit();
		m_pUi = NULL;
	}
	
	// プレイヤーの終了
	if (m_pPlayer != NULL)
	{
		m_pPlayer->Uninit();
		m_pPlayer = NULL;
	}

	// オブジェクトポインタの開放
	if (m_ppObject != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumObject; nCnt++)
		{
			m_ppObject[nCnt]->Uninit();
		}

		// インスタンスの破棄
		if (m_nNumObject > 1)
		{
			delete[] m_ppObject;
		}
		else
		{
			delete m_ppObject;
		}
		
		m_ppObject = NULL;
	}

	// メッシュフィールドポインタの開放
	if (m_ppMeshField != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumMeshfield; nCnt++)
		{
			m_ppMeshField[nCnt]->Uninit();
		}

		// インスタンスの破棄
		if (m_nNumMeshfield > 1)
		{
			delete[] m_ppMeshField;
		}
		else
		{
			delete m_ppMeshField;
		}

		m_ppMeshField = NULL;
	}

	// フォグポインタの開放
	if (m_pFog != NULL)
	{
		m_pFog->Uninit();
		m_pFog = NULL;
	}

	// 死亡判定地形の開放
	if (m_pTerrain != NULL)
	{
		m_pTerrain = NULL;
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// ゲームモードの更新処理
//==============================================================================
void CGame::Update(void)
{
	// メッシュ地形の更新
	if (m_pTerrain != NULL)
	{
		m_pTerrain->Update();
	}

	// タイマーによる終了
	if (m_pUi != NULL)
	{
		if (m_pUi->GetTimer()->GetTimerCnt()->GetNumber() <= 0)
		{
			// 時間切れでゲームオーバー
			CResult::SetEndState(CResult::ENDSTATE_GAMEOVER);
			m_pPlayer->SetGameOver(true);
			m_bGameEnd = true;
		}
	}

#ifdef _DEBUG
	
	// フォグポインタの更新
	if (m_pFog != NULL)
	{
		// フォグ効果の調整
		m_pFog->Update();
	}

#endif

	// ゲーム終了
	if (m_bGameEnd == true)
	{
		EndGame();
	}
}

//==============================================================================
// ゲームモードの描画処理
//==============================================================================
void CGame::Draw(void)
{
	// 死亡判定メッシュ描画
	if (m_pTerrain != NULL)
	{
		m_pTerrain->Draw();
	}

	// フォグの描画
	if (m_pFog != NULL)
	{
		m_pFog->Draw();
	}
}

//==============================================================================
// ゲームモードの生成処理
//==============================================================================
CGame *CGame::Create()
{
	// インスタンス生成
	CGame *pGame;
	pGame = new CGame;

	// 初期化
	if (pGame != NULL)
	{
		pGame->Init();
	}

	return pGame;
}

//==============================================================================
// ステージオブジェクト設定
//==============================================================================
void CGame::SetStageObject(void)
{
	// レンダラーの取得
	CRenderer *pRenderer = CManager::GetRenderer();

	// メッシュデータの取得
	CTextDataMeshfield *pDataMeshfield = CTextData::GetDataMeshfield();
	m_nNumMeshfield = pDataMeshfield->GetNumMesh();

	// 動的確保
	m_ppMeshField = new CMeshfield*[m_nNumMeshfield];

	for (int nCntMesh = 0; nCntMesh < m_nNumMeshfield; nCntMesh++)
	{
		// メッシュフィールドの生成
		m_ppMeshField[nCntMesh] = CMeshfield::Create(nCntMesh);
	}

	// フォグの生成
	m_pFog = CFog::Create();

	// ステージデータの取得
	CStageSelect::STAGE_SELECT select = CStageSelect::GetSelectingStage();

	// データテキストの取得
	CTextDataObject *pDataObject = CTextData::GetDataObject((CTextDataObject::STAGEINFO)select);

	if (pDataObject != NULL)
	{
		// ステージオブジェクト数の取得
		m_nNumObject = pDataObject->GetNumObject();

		// オブジェクトの動的確保
		m_ppObject = new CObject*[m_nNumObject];

		// テキストからオブジェクトの生成
		for (int nCnt = 0; nCnt < m_nNumObject; nCnt++)
		{
			m_ppObject[nCnt] = CObject::CreateFromData(nCnt);
		}
	}

	// ステージごとの詳細
	switch (select)
	{
	case CStageSelect::STAGE_SELECT_1:	// 渓谷
		
		// BGMの生成
		if (m_pSound != NULL && CManager::GetBGMPlay(CSound::BGMTYPE_GAME) == true)
		{
			m_pSound->Play(CSound::SOUND_LABEL_BGM002);
		}

		m_pTerrain = CTerrain::Create(CTerrainInfo::TERRAIN_TYPE::TERRAIN_VALLEY);	// 死亡判定メッシュ生成

		// フォグ効果と背景色の設定
		if (m_pFog != NULL)
		{
			m_pFog->SetFogCol(FOG_COL_VALLEY);
		}

		pRenderer->SetBSColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));	// 背景色変更

		break;

	case CStageSelect::STAGE_SELECT_2:	// 海洋

		// BGMの生成
		if (m_pSound != NULL && CManager::GetBGMPlay(CSound::BGMTYPE_GAME) == true)
		{
			m_pSound->Play(CSound::SOUND_LABEL_BGM003);
		}

		m_pTerrain = CTerrain::Create(CTerrainInfo::TERRAIN_TYPE::TERRAIN_OCEAN);	// 死亡判定メッシュ生成

		// フォグ効果の設定
		if (m_pFog != NULL)
		{
			m_pFog->SetFogCol(FOG_COL_OCEAN);
		}

		pRenderer->SetBSColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));	// 背景色変更

		break;

	case CStageSelect::STAGE_SELECT_3:	// 溶岩
		
		// BGMの生成
		if (m_pSound != NULL && CManager::GetBGMPlay(CSound::BGMTYPE_GAME) == true)
		{
			m_pSound->Play(CSound::SOUND_LABEL_BGM004);
		}

		m_pTerrain = CTerrain::Create(CTerrainInfo::TERRAIN_TYPE::TERRAIN_LAVA);	// 死亡判定メッシュ生成

		// フォグ効果の設定
		if (m_pFog != NULL)
		{
			m_pFog->SetFogCol(FOG_COL_LAVA);
		}

		pRenderer->SetBSColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));					// 背景色変更

		break;
	}
}

//==============================================================================
// ゲーム終了演出
//==============================================================================
void CGame::EndGame(void)
{
	// クリア演出カウントの加算
	m_nClearDirectingCnt++;

	switch (CResult::GetEndState())
	{
	case CResult::ENDSTATE_CLEAR:		// クリア時

		GameClear();	// ゲームクリア演出

		break;

	case CResult::ENDSTATE_GAMEOVER:	// ゲームオーバー時

		GameOver();		// ゲームオーバー演出

		break;
	}
}

//==============================================================================
// ゲームクリア時の処理
//==============================================================================
void CGame::GameClear(void)
{
	// フェード情報の取得
	CFade *pFade;
	pFade = CManager::GetFade();

	switch (m_clearPhase)
	{
	case CLEARPHASE_NONE:	// 演出開始前状態

		SetClearPhase(CGame::CLEARPHASE_WAIT);	// 待機状態へ移行
		
		break;
		
	case CLEARPHASE_WAIT:	// 演出まで待機
		
		if (m_nClearDirectingCnt >= CNT_ONE_SECOND)
		{
			m_pSound->Play(CSound::SOUND_LABEL_SE_STAGECLEAR);	// ファンファーレ
															
			m_pGamesetLogo = CGamesetLogo::Create();	// ロゴ生成
			SetClearPhase(CLEARPHASE_PAUSE);			// フェーズの移行			
			m_nClearDirectingCnt = 0;					// 演出カウント初期化
		}

		break;

	case CLEARPHASE_PAUSE:	// ポージング

		m_pPlayer->GetMotionPlayer()->SetMotion(CMotionPlayer::MOTIONTYPE_CLEAR);

		if (m_nClearDirectingCnt % 6 == 0)
		{
			CEffectDetail::Create(CEffectDetail::EFFECTTYPE_GAMECLEAR, VECTOR3_NULL);

		}

		// 演出終了
		if (m_nClearDirectingCnt >= CNT_ONE_SECOND * 6)
		{
			SetClearPhase(CLEARPHASE_FADE);		// フェーズの移行
		}

		break;

	case CLEARPHASE_FADE:	// フェードアウト

		CResult::SetEndState(CResult::ENDSTATE_CLEAR);			// 結果の反映
		pFade->SetFade(CFade::FADE_OUT, CMode::MODE_RESULT);	// フェードアウト	

		break;
	}
}

//==============================================================================
// ゲームオーバー時の処理
//==============================================================================
void CGame::GameOver(void)
{
	// フェード情報の取得
	CFade *pFade;
	pFade = CManager::GetFade();

	switch (m_gameoverPhase)
	{
	case GAMEOVERPHASE_NONE:		// 演出開始前
		
		m_pSound->Stop();									// BGM停止
		m_pSound->Play(CSound::SOUND_LABEL_SE_TIMEOVER);	// SE再生
		SetOverPhase(GAMEOVERPHASE_WAIT);					// フェーズ移行
		
		break;

	case GAMEOVERPHASE_WAIT:		// 待機状態

		if (m_nClearDirectingCnt >= CNT_ONE_SECOND * 2)
		{
			SetOverPhase(GAMEOVERPHASE_DIRECTION);				// 演出フェーズへ移行
			m_pSound->Play(CSound::SOUND_LABEL_SE_GAMEOVER);	// SE再生
			m_nClearDirectingCnt = 0;							// 演出カウント初期化
		}
		break;

	case GAMEOVERPHASE_DIRECTION:	// 演出

		// ロゴ生成
		m_pGamesetLogo = CGamesetLogo::Create();

		if (m_nClearDirectingCnt >= CNT_ONE_SECOND * 6)
		{
			SetOverPhase(GAMEOVERPHASE_FADE);	// フェーズ移行
		}
		break;

	case GAMEOVERPHASE_FADE:		// フェードアウト

		CResult::SetEndState(CResult::ENDSTATE_GAMEOVER);		// 結果の反映
		pFade->SetFade(CFade::FADE_OUT, CMode::MODE_RESULT);	// フェードアウト

		break;
	}
}