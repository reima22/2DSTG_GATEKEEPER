//==============================================================================
//
// �|�[�Y���S�̕`��kpause_logo.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "pause_logo.h"
#include "pause.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define PAUSELOGO_POS	(D3DXVECTOR3(SCREEN_CENTERX, 100.0f, 0.0f))
#define PAUSELOGO_X		(180.0f)
#define PAUSELOGO_Y		(50.0f)
#define PAUSELOGO_COL	(D3DXCOLOR(1.0f, 1.0f, 0.01f, 1.0f))

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
PauseLogo g_PauseLogo;

//==============================================================================
// �`���[�g���A����ʂ̏���������
//==============================================================================
HRESULT InitPauseLogo(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �w�i�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/pause_logo.png",
		&g_PauseLogo.pTexture);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_PauseLogo.pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// �ϐ��̏�����
	g_PauseLogo.pos = PAUSELOGO_POS;
	g_PauseLogo.col = PAUSELOGO_COL;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_PauseLogo.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �|���S���̊e���_���W
	pVtx[0].pos = D3DXVECTOR3(g_PauseLogo.pos.x - PAUSELOGO_X, g_PauseLogo.pos.y + PAUSELOGO_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_PauseLogo.pos.x - PAUSELOGO_X, g_PauseLogo.pos.y - PAUSELOGO_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_PauseLogo.pos.x + PAUSELOGO_X, g_PauseLogo.pos.y + PAUSELOGO_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_PauseLogo.pos.x + PAUSELOGO_X, g_PauseLogo.pos.y - PAUSELOGO_Y, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// �e���_�J���[�̐ݒ�
	pVtx[0].col = g_PauseLogo.col;
	pVtx[1].col = g_PauseLogo.col;
	pVtx[2].col = g_PauseLogo.col;
	pVtx[3].col = g_PauseLogo.col;

	// ���_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	// ���_�o�b�t�@���A�����b�N����
	g_PauseLogo.pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================
// �`���[�g���A����ʂ̏I������
//==============================================================================
void UninitPauseLogo(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_PauseLogo.pVtxBuff != NULL)
	{
		g_PauseLogo.pVtxBuff->Release();
		g_PauseLogo.pVtxBuff = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_PauseLogo.pTexture != NULL)
	{
		g_PauseLogo.pTexture->Release();
		g_PauseLogo.pTexture = NULL;
	}
}

//==============================================================================
// �`���[�g���A����ʂ̍X�V����
//==============================================================================
void UpdatePauseLogo(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	PAUSE pause = GetPause();

	if (pause.bOption == true)
	{
		g_PauseLogo.col.a = 0.0f;
	}
	else
	{
		if (g_PauseLogo.bPlus == true)
		{
			g_PauseLogo.col.a += 0.01f;
		}
		else
		{
			g_PauseLogo.col.a -= 0.01f;
		}

		if (g_PauseLogo.col.a <= 0.5f)
		{
			g_PauseLogo.bPlus = true;
		}
		else if (g_PauseLogo.col.a >= 1.0f)
		{
			g_PauseLogo.bPlus = false;
		}
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_PauseLogo.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e���_�J���[�̐ݒ�
	pVtx[0].col = g_PauseLogo.col;
	pVtx[1].col = g_PauseLogo.col;
	pVtx[2].col = g_PauseLogo.col;
	pVtx[3].col = g_PauseLogo.col;

	// ���_�o�b�t�@���A�����b�N����
	g_PauseLogo.pVtxBuff->Unlock();
}

//==============================================================================
// �`���[�g���A�����S�̕`�揈��
//==============================================================================
void DrawPauseLogo(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_PauseLogo.pVtxBuff,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_PauseLogo.pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,				// �`����J�n���钸�_�C���f�b�N�X
		2);						// �`�悷��v���~�e�B�u��
}
