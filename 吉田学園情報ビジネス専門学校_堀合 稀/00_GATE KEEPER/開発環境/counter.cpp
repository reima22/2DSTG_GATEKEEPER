//==============================================================================
//
// �J�E���^�[�̕`��kcounter.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "counter.h"
#include "enemy.h"
#include "player.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define COUNTER_SIZEX	(10)	// �J�E���^�[�̕�
#define COUNTER_SIZEY	(20)	// �J�E���^�[�̍���
#define DESTROY_POSX	(720)	// ���j����X���W
#define DESTROY_POSY	(690)	// ���j����Y���W
#define COUNTER_IN_POSX	(840)	// �N������X���W
#define COUNTER_IN_POSY	(690)	// �N������Y���W
#define DESTROY_MAX		(3)		// ���j���̌���
#define COUNTER_IN_MAX	(2)		// �N�����̌���
#define ALL_MAX			(5)		// �g�p���鑍����

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureCounter = NULL;		// �J�E���g�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCounter = NULL;	// �o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posDestroy[DESTROY_MAX];				// ���j���\���̒��S�̈ʒu
D3DXVECTOR3 g_posInCount[COUNTER_IN_MAX];			// �N�����\���̒��S�̈ʒu				

//==============================================================================
// �J�E���^�[�̏���������
//==============================================================================
HRESULT InitCounter(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �w�i�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureCounter);	

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ALL_MAX,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffCounter,
		NULL)))
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < DESTROY_MAX; nCnt++, pVtx = pVtx + 4)
	{
		// ���j���J�E���^�[1���ڂ̒��S���W
		g_posDestroy[nCnt] = D3DXVECTOR3(DESTROY_POSX + ((float)nCnt * 20.0f), DESTROY_POSY, 0.0f);

		// �|���S���̊e���_���W
		pVtx[0].pos = D3DXVECTOR3(g_posDestroy[nCnt].x - COUNTER_SIZEX, g_posDestroy[nCnt].y + COUNTER_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posDestroy[nCnt].x - COUNTER_SIZEX, g_posDestroy[nCnt].y - COUNTER_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posDestroy[nCnt].x + COUNTER_SIZEX, g_posDestroy[nCnt].y + COUNTER_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posDestroy[nCnt].x + COUNTER_SIZEX, g_posDestroy[nCnt].y - COUNTER_SIZEY, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCounter->Unlock();


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCounter = 0; nCounter < COUNTER_IN_MAX; nCounter++, pVtx += 4)
	{
		// �N�����J�E���^�[�̒��S���W
		g_posInCount[nCounter] = D3DXVECTOR3(COUNTER_IN_POSX + ((float)nCounter * 20.0f), COUNTER_IN_POSY, 0.0f);

		// �|���S���̊e���_���W
		pVtx[12].pos = D3DXVECTOR3(g_posInCount[nCounter].x - COUNTER_SIZEX, g_posInCount[nCounter].y + COUNTER_SIZEY, 0.0f);
		pVtx[13].pos = D3DXVECTOR3(g_posInCount[nCounter].x - COUNTER_SIZEX, g_posInCount[nCounter].y - COUNTER_SIZEY, 0.0f);
		pVtx[14].pos = D3DXVECTOR3(g_posInCount[nCounter].x + COUNTER_SIZEX, g_posInCount[nCounter].y + COUNTER_SIZEY, 0.0f);
		pVtx[15].pos = D3DXVECTOR3(g_posInCount[nCounter].x + COUNTER_SIZEX, g_posInCount[nCounter].y - COUNTER_SIZEY, 0.0f);

		// rhw�̐ݒ�
		pVtx[12].rhw = 1.0f;
		pVtx[13].rhw = 1.0f;
		pVtx[14].rhw = 1.0f;
		pVtx[15].rhw = 1.0f;

		// �e���_�J���[�̐ݒ�
		pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// ���_���W�̐ݒ�
		pVtx[12].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[13].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[14].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[15].tex = D3DXVECTOR2(0.1f, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCounter->Unlock();

	return S_OK;
}

//==============================================================================
// �J�E���^�[�̏I������
//==============================================================================
void UninitCounter(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffCounter != NULL)
	{
		g_pVtxBuffCounter->Release();
		g_pVtxBuffCounter = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureCounter != NULL)
	{
		g_pTextureCounter->Release();
		g_pTextureCounter = NULL;
	}
}

//==============================================================================
// �J�E���^�[�̍X�V����
//==============================================================================
void UpdateCounter(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	PLAYER *Player;
	ENEMY *pEnemy;
	ENEMYINFO info = GetInfo();
	int aNumberD[DESTROY_MAX];						// ���j���̊e����
	int aNumberC[COUNTER_IN_MAX];					// �N�����̊e����
	int nDataD = (int)pow(10, DESTROY_MAX - 1);		// ���j���̌���
	int nDataC = (int)pow(10, COUNTER_IN_MAX - 1);	// �N�����̌���

	// �v���C���[�̎擾
	Player = GetPlayer();

	// �G�̎擾
	pEnemy = GetEnemy();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCounter->Lock(0, 0, (void**)&pVtx, 0);

	// ���j���J�E���^�[���_���W�̍X�V
	for (int nCnt = 0; nCnt < DESTROY_MAX; nCnt++, pVtx = pVtx + 4)
	{
		aNumberD[nCnt] = (info.nDestroy / nDataD) % 10;
		nDataD /= 10;

		pVtx[0].tex = D3DXVECTOR2(0.0f + ((float)aNumberD[nCnt] / 10), 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f + ((float)aNumberD[nCnt] / 10), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f + ((float)aNumberD[nCnt] / 10), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + ((float)aNumberD[nCnt] / 10), 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCounter->Unlock();


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCounter->Lock(0, 0, (void**)&pVtx, 0);

	// �N���J�E���^�[�̒��_���W�̍X�V
	for (int nCnt = 0; nCnt < COUNTER_IN_MAX; nCnt++, pVtx += 4)
	{
		aNumberC[nCnt] = (Player->nLineOver / nDataC) % 10;
		nDataC /= 10;

		pVtx[12].tex = D3DXVECTOR2(0.0f + ((float)aNumberC[nCnt] / 10), 1.0f);
		pVtx[13].tex = D3DXVECTOR2(0.0f + ((float)aNumberC[nCnt] / 10), 0.0f);
		pVtx[14].tex = D3DXVECTOR2(0.1f + ((float)aNumberC[nCnt] / 10), 1.0f);
		pVtx[15].tex = D3DXVECTOR2(0.1f + ((float)aNumberC[nCnt] / 10), 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCounter->Unlock();
}

//==============================================================================
// �J�E���^�[�̕`�揈��
//==============================================================================
void DrawCounter(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffCounter,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCnt = 0; nCnt < ALL_MAX; nCnt++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureCounter);

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
			nCnt * 4,				// �`����J�n���钸�_�C���f�b�N�X
			2);						// �`�悷��v���~�e�B�u��
	}
}