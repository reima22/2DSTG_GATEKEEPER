//==============================================================================
//
// ���f��X�`�揈���ksceneX.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene3D.h"
#include "renderer.h"
#include "manager.h"
#include "sceneX.h"
#include "texture.h"
#include "model.h"

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CSceneX::CSceneX(int nPriority) : CScene3D(nPriority)
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CSceneX::~CSceneX()
{

}

//==============================================================================
// �|���S���`��̏���������
//==============================================================================
HRESULT CSceneX::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rotMove,int nType)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �����ݒ�
	CScene3D::SetPos(pos);
	m_move = move;
	m_rotMove = rotMove;
	m_type = (X_TYPE)nType;

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		CModel::GetFileName(nType),
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	D3DXMATERIAL *pMat;		// �}�e���A���ւ̃|�C���^
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			m_pTexture = CTexture::BindTex3D(pMat[nCntMat].pTextureFilename);

			// �t�@�C�������g�p���ăe�N�X�`����ǂݍ���
			D3DXCreateTextureFromFile(
				pDevice,
				pMat[nCntMat].pTextureFilename,
				&m_pTexture);
		}
	}

	return S_OK;
}

//==============================================================================
// �|���S���`��̏I������
//==============================================================================
void CSceneX::Uninit(void)
{
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	CScene3D::Uninit();
}

//==============================================================================
// �|���S���`��̍X�V����
//==============================================================================
void CSceneX::Update(void)
{
	// �ʒu
	D3DXVECTOR3 pos = CScene3D::GetPosition();
	
	// �ړ�
	pos += m_move;

	// �ړ��̃��[�v
	if (pos.x < -300.0f)
	{
		pos.x = 300.0f;
	}
	if (pos.y < -200.0f)
	{
		pos.y = 200.0f;
	}

	// �ʒu�̔��f
	CScene3D::SetPos(pos);

	// ��]
	D3DXVECTOR3 rot = CScene3D::GetRot();

	rot += m_rotMove;

	// ��]�p�x�␳
	if (rot.y >= D3DX_PI)
	{
		rot.y -= D3DX_PI * 2;
	}
	else if (rot.y <= -D3DX_PI)
	{
		rot.y += D3DX_PI * 2;
	}

	if (rot.x >= D3DX_PI)
	{
		rot.x -= D3DX_PI * 2;
	}
	else if (rot.x <= -D3DX_PI)
	{
		rot.x += D3DX_PI * 2;
	}
	
	// �p�x�̔��f
	CScene3D::SetRot(rot);
}

//==============================================================================
// �|���S���`��̕`�揈��
//==============================================================================
void CSceneX::Draw(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							// �}�e���A���f�[�^�ւ̃|�C���^

	// �ʒu�ƌ���
	D3DXVECTOR3 pos = CScene3D::GetPosition();
	D3DXVECTOR3 rot = CScene3D::GetRot();

	// Z�e�X�g�̍X�V
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
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

		if (m_pTexture != NULL)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pTexture);
		}
		else
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);
		}

		// ���f��(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	// ���ɖ߂�
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//==============================================================================
// ��������
//==============================================================================
CSceneX *CSceneX::Create(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXVECTOR3 rotMove,int nType)
{
	CSceneX *pSceneX;
	pSceneX = new CSceneX;

	if (pSceneX != NULL)
	{
		pSceneX->Init(pos,move,rotMove,nType);
	}

	return pSceneX;
}