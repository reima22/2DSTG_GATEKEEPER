//=============================================================================
//
// �e�L�X�g�f�[�^�Ǘ� [textdata.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTDATA_H_
#define _TEXTDATA_H_

#include "main.h"
#include "meshfield.h"

// �}�N����`
#define TEXT_NUM	(128)	// �e�L�X�g�f�[�^�o�C�g��

//==============================================================================
// �e�L�X�g�Ǘ��N���X
//==============================================================================
class CTextData
{
public:
	// �e�L�X�g�f�[�^�̎��
	typedef enum
	{
		TEXTDATA_TEXTURE = 0,		// �e�N�X�`���f�[�^
		TEXTDATA_MESHFIELD,		// ���b�V���t�B�[���h�f�[�^
		TEXTDATA_MAX
	}TEXTDATA;

	CTextData();
	~CTextData();

	static CTextData *Create(void);	// ����

	void LoadText(void);
	void UnloadText(void);

	void SaveText(void);

	// �f�[�^�̎擾
	D3DXVECTOR3 GetPosition(void) { return m_pos; }			// �ʒu
	D3DXVECTOR3 GetRotation(void) { return m_rot; }			// �p�x
	D3DXVECTOR2 GetSize(void) { return m_size; }			// �T�C�Y
	int GetWidthNum(void) { return m_nWidth; }				// �ʂ̉�������
	int GetDepthNum(void) { return m_nDepth; }				// �ʂ̉��s����
	int GetType(void) { return m_nType; }					// �e�N�X�`�����
	int GetTypeSub(void) { return m_nTypeSub; }				// �e�N�X�`��2����
	float GetHeightWave(void) { return m_fHeightWave; }		// �g�̍���
	float GetDistanceWave(void) { return m_fDistanceWave; }	// �g�̊Ԋu
	float GetSpeedWave(void) { return m_fSpeedWave; }			// �g�̑���

	int GetSyntheticType(void) { return m_nSyntheticType; }	// �������@
	int GetWaveType(void) { return m_nWaveType; }			// �g�̕���
	bool GetCutTex(void) { return m_bCutTex; }				// �e�N�X�`���̕���
	float GetTexMoveRot(int nIdx) { return m_fTexMoveRot[nIdx]; }	// �e�N�X�`���̗�������
	float GetTexMove(int nIdx) { return m_fTexMove[nIdx]; }			// �e�N�X�`���̈ړ���
	int GetNumTexture(void) { return m_nNumTexture; }
	char *GetFileName(int nIdx) { return m_pFileTextureName[nIdx]; }

private:
	int m_nNumTexture;			// �e�N�X�`���̐�
	char **m_pFileTextureName;	// �t�@�C�������i�[����|�C���^
	int *m_pIdx;				// �e�N�X�`���̃C���f�b�N�X

	int m_nType;				// �e�N�X�`�����
	int m_nTypeSub;				// �e�N�X�`��2����

	D3DXVECTOR3 m_pos;			// �ʒu
	D3DXVECTOR3 m_rot;			// �p�x
	D3DXVECTOR2 m_size;			// 1�ʓ�����̃T�C�Y
	int m_nWidth;				// �����̃|���S������
	int m_nDepth;				// ���s�̃|���S������

	int m_nIdxPoint;			// �����_��
	float m_fHeightWave;		// �g�̍���
	float m_fDistanceWave;		// �g�̊Ԋu
	float m_fSpeedWave;			// �g�̑���

	int m_nSyntheticType;		// �`�捇�����@
	int m_nWaveType;			// �g�̌���
	bool m_bCutTex;				// �e�N�X�`���̕���

	float m_fTexMoveRot[CMeshfield::TEXTUREINFO_MAX];		// �e�N�X�`���̗�������
	float m_fTexMove[CMeshfield::TEXTUREINFO_MAX];		// �e�N�X�`���̈ړ���


};
#endif