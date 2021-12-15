//==============================================================================
//
// カメラの設定〔camera.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "gamepad.h"
#include "player.h"
#include "exe.h"

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
	// カメラ操作
	ControlCamera();

	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CExe::GetPlayer();

	D3DXVECTOR3 posPlayer = VECTOR3_NULL;
	D3DXVECTOR3 rotDestPlayer = VECTOR3_NULL;
	
	if (pPlayer != NULL)
	{
		posPlayer = pPlayer->GetPosition();
		rotDestPlayer = pPlayer->GetRotDest();
	}

	// 視点間同士ベクトル
	m_vecInterval = m_posV - m_posR;

	// 編集モード切替
	if (CExe::GetEditMode() == CExe::EDITMODE_EDIT)
	{
		if (keyboard->GetTrigger(CInput::KEYINFO_MODECHANGE) == true)
		{
			SetInit();
		}	
	}
	else if (CExe::GetEditMode() == CExe::EDITMODE_PREVIEW)
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
		10.0f,											// 手前限界の描画距離
		1500.0f);										// 奥限界の描画距離

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
	m_posV = D3DXVECTOR3(0.0f, 300.0f, -700.0f);	// カメラ視点の位置
	m_posR = VECTOR3_NULL;							// 注視点位置

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 上方向ベクトル

	m_vecInterval = m_posV - m_posR;

	m_fHeightR = m_posR.y;							// 注視点の高さ
	m_fHeightV = m_vecInterval.y;					// 視点の高さ

	m_rot.x = D3DX_PI / 4.0f;
	m_rot.y = 0.0f;
	m_rot.z = 0.0f;

	// 地上の距離
	m_fGroundLength = sqrtf(m_vecInterval.x * m_vecInterval.x + m_vecInterval.z * m_vecInterval.z);

	m_fLength = 700.0f;

	m_fFront = 15.0f;								// プレイヤーの前方
}

//==============================================================================
// カメラ操作
//==============================================================================
void CCamera::ControlCamera(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// マウスの取得
	CInputMouse *pMouse = CManager::GetInputMouse();

	// 実行クラス
	CExe *pExe = CManager::GetExe();

	if (pExe->GetEditMode() == CExe::EDITMODE_EDIT)
	{
		if (pMouse->GetPress(pMouse->GetButton(CInputMouse::MOUSEINFO_LEFT)) == true)
		{
			// カメラの移動
			if (pMouse->GetMouselY() < 0.0f)
			{
				m_posV.x += sinf(m_rot.y) * pMouse->GetMouselY();
				m_posR.x += sinf(m_rot.y) * pMouse->GetMouselY();
				m_posV.z += cosf(m_rot.y) * pMouse->GetMouselY();
				m_posR.z += cosf(m_rot.y) * pMouse->GetMouselY();
			}
			if (pMouse->GetMouselY() > 0.0f)
			{
				m_posV.x += sinf(m_rot.y) * pMouse->GetMouselY();
				m_posR.x += sinf(m_rot.y) * pMouse->GetMouselY();
				m_posV.z += cosf(m_rot.y) * pMouse->GetMouselY();
				m_posR.z += cosf(m_rot.y) * pMouse->GetMouselY();
			}
			if (pMouse->GetMouselX() < 0.0f)
			{
				m_posV.x -= cosf(m_rot.y) * pMouse->GetMouselX();
				m_posR.x -= cosf(m_rot.y) * pMouse->GetMouselX();
				m_posV.z += sinf(m_rot.y) * pMouse->GetMouselX();
				m_posR.z += sinf(m_rot.y) * pMouse->GetMouselX();
			}
			if (pMouse->GetMouselX() > 0.0f)
			{
				m_posV.x -= cosf(m_rot.y) * pMouse->GetMouselX();
				m_posR.x -= cosf(m_rot.y) * pMouse->GetMouselX();
				m_posV.z += sinf(m_rot.y) * pMouse->GetMouselX();
				m_posR.z += sinf(m_rot.y) * pMouse->GetMouselX();
			}
		}	
			
		// カメラの移動
		//if (keyboard->GetPress(CInput::KEYINFO_CAMERA_FRONT) == true)
		//{
		//	m_posV.x += sinf(m_rot.y) * CAMERA_MOVE;
		//	m_posR.x += sinf(m_rot.y) * CAMERA_MOVE;
		//	m_posV.z += cosf(m_rot.y) * CAMERA_MOVE;
		//	m_posR.z += cosf(m_rot.y) * CAMERA_MOVE;
		//}
		//if (keyboard->GetPress(CInput::KEYINFO_CAMERA_BACK) == true)
		//{
		//	m_posV.x -= sinf(m_rot.y) * CAMERA_MOVE;
		//	m_posR.x -= sinf(m_rot.y) * CAMERA_MOVE;
		//	m_posV.z -= cosf(m_rot.y) * CAMERA_MOVE;
		//	m_posR.z -= cosf(m_rot.y) * CAMERA_MOVE;
		//}
		//if (keyboard->GetPress(CInput::KEYINFO_CAMERA_LEFT) == true)
		//{
		//	m_posV.x -= cosf(m_rot.y) * CAMERA_MOVE;
		//	m_posR.x -= cosf(m_rot.y) * CAMERA_MOVE;
		//	m_posV.z += sinf(m_rot.y) * CAMERA_MOVE;
		//	m_posR.z += sinf(m_rot.y) * CAMERA_MOVE;
		//}
		//if (keyboard->GetPress(CInput::KEYINFO_CAMERA_RIGHT) == true)
		//{
		//	m_posV.x += cosf(m_rot.y) * CAMERA_MOVE;
		//	m_posR.x += cosf(m_rot.y) * CAMERA_MOVE;
		//	m_posV.z -= sinf(m_rot.y) * CAMERA_MOVE;
		//	m_posR.z -= sinf(m_rot.y) * CAMERA_MOVE;
		//}

		if (keyboard->GetPress(CInput::KEYINFO_CAMERA_UP) == true)
		{
			m_posV.y += 2.0f;
		}

		if (keyboard->GetPress(CInput::KEYINFO_CAMERA_DOWN) == true)
		{
			m_posV.y -= 2.0f;
		}
	}
	else
	{
		if (keyboard->GetPress(CInput::KEYINFO_CAMERA_UP) == true)
		{
			m_fHeightV += 2.0f;
		}

		if (keyboard->GetPress(CInput::KEYINFO_CAMERA_DOWN) == true)
		{
			m_fHeightV -= 2.0f;
		}
	}

	if (keyboard->GetTrigger(CInput::KEYINFO_CAMERA_RESET) == true)
	{
		SetInit();
	}

	// マウスでのカメラ操作
	if (pMouse->GetPress(pMouse->GetButton(CInputMouse::MOUSEINFO_RIGHT)) == true)
	{
		m_rot.y += (float)pMouse->GetMouselX() / 100.0f;
		m_rot.x += (float)pMouse->GetMouselY() / 100.0f;
		//m_rot.z += (float)pMouse->GetMouselY() / 100.0f;
	}

	m_fLength -= (float)pMouse->GetMouselZ() / 5.0f;

	// 角度限界
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	if (m_rot.x >= D3DX_PI / 2.0f)
	{
		m_rot.x = D3DX_PI / 2.0f;
		m_vecU = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	if (m_rot.x <= 0.0f)
	{
		m_rot.x = 0.0f;
	}

	//if (m_rot.z >= D3DX_PI / 2.0f)
	//{
	//	m_rot.z = D3DX_PI / 2.0f;
	//	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	//}
	//else
	//{
	//	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//}
	//if (m_rot.z <= 0.0f)
	//{
	//	m_rot.z = 0.0f;
	//}

	// 視点間同士ベクトル
	m_vecInterval = m_posV - m_posR;

	// 注視点基準の視点の高さ
	m_fHeightV = m_vecInterval.y;

	// 注視点
	m_fHeightR = m_posR.y;
	
	// 地上の距離
	m_fGroundLength = cosf(m_rot.x) * m_fLength;

	// 角度を元に計算
	m_posV.y = m_posR.y + sinf(m_rot.x) * m_fLength;
	m_posV.x = m_posR.x - sinf(m_rot.y) * m_fGroundLength;
	m_posV.z = m_posR.z - cosf(m_rot.y) * m_fGroundLength;
}