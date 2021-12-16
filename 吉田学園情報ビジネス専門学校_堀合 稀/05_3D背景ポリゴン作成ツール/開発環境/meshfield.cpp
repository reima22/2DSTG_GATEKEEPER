//==============================================================================
//
// ���b�V���t�B�[���h�̕`��kmeshfield.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "meshfield.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "textdata.h"
#include "texture.h"
#include "input.h"
#include "scene2D.h"

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CMeshfield::CMeshfield()
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CMeshfield::~CMeshfield()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CMeshfield::Init(void)
{
	// ���[�J���ϐ��錾
	VERTEX_3D_1 *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	WORD *pIdx;

	// �f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̎擾
	CTexture *pTexture = CManager::GetTexture();

	// �e�L�X�g�̃f�[�^
	CTextData *pData;
	pData = CManager::GetTextData();

	// �f�[�^�̊��蓖��
	m_pos = pData->GetPosition();		// �ʒu
	m_rot = pData->GetRotation();		// �p�x
	m_nWidth = pData->GetWidthNum();	// ������
	m_nDepth = pData->GetDepthNum();	// ���s����
	m_fWidth = pData->GetSize().x;		// �T�C�Y����
	m_fDepth = pData->GetSize().y;		// �T�C�Y���s

	m_fHeightWave = pData->GetHeightWave();						// �g�̍���
	m_fDistanceWave = pData->GetDistanceWave();					// �g�̊Ԋu
	m_fSpeedWave = pData->GetSpeedWave();						// �g�̑���

	m_syntheticType = (SYNTHETICTYPE)pData->GetSyntheticType();	// �������@
	m_waveType = (WAVETYPE)pData->GetWaveType();				// �g�̕���
	m_bCutTex = pData->GetCutTex();								// �e�N�X�`������
	m_bWireFrame = false;										// ���C���[�t���[���̗L����

	// �e�N�X�`���@�\�̏�����
	m_aTexMoveRot[TEXTUREINFO_0] = 0.0f;
	m_aTexMove[TEXTUREINFO_0] = D3DXVECTOR2(sinf(m_aTexMoveRot[TEXTUREINFO_0]) / 100.0f, cosf(m_aTexMoveRot[TEXTUREINFO_0]) / 100.0f);

	m_aTexMoveRot[TEXTUREINFO_1] = 0.0f;
	m_aTexMove[TEXTUREINFO_1] = D3DXVECTOR2(sinf(m_aTexMoveRot[TEXTUREINFO_1]) / 100.0f, cosf(m_aTexMoveRot[TEXTUREINFO_1]) / 100.0f);

	// �e�N�X�`���̐ݒ�
	m_pTexture[TEXTUREINFO_0] = pTexture->GetAddress(pData->GetType());
	m_pTexture[TEXTUREINFO_1] = pTexture->GetAddress(pData->GetTypeSub());
	m_nTextureNum = pTexture->GetTexMax();
	m_aTextureIdx[TEXTUREINFO_0] = pData->GetType();
	m_aTextureIdx[TEXTUREINFO_1] = pData->GetTypeSub();

	// UI�̐���
	CreateUI();

	// �e�ϐ��̌v�Z
	m_nAllPoint = m_nDepth * 2 * (m_nWidth + 2) - 2;				// �����_��
	m_nPolygon = m_nWidth * m_nDepth * 2 + (4 * (m_nDepth - 1));	// ���|���S����
	m_nWidthPoint = m_nWidth + 1;
	m_nDepthPoint = m_nDepth + 1;

	m_nIdxPoint = m_nWidthPoint * m_nDepthPoint;					// �C���f�b�N�X�o�b�t�@�ł̒��_��

	float fWidthMax = m_fWidth * (float)m_nWidth;
	float fDepthMax = m_fDepth * (float)m_nDepth;

	// �@�������߂�|���S����
	m_nNor = m_nWidth * m_nDepth * 2;

	// �@���l���I�m��
	m_pNor = new D3DXVECTOR3[m_nNor];

	m_nIdxNor = 0;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D_1) * m_nIdxPoint,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D_1,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_nDepthPoint; nCnt++)
	{// ���s��
		for (int nCntA = 0; nCntA < m_nWidthPoint; nCntA++)
		{// ����
		 // �|���S���̊e���_���W
			pVtx[0].pos = D3DXVECTOR3(
				m_pos.x - fWidthMax / 2.0f + (float)nCntA * m_fWidth,
				m_pos.y,
				m_pos.z + fDepthMax / 2.0f - (float)nCnt * m_fDepth);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �e���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			if (m_bCutTex == true)
			{
				// �e�N�X�`�����_���̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(1.0f * nCntA, 1.0f * nCnt);

				// �e�N�X�`�����_���̐ݒ�
				pVtx[0].tex1 = D3DXVECTOR2(1.0f * nCntA, 1.0f * nCnt);
			}
			else
			{
				// �e�N�X�`�����_���̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(1.0f * nCntA / m_nWidth, 1.0f * nCnt / m_nDepth);

				// �e�N�X�`�����_���̐ݒ�
				pVtx[0].tex1 = D3DXVECTOR2(1.0f * nCntA / m_nWidth, 1.0f * nCnt / m_nDepth);
			}

			pVtx++;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nAllPoint,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntA = 0; nCntA < m_nDepth; nCntA++)
	{
		for (int nCnt = 0; nCnt < m_nWidthPoint + 1; nCnt++, pIdx += 2)
		{
			if (nCnt != 0 && nCnt == m_nWidthPoint && nCntA != m_nDepth - 1)
			{// �E�[����܂�Ԃ���
				pIdx[0] = pIdx[-1];
				pIdx[1] = pIdx[-2] + 1;
			}
			else if (nCntA == m_nDepth - 1 && nCnt == m_nWidthPoint)
			{// �I�����ɖ�������
				break;
			}
			else
			{// �ʏ�z�u
				pIdx[0] = m_nWidthPoint + (m_nWidthPoint * nCntA) + nCnt;
				pIdx[1] = pIdx[0] - m_nWidthPoint;
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CMeshfield::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < TEXTUREINFO_MAX; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}

	// �C���f�b�N�X�o�b�t�@�̊J��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// �@���J��
	if (m_pNor != NULL)
	{
		delete[] m_pNor;
		m_pNor = NULL;
	}

	// UI�̔j��
	UninitUI();
}

//==============================================================================
// �X�V����
//==============================================================================
void CMeshfield::Update(void)
{
	// �g�`�̋���
	MeshWave();

	// �@���̌v�Z
	CalcuNormal();

	// ���b�V���Ǘ��̑���
	ControlMesh();

	// �e�N�X�`���̎擾
	CTexture *pTexture = CManager::GetTexture();

	m_pTexture[TEXTUREINFO_0] = pTexture->GetAddress(m_aTextureIdx[TEXTUREINFO_0]);
	m_pTexture[TEXTUREINFO_1] = pTexture->GetAddress(m_aTextureIdx[TEXTUREINFO_1]);

	// UI�̍X�V
	UpdateUI();
}

//==============================================================================
// �`�揈��
//==============================================================================
void CMeshfield::Draw(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	if (m_bWireFrame == true)
	{
		// ���C���[�t���[���L��
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//// �����̔��f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//// �ʒu�𔽉f
	//D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D_1));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D_1);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture[TEXTUREINFO_0]);
	pDevice->SetTexture(1, m_pTexture[TEXTUREINFO_1]);

	// �e�N�X�`���̍������@
	switch (m_syntheticType)
	{
		case SYNTHETICTYPE_NONE:	// �����Ȃ�
			
			// �ꖇ�ڂ̃e�N�X�`���̃e�N�X�`���X�e�[�W�ݒ�
			pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

			// �񖇖ڂ̃e�N�X�`���̃e�N�X�`���X�e�[�W�ݒ�
			pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			
			break;

		case SYNTHETICTYPE_ADD:	// ���Z����

			// �ꖇ�ڂ̃e�N�X�`���̃e�N�X�`���X�e�[�W�ݒ�
			pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

			// �񖇖ڂ̃e�N�X�`���̃e�N�X�`���X�e�[�W�ݒ�
			pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

			break;

		case SYNTHETICTYPE_SUBTRA:	// ���Z����

			// �ꖇ�ڂ̃e�N�X�`���̃e�N�X�`���X�e�[�W�ݒ�
			pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			
			// �񖇖ڂ̃e�N�X�`���̃e�N�X�`���X�e�[�W�ݒ�
			pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

			break;

		case SYNTHETICTYPE_MULTI:	// ��Z����

			// �ꖇ�ڂ̃e�N�X�`���̃e�N�X�`���X�e�[�W�ݒ�
			pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

			// �񖇖ڂ̃e�N�X�`���̃e�N�X�`���X�e�[�W�ݒ�
			pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

			break;
	}

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,
		0,
		m_nIdxPoint,			// ���_��
		0,
		m_nPolygon);			// �v���~�e�B�u��

	pDevice->SetTexture(1, NULL);
	pDevice->SetTexture(0, NULL);

	// �ʏ�̃A���t�@�u�����f�B���O
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

//==============================================================================
// �t�B�[���h�̐���
//==============================================================================
CMeshfield *CMeshfield::Create(void)
{
	// ���[�J���ϐ��錾
	CMeshfield *pMeshField;
	pMeshField = new CMeshfield;

	if (pMeshField != NULL)
	{
		// ������
		pMeshField->Init();
	}

	return pMeshField;
}

//==============================================================================
// �@���v�Z
//==============================================================================
void CMeshfield::CalcuNormal(void)
{
	// ���[�J���ϐ��錾
	VERTEX_3D_1 *pVtx;
	D3DXVECTOR3 vecA = VECTOR3_NULL;
	D3DXVECTOR3 vecB = VECTOR3_NULL;

	int nCntNormal = 0;	// �@���J�E���g

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�ʂ̖@���Z�o
	for (int nCntDepth = 0; nCntDepth < m_nDepth; nCntDepth++)
	{// ���s�ʐ�
		int nCntOdd = 0;	// ��J�E���g
		int nCntEven = 0;	// �����J�E���g

		for (int nCntWidth = 0; nCntWidth < m_nWidth * 2; nCntWidth++)
		{// �������_���i�O�p�`�|���S�����j						
			if (nCntWidth % 2 == 0)
			{
				// �������x�N�g��
				vecA =
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) - m_nWidthPoint + nCntEven].pos -
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) + nCntEven].pos;

				vecB =
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) + nCntEven + 1].pos -
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) + nCntEven].pos;

				nCntEven++;		// �����J�E���g
			}
			else
			{
				// ����x�N�g��
				vecA =
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) + nCntOdd - m_nWidth + m_nWidthPoint].pos -
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) + nCntOdd - m_nWidth].pos;

				vecB =
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) + nCntOdd - m_nWidth - 1].pos -
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) + nCntOdd - m_nWidth].pos;

				nCntOdd++;		// ��J�E���g
			}

			// �@���x�N�g���̌v�Z
			D3DXVec3Cross(&m_pNor[nCntNormal], &vecA, &vecB);

			// �@���x�N�g���̐��K��
			D3DXVec3Normalize(&m_pNor[nCntNormal], &m_pNor[nCntNormal]);

			// ���݂̃J�E���g
			nCntNormal++;
		}
	}

	// ���[�J���ϐ��錾
	int nCntUnder = m_nWidth * 2 * (m_nDepth - 1);	// ���ӊJ�n�ʖ@���C���f�b�N�X
	int nCntLeft = m_nWidth * 2;					// ���ӊJ�n�ʖ@���C���f�b�N�X
	int nCntRight = m_nWidth * 2 - 1;				// �E�ӊJ�n�ʖ@���C���f�b�N�X
	int nRight = 1;									// �E�ӊJ�n�n�_�C���f�b�N�X�v�Z�p

	int nCntCenterA = 0;							// �������_�v�Z�p�㑤
	int nCntCenterB
		= nCntCenterA + (m_nWidthPoint * 2 - 1);	// �������_�v�Z�p����
	int nCntCenterC = 1;							// �������_�v�Z�p�܂�Ԃ��J�E���g

													// �e���_�̖@���Z�o
	for (int nCntNor = 0; nCntNor < m_nIdxPoint; nCntNor++)
	{
		// 2�ʖ@��
		if (nCntNor == 0)
		{// �ŏ�(����)
			pVtx[nCntNor].nor = m_pNor[0] + m_pNor[1];
		}
		else if (nCntNor == m_nIdxPoint - 1)
		{// �Ō�(�E��)
			pVtx[nCntNor].nor = m_pNor[m_nNor - 1] + m_pNor[m_nNor - 2];
		}

		// 1�ʖ@��
		else if (nCntNor == m_nWidth)
		{// �E��
			pVtx[nCntNor].nor = m_pNor[m_nWidth * 2 - 1];
		}
		else if (nCntNor == (m_nWidth + 1) + ((m_nDepth - 1) * (m_nWidth + 1)))
		{// ����
			pVtx[nCntNor].nor = m_pNor[m_nWidth * 2 * (m_nDepth - 1)];
		}

		// 3�ʖ@��
		else if (nCntNor > 0 && nCntNor < m_nWidth)
		{// ��Ӓ�����
		 // ���[�J���ϐ��錾
			int nCntA = nCntNor + (nCntNor - 1);	// �n���ƂȂ�ʃC���f�b�N�X

			pVtx[nCntNor].nor = m_pNor[nCntA] + m_pNor[nCntA + 1] + m_pNor[nCntA + 2];
		}
		else if (nCntNor >(m_nWidth + 1) + ((m_nDepth - 1) * (m_nWidth + 1)) && nCntNor < m_nIdxPoint - 1)
		{// ���Ӓ�����
			pVtx[nCntNor].nor = m_pNor[nCntUnder] + m_pNor[nCntUnder + 1] + m_pNor[nCntUnder + 2];
			nCntUnder += 2;
		}
		else if (nCntNor % m_nWidthPoint == 0 && nCntNor != 0)
		{// ���Ӓ�����
			pVtx[nCntNor].nor = m_pNor[nCntLeft] + m_pNor[nCntLeft + 1] + m_pNor[nCntLeft - (m_nWidth * 2)];
			nCntLeft += m_nWidth * 2;
		}
		else if (nCntNor % (m_nWidth + m_nWidthPoint * nRight) == 0 && nCntNor != 0)
		{// �E�Ӓ�����
			pVtx[nCntNor].nor = m_pNor[nCntRight] + m_pNor[nCntRight - 1] + m_pNor[nCntRight + (m_nWidth * 2)];
			nCntRight += m_nWidth * 2;
			nRight++;
		}

		// ��O�I���_(6�ʖ@��)
		else
		{// �����̒��_
			pVtx[nCntNor].nor =
				m_pNor[nCntCenterA] + m_pNor[nCntCenterA + 1] + m_pNor[nCntCenterA + 2] +
				m_pNor[nCntCenterB] + m_pNor[nCntCenterB + 1] + m_pNor[nCntCenterB + 2];

			if (nCntNor % ((m_nWidth - 1) + m_nWidthPoint * nCntCenterC) == 0)
			{// �܂�Ԃ��n�_
				nCntCenterA += 4;
				nCntCenterB += 4;
				nCntCenterC++;
			}
			else
			{// ��O
				nCntCenterA += 2;
				nCntCenterB += 2;
			}
		}

		// �x�N�g���̐��K��
		D3DXVec3Normalize(&pVtx[nCntNor].nor, &pVtx[nCntNor].nor);
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==============================================================================
// ���b�V���̔g�`
//==============================================================================
void CMeshfield::MeshWave(void)
{
	// �g�`�J�E���g
	m_nCntWave++;

	// ���[�J���ϐ��錾
	VERTEX_3D_1 *pVtx;

	m_aTexMove[TEXTUREINFO_0] = D3DXVECTOR2(sinf(m_aTexMoveRot[TEXTUREINFO_0]) * m_aTexMoveSpeed[TEXTUREINFO_0], cosf(m_aTexMoveRot[TEXTUREINFO_0]) * m_aTexMoveSpeed[TEXTUREINFO_0]);
	m_aTexMove[TEXTUREINFO_1] = D3DXVECTOR2(sinf(m_aTexMoveRot[TEXTUREINFO_1]) * m_aTexMoveSpeed[TEXTUREINFO_1], cosf(m_aTexMoveRot[TEXTUREINFO_1]) * m_aTexMoveSpeed[TEXTUREINFO_1]);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDepth = 0; nCntDepth < m_nDepthPoint; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nWidthPoint; nCntWidth++)
		{
			// �g�`�̃^�C�v
			switch (m_waveType)
			{
			case WAVETYPE_NONE:
				pVtx[0].pos.y = m_pos.y;
				break;

			case WAVETYPE_WAVE_X:

				pVtx[0].pos.y +=
					sinf(pVtx[0].pos.x * m_fDistanceWave + m_nCntWave * m_fSpeedWave) * m_fHeightWave;
				
				break;

			case WAVETYPE_WAVE_Z:

				pVtx[0].pos.y +=
					sinf(pVtx[0].pos.z * m_fDistanceWave + m_nCntWave * m_fSpeedWave) * m_fHeightWave;

				break;

			case WAVETYPE_WAVE_XZ:

				pVtx[0].pos.y +=
					sinf(pVtx[0].pos.x * m_fDistanceWave + m_nCntWave * m_fSpeedWave) * m_fHeightWave;

				pVtx[0].pos.y +=
					sinf(pVtx[0].pos.z * m_fDistanceWave + m_nCntWave * m_fSpeedWave) * m_fHeightWave;

				break;

			case WAVETYPE_CIRCLE:
				// ����
				D3DXVECTOR3 vec = pVtx[0].pos - m_pos;

				// ����
				float fDistance = sqrtf(vec.x * vec.x + vec.z * vec.z);

				pVtx[0].pos.y += sinf(fDistance * m_fDistanceWave + m_nCntWave * m_fSpeedWave) * m_fHeightWave;
				break;
			}

			switch (m_texMove)
			{
			case TEXMOVE_STRAIGHT:
				// �e�N�X�`���̈ړ�
				pVtx[0].tex += m_aTexMove[TEXTUREINFO_0];
				pVtx[0].tex1 += m_aTexMove[TEXTUREINFO_1];
				break;

			case TEXMOVE_WAVE:
				break;
			}


			pVtx++;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==============================================================================
// ���b�V���̑���
//==============================================================================
void CMeshfield::ControlMesh(void)
{	
	// ���[�J���ϐ��錾
	VERTEX_3D_1 *pVtx;
	int nIdx = 0;

	// �e�L�X�g�̃f�[�^
	CTextData *pData;
	pData = CManager::GetTextData();

	// �L�[�{�[�h�̎擾
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// �g�̔���
	if (keyboard->GetTrigger(CInput::KEYINFO_WAVE_FLUG) == true)
	{
		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntDepth = 0; nCntDepth < m_nDepthPoint; nCntDepth++)
		{
			for (int nCntWidth = 0; nCntWidth < m_nWidthPoint; nCntWidth++)
			{
				// ��x���W��߂�
				pVtx[nIdx].pos.y = m_pos.y;
				nIdx++;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();

		// ���ڂ̎w��
		if (m_waveType == WAVETYPE_MAX - 1)
		{
			m_waveType = (WAVETYPE)0;
		}
		else
		{
			m_waveType = (WAVETYPE)(m_waveType + 1);	
		}
	}

	// �������@�̐؂�ւ�
	if (keyboard->GetTrigger(CInput::KEYINFO_WAVE_SYNTHETIC) == true)
	{
		// ���ڂ̎w��
		if (m_syntheticType == SYNTHETICTYPE_MAX - 1)
		{
			m_syntheticType = (SYNTHETICTYPE)0;
		}
		else
		{
			m_syntheticType = (SYNTHETICTYPE)(m_syntheticType + 1);
		}
	}

	// �����̐؂�ւ�
	if (keyboard->GetTrigger(CInput::KEYINFO_WAVE_CUTTEX) == true)
	{
		m_bCutTex = !m_bCutTex;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < m_nDepthPoint; nCnt++)
		{// ���s��
			for (int nCntA = 0; nCntA < m_nWidthPoint; nCntA++)
			{// ����

				if (m_bCutTex == true)
				{// ��������
				 // �e�N�X�`�����_���̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(1.0f * nCntA, 1.0f * nCnt);

					// �e�N�X�`�����_���̐ݒ�
					pVtx[0].tex1 = D3DXVECTOR2(1.0f * nCntA, 1.0f * nCnt);
				}
				else
				{
					// �e�N�X�`�����_���̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(1.0f * nCntA / m_nWidth, 1.0f * nCnt / m_nDepth);

					// �e�N�X�`�����_���̐ݒ�
					pVtx[0].tex1 = D3DXVECTOR2(1.0f * nCntA / m_nWidth, 1.0f * nCnt / m_nDepth);
				}

				pVtx++;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}

	// �f�[�^�̏����o��
	if (keyboard->GetTrigger(CInput::KEYINFO_SAVE) == true)
	{
		pData->SaveText();
	}

	// ���삷��e�N�X�`���̐؂�ւ�
	if (keyboard->GetTrigger(CInput::KEYINFO_WAVE_TEX_CHANGE) == true)
	{
		if (m_texInfo == TEXTUREINFO_MAX - 1)
		{// �܂�Ԃ�
			m_texInfo = (TEXTUREINFO)0;
		}
		else
		{// ����
			m_texInfo = (TEXTUREINFO)(m_texInfo + 1);
		}
	}

	// �e�N�X�`���̃C���f�b�N�X����
	if (keyboard->GetTrigger(CInput::KEYINFO_WAVE_TEXTURE_IDX_PLUS) == true)
	{
		if (m_aTextureIdx[m_texInfo] == m_nTextureNum - 1)
		{
			m_aTextureIdx[m_texInfo] = 0;
		}
		else
		{
			m_aTextureIdx[m_texInfo]++;
		}	
	}
	if (keyboard->GetTrigger(CInput::KEYINFO_WAVE_TEXTURE_IDX_MINUS) == true)
	{
		if (m_aTextureIdx[m_texInfo] == 0)
		{
			m_aTextureIdx[m_texInfo] = m_nTextureNum - 1;
		}
		else
		{
			m_aTextureIdx[m_texInfo]--;
		}
	}

	// ���C���[�t���[���̐؂�ւ�
	if (keyboard->GetTrigger(CInput::KEYINFO_WIREFRAME) == true)
	{
		m_bWireFrame = !m_bWireFrame;
	}

	// �g�̍����̒���
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_HEIGHTUP) == true)
	{
		m_fHeightWave += WAVE_HEIGHT_ADJUST;
	}
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_HEIGHTDOWN) == true)
	{
		m_fHeightWave -= WAVE_HEIGHT_ADJUST;
	}

	// �g�`�Ԋu�̒���
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_DISTANCEUP) == true)
	{
		m_fDistanceWave += WAVE_DISTANCE_ADJUST;
	}
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_DISTANCEDOWN) == true)
	{
		m_fDistanceWave -= WAVE_DISTANCE_ADJUST;
	}

	// �����̒���
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_SPEEDUP) == true)
	{
		m_fSpeedWave += WAVE_SPEED_ADJUST;
	}
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_SPEEDDOWN) == true)
	{
		m_fSpeedWave -= WAVE_SPEED_ADJUST;
	}

	// �e�N�X�`���̗�������
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_TEXMOVEROT_PLUS) == true)
	{// ���Z
		m_aTexMoveRot[m_texInfo] += 0.01f;

		// �p�x�̕␳
		if (m_aTexMoveRot[m_texInfo] > D3DX_PI)
		{
			m_aTexMoveRot[m_texInfo] -= PI_RESET;
		}
	}
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_TEXMOVEROT_MINUS) == true)
	{// ���Z
		m_aTexMoveRot[m_texInfo] -= 0.01f;

		// �p�x�̕␳
		if (m_aTexMoveRot[m_texInfo] < 0.0f)
		{
			m_aTexMoveRot[m_texInfo] += PI_RESET;
		}
	}

	// �e�N�X�`���̗���鑬��
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_TEXMOVE_PLUS) == true)
	{// ���Z
		m_aTexMoveSpeed[m_texInfo] += 0.001f;
	}
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_TEXMOVE_MINUS) == true)
	{// ���Z
		m_aTexMoveSpeed[m_texInfo] -= 0.001f;
	}

	// �g�`�̍�������
	if (m_fHeightWave < 1.0f)
	{
		m_fHeightWave = 1.0f;
	}
}

// 2D�|���S��UI�̐���
void CMeshfield::CreateUI(void)
{
	for (int nCntPoly = 0; nCntPoly < TEXTUREINFO_MAX; nCntPoly++)
	{
		if (m_apScene2D[nCntPoly] == NULL)
		{
			m_apScene2D[nCntPoly] = CScene2D::Create();
			m_apScene2D[nCntPoly]->SetSize(TEXTURE_UI_SIZE);
			m_apScene2D[nCntPoly]->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
			m_apScene2D[nCntPoly]->BindTexture(m_aTextureIdx[nCntPoly]);
		}
	}

	m_apScene2D[TEXTUREINFO_0]->SetPosition(TEXTURE_UI_POS_0);
	m_apScene2D[TEXTUREINFO_1]->SetPosition(TEXTURE_UI_POS_1);
}

// 2D�|���S��UI�̔j��
void CMeshfield::UninitUI(void)
{
	for (int nCntPoly = 0; nCntPoly < TEXTUREINFO_MAX; nCntPoly++)
	{
		if (m_apScene2D[nCntPoly] != NULL)
		{// �|���S���̔j��
			m_apScene2D[nCntPoly]->Uninit();
			m_apScene2D[nCntPoly] = NULL;
		}
	}
}

// 2D�|���S��UI�̍X�V
void CMeshfield::UpdateUI(void)
{
	for (int nCntPoly = 0; nCntPoly < TEXTUREINFO_MAX; nCntPoly++)
	{
		if (m_apScene2D[nCntPoly] != NULL)
		{// �e�N�X�`���̕ω�
			m_apScene2D[nCntPoly]->BindTexture(m_aTextureIdx[nCntPoly]);
		}

		if (nCntPoly == m_texInfo)
		{
			m_apScene2D[nCntPoly]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			m_apScene2D[nCntPoly]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		}
	}
}