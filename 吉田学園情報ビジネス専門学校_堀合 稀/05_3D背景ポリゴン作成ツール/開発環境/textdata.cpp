//==============================================================================
//
// �e�L�X�g�f�[�^�Ǘ��ktextdata.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include "textdata.h"
#include "stdio.h"
#include "meshfield.h"
#include "exe.h"

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CTextData::CTextData()
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CTextData::~CTextData()
{

}

//==============================================================================
// ��������
//==============================================================================
CTextData *CTextData::Create(void)
{
	// ���[�J���ϐ��錾
	CTextData *pTextData;
	pTextData = new CTextData;	// �C���X�^���X����

	// �e�L�X�g�f�[�^
	if (pTextData != NULL)
	{
		// �e�L�X�g�f�[�^�̓ǂݍ���
		pTextData->LoadText();
	}

	return pTextData;
}

//==============================================================================
// �e�L�X�g�f�[�^�̓ǂݍ���
//==============================================================================
void CTextData::LoadText(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile;
	char aLoadText[TEXT_NUM] = {};
	char *pLoadText = &aLoadText[0];
	int nTexCnt = 0;

	// �f�[�^�̓ǂݍ���
	pFile = fopen("data/TEXT/meshtooldata.txt", "r");

	if (pFile != NULL)
	{ //�t�@�C���W�J�\

	  // �X�N���v�g�ǂݍ��ݏI���܂�
		while (strcmp("END_SCRIPT", pLoadText) != 0)
		{
			// ���ړǂݍ���
			fscanf(pFile, "%s", pLoadText);

			// �e�N�X�`����
			if (strcmp("NUM_TEXTURE", pLoadText) == 0)
			{
				fscanf(pFile, "%s %d", &aLoadText[0], &m_nNumTexture);

				// �e�N�X�`�������A���I�m��
				m_pFileTextureName = new char*[m_nNumTexture];
				m_pIdx = new int[m_nNumTexture];

				// 2�����z��̊m��
				for (int nCnt = 0; nCnt < m_nNumTexture; nCnt++)
				{
					m_pFileTextureName[nCnt] = new char[TEXT_NUM];
				}
			}

			// �t�@�C����
			if (strcmp("TEXTURE_FILENAME", pLoadText) == 0)
			{
				// �t�@�C����
				fscanf(pFile, "%s %s", &aLoadText[0], &m_pFileTextureName[nTexCnt][0]);

				// �C���f�b�N�X�̊��蓖��
				m_pIdx[nTexCnt] = nTexCnt;

				// �J�E���g����
				nTexCnt++;
			}

			// �z�u���̃J�E���g
			if (strcmp("FIELDSET", pLoadText) == 0)
			{
				char aObjectData[TEXT_NUM] = {};
				char *pObjectData = &aObjectData[0];

				while (strcmp("END_FIELDSET", pObjectData) != 0)
				{
					// ���ړǂݍ���
					fscanf(pFile, "%s", pObjectData);

					// ���
					if (strcmp("TEXTYPE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0], &m_nType);
					}

					// ���
					if (strcmp("TEXTYPE_SUB", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0], &m_nTypeSub);
					}

					// �ʒu
					if (strcmp("POS", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aObjectData[0],
							&m_pos.x,
							&m_pos.y,
							&m_pos.z);
					}

					// ����
					if (strcmp("ROT", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aObjectData[0],
							&m_rot.x,
							&m_rot.y,
							&m_rot.z);
					}

					// �T�C�Y
					if (strcmp("SIZE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f %f", &aObjectData[0],
							&m_size.x,
							&m_size.y);
					}

					// �����̖���
					if (strcmp("WIDTH", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0],
							&m_nWidth);
					}

					// ���s�̖���
					if (strcmp("DEPTH", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0],
							&m_nDepth);

						m_nIdxPoint = (m_nWidth + 1)*(m_nDepth + 1);
					}

					// �g�̍���
					if (strcmp("HEIGHT_WAVE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f", &aObjectData[0], &m_fHeightWave);
					}

					// �g�̊Ԋu
					if (strcmp("DISTANCE_WAVE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f", &aObjectData[0], &m_fDistanceWave);
					}

					// �g�̑���
					if (strcmp("SPEED_WAVE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f", &aObjectData[0], &m_fSpeedWave);
					}

					// �`�捇�����@
					if (strcmp("SYNTHETICTYPE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0], &m_nSyntheticType);
					}

					// �g�̕���
					if (strcmp("WAVETYPE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0], &m_nWaveType);
					}

					// �`�捇�����@
					if (strcmp("CUTTEX", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0], (int*)&m_bCutTex);
					}

					// �e�N�X�`���̗�������
					if (strcmp("TEXMOVEROT_0", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f", &aObjectData[0], &m_fTexMoveRot[0]);
					}
					// �e�N�X�`���̗�������
					if (strcmp("TEXMOVE_0", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f", &aObjectData[0], &m_fTexMove[0]);
					}
					// �e�N�X�`���̗�������
					if (strcmp("TEXMOVEROT_1", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f", &aObjectData[0], &m_fTexMoveRot[1]);
					}
					// �e�N�X�`���̗�������
					if (strcmp("TEXMOVE_1", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f", &aObjectData[0], &m_fTexMove[1]);
					}
				}
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C���W�J�s��
		return;
	}
}

// �e�L�X�g�f�[�^�̔j��
void CTextData::UnloadText(void)
{
	if (m_pFileTextureName != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumTexture; nCnt++)
		{
			delete[] m_pFileTextureName[nCnt];
		}

		delete[] m_pFileTextureName;
		m_pFileTextureName = NULL;
	}

	if (m_pIdx != NULL)
	{
		delete[] m_pIdx;
		m_pIdx = NULL;
	}
}

// �e�L�X�g�f�[�^�̏����o��
void CTextData::SaveText(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile;
	CMeshfield *pMeshfield = CExe::GetMeshField();

	// �f�[�^�̓ǂݍ���
	pFile = fopen("data/TEXT/output.txt", "w");

	if (pFile != NULL)
	{
		fprintf(pFile, "# �����o�������b�V�����\n");

		// �e�N�X�`��0�̃C���f�b�N�X
		fprintf(pFile, "TEXTYPE = %d\n", pMeshfield->GetTexIdx(0));

		// �e�N�X�`��1�̃C���f�b�N�X
		fprintf(pFile, "TEXTYPE_SUB = %d\n", pMeshfield->GetTexIdx(1));

		// �ʒu
		fprintf(pFile, "POS = %.1f %.1f %.1f\n",
			pMeshfield->GetPosition().x,
			pMeshfield->GetPosition().y,
			pMeshfield->GetPosition().z);

		// �p�x
		fprintf(pFile, "ROT = %.1f %.1f %.1f\n",
			pMeshfield->GetRotation().x,
			pMeshfield->GetRotation().y,
			pMeshfield->GetRotation().z);

		// �T�C�Y
		fprintf(pFile, "SIZE = %.1f %.1f\n",
			pMeshfield->GetWidthSize(),
			pMeshfield->GetDepthSize());

		// �|���S���̖���
		fprintf(pFile, "WIDTH = %d\n", pMeshfield->GetWidthNum());
		fprintf(pFile, "DEPTH = %d\n", pMeshfield->GetDepthNum());

		// �g�̍���
		fprintf(pFile, "HEIGHT_WAVE = %.3f\n",pMeshfield->GetHeightWave());

		// �g�̊Ԋu
		fprintf(pFile, "DISTANCE_WAVE = %.3f\n", pMeshfield->GetDistanceWave());

		// �g�̑���
		fprintf(pFile, "SPEED_WAVE = %.2f\n", pMeshfield->GetSpeedWave());

		// �e�N�X�`���`�捇�����@
		fprintf(pFile, "SYNTHETICTYPE = %d\n", pMeshfield->GetSyntheticType());

		// �g�̓���
		fprintf(pFile, "WAVETYPE = %d\n", pMeshfield->GetWaveType());

		// �e�N�X�`���̕���
		fprintf(pFile, "CUTTEX = %d\n", (int)pMeshfield->GetCutTex());

		// �e�N�X�`���̗�������,0
		fprintf(pFile, "TEXMOVEROT_0 = %.2f\n", pMeshfield->GetTexMoveRot(0));

		// �e�N�X�`���̗���鑬��,0
		fprintf(pFile, "TEXMOVE_0 = %.2f\n", pMeshfield->GetTexMove(0));

		// �e�N�X�`���̗�������,1
		fprintf(pFile, "TEXMOVEROT_1 = %.2f\n", pMeshfield->GetTexMoveRot(1));

		// �e�N�X�`���̗���鑬��,1
		fprintf(pFile, "TEXMOVE_1 = %.2f\n", pMeshfield->GetTexMove(1));
	}
	else
	{
		/*�ǂݍ��ݎ��s*/
	}
}