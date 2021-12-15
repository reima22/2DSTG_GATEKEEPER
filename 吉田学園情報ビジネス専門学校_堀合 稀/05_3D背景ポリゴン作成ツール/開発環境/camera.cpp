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
	// �L�[�{�[�h�̎擾
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// �Y�[���C���A�E�g
	if (keyboard->GetPress(CInput::KEYINFO_CAMERA_ZOOMIN) == true)
	{
		m_fLength -= 1.0f;
	}
	if (keyboard->GetPress(CInput::KEYINFO_CAMERA_ZOOMOUT) == true)
	{
		m_fLength += 1.0f;
	}

	// �ړ��͂̉��Z
	m_posR += m_move;
	m_posV += m_move;

	//���_�̈ړ�(���E)(Z,C�L�[)
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

	m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;
	m_posV.y = m_posR.y + cosf(m_rot.x) * m_fLength;
	m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLength;

	// �J�����̈ړ�
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
	
	// ��]�p�̕␳
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	// ������̌�������
	m_move.x += (0.0f - m_move.x) * SPEED_DOWN;
	m_move.z += (0.0f - m_move.z) * SPEED_DOWN;
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
		CAMERA_LIMIT_MIN,								// ��O���E�̕`�拗��
		CAMERA_LIMIT_MAX);								// �����E�̕`�拗��

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
	m_posV = D3DXVECTOR3(0.0f, 200.0f, -400.0f);						// �J�������_�̈ʒu
	m_posR = VECTOR3_NULL;						// �����_�ʒu
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ������x�N�g��
	m_rot = VECTOR3_NULL;						// �J�����̊p�x
	m_fLength = CAMERA_LENGTH;					// ���_�E�����_�̋���
	m_fFront = CAMERA_FORNT;					// �v���C���[�̑O��
	m_fHeightR = CAMERA_HEIGHT_R;				// �����_�̍���
	m_fHeightV = CAMERA_HEIGHT_V;				// ���_�̍���
}