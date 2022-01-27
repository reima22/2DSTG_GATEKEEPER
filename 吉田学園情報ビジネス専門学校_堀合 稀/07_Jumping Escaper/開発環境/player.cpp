//==============================================================================
//
// プレイヤー管理〔player.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "bonus.h"
#include "bonus_count.h"
#include "camera.h"
#include "checkpoint_logo.h"
#include "collision.h"
#include "effect.h"
#include "fade.h"
#include "fall_count.h"
#include "game.h"
#include "gamepad.h"
#include "input.h"
#include "main.h"
#include "manager.h"
#include "motion.h"
#include "motion_player.h"
#include "object.h"
#include "player.h"
#include "renderer.h"
#include "result.h"
#include "select.h"
#include "shadowS.h"
#include "start_cntdown.h"
#include "Terrain.h"
#include "textdata_player.h"
#include "textdata_object.h"
#include "timer.h"
#include "timer_count.h"
#include "ui.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CPlayer::CPlayer()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CPlayer::~CPlayer()
{

}

//==============================================================================
// プレイヤーの初期化処理
//==============================================================================
HRESULT CPlayer::Init(void)
{
	// テキストデータの取得
	CTextDataPlayer *pDataPlayer = CTextDataPlayer::GetDataPlayer();

	// 当たり判定の値
	m_pos = pDataPlayer->GetPosition();
	m_rot = pDataPlayer->GetRotation();
	m_fRadius = pDataPlayer->GetRadius();
	m_fHeight = pDataPlayer->GetHeight();

	m_posNeutral = pDataPlayer->GetPosition();

	// 初期位置のリスポーン地点
	m_posSave = m_pos;

	// 過去の位置
	m_posOld = VECTOR3_NULL;

	m_nCntSandEffect = 0;

	// オブジェクトタイプ
	SetObjType(OBJTYPE_PLAYER);

	// モーション情報の生成
	m_pMotionPlayer = CMotionPlayer::Create();

	// 影の設定
	m_pShadowS = CShadowS::Create(m_pos);

	// 終了時判定初期化
	m_bSetGameClear = false;
	m_bSetGameOver = false;

	return S_OK;
}

//==============================================================================
// プレイヤーの終了処理
//==============================================================================
void CPlayer::Uninit(void)
{
	// モーションの破棄
	if (m_pMotionPlayer != NULL)
	{
		m_pMotionPlayer->Uninit();
		m_pMotionPlayer = NULL;
	}

	// 影の開放
	if (m_pShadowS != NULL)
	{
		m_pShadowS->Uninit();
		m_pShadowS = NULL;
	}
}

//==============================================================================
// プレイヤーの更新処理
//==============================================================================
void CPlayer::Update(void)
{
	// UI情報
	CUi *pUi = CGame::GetUi();

	// タイマーによる判定
	if (pUi != NULL)
	{
		if (pUi->GetTimer()->GetTimerCnt()->GetNumber() <= 0 && m_bSetGameOver == false)
		{
			m_bSetGameOver = true;
			m_pSound->Play(CSound::SOUND_LABEL_SE_GAMEOVER);
		}
	}

	// 移動処理
	if (pUi->GetStartCntdown()->GetCntDown() == false && m_resetPhase == RESETPHASE_NONE)
	{
		PlayerMove();
	}
	else
	{
		m_nDashCnt = 0;
		m_nJumpCnt = 0;
		m_pMotionPlayer->SetMotion(CMotionPlayer::MOTIONTYPE_NEUTRAL);
	}

	// リスポーン位置への送還処理
	PlayerReset();

	// モーションの更新
	if (m_pMotionPlayer != NULL)
	{
		m_pMotionPlayer->Update(m_pMotionPlayer->GetMotion());
	}

	// 影の追従
	if (m_pShadowS != NULL)
	{
		m_pShadowS->SetPosition(D3DXVECTOR3(m_pos.x, m_pos.y + 5.0f, m_pos.z));
	}

	// 位置の更新
	SetPosition(m_pos);
	SetRotation(m_rot);

	// クリア時の挙動
	if (m_bSetGameClear == true)
	{
		ClearMotion();
	}
}

//==============================================================================
// プレイヤーの描画処理
//==============================================================================
void CPlayer::Draw(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;										// 計算用マトリックス
	D3DMATERIAL9 matDef;												// 現在のマテリアル保存用

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// モデルの描画
	if (m_pMotionPlayer != NULL)
	{
		m_pMotionPlayer->Draw();
	}
}

//==============================================================================
// 生成処理
//==============================================================================
CPlayer *CPlayer::Create(void)
{
	// ローカル変数宣言
	CPlayer *pPlayer;
	pPlayer = new CPlayer;	// インスタンス

	// 初期化
	if (pPlayer != NULL)
	{
		pPlayer->Init();
	}

	return pPlayer;
}

//==============================================================================
// 挙動処理
//==============================================================================
void CPlayer::PlayerMove(void)
{
	// フェード情報の取得
	CFade *pFade;
	pFade = CManager::GetFade();

	// テキストデータの取得
	CTextDataPlayer *pDataPlayer = CTextDataPlayer::GetDataPlayer();

	if (CGame::GetGameEnd() == false)
	{
		// 他オブジェクトとの当たり判定
		PlayerCollision();
	}

	// 過去位置の更新
	m_posOld = m_pos;

	// 重力の発生
	if (m_bLandObj == false && m_bLandMesh == false)
	{
		m_move.y -= GRAVITY_SCALE;
		m_nCntAir++;
		if (m_nCntAir >= AIR_CONTROL)
		{
			m_bAirControl = false;
		}
	}
	else
	{// 着地状態
		if (m_bJump == true)
		{// ジャンプから着地
			if (m_pMotionPlayer->GetMotion() == CMotionPlayer::MOTIONTYPE_JUMP || m_pMotionPlayer->GetMotion() == CMotionPlayer::MOTIONTYPE_RUN)
			{
				// 砂煙エフェクト
				CEffectDetail::Create(CEffectDetail::EFFECTTYPE_LANDING, m_pos);

				// 着地モーション
				m_pMotionPlayer->SetMotion(CMotionPlayer::MOTIONTYPE_LANDING);
			}
		}

		m_nCntAir = 0;
		m_bAirControl = true;
		m_bJump = false;
		m_move.y = 0.0f;
	}

	// ゲーム終了まで操作可能
	if (m_bSetGameClear == false && m_bSetGameOver == false)
	{
		PlayerControl();
	}

	// モーションの切り替え
	if (m_move.x > MOVE_MOTION || m_move.z > MOVE_MOTION || m_move.x < -MOVE_MOTION || m_move.z < -MOVE_MOTION)
	{// 移動力でモーションを維持
		if (m_bLandObj == true || m_bLandMesh == true)
		{// 地上にいる時
			if (m_move.x > RUN_MOTION || m_move.z > RUN_MOTION || m_move.x < -RUN_MOTION || m_move.z < -RUN_MOTION)
			{
				// 走るモーション
				m_pMotionPlayer->SetMotion(CMotionPlayer::MOTIONTYPE_RUN);
			}
			else
			{
				// 歩くモーション
				m_pMotionPlayer->SetMotion(CMotionPlayer::MOTIONTYPE_WALK);
			}		
		}		
	}
	else if(m_pMotionPlayer->GetMotion() == CMotionPlayer::MOTIONTYPE_RUN || m_pMotionPlayer->GetMotion() == CMotionPlayer::MOTIONTYPE_WALK)
	{
		// ニュートラルへ戻る
		m_pMotionPlayer->SetMotion(CMotionPlayer::MOTIONTYPE_NEUTRAL);
	}
}

//==============================================================================
// プレイヤー操作
//==============================================================================
void CPlayer::PlayerControl(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// ジャンプ：着地している
	if (keyboard->GetTrigger(CInput::KEYINFO_JUMP) == true ||
		gamepad->GetTrigger(CInput::KEYINFO_OK) == true)
	{
		// 着地しているときのみジャンプ
		if (m_bJump == false)
		{
			m_pMotionPlayer->SetMotion(CMotionPlayer::MOTIONTYPE_JUMP);		// ジャンプモーション切り替え
			m_move.y = 0.5f;												// 初期ジャンプ力の設定
			m_pos.y += 1.5f;
			m_pSound->Play(CSound::SOUND_LABEL_SE_JUMP);					// ジャンプ音
		}

		// 着地判定の解除
		m_bLandObj = false;
		m_bLandMesh = false;
	}
	if (keyboard->GetPress(CInput::KEYINFO_JUMP) == true ||
		gamepad->GetPress(CInput::KEYINFO_OK) == true)
	{
		// 滞空状態
		if (m_bJump == false && m_bLanding == false)
		{
			if (m_nJumpCnt == 0)
			{
				m_move.y = 5.0f;
			}
			if (m_nJumpCnt < JUMP_CNT_MAX)
			{// 限界値
				m_nJumpCnt++;
				m_move.y += 1.0f;
			}
		}
	}

	// 最大ジャンプエフェクト
	if (m_nJumpCnt >= JUMP_CNT_MAX && m_move.y > 0.0f)
	{
		CEffectDetail::Create(CEffectDetail::EFFECTTYPE_MAXJUMP, m_pos);
	}
	
	// ジャンプボタンを離す
	if (keyboard->GetRelease(CInput::KEYINFO_JUMP) == true ||
		gamepad->GetRelease(CInput::KEYINFO_OK) == true)
	{
		m_nJumpCnt = 0;
		m_bJump = true;
	}

	if (m_bAirControl == true)
	{
		// 歩行処理	
		PlayerWalk();

		// 走行処理
		PlayerRun();
	}

	// ゲームが終了していない
	if (CGame::GetGameEnd() == false)
	{
		// 移動
		m_pos += m_move;
	}

	// ダッシュによる加速
	if (m_nDashCnt < SPEED_RANK_CNT)
	{// 一段階
		m_fDashPower = 1.0f;
	}
	else if (m_nDashCnt <= 0)
	{// 加速無し
		m_fDashPower = 0.0f;
	}
	else if (m_nDashCnt < SPEED_RANK_CNT * 2)
	{// 2段階
		m_fDashPower = 1.5f;
	}
	else
	{// 最大加速
		m_fDashPower = 2.0f;
	
		// エフェクトの発生
		if (m_bLanding == true)
		{
			CEffectDetail::Create(CEffectDetail::EFFECTTYPE_SANDSMOKE, m_pos);
		}
	}

	// 目的の回転角の上限
	if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rotDest.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rotDest.y -= D3DX_PI * 2.0f;
	}

	// 向きの更新
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.1f;

	// 現在の回転角の上限
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	// 制御有効間
	if (m_bAirControl == true)
	{
		// 加速後の減速処理
		m_move.x += (0.0f - m_move.x) * SPEED_DOWN;
		m_move.z += (0.0f - m_move.z) * SPEED_DOWN;
	}

	// ジャンプ可能状態
	if (m_bLanding == true)
	{
		m_nJumpLimit = 0;
		m_nJumpCnt = 0;
	}
	else
	{
		// フレームカウント数の加算
		m_nJumpLimit++;

		// 一定フレーム数を越える
		if (m_nJumpLimit > JUMP_LIMIT_FRAME)
		{
			m_bJump = true;
		}
	}
}

//==============================================================================
// 歩行処理
//==============================================================================
void CPlayer::PlayerWalk(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	// カメラの方向
	D3DXVECTOR3 rotCamera = pCamera->GetRotation();

	// 入力情報
	if (keyboard->GetPress(CInput::KEYINFO_WALK_UP) == true ||
		keyboard->GetPress(CInput::KEYINFO_WALK_DOWN) == true ||
		keyboard->GetPress(CInput::KEYINFO_WALK_LEFT) == true ||
		keyboard->GetPress(CInput::KEYINFO_WALK_RIGHT) == true ||
		gamepad->GetPress(CInput::KEYINFO_WALK_UP) == true ||
		gamepad->GetPress(CInput::KEYINFO_WALK_DOWN) == true ||
		gamepad->GetPress(CInput::KEYINFO_WALK_LEFT) == true ||
		gamepad->GetPress(CInput::KEYINFO_WALK_RIGHT) == true)
	{// 歩く
		if (keyboard->GetPress(CInput::KEYINFO_WALK_UP) == true ||
			gamepad->GetPress(CInput::KEYINFO_WALK_UP) == true)
		{// 上方向
			if (keyboard->GetPress(CInput::KEYINFO_WALK_LEFT) == true ||
				gamepad->GetPress(CInput::KEYINFO_WALK_LEFT) == true)
			{// 左上方向
				m_move.x -= cosf(rotCamera.y + D3DX_PI / 4) * WALK_SPEED;
				m_move.z += sinf(rotCamera.y + D3DX_PI / 4) * WALK_SPEED;
				m_rotDest.y = rotCamera.y + (D3DX_PI * 3 / 4);
			}
			else if (keyboard->GetPress(CInput::KEYINFO_WALK_RIGHT) == true ||
				gamepad->GetPress(CInput::KEYINFO_WALK_RIGHT) == true)
			{// 右上方向
				m_move.x += cosf(rotCamera.y - D3DX_PI / 4) * WALK_SPEED;
				m_move.z -= sinf(rotCamera.y - D3DX_PI / 4) * WALK_SPEED;
				m_rotDest.y = rotCamera.y - (D3DX_PI * 3 / 4);
			}
			else
			{// 上方向
				m_move.x += sinf(rotCamera.y) * WALK_SPEED;
				m_move.z += cosf(rotCamera.y) * WALK_SPEED;
				m_rotDest.y = rotCamera.y + D3DX_PI;
			}
		}
		else if (keyboard->GetPress(CInput::KEYINFO_WALK_DOWN) == true ||
			gamepad->GetPress(CInput::KEYINFO_WALK_DOWN) == true)
		{
			if (keyboard->GetPress(CInput::KEYINFO_WALK_LEFT) == true ||
				gamepad->GetPress(CInput::KEYINFO_WALK_LEFT) == true)
			{// 左下方向
				m_move.x += cosf(rotCamera.y + D3DX_PI * 3 / 4) * WALK_SPEED;
				m_move.z -= sinf(rotCamera.y + D3DX_PI * 3 / 4) * WALK_SPEED;
				m_rotDest.y = rotCamera.y + D3DX_PI / 4;
			}
			else if (keyboard->GetPress(CInput::KEYINFO_WALK_RIGHT) == true ||
				gamepad->GetPress(CInput::KEYINFO_WALK_RIGHT) == true)
			{// 右下方向
				m_move.x -= cosf(rotCamera.y - D3DX_PI * 3 / 4) * WALK_SPEED;
				m_move.z += sinf(rotCamera.y - D3DX_PI * 3 / 4) * WALK_SPEED;
				m_rotDest.y = rotCamera.y + D3DX_PI / -4;
			}
			else
			{// 下方向
				m_move.x += sinf(rotCamera.y - D3DX_PI) * WALK_SPEED;
				m_move.z += cosf(rotCamera.y - D3DX_PI) * WALK_SPEED;
				m_rotDest.y = rotCamera.y;
			}
		}
		else if (keyboard->GetPress(CInput::KEYINFO_WALK_LEFT) == true ||
			gamepad->GetPress(CInput::KEYINFO_WALK_LEFT) == true)
		{
			m_move.x -= cosf(rotCamera.y) * WALK_SPEED;
			m_move.z += sinf(rotCamera.y) * WALK_SPEED;
			m_rotDest.y = rotCamera.y + (D3DX_PI / 2);
		}
		else if (keyboard->GetPress(CInput::KEYINFO_WALK_RIGHT) == true ||
			gamepad->GetPress(CInput::KEYINFO_WALK_RIGHT) == true)
		{
			m_move.x += cosf(rotCamera.y) * WALK_SPEED;
			m_move.z -= sinf(rotCamera.y) * WALK_SPEED;
			m_rotDest.y = rotCamera.y + (D3DX_PI / -2);
		}
	}
}

//==============================================================================
// 走行処理
//==============================================================================
void CPlayer::PlayerRun(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	// カメラの方向
	D3DXVECTOR3 rotCamera = pCamera->GetRotation();

	// 入力処理
	if (keyboard->GetPress(CInput::KEYINFO_UP) == true ||
		keyboard->GetPress(CInput::KEYINFO_DOWN) == true ||
		keyboard->GetPress(CInput::KEYINFO_LEFT) == true ||
		keyboard->GetPress(CInput::KEYINFO_RIGHT) == true ||
		gamepad->GetPress(CInput::KEYINFO_UP) == true ||
		gamepad->GetPress(CInput::KEYINFO_DOWN) == true ||
		gamepad->GetPress(CInput::KEYINFO_LEFT) == true ||
		gamepad->GetPress(CInput::KEYINFO_RIGHT) == true)
	{// 走る
		m_nDashCnt++;	// ダッシュカウント加算

		if (keyboard->GetPress(CInput::KEYINFO_UP) == true ||
			gamepad->GetPress(CInput::KEYINFO_UP) == true)
		{// 上方向
			if (keyboard->GetPress(CInput::KEYINFO_LEFT) == true ||
				gamepad->GetPress(CInput::KEYINFO_LEFT) == true)
			{// 左上方向
				m_move.x -= cosf(rotCamera.y + D3DX_PI / 4) * m_fDashPower;
				m_move.z += sinf(rotCamera.y + D3DX_PI / 4) * m_fDashPower;
				m_rotDest.y = rotCamera.y + (D3DX_PI * 3 / 4);
			}
			else if (keyboard->GetPress(CInput::KEYINFO_RIGHT) == true ||
				gamepad->GetPress(CInput::KEYINFO_RIGHT) == true)
			{// 右上方向
				m_move.x += cosf(rotCamera.y - D3DX_PI / 4) * m_fDashPower;
				m_move.z -= sinf(rotCamera.y - D3DX_PI / 4) * m_fDashPower;
				m_rotDest.y = rotCamera.y - (D3DX_PI * 3 / 4);
			}
			else
			{// 上方向
				m_move.x += sinf(rotCamera.y) * m_fDashPower;
				m_move.z += cosf(rotCamera.y) * m_fDashPower;
				m_rotDest.y = rotCamera.y + D3DX_PI;
			}
		}
		else if (keyboard->GetPress(CInput::KEYINFO_DOWN) == true ||
			gamepad->GetPress(CInput::KEYINFO_DOWN) == true)
		{
			if (keyboard->GetPress(CInput::KEYINFO_LEFT) == true ||
				gamepad->GetPress(CInput::KEYINFO_LEFT) == true)
			{// 左下方向
				m_move.x += cosf(rotCamera.y + D3DX_PI * 3 / 4) * m_fDashPower;
				m_move.z -= sinf(rotCamera.y + D3DX_PI * 3 / 4) * m_fDashPower;
				m_rotDest.y = rotCamera.y + D3DX_PI / 4;
			}
			else if (keyboard->GetPress(CInput::KEYINFO_RIGHT) == true ||
				gamepad->GetPress(CInput::KEYINFO_RIGHT) == true)
			{// 右下方向
				m_move.x -= cosf(rotCamera.y - D3DX_PI * 3 / 4) * m_fDashPower;
				m_move.z += sinf(rotCamera.y - D3DX_PI * 3 / 4) * m_fDashPower;
				m_rotDest.y = rotCamera.y + D3DX_PI / -4;
			}
			else
			{// 下方向
				m_move.x += sinf(rotCamera.y - D3DX_PI) * m_fDashPower;
				m_move.z += cosf(rotCamera.y - D3DX_PI) * m_fDashPower;
				m_rotDest.y = rotCamera.y;
			}
		}
		else if (keyboard->GetPress(CInput::KEYINFO_LEFT) == true ||
			gamepad->GetPress(CInput::KEYINFO_LEFT) == true)
		{
			m_move.x -= cosf(rotCamera.y) * m_fDashPower;
			m_move.z += sinf(rotCamera.y) * m_fDashPower;
			m_rotDest.y = rotCamera.y + (D3DX_PI / 2);
		}
		else if (keyboard->GetPress(CInput::KEYINFO_RIGHT) == true ||
			gamepad->GetPress(CInput::KEYINFO_RIGHT) == true)
		{
			m_move.x += cosf(rotCamera.y) * m_fDashPower;
			m_move.z -= sinf(rotCamera.y) * m_fDashPower;
			m_rotDest.y = rotCamera.y + (D3DX_PI / -2);
		}
	}
	else
	{
		if (m_nDashCnt > 0 && m_bLanding == true)
		{
			// 無操作間の減少
			m_nDashCnt -= RUN_CNT_DOWN;
		}
	}
}

//==============================================================================
// 当たり判定
//==============================================================================
void CPlayer::PlayerCollision(void)
{
	// UI
	CUi *pUi = CGame::GetUi();

	// メッシュ
	CTerrain *pTerrain = CGame::GetTerrain();

	// チェックポイントロゴ
	CCheckpointLogo *pCheckPointLogo = pUi->GetCheckPointLogo();

	// 死亡判定メッシュとの当たり判定
	if (pTerrain != NULL)
	{
		if (pTerrain->FallCollider(this) == true)
		{
			if (pTerrain->GetType() == CTerrainInfo::TERRAIN_OCEAN || pTerrain->GetType() == CTerrainInfo::TERRAIN_VALLEY)
			{
				m_pSound->Play(CSound::SOUND_LABEL_SE_DIVE02);
				CEffectDetail::Create(CEffectDetail::EFFECTTYPE_DIVE_WATER, m_pos);
			}
			else if (pTerrain->GetType() == CTerrainInfo::TERRAIN_LAVA)
			{
				m_pSound->Play(CSound::SOUND_LABEL_SE_DIVE00);
				m_bIntoLava = true;
				m_posLava = m_pos;
			}

			// 落下数加算
			pUi->GetFall()->GetFallCount()->AddFall(1);

			// リセット開始
			ResetOn();
		}
	}

	// ステージ外落下判定
	if (m_pos.y < -FALL_RETURN)
	{
		// リセット開始
		ResetOn();

		// 落下音
		m_pSound->Play(CSound::SOUND_LABEL_SE_DIVE01);

		// 落下数加算
		pUi->GetFall()->GetFallCount()->AddFall(1);
	}

	// 他オブジェクトとの当たり判定
	CCollision *pCollision = NULL;
	pCollision = CManager::GetCollision(); // 当たり判定取得

	// 開始時カウント
	CStartCntdown *pStartCntdown;
	pStartCntdown = pUi->GetStartCntdown();

	if (pCollision != NULL)
	{
		CScene *pScene = CScene::GetScene(PRIORITY_DEFAULT);
		CScene *pSceneNext = NULL;

		m_bLandObj = false;
		m_bLanding = false;

		while (pScene)
		{
			pSceneNext = CScene::GetSceneNext(pScene);

			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{
				CObject *pObject = (CObject*)pScene;

				// モデルポインタ
				CModel *pModel = NULL;

				// モデル情報の取得
				pModel = pObject->GetModel();

				if (pObject->GetObjAttribute() != CObject::OBJATTRIBUTE_PHANTOM)
				{
					if (pCollision->Collision3D(this, pModel) == true)
					{
						// 足場ボーナス
						if ((pObject->GetObjAttribute() == CObject::OBJATTRIBUTE_SCAFFORD ||
							pObject->GetObjAttribute() == CObject::OBJATTRIBUTE_SCAFFORD_NODROP) && pObject->GetBonus() == false)
						{
							// 一度踏んだ判定
							pObject->SetBonus(true);

							pUi->GetBonus()->GetCount()->AddBonus(1);
						}
						
						m_bLandObj = true;	// オブジェクトへの着地判定

						switch (pObject->GetObjAttribute())
						{
						case CObject::OBJATTRIBUTE_SAVE:	// 中間地点
							if (m_posSave != D3DXVECTOR3(pModel->GetPosition().x, m_posSave.y, pModel->GetPosition().z))
							{
								// 一時的保存処理
								pUi->GetBonus()->GetCount()->SaveBonus(pUi->GetBonus()->GetCount()->GetNumber());
								m_posSave = D3DXVECTOR3(pModel->GetPosition().x, m_posSave.y, pModel->GetPosition().z);
								m_pSound->Play(CSound::SOUND_LABEL_SE_SAVEPOINT);
								pCheckPointLogo->MoveTypeNext();
							}
							break;

						case CObject::OBJATTRIBUTE_START:	// スタート地点
															// メインスタートポイントの取得とリスポーン地点の設定
							m_posSave = D3DXVECTOR3(
								pObject->GetModel()->GetPosition().x,
								m_posSave.y,
								pObject->GetModel()->GetPosition().z - 200.0f);

							pStartCntdown->SetStartCntdown(true);

							break;

						case CObject::OBJATTRIBUTE_GOAL:	// ゴール地点
															// リザルトへ
							if (m_bSetGameClear == false)
							{
								m_bSetGameClear = true;
								m_pSound->Stop();
								m_pSound->Play(CSound::SOUND_LABEL_SE_GOAL);
							}

							CGame::SetGameEnd(true);

							break;
						}

						if (pObject->GetObjAttribute() == CObject::OBJATTRIBUTE_SCAFFORD || pObject->GetObjAttribute() == CObject::OBJATTRIBUTE_SCAFFORD_TUTO)
						{
							pObject->SetDropOut(true);
						}
					}
				}
			}

			pScene = pSceneNext;
		}

		for (int nCntMesh = 0; nCntMesh < 1; nCntMesh++)
		{
			if (pCollision->CollisionMesh(this, CGame::GetMeshField(nCntMesh)) == true)
			{
				m_bLandMesh = true;
				break;
			}
			else
			{
				m_bLandMesh = false;
			}
		}
	}

	if (m_bLandMesh == true || m_bLandObj == true)
	{
		m_bLanding = true;
	}
	else
	{
		m_bLanding = false;
	}


	// 着地判定
	if (m_bLanding == true)
	{
		m_nCntSandEffect++;
	}
	else
	{
		m_nCntSandEffect = 0;
	}
}

//==============================================================================
// プレイヤーの復活
//==============================================================================
void CPlayer::PlayerReset(void)
{
	switch (m_resetPhase)
	{
	case RESETPHASE_NONE:	// 実行待ち

		//m_resetPhase = RESETPHASE_WAIT;	// 待機状態へ移行
		
		break;

	case RESETPHASE_WAIT:	// 一時待機

		m_nCntReset++;

		if (m_bIntoLava == true && m_nCntReset % 6 == 0)
		{
			CEffectDetail::Create(CEffectDetail::EFFECTTYPE_DIVE_LAVA, m_posLava);
		}

		if (m_nCntReset >= CNT_ONE_SECOND)
		{
			m_resetPhase = RESETPHASE_ON;
		}

		break;

	case RESETPHASE_ON:	// リセット実行
		
		m_move = VECTOR3_NULL;
		m_pos = m_posSave;
		m_nDashCnt = 0;
		m_nCntReset = 0;
		m_resetPhase = RESETPHASE_NONE;

		if (m_bIntoLava == true)
		{
			m_bIntoLava = false;
		}

		// 足場の復活
		CScene *pScene = CScene::GetScene(PRIORITY_DEFAULT);
		CScene *pSceneNext = NULL;

		while (pScene)
		{
			pSceneNext = CScene::GetSceneNext(pScene);

			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{
				CObject *pObject = (CObject*)pScene;

				pObject->ResetObject();
			}

			pScene = pSceneNext;
		}
		break;
	}
}

//==============================================================================
// クリア時のプレイヤーモーション
//==============================================================================
void CPlayer::ClearMotion(void)
{
	// 移動力と向きの補正
	m_move = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;

	// クリア判定
	CResult::SetEndState(CResult::ENDSTATE_CLEAR);
}
