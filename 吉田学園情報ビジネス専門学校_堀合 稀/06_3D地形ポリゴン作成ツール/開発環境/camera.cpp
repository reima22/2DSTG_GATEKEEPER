//==============================================================================
//
// �J�����̐ݒ�kcamera.cpp�l
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
// �R���X�g���N�^
//==============================================================================
CCamera::CCamera()
{

}

//==============================================================================
// �f�B�X�g���N�^
//==============================================================================
CCamera::~CCamera()
{

}

//==============================================================================
// ����������
//==============================================================================
void CCamera::Init(void)
{
	SetInit();
}

//==============================================================================
// �I������
//==============================================================================
void CCamera::Uninit(void)
{

}

//==============================================================================
// �X�V����
//==============================================================================
void CCamera::Update(void)
{
	// �J��������
	ControlCamera();

	// �L�[�{�[�h�̎擾
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// �v���C���[�̎擾
	CPlayer *pPlayer;
	pPlayer = CExe::GetPlayer();

	D3DXVECTOR3 posPlayer = VECTOR3_NULL;
	D3DXVECTOR3 rotDestPlayer = VECTOR3_NULL;
	
	if (pPlayer != NULL)
	{
		posPlayer = pPlayer->GetPosition();
		rotDestPlayer = pPlayer->GetRotDest();
	}

	// ���_�ԓ��m�x�N�g��
	m_vecInterval = m_posV - m_posR;

	// �ҏW���[�h�ؑ�
	if (CExe::GetEditMode() == CExe::EDITMODE_EDIT)
	{
		if (keyboard->GetTrigger(CInput::KEYINFO_MODECHANGE) == true)
		{
			SetInit();
		}	
	}
	else if (CExe::GetEditMode() == CExe::EDITMODE_PREVIEW)
	{
		// �ړI�̒����_
		m_posRDest.x = posPlayer.x - m_fFront * sinf(rotDestPlayer.y);
		m_posRDest.z = posPlayer.z - m_fFront * cosf(rotDestPlayer.y);
		m_posRDest.y = posPlayer.y + m_fHeightR;

		// �ړI�̎��_
		m_posVDest.x = posPlayer.x - sinf(m_rot.y) * m_fLength;
		m_posVDest.z = posPlayer.z - cosf(m_rot.y) * m_fLength;
		m_posVDest.y = posPlayer.y + cosf(m_rot.x) * (m_fLength + m_fHeightV);

		// �����_�̈ʒu�X�V
		m_posR.x += (m_posRDest.x - m_posR.x) * CAMERA_SPDOWN;
		m_posR.z += (m_posRDest.z - m_posR.z) * CAMERA_SPDOWN;
		m_posR.y += (m_posRDest.y - m_posR.y) * CAMERA_SPDOWN;

		// ���_�̈ʒu�X�V
		m_posV.x += (m_posVDest.x - m_posV.x) * CAMERA_SPDOWN;
		m_posV.z += (m_posVDest.z - m_posV.z) * CAMERA_SPDOWN;
		m_posV.y += (m_posVDest.y - m_posV.y) * CAMERA_SPDOWN;
	}	
}

//==============================================================================
// �J�����̐���
//==============================================================================
CCamera *CCamera::Create(void)
{
	// ���[�J���ϐ��錾
	CCamera *pCamera;
	pCamera = new CCamera;

	// ������
	if (pCamera != NULL)
	{
		pCamera->Init();
	}

	return pCamera;
}

//==============================================================================
// �J�����̐ݒ菈��
//==============================================================================
void CCamera::SetCamera(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(
		&m_mtxProjection,
		D3DXToRadian(45.0f),							// ��p�̐ݒ�
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// ��ʔ䗦�̐ݒ�
		10.0f,											// ��O���E�̕`�拗��
		1500.0f);										// �����E�̕`�拗��

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(
		&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//==============================================================================
// �J�����̈ʒu������
//==============================================================================
void CCamera::SetInit(void)
{
	// �ϐ��ݒ�
	m_posV = D3DXVECTOR3(0.0f, 300.0f, -700.0f);	// �J�������_�̈ʒu
	m_posR = VECTOR3_NULL;							// �����_�ʒu

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// ������x�N�g��

	m_vecInterval = m_posV - m_posR;

	m_fHeightR = m_posR.y;							// �����_�̍���
	m_fHeightV = m_vecInterval.y;					// ���_�̍���

	m_rot.x = D3DX_PI / 4.0f;
	m_rot.y = 0.0f;
	m_rot.z = 0.0f;

	// �n��̋���
	m_fGroundLength = sqrtf(m_vecInterval.x * m_vecInterval.x + m_vecInterval.z * m_vecInterval.z);

	m_fLength = 700.0f;

	m_fFront = 15.0f;								// �v���C���[�̑O��
}

//==============================================================================
// �J��������
//==============================================================================
void CCamera::ControlCamera(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�̎擾
	CGamepad *gamepad = CManager::GetInputGamepad();

	// �}�E�X�̎擾
	CInputMouse *pMouse = CManager::GetInputMouse();

	// ���s�N���X
	CExe *pExe = CManager::GetExe();

	if (pExe->GetEditMode() == CExe::EDITMODE_EDIT)
	{
		if (pMouse->GetPress(pMouse->GetButton(CInputMouse::MOUSEINFO_LEFT)) == true)
		{
			// �J�����̈ړ�
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
			
		// �J�����̈ړ�
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

	// �}�E�X�ł̃J��������
	if (pMouse->GetPress(pMouse->GetButton(CInputMouse::MOUSEINFO_RIGHT)) == true)
	{
		m_rot.y += (float)pMouse->GetMouselX() / 100.0f;
		m_rot.x += (float)pMouse->GetMouselY() / 100.0f;
		//m_rot.z += (float)pMouse->GetMouselY() / 100.0f;
	}

	m_fLength -= (float)pMouse->GetMouselZ() / 5.0f;

	// �p�x���E
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

	// ���_�ԓ��m�x�N�g��
	m_vecInterval = m_posV - m_posR;

	// �����_��̎��_�̍���
	m_fHeightV = m_vecInterval.y;

	// �����_
	m_fHeightR = m_posR.y;
	
	// �n��̋���
	m_fGroundLength = cosf(m_rot.x) * m_fLength;

	// �p�x�����Ɍv�Z
	m_posV.y = m_posR.y + sinf(m_rot.x) * m_fLength;
	m_posV.x = m_posR.x - sinf(m_rot.y) * m_fGroundLength;
	m_posV.z = m_posR.z - cosf(m_rot.y) * m_fGroundLength;
}