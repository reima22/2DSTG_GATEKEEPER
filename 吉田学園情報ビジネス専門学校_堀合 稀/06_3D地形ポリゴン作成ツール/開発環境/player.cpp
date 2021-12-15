//==============================================================================
//
// プレイヤー管理〔player.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "input.h"
#include "gamepad.h"
#include "textdata_player.h"
#include "camera.h"
#include "collision.h"
#include "shadowS.h"
#include "motion.h"
#include "motion_player.h"
#include "exe.h"

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

	// 過去の位置
	m_posOld = VECTOR3_NULL;

	// オブジェクトタイプ
	SetObjType(OBJTYPE_PLAYER);

	// モーション情報の生成
	m_pMotionPlayer = CMotionPlayer::Create();

	// 影の設定
	m_pShadowS = CShadowS::Create(m_pos);

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
}

//==============================================================================
// プレイヤーの更新処理
//==============================================================================
void CPlayer::Update(void)
{
	// テキストデータの取得
	CTextDataPlayer *pDataPlayer = CTextDataPlayer::GetDataPlayer();

	// 移動処理
	MovePlayer();

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

	if (m_pos.y <= -1000.0f)
	{
		m_pos = pDataPlayer->GetPosition();
		m_move.y = 0.0f;
	}

	// 位置の更新
	SetPosition(m_pos);
	SetRotation(m_rot);
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
void CPlayer::MovePlayer(void)
{
	// テキストデータの取得
	CTextDataPlayer *pDataPlayer = CTextDataPlayer::GetDataPlayer();

	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	// カメラの方向
	D3DXVECTOR3 rotCamera = pCamera->GetRotation();

	// 他オブジェクトとの当たり判定
	CCollision *pCollision;
	pCollision = CManager::GetCollision(); // 当たり判定取得

	if (pCollision != NULL)
	{
		if (pCollision->CollisionMesh(this, CExe::GetMeshField()) == true)
		{
			m_bLand = true;
		}
		//else
		//{
		//	m_bLand = false;
		//}
	}

	//if (m_pos.y <= 0.0f)
	//{
	//	m_pos.y = 0.0f;
	//	m_bLand = true;
	//}

	// 過去位置の更新
	m_posOld = m_pos;

	// 重力の発生
	if (m_bLand == false)
	{
		m_move.y -= GRAVITY_SCALE;
	}
	else if (m_bLand == true)
	{// 着地状態
		if (m_bJump == true)
		{// ジャンプから着地
			if (m_pMotionPlayer->GetMotion() == CMotionPlayer::MOTIONTYPE_JUMP || m_pMotionPlayer->GetMotion() == CMotionPlayer::MOTIONTYPE_RUN)
			{
				m_pMotionPlayer->SetMotion(CMotionPlayer::MOTIONTYPE_LANDING);
			}
		}

		m_bJump = false;
		m_move.y = 0.0f;
	}

	// ジャンプ：着地している
	if (keyboard->GetTrigger(CInput::KEYINFO_JUMP) == true ||
		gamepad->GetTrigger(CInput::KEYINFO_OK) == true)
	{
		// 着地しているとき
		if (m_bJump == false && m_bLand == true)
		{
			m_pMotionPlayer->SetMotion(CMotionPlayer::MOTIONTYPE_JUMP);		// ジャンプモーション切り替え
			m_move.y = 0.5f;												// 初期ジャンプ力の設定

			// ジャンプ判定
			m_bJumpAir = true;
		}

		// 着地判定の解除
		m_bLand = false;


	}

	if (keyboard->GetPress(CInput::KEYINFO_JUMP) == true ||
		gamepad->GetPress(CInput::KEYINFO_OK) == true)
	{
		// 滞空状態
		if (m_bJump == false && m_bLand == false && m_bJumpAir == true)
		{
			if (m_nJumpCnt == 0)
			{
				m_move.y = 5.0f;
			}
			if (m_nJumpCnt < 9)
			{// 限界値
				m_nJumpCnt++;
				m_move.y += 1.0f;
			}
			else
			{
				m_bJump = true;
			}
		}
	}
	if (keyboard->GetRelease(CInput::KEYINFO_JUMP) == true ||
		gamepad->GetRelease(CInput::KEYINFO_OK) == true)
	{
		m_nJumpCnt = 0;
		m_bJump = true;
		m_bJumpAir = false;
	}

	// 移動
	m_pos += m_move;

	// 移動処理	
	//if (keyboard->GetPress(CInput::KEYINFO_WALK_UP) == true ||
	//	keyboard->GetPress(CInput::KEYINFO_WALK_DOWN) == true ||
	//	keyboard->GetPress(CInput::KEYINFO_WALK_LEFT) == true ||
	//	keyboard->GetPress(CInput::KEYINFO_WALK_RIGHT) == true ||
	//	gamepad->GetPress(CInput::KEYINFO_WALK_UP) == true ||
	//	gamepad->GetPress(CInput::KEYINFO_WALK_DOWN) == true ||
	//	gamepad->GetPress(CInput::KEYINFO_WALK_LEFT) == true ||
	//	gamepad->GetPress(CInput::KEYINFO_WALK_RIGHT) == true)
	//{// 歩く
	//	if (keyboard->GetPress(CInput::KEYINFO_WALK_UP) == true ||
	//		gamepad->GetPress(CInput::KEYINFO_WALK_UP) == true)
	//	{// 上方向
	//		if (keyboard->GetPress(CInput::KEYINFO_WALK_LEFT) == true ||
	//			gamepad->GetPress(CInput::KEYINFO_WALK_LEFT) == true)
	//		{// 左上方向
	//			m_move.x -= cosf(rotCamera.y + D3DX_PI / 4) * WALK_SPEED;
	//			m_move.z += sinf(rotCamera.y + D3DX_PI / 4) * WALK_SPEED;
	//			m_rotDest.y = rotCamera.y + (D3DX_PI * 3 / 4);
	//		}
	//		else if (keyboard->GetPress(CInput::KEYINFO_WALK_RIGHT) == true ||
	//			gamepad->GetPress(CInput::KEYINFO_WALK_RIGHT) == true)
	//		{// 右上方向
	//			m_move.x += cosf(rotCamera.y - D3DX_PI / 4) * WALK_SPEED;
	//			m_move.z -= sinf(rotCamera.y - D3DX_PI / 4) * WALK_SPEED;
	//			m_rotDest.y = rotCamera.y - (D3DX_PI * 3 / 4);
	//		}
	//		else
	//		{// 上方向
	//			m_move.x += sinf(rotCamera.y) * WALK_SPEED;
	//			m_move.z += cosf(rotCamera.y) * WALK_SPEED;
	//			m_rotDest.y = rotCamera.y + D3DX_PI;
	//		}
	//	}
	//	else if (keyboard->GetPress(CInput::KEYINFO_WALK_DOWN) == true ||
	//		gamepad->GetPress(CInput::KEYINFO_WALK_DOWN) == true)
	//	{
	//		if (keyboard->GetPress(CInput::KEYINFO_WALK_LEFT) == true ||
	//			gamepad->GetPress(CInput::KEYINFO_WALK_LEFT) == true)
	//		{// 左下方向
	//			m_move.x += cosf(rotCamera.y + D3DX_PI * 3 / 4) * WALK_SPEED;
	//			m_move.z -= sinf(rotCamera.y + D3DX_PI * 3 / 4) * WALK_SPEED;
	//			m_rotDest.y = rotCamera.y + D3DX_PI / 4;
	//		}
	//		else if (keyboard->GetPress(CInput::KEYINFO_WALK_RIGHT) == true ||
	//			gamepad->GetPress(CInput::KEYINFO_WALK_RIGHT) == true)
	//		{// 右下方向
	//			m_move.x -= cosf(rotCamera.y - D3DX_PI * 3 / 4) * WALK_SPEED;
	//			m_move.z += sinf(rotCamera.y - D3DX_PI * 3 / 4) * WALK_SPEED;
	//			m_rotDest.y = rotCamera.y + D3DX_PI / -4;
	//		}
	//		else
	//		{// 下方向
	//			m_move.x += sinf(rotCamera.y - D3DX_PI) * WALK_SPEED;
	//			m_move.z += cosf(rotCamera.y - D3DX_PI) * WALK_SPEED;
	//			m_rotDest.y = rotCamera.y;
	//		}
	//	}
	//	else if (keyboard->GetPress(CInput::KEYINFO_WALK_LEFT) == true ||
	//		gamepad->GetPress(CInput::KEYINFO_WALK_LEFT) == true)
	//	{
	//		m_move.x -= cosf(rotCamera.y) * WALK_SPEED;
	//		m_move.z += sinf(rotCamera.y) * WALK_SPEED;
	//		m_rotDest.y = rotCamera.y + (D3DX_PI / 2);
	//	}
	//	else if (keyboard->GetPress(CInput::KEYINFO_WALK_RIGHT) == true ||
	//		gamepad->GetPress(CInput::KEYINFO_WALK_RIGHT) == true)
	//	{
	//		m_move.x += cosf(rotCamera.y) * WALK_SPEED;
	//		m_move.z -= sinf(rotCamera.y) * WALK_SPEED;
	//		m_rotDest.y = rotCamera.y + (D3DX_PI / -2);
	//	}
	//}

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
		if (m_nDashCnt > 0)
		{
			// 無操作間の減少
			m_nDashCnt -= 5;
		}
	}

	// ダッシュによる加速
	if (m_nDashCnt < SPEED_RANK_CNT)
	{
		m_fDashPower = 1.0f;
	}
	else if (m_nDashCnt <= 0)
	{
		m_fDashPower = 0.0f;
	}
	else if (m_nDashCnt < SPEED_RANK_CNT * 2)
	{
		m_fDashPower = 1.5f;
	}
	else
	{
		m_fDashPower = 2.0f;
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

	// 加速後の減速処理
	m_move.x += (0.0f - m_move.x) * SPEED_DOWN;
	m_move.z += (0.0f - m_move.z) * SPEED_DOWN;

	// モーションの切り替え
	if (m_move.x > MOVE_MOTION || m_move.z > MOVE_MOTION || m_move.x < -MOVE_MOTION || m_move.z < -MOVE_MOTION)
	{// 移動力でモーションを維持
		if (m_bLand == true)
		{// 地上にいる時
			//if (m_move.x > RUN_MOTION || m_move.z > RUN_MOTION || m_move.x < -RUN_MOTION || m_move.z < -RUN_MOTION)
			{
				// 走るモーション
				m_pMotionPlayer->SetMotion(CMotionPlayer::MOTIONTYPE_RUN);
			}
			//else
			//{
			//	// 歩くモーション
			//	m_pMotionPlayer->SetMotion(CMotionPlayer::MOTIONTYPE_WALK);
			//}		
		}		
	}
	else if(m_pMotionPlayer->GetMotion() == CMotionPlayer::MOTIONTYPE_RUN/* || m_pMotionPlayer->GetMotion() == CMotionPlayer::MOTIONTYPE_WALK*/)
	{
		// ニュートラルへ戻る
		m_pMotionPlayer->SetMotion(CMotionPlayer::MOTIONTYPE_NEUTRAL);
	}
}