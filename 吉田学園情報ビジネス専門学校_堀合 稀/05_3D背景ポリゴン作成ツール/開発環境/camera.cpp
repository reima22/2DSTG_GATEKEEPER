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
	m_posV = D3DXVECTOR3(0.0f, 200.0f, -400.0f);						// カメラ視点の位置
	m_posR = VECTOR3_NULL;						// 注視点位置
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
	m_rot = VECTOR3_NULL;						// カメラの角度
	m_fLength = CAMERA_LENGTH;					// 視点・注視点の距離

	m_fHeightR = CAMERA_HEIGHT_R;				// 注視点の高さ
	m_fHeightV = CAMERA_HEIGHT_V;				// 視点の高さ

	m_rot.x = D3DX_PI / 4.0f;
	m_rot.y = 0.0f;
	m_rot.z = 0.0f;
}

//==============================================================================
// カメラの操作
//==============================================================================
void CCamera::ControlCamera(void)
{
	// マウスの取得
	CInputMouse *pMouse = CManager::GetInputMouse();

	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ズームインアウト
	if (keyboard->GetPress(CInput::KEYINFO_CAMERA_ZOOMIN) == true)
	{
		m_fLength -= 1.0f;
	}
	if (keyboard->GetPress(CInput::KEYINFO_CAMERA_ZOOMOUT) == true)
	{
		m_fLength += 1.0f;
	}

	// 移動力の加算
	m_posR += m_move;
	m_posV += m_move;

	//視点の移動(左右)(Z,Cキー)
	if (keyboard->GetPress(CInput::KEYINFO_TURN_LEFT) == true)
	{
		m_rot.y += CAMERA_TURN;
	}
	if (keyboard->GetPress(CInput::KEYINFO_TURN_RIGHT) == true)
	{
		m_rot.y -= CAMERA_TURN;
	}
	//if (keyboard->GetPress(CInput::KEYINFO_TURN_UP) == true)
	//{
	//	m_rot.x -= CAMERA_TURN;
	//	m_rot.z -= CAMERA_TURN;
	//}
	//if (keyboard->GetPress(CInput::KEYINFO_TURN_DOWN) == true)
	//{
	//	m_rot.y -= CAMERA_TURN;
	//}

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

	//// 視点間同士ベクトル
	//m_vecInterval = m_posV - m_posR;

	//// 注視点基準の視点の高さ
	//m_fHeightV = m_vecInterval.y;

	//// 注視点
	//m_fHeightR = m_posR.y;

	// 地上の距離
	m_fGroundLength = cosf(m_rot.x) * m_fLength;

	// 角度を元に計算
	m_posV.y = m_posR.y + sinf(m_rot.x) * m_fLength;
	m_posV.x = m_posR.x - sinf(m_rot.y) * m_fGroundLength;
	m_posV.z = m_posR.z - cosf(m_rot.y) * m_fGroundLength;

	//m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;
	//m_posV.y = m_posR.y + cosf(m_rot.x) * m_fLength;
	//m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLength;

	// カメラの移動
	if (keyboard->GetPress(CInput::KEYINFO_UP) == true)
	{
		m_move.x += sinf(m_rot.y) * SPEED_UP;
		m_move.z += cosf(m_rot.y) * SPEED_UP;
	}
	if (keyboard->GetPress(CInput::KEYINFO_DOWN) == true)
	{
		m_move.x += sinf(m_rot.y - D3DX_PI) * SPEED_UP;
		m_move.z += cosf(m_rot.y - D3DX_PI) * SPEED_UP;
	}
	if (keyboard->GetPress(CInput::KEYINFO_LEFT) == true)
	{
		m_move.x -= cosf(m_rot.y) * SPEED_UP;
		m_move.z += sinf(m_rot.y) * SPEED_UP;
	}
	if (keyboard->GetPress(CInput::KEYINFO_RIGHT) == true)
	{
		m_move.x += cosf(m_rot.y) * SPEED_UP;
		m_move.z -= sinf(m_rot.y) * SPEED_UP;
	}

	if (keyboard->GetPress(CInput::KEYINFO_CAMERA_UP) == true)
	{
		m_posV.y += CAMERA_MOVE;
		m_posR.y += CAMERA_MOVE;
	}
	if (keyboard->GetPress(CInput::KEYINFO_CAMERA_DOWN) == true)
	{
		m_posV.y -= CAMERA_MOVE;
		m_posR.y -= CAMERA_MOVE;
	}

	// 回転角の補正
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	// 加速後の減速処理
	m_move.x += (0.0f - m_move.x) * SPEED_DOWN;
	m_move.z += (0.0f - m_move.z) * SPEED_DOWN;
}