//==============================================================================
//
// �c�@�\���̕`��kstock.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "stock.h"
#include "player.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define STOCK_POSX		(600)	// �c�@�\����X���W
#define STOCK_POSY		(676)	// �c�@�\����Y���W
#define STOCK_SIZEX		(20)	// �c�@�\���̕�
#define STOCK_SIZEY		(30)	// �c�@�\���̍���

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureStock = NULL;		// ���[�U�[�C���^�[�t�F�[�X�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStock = NULL;	// �o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posStock;							// �c�@�\���̒��S�̈ʒu

//==============================================================================
// �c�@�\���̏���������
//==============================================================================
HRESULT InitStock(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureStock);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffStock,
		NULL)))
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

	// ���[�U�[�C���^�[�t�F�[�X�̒��S���W
	g_posStock = D3DXVECTOR3(STOCK_POSX, STOCK_POSY, 0.0f);

	// �|���S���̊e���_���W
	pVtx[0].pos = D3DXVECTOR3(g_posStock.x - STOCK_SIZEX, g_posStock.y + STOCK_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posStock.x - STOCK_SIZEX, g_posStock.y - STOCK_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posStock.x + STOCK_SIZEX, g_posStock.y + STOCK_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posStock.x + STOCK_SIZEX, g_posStock.y - STOCK_SIZEY, 0.0f);

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
	g_pVtxBuffStock->Unlock();

	return S_OK;
}

//==============================================================================
// �c�@�\���̏I������
//==============================================================================
void UninitStock(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffStock != NULL)
	{
		g_pVtxBuffStock->Release();
		g_pVtxBuffStock = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureStock != NULL)
	{
		g_pTextureStock->Release();
		g_pTextureStock = NULL;
	}
}

//==============================================================================
// �c�@�\���̍X�V����
//==============================================================================
void UpdateStock(void)
{
	VERTEX_2D *pVtx;
	PLAYER *Player;

	// �v���C���[�̎擾
	Player = GetPlayer();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f + ((float)Player->nStock / 10), 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + ((float)Player->nStock / 10), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f + ((float)Player->nStock / 10), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + ((float)Player->nStock / 10), 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStock->Unlock();
}

//==============================================================================
// �c�@�\���̕`�揈��
//==============================================================================
void DrawStock(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffStock,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTextureStock);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,						// �`����J�n���钸�_�C���f�b�N�X
		2);						// �`�悷��v���~�e�B�u��
}