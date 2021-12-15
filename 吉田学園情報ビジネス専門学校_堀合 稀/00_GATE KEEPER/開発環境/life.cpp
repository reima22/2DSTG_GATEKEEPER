//==============================================================================
//
// �̗͂̕`��klife.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "player.h"
#include "life.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MAX_LIFE	(3)			// �̗͂̍ő�l
#define LIFE_SHIFT	(60)		// �̗͂̂��炵�z�u
#define LIFE_POSX	(30.0f)		// �̗͂�X���W
#define LIFE_POSY	(40.0f)		// �̗͂�Y���W
#define LIFE_SIZEX	(30.0f)		// �̗̓T�C�Y�̕�
#define LIFE_SIZEY	(30.0f)		// �̗̓T�C�Y�̍���

//==============================================================================
// �̗͂̍\����
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// �̗͂̈ʒu
	float fLifeAlpha;	// �̗͂̓����x
} LIFESTATE;

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;		// �e�e�N�X�`���[�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;	// �e�o�b�t�@�ւ̃|�C���^
LIFESTATE lifestate[MAX_LIFE];					// �̗͂̍\����

//==============================================================================
// �̗͂̏���������
//==============================================================================
HRESULT InitLife(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/life000.png",
		&g_pTextureLife);

	// �ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
	{
		lifestate[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		lifestate[nCnt].fLifeAlpha = 1.0f;
	}

	// ���_���̐ݒ�
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL)))
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		// �e�̒��S���W�̐ݒ�
		lifestate[nCntLife].pos = D3DXVECTOR3(LIFE_POSX + (nCntLife * LIFE_SHIFT), LIFE_POSY, 0.0f);

		// �e�̒��_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(lifestate[nCntLife].pos.x - LIFE_SIZEX, lifestate[nCntLife].pos.y + LIFE_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(lifestate[nCntLife].pos.x - LIFE_SIZEX, lifestate[nCntLife].pos.y - LIFE_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(lifestate[nCntLife].pos.x + LIFE_SIZEX, lifestate[nCntLife].pos.y + LIFE_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(lifestate[nCntLife].pos.x + LIFE_SIZEX, lifestate[nCntLife].pos.y - LIFE_SIZEY, 0.0f);

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

		// �e�N�X�`�����_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;	// ���_�|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife->Unlock();

	return S_OK;
}

//==============================================================================
// �̗͂̏I������
//==============================================================================
void UninitLife(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}
}

//==============================================================================
// �̗͂̍X�V����
//==============================================================================
void UpdateLife(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	PLAYER *Player;

	// �v���C���[�̎擾
	Player = GetPlayer();

	// �e�N�X�`���̓����x�̕ύX
	if (Player->nLife != MAX_LIFE)
	{
		// �����x�ő�
		lifestate[Player->nLife].fLifeAlpha = 0.0f;

		if (Player->nLife == 0)
		{ // �̗�0�̎�
			for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
			{ // �����x�����ׂčő��
				lifestate[nCnt].fLifeAlpha = 0.0f;
			}
		}
	}
	else if(Player->nLife == MAX_LIFE)
	{ // �̗͍ő��
		for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
		{
			// �����x�ŏ���
			lifestate[nCnt].fLifeAlpha = 1.0f;
		}
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++,pVtx = pVtx + 4)
	{
		// �e���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, lifestate[nCnt].fLifeAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, lifestate[nCnt].fLifeAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, lifestate[nCnt].fLifeAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, lifestate[nCnt].fLifeAlpha);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife->Unlock();
}

//==============================================================================
// �̗͂̕`�揈��
//==============================================================================
void DrawLife(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffLife,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureLife);

	// �|���S���̕`��
	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
	{
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
			nCnt * 4,				// �`����J�n���钸�_�C���f�b�N�X
			2);						// �`�悷��v���~�e�B�u��
	}
}