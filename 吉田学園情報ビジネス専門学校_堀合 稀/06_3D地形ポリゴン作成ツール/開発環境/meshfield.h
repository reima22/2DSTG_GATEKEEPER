//==============================================================================
//
// ���b�V���t�B�[���h�̕`��kmeshfield.h�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"
#include "scene.h"

// �O���錾
class CScene2D;	// 2D�|���S���N���X

// �}�N����`

#define WAVE_HEIGHT_ADJUST		(0.001f)	// �g�̍����̒������l
#define WAVE_DISTANCE_ADJUST	(0.001f)	// �g�`�Ԋu�̒������l
#define WAVE_SPEED_ADJUST		(0.001f)	// �T�C���g�̑傫�������l

#define TEXTURE_UI_POS_0		(D3DXVECTOR3(1060.0f, 50.0f,0.0f))	// UI0�̈ʒu
#define TEXTURE_UI_POS_1		(D3DXVECTOR3(1200.0f, 50.0f,0.0f))	// UI1�̈ʒu
#define TEXTURE_UI_SIZE			(D3DXVECTOR2(50.0f, 50.0f))			// �e�N�X�`��UI�̃T�C�Y

//==============================================================================
// ���b�V���t�B�[���h�N���X
//==============================================================================
class CMeshfield : public CScene
{
public:
	typedef enum
	{
		WAVETYPE_NONE = 0,	// �g�`����
		WAVETYPE_WAVE_X,	// ������
		WAVETYPE_WAVE_Z,	// ������
		WAVETYPE_WAVE_XZ,	// �o��
		WAVETYPE_CIRCLE,	// �~�`
		WAVETYPE_MAX
	}WAVETYPE;

	// �e�N�X�`���̍���
	typedef enum
	{
		SYNTHETICTYPE_NONE = 0,		// �����Ȃ�
		SYNTHETICTYPE_ADD,			// ���Z����
		SYNTHETICTYPE_SUBTRA,		// ���Z����
		SYNTHETICTYPE_MULTI,		// ��Z����
		SYNTHETICTYPE_MAX
	}SYNTHETICTYPE;

	// �e�N�X�`���̏��
	typedef enum
	{
		TEXTUREINFO_0 = 0,		// ���
		TEXTUREINFO_1,			// ���
		TEXTUREINFO_MAX
	}TEXTUREINFO;

	// �e�N�X�`���ړ��̕��@
	typedef enum
	{
		TEXMOVE_STRAIGHT = 0,	// ����
		TEXMOVE_WAVE,			// �֍s
		TEXMOVE_MAX
	}TEXMOVE;

	CMeshfield();
	~CMeshfield();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshfield *Create(void);

	void CalcuNormal(void);	// �@���̌v�Z
	void MeshWave(void);	// �g�`
	void ControlMesh(void);	// ���b�V���̑���

	void CreateUI(void);	// 2D�|���S��UI�̐���
	void UninitUI(void);	// 2D�|���S��UI�̔j��
	void UpdateUI(void);	// 2D�|���S��UI�̍X�V

	// �ʒu�̎擾�E�ݒ�
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetPostion(D3DXVECTOR3 pos) { m_pos = pos; }

	// �p�x�̎擾�E�ݒ�
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }

	// �ʂ̐��̎擾�E�ݒ�
	int GetWidthNum(void) { return m_nWidth; }
	void SetWidthNum(int nWidth) { m_nWidth = nWidth; }
	int GetDepthNum(void) { return m_nDepth; }
	void SetDepthNum(int nDepth) { m_nDepth = nDepth; }

	// �����̎擾�E�ݒ�
	float GetWidthSize(void) { return m_fWidth; }
	void SetWidthSize(float fWidth) { m_fWidth = fWidth; }
	float GetDepthSize(void) { return m_fDepth; }
	void SetDepthSize(float fDepth) { m_fDepth = fDepth; }

	// ���_���̎擾�E�ݒ�
	int GetWidthPoint(void) { return m_nWidthPoint; }
	void SetWidthPoint(int nWidthPoint) { m_nWidthPoint = nWidthPoint; }
	int GetDepthPoint(void) { return m_nDepthPoint; }
	void SetDepthPoint(int nDepthPoint) { m_nDepthPoint = nDepthPoint; }

	// �g�`���̎擾
	// ������
	char *GetWaveTypeChar(void) { return m_pWaveType[m_waveType]; }
	char *GetSyntheticTypeChar(void) { return m_pSyntheticType[m_syntheticType]; }
	char *GetTexChar(void) { return m_pTex[m_texInfo]; }
	char *GetWireONOFF(void) { return m_apOnOff[m_bWireFrame]; }		// �I���I�t�̕�����

	// �ڍׂ̎擾
	float GetHeightWave(void) { return m_fHeightWave; }					// �g�̍���
	float GetDistanceWave(void) { return m_fDistanceWave; }				// �g�̊Ԋu
	float GetSpeedWave(void) { return m_fSpeedWave; }					// �g�̑���
	SYNTHETICTYPE GetSyntheticType(void) { return m_syntheticType; }	// �������@
	WAVETYPE GetWaveType(void) { return m_waveType; }					// �g�̕���
	TEXTUREINFO GetTex(void) { return m_texInfo; }						// ���쒆�̃e�N�X�`��
	bool GetCutTex(void) { return m_bCutTex; }							// �e�N�X�`���̕����L��
	bool GetWireFrame(void) { return m_bWireFrame; }					// ���C���[�t���[���̗L��

	float GetTexMoveRot(int nIdx) { return m_aTexMoveRot[nIdx]; }		// �e�N�X�`���̗�������
	float GetTexMove(int nIdx) { return m_aTexMoveSpeed[nIdx]; }		// �e�N�X�`���̗���鑬���{��
	int GetTexIdx(int nIdx) { return m_aTextureIdx[nIdx]; }				// �e�N�X�`���̃C���f�b�N�X

	// �����_��
	int GetIdxPoint(void) { return m_nIdxPoint; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// �o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff = NULL;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_pTexture[TEXTUREINFO_MAX] = {};	// �e�N�X�`���ւ̃|�C���^

	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_rot;							// �p�x
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X

	int m_nAllPoint;							// �����_��
	int m_nPolygon;								// �|���S����
	int m_nIdxPoint;							// �C���f�b�N�X�o�b�t�@�̕K�v�Ȋm�ې�

	float m_fWidth;								// �|���S���������̕�
	float m_fDepth;								// �|���S���������̉��s

	int m_nWidth;								// �����̖ʂ̐�
	int m_nDepth;								// ���s�̖ʂ̐�

	int m_nWidthPoint;							// �����̒��_��
	int m_nDepthPoint;							// ���s�̒��_��

	float m_fWidthMax;							// �����̍ő�T�C�Y
	float m_fDepthMax;							// ���s�̍ő�T�C�Y

	int m_nNor;									// �@�������߂�|���S����
	D3DXVECTOR3 *m_pNor;						// �@���̒l
	int m_nIdxNor;								// ���삷��C���f�b�N�X

	/**/

	int m_nCntWave;								// �g�`�A�j���[�V�����̃J�E���g

	float m_fHeightWave;						// �g�̍���
	float m_fDistanceWave;						// �g�`�Ԋu
	float m_fSpeedWave;							// �g�̑���

	SYNTHETICTYPE m_syntheticType;				// �����̎��
	WAVETYPE m_waveType;						// �g�̎��
	bool m_bCutTex;								// �e�N�X�`�������̗L��
	bool m_bWireFrame;							// ���C���[�t���[���̗L����		

	TEXMOVE	m_texMove;							// �e�N�X�`�����W�ړ����@
	TEXTUREINFO m_texInfo;						// ���쒆�̃e�N�X�`�����

	// �e�N�X�`���̏��
	D3DXVECTOR2 m_aTexMove[TEXTUREINFO_MAX];	// �����ړ���
	float m_aTexMoveRot[TEXTUREINFO_MAX];		// �e�N�X�`�����W�̗�������(Y��)
	float m_aTexMoveSpeed[TEXTUREINFO_MAX];		// ����鑬���̔{��
	int m_nTextureNum;							// �e�N�X�`���̑���
	int m_aTextureIdx[TEXTUREINFO_MAX];			// �e�N�X�`���C���f�b�N�X

	CScene2D *m_apScene2D[TEXTUREINFO_MAX];		// �e�N�X�`����`�悷��UI

	// �\�L������
	char *m_pWaveType[WAVETYPE_MAX] =
	{
		{ "�g�`����" },
		{ "X������" },
		{ "Z������" },
		{ "XZ���o��" },
		{ "�~�`�g��" }
	};

	char *m_pSyntheticType[SYNTHETICTYPE_MAX] =
	{
		{ "��������" },
		{ "���Z����" },
		{ "���Z����" },
		{ "��Z����" }
	};

	char *m_pTex[TEXTUREINFO_MAX] = 
	{
		{ "�e�N�X�`��(��)" },
		{ "�e�N�X�`��(��)" }
	};

	char *m_apOnOff[2] =
	{
		{"OFF"},
		{"ON"}
	};
};

#endif
