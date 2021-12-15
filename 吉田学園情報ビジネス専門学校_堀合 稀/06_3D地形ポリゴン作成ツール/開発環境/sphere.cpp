//==============================================================================
//
// ���b�V���t�B�[���h�̕`��kmeshfield.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
//#include "meshfield.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "textdata.h"
//#include "textdata_meshfield.h"
#include "texture.h"
#include "sphere.h"
#include "exe.h"
#include "collision.h"
#include "camera.h"

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CSphere::CSphere()
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CSphere::~CSphere()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CSphere::Init(void)
{
	// ���[�J���ϐ��錾
	//VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();


	m_fRadius = 5.0f;
	m_nSlice = 10;
	m_nStack = 10;

	// ���̂̐���
	D3DXCreateSphere(
		pDevice,
		m_fRadius,
		m_nSlice,
		m_nStack,
		&m_pMeshSphere,
		&m_pBuffMatSphere);

	m_pos.y += m_fRadius / 2.0f;

	SetLanding(true);

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CSphere::Uninit(void)
{

}

//==============================================================================
// �X�V����
//==============================================================================
void CSphere::Update(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// �J�����̎擾
	CCamera *pCamera = CManager::GetCamera();

	// �J�����̕���
	D3DXVECTOR3 rotCamera = pCamera->GetRotation();

	// ���I�u�W�F�N�g�Ƃ̓����蔻��
	CCollision *pCollision;
	pCollision = CManager::GetCollision(); // �����蔻��擾

	if (pCollision != NULL)
	{
		if (pCollision->CollisionMesh(this, CExe::GetMeshField()) == true)
		{
			m_bLand = true;
		}
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CSphere::Draw(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRotAttack, mtxTransAttack;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATRIX mtxParent;						// �e�̃}�g���b�N�X
	D3DXMATERIAL *pMat;
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	//D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �e�p�[�c�̈ʒu�𔽉f
	D3DXMatrixTranslation(
		&mtxTransAttack,
		m_pos.x,
		m_pos.y,
		m_pos.z);

	D3DXMatrixMultiply(
		&m_mtxWorld,
		&m_mtxWorld,
		&mtxTransAttack);

	// ����̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMatSphere->GetBufferPointer();

	pMat[0].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// �}�e���A���̐ݒ�
	pDevice->SetMaterial(&pMat[0].MatD3D);

	// ���̂̕`��
	m_pMeshSphere->DrawSubset(0);

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//==============================================================================
// �t�B�[���h�̐���
//==============================================================================
CSphere *CSphere::Create(void)
{
	// ���[�J���ϐ��錾
	CSphere *pSphere;
	pSphere = new CSphere;

	if (pSphere != NULL)
	{
		// ������
		pSphere->Init();
	}

	return pSphere;
}