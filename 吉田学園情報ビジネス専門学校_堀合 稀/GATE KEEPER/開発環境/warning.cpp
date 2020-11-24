//==============================================================================
//
// �x����ʂ̕`��kwarning.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "warning.h"
#include "enemy.h"
#include "sound.h"

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPDIRECT3DTEXTURE9 g_apTextureWarning = NULL;		// �x����ʂ̃e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWarning = NULL;	// �x����ʂ̃o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posWarning;							// �x����ʂ̈ʒu
int g_nCntAlert;									// �x�����J�E���g
float g_fAlpha;										// ��ʂ̃A���t�@�l
float g_fChange;									// �A���t�@�l�̕ω��l
PHASESTATE phase;									// �x���̏��
int g_nPhaseCnt;									// �x���\���J�E���g

//==============================================================================
// �x����ʂ̏���������
//==============================================================================
HRESULT InitWarning(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/warning.png",
		&g_apTextureWarning);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffWarning,
		NULL)))
	{
		return E_FAIL;
	}

	// �ϐ��̏�����
	g_fAlpha = 0.0f;
	g_fChange = 0.03f;
	g_nCntAlert = 0;
	phase = PHASESTATE_NORMAL;
	g_nPhaseCnt = 0;

	// ���_�o�b�t�@�����b�N����
	g_pVtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

	// �x����ʂ̒��S���W
	g_posWarning = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	// �|���S���̊e���_���W
	pVtx[0].pos = D3DXVECTOR3(g_posWarning.x - (SCREEN_WIDTH / 2), g_posWarning.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posWarning.x - (SCREEN_WIDTH / 2), g_posWarning.y - (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posWarning.x + (SCREEN_WIDTH / 2), g_posWarning.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posWarning.x + (SCREEN_WIDTH / 2), g_posWarning.y - (SCREEN_HEIGHT / 2), 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// �e���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	// ���_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWarning->Unlock();

	return S_OK;
}

//==============================================================================
// �x����ʂ̏I������
//==============================================================================
void UninitWarning(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffWarning != NULL)
	{
		g_pVtxBuffWarning->Release();
		g_pVtxBuffWarning = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_apTextureWarning != NULL)
	{
		g_apTextureWarning->Release();
		g_apTextureWarning = NULL;
	}
}

//==============================================================================
// �x����ʂ̍X�V����
//==============================================================================
void UpdateWarning(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	ENEMYINFO info = GetInfo();	// �G�S�̏��̎擾

	// �t�F�[�Y�̐؂�ւ�
	if (info.nDestroy == BIGENEMY_APPEAR && g_nPhaseCnt < 180)
	{
		phase = PHASESTATE_WARNING;	// �x����Ԃֈڍs

		// �J�E���g�J�n
		g_nPhaseCnt++;

		// �J�E���g�I��
		if (g_nPhaseCnt >= 180)
		{
			phase = PHASESTATE_NORMAL;	// �ʏ��Ԃֈڍs

			// �T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_BGM005);
		}
	}

	// �t�F�[�Y�̏��
	switch (phase)
	{
	case PHASESTATE_NORMAL:
		g_nCntAlert = 0;
		g_fAlpha = 0.0f;

		// ���_�o�b�t�@�����b�N����
		g_pVtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

		// �e�N�X�`���̍X�V
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffWarning->Unlock();

		break;

	case PHASESTATE_WARNING:
		// �J�E���g�J�n
		if (g_nCntAlert <= 180)
		{
			g_nCntAlert++;
		}

		if (g_nCntAlert % 1 == 0)
		{
			// �����x�̕ω�
			g_fAlpha += g_fChange;

			// �����x�̏�Ԃŉ����Z�̕ω�
			if (g_fAlpha <= 0.0f)
			{
				g_fChange = 0.03f;
			}
			else if (g_fAlpha >= 1.0f)
			{
				g_fChange = -0.03f;
			}

			// ���_�o�b�t�@�����b�N����
			g_pVtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

			// �e�N�X�`���̍X�V
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffWarning->Unlock();
		}

		break;

	default:
		break;
	}
}

//==============================================================================
// �x����ʂ̕`�揈��
//==============================================================================
void DrawWarning(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffWarning,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureWarning);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,						// �`����J�n���钸�_�C���f�b�N�X
		2);						// �`�悷��v���~�e�B�u��
}

//==============================================================================
// �x����ʂ̏��
//==============================================================================
PHASESTATE GetPhase(void)
{
	return phase;
}