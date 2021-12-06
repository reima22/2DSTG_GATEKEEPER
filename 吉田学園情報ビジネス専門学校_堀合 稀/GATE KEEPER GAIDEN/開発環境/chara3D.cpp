//==============================================================================
//
// 3D���f��X�`�揈���ksceneX.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene3D.h"
#include "renderer.h"
#include "manager.h"
#include "chara3D.h"
#include "texture.h"
//#include "model.h"

// �ÓI�����o�ϐ��錾
char *CChara3D::m_pFileName[CHARA3DTYPE_MAX] =
{
	{ "data/MODEL/meteor.x" },		// 覐�
	{ "data/MODEL/debri00.x" },		// �F���S�~0
};

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CChara3D::CChara3D(int nPriority) : CScene3D(nPriority)
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CChara3D::~CChara3D()
{

}

//==============================================================================
// 3D���f���`��̏���������
//==============================================================================
HRESULT CChara3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���|�C���^�̎擾
	CTexture *pTexture;
	pTexture = CManager::GetTexture();

	// �����ݒ�
	m_pos = pos;
	m_move = move;
	//m_rotMove = rotMove;
	m_type = (CHARA3DTYPE)nType;

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		m_pFileName[m_type],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	// �e�N�X�`���|�C���^�z��̓��I�m��(�}�e���A���̐�����)
	m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

	D3DXMATERIAL *pMat;		// �}�e���A���ւ̃|�C���^

	// �o�b�t�@�|�C���^�̎擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// �e�N�X�`������
		 // �e�N�X�`�����̊��蓖��
			m_pTexture[nCntMat] = pTexture->GetAddress3D(pMat[nCntMat].pTextureFilename);
		}
		else
		{// �e�N�X�`���̊��蓖�Ă��Ȃ�
			m_pTexture[nCntMat] = NULL;
		}
	}

	return S_OK;
}

//==============================================================================
// 3D���f���`��̏I������
//==============================================================================
void CChara3D::Uninit(void)
{
	// ���b�V���|�C���^�̔j��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// �}�e���A���o�b�t�@�̔j��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		delete[] m_pTexture;
		m_pTexture = NULL;
	}

	Release();
}

//==============================================================================
// 3D���f���`��̍X�V����
//==============================================================================
void CChara3D::Update(void)
{
	// �����v�Z
	m_pos += m_move;
	m_rot += m_rotMove;

	// ��]�p�x�␳
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= PI_RESET;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += PI_RESET;
	}

	if (m_rot.x >= D3DX_PI)
	{
		m_rot.x -= PI_RESET;
	}
	else if (m_rot.x <= -D3DX_PI)
	{
		m_rot.x += PI_RESET;
	}

	CScene3D::SetPos(m_pos);
}

//==============================================================================
// 3D���f���`��̕`�揈��
//==============================================================================
void CChara3D::Draw(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;										// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;												// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;													// �}�e���A���f�[�^�ւ̃|�C���^

	// Z�e�X�g�̍X�V
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

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

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		if (m_pTexture[nCntMat] != NULL)
		{
			pDevice->SetTexture(0, m_pTexture[nCntMat]);
		}
		else
		{
			pDevice->SetTexture(0, NULL);
		}

		// ���f��(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	// ���ɖ߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//==============================================================================
// ��������
//==============================================================================
CChara3D *CChara3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType)
{
	// ���[�J���ϐ��錾
	CChara3D *pChara3D;
	pChara3D = new CChara3D;	// �C���X�^���X����

	// ������
	if (pChara3D != NULL)
	{
		pChara3D->Init(pos, move, nType);
	}

	return pChara3D;
}