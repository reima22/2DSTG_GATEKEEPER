//==============================================================================
//
// ���[�U�[�C���^�[�t�F�[�X�̕`��kui.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "ui.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define UI_POSX			(640)	// ���[�U�[�C���^�[�t�F�[�X�̒��SX���W
#define UI_POSY			(676)	// ���[�U�[�C���^�[�t�F�[�X�̒��SY���W
#define UI_SIZEX		(640)	// ���[�U�[�C���^�[�t�F�[�X�̒��S����̕�
#define UI_SIZEY		(44)	// ���[�U�[�C���^�[�t�F�[�X�̒��S����̍���

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureUi = NULL;	// ���[�U�[�C���^�[�t�F�[�X�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi = NULL;	// �o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posUi;							// ���[�U�[�C���^�[�t�F�[�X(���C��)�̒��S�̈ʒu

//==============================================================================
// ���[�U�[�C���^�[�t�F�[�X�̏���������
//==============================================================================
HRESULT InitUI(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �w�i�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/Main_UI00.png",
		&g_pTextureUi);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi,
		NULL)))
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi->Lock(0, 0, (void**)&pVtx, 0);

	// ���[�U�[�C���^�[�t�F�[�X�̒��S���W
	g_posUi = D3DXVECTOR3(UI_POSX, UI_POSY, 0.0f);

	// �|���S���̊e���_���W
	pVtx[0].pos = D3DXVECTOR3(g_posUi.x - UI_SIZEX, g_posUi.y + UI_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posUi.x - UI_SIZEX, g_posUi.y - UI_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posUi.x + UI_SIZEX, g_posUi.y + UI_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posUi.x + UI_SIZEX, g_posUi.y - UI_SIZEY, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// �e���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// ���_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi->Unlock();

	return S_OK;
}

//==============================================================================
// ���[�U�[�C���^�[�t�F�[�X�̏I������
//==============================================================================
void UninitUI(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffUi != NULL)
	{
		g_pVtxBuffUi->Release();
		g_pVtxBuffUi = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureUi != NULL)
	{
		g_pTextureUi->Release();
		g_pTextureUi = NULL;
	}
}

//==============================================================================
// ���[�U�[�C���^�[�t�F�[�X�̍X�V����
//==============================================================================
void UpdateUI(void)
{
}

//==============================================================================
// ���[�U�[�C���^�[�t�F�[�X�̕`�揈��
//==============================================================================
void DrawUI(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffUi,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTextureUi);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,				// �`����J�n���钸�_�C���f�b�N�X
		2);						// �`�悷��v���~�e�B�u��
}