//==============================================================================
//
// カメラの設定〔camera.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "camera.h"
#include "game.h"
#include "gamepad.h"
#include "input.h"
#include "manager.h"
#include "player.h"
#include "renderer.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CCamera::CCamera()
{

}

//==============================================================================
// ディストラクタ
//==============================================================================
CCamera::~CCamera()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
void CCamera::Init(void)
{
	SetInit();
}

//==============================================================================
// 終了処理
//==============================================================================
void CCamera::Uninit(void)
{

}

//==============================================================================
// 更新処理
//==============================================================================
void CCamera::Update(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	//視点の移動(左右)(Z,Cキー)
	if (keyboard->GetPress(CInput::KEYINFO_TURN_LEFT) == true)
	{
		m_rot.y += CAMERA_TURN;

		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}

		m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;
		m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLength;
	}
	if (keyboard->GetPress(CInput::KEYINFO_TURN_RIGHT) == true)
	{
		m_rot.y -= CAMERA_TURN;

		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;
		m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLength;
	}

	if (keyboard->GetPress(CInput::KEYINFO_CAMERA_UP) == true)
	{
		m_fHeightV += 2.0f;
	}

	if (keyboard->GetPress(CInput::KEYINFO_CAMERA_DOWN) == true)
	{
		m_fHeightV -= 2.0f;
	}

	// プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	D3DXVECTOR3 posPlayer = VECTOR3_NULL;
	D3DXVECTOR3 rotDestPlayer = VECTOR3_NULL;
	
	if (pPlayer != NULL)
	{
		posPlayer = pPlayer->GetPosition();
		rotDestPlayer = pPlayer->GetRotDest();
		if (pPlayer->GetResetPhase() != CPlayer::RESETPHASE_WAIT)
		{
			// 目的の注視点
			m_posRDest.x = posPlayer.x - m_fFront * sinf(rotDestPlayer.y);
			m_posRDest.z = posPlayer.z - m_fFront * cosf(rotDestPlayer.y);
			m_posRDest.y = posPlayer.y + m_fHeightR;

			// 目的の視点
			m_posVDest.x = posPlayer.x - sinf(m_rot.y) * m_fLength;
			m_posVDest.z = posPlayer.z - cosf(m_rot.y) * m_fLength;
			m_posVDest.y = posPlayer.y + cosf(m_rot.x) * (m_fLength + m_fHeightV);

			// 注視点の位置更新
			m_posR.x += (m_posRDest.x - m_posR.x) * CAMERA_SPDOWN;
			m_posR.z += (m_posRDest.z - m_posR.z) * CAMERA_SPDOWN;
			m_posR.y += (m_posRDest.y - m_posR.y) * CAMERA_SPDOWN;

			// 視点の位置更新
			m_posV.x += (m_posVDest.x - m_posV.x) * CAMERA_SPDOWN;
			m_posV.z += (m_posVDest.z - m_posV.z) * CAMERA_SPDOWN;
			m_posV.y += (m_posVDest.y - m_posV.y) * CAMERA_SPDOWN;
		}
	}

	// カメラの設定
	if (CGame::GetClearPhase() == CGame::CLEARPHASE_PAUSE)
	{
		GameClear();
	}


}

//==============================================================================
// カメラの生成
//==============================================================================
CCamera *CCamera::Create(void)
{
	// ローカル変数宣言
	CCamera *pCamera;
	pCamera = new CCamera;

	// 初期化
	if (pCamera != NULL)
	{
		pCamera->Init();
	}

	return pCamera;
}

//==============================================================================
// カメラの設定処理
//==============================================================================
void CCamera::SetCamera(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(
		&m_mtxProjection,
		D3DXToRadian(45.0f),							// 画角の設定
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// 画面比率の設定
		CAMERA_LIMIT_MIN,								// 手前限界の描画距離
		CAMERA_LIMIT_MAX);								// 奥限界の描画距離

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(
		&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//==============================================================================
// カメラの位置初期化
//==============================================================================
void CCamera::SetInit(void)
{
	// 変数設定
	m_posV = VECTOR3_NULL;						// カメラ視点の位置
	m_posR = VECTOR3_NULL;						// 注視点位置
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
	m_rot = VECTOR3_NULL;						// カメラの角度
	m_fLength = CAMERA_LENGTH;					// 視点・注視点の距離
	m_fFront = CAMERA_FORNT;					// プレイヤーの前方
	m_fHeightR = CAMERA_HEIGHT_R;				// 注視点の高さ
	m_fHeightV = CAMERA_HEIGHT_V;				// 視点の高さ
}

//==============================================================================
// カメラのクリア時配置
//==============================================================================
void CCamera::GameClear(void)
{
	// クリア時の配置
	m_rot = RESET_ROTATION;
	m_fHeightR = RESET_HEIGHT_R;
	m_fLength = RESET_LENGTH;
	m_fFront = RESET_FRONT;
}