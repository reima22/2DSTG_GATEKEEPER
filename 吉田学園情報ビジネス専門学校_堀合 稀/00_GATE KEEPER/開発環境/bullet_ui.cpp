//==============================================================================
//
// �e���[�U�[�C���^�[�t�F�[�X�̕`��kbullet_ui.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "bullet_ui.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "gamepad.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define BULLET_UI_POLYGON	(4)			// UI�̃|���S����
#define UI_BULLET_SIZEX		(32)		// �eUI�̕�
#define UI_BULLET_SIZEY		(34)		// �eUI�̍���
#define UI_BULLET_POSX		(48.0f)		// �e��UI��X���W
#define UI_BULLET_POSY		(676.0f)	// �e��UI��X���W
#define CUT_UI_TEXTURE		(3.0f)		// �e(BCD)UI�e�N�X�`���̕�����

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureBulletUi[BULLET_UI_POLYGON] = {};	// ���[�U�[�C���^�[�t�F�[�X�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBulletUi = NULL;				// �o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posBulletUi;										// �e���[�U�[�C���^�[�t�F�[�X(���C��)�̒��S�̈ʒu
float g_fBullet;												// �ʏ�e�̃e�N�X�`������
float g_fBulletPlus[BULLET_PLUS];								// �ǉ��e�̃e�N�X�`������

//==============================================================================
// �e���[�U�[�C���^�[�t�F�[�X�̏���������
//==============================================================================
HRESULT InitBulletUI(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �w�i�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bullet_UI000.png",
		&g_pTextureBulletUi[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bullet_UI001.png",
		&g_pTextureBulletUi[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bullet_UI002.png",
		&g_pTextureBulletUi[2]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bullet_UI003.png",
		&g_pTextureBulletUi[3]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * BULLET_UI_POLYGON,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBulletUi,
		NULL)))
	{
		return E_FAIL;
	}

	g_fBullet = 0.0f;
	for (int nCnt = 0; nCnt < BULLET_PLUS; nCnt++)
	{
		g_fBulletPlus[nCnt] = 0.0f;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBulletUi->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < BULLET_UI_POLYGON; nCnt++, pVtx += 4)
	{
		// �e���[�U�[�C���^�[�t�F�[�X�̒��S���W
		g_posBulletUi = D3DXVECTOR3(UI_BULLET_POSX + ((float)nCnt * 80.0f), UI_BULLET_POSY, 0.0f);

		// �|���S���̊e���_���W
		pVtx[0].pos = D3DXVECTOR3(g_posBulletUi.x - UI_BULLET_SIZEX, g_posBulletUi.y + UI_BULLET_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posBulletUi.x - UI_BULLET_SIZEX, g_posBulletUi.y - UI_BULLET_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posBulletUi.x + UI_BULLET_SIZEX, g_posBulletUi.y + UI_BULLET_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posBulletUi.x + UI_BULLET_SIZEX, g_posBulletUi.y - UI_BULLET_SIZEY, 0.0f);

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
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBulletUi->Unlock();

	return S_OK;
}

//==============================================================================
// �e���[�U�[�C���^�[�t�F�[�X�̏I������
//==============================================================================
void UninitBulletUI(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBulletUi != NULL)
	{
		g_pVtxBuffBulletUi->Release();
		g_pVtxBuffBulletUi = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < BULLET_UI_POLYGON; nCnt++)
	{
		if (g_pTextureBulletUi[nCnt] != NULL)
		{
			g_pTextureBulletUi[nCnt]->Release();
			g_pTextureBulletUi[nCnt] = NULL;
		}
	}
}

//==============================================================================
// �e���[�U�[�C���^�[�t�F�[�X�̍X�V����
//==============================================================================
void UpdateBulletUI(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	PLAYER *Player;
	SETBULLET state;

	// �v���C���[�̎擾
	Player = GetPlayer();

	// �e�̑��U��Ԃ̎擾
	state = GetState();

	// �A�C�e���擾�ɂ��UI�e�N�X�`���̐؂�ւ�
	for (int nCnt = 0; nCnt < BULLET_PLUS; nCnt++)
	{
		if (Player->aBullet[nCnt] == true && g_fBulletPlus[nCnt] == 0.0f)
		{
			g_fBulletPlus[nCnt] += 1.0f / CUT_UI_TEXTURE;
		}
	}

	// �A�C�e���؂�ւ��ɂ��e�N�X�`���̐؂�ւ�
	if (state == SETBULLET_NORMAL && Player->aBullet[0] == true)
	{ // �ʏ�e����E�F�[�u��
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_fBullet += 0.5f;
			g_fBulletPlus[0] += 1.0f / CUT_UI_TEXTURE;
		}
	}
	else if (state == SETBULLET_NORMAL && Player->aBullet[0] == false && Player->aBullet[1] == true)
	{ // �ʏ�e����~�T�C����
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_fBullet += 0.5f;
			g_fBulletPlus[1] += 1.0f / CUT_UI_TEXTURE;
		}
	}
	else if (state == SETBULLET_NORMAL && Player->aBullet[0] == false && Player->aBullet[1] == false && Player->aBullet[2] == true)
	{ // �ʏ�e���烊�o�[�X�E�F�[�u��
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_fBullet += 0.5f;
			g_fBulletPlus[2] += 1.0f / CUT_UI_TEXTURE;
		}
	}
	else if (state == SETBULLET_WAVE && Player->aBullet[1] == true)
	{ // �E�F�[�u����~�T�C����
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_fBulletPlus[0] -= 1.0f / CUT_UI_TEXTURE;
			g_fBulletPlus[1] += 1.0f / CUT_UI_TEXTURE;
		}
	}
	else if (state == SETBULLET_WAVE && Player->aBullet[1] == false && Player->aBullet[2] == true)
	{ // �E�F�[�u���烊�o�[�X�E�F�[�u��
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_fBulletPlus[0] -= 1.0f / CUT_UI_TEXTURE;
			g_fBulletPlus[2] += 1.0f / CUT_UI_TEXTURE;
		}
	}
	else if (GetKeyboardTrigger(KEYINFO_CHANGE) == true && state == SETBULLET_WAVE && Player->aBullet[1] == false && Player->aBullet[2] == false)
	{ // �E�F�[�u����ʏ�e��
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_fBullet -= 0.5f;
			g_fBulletPlus[0] -= 1.0f / CUT_UI_TEXTURE;
		}
	}
	else if (state == SETBULLET_MISSILE && Player->aBullet[2] == true)
	{ // �~�T�C�����烊�o�[�X�E�F�[�u��
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_fBulletPlus[1] -= 1.0f / CUT_UI_TEXTURE;
			g_fBulletPlus[2] += 1.0f / CUT_UI_TEXTURE;
		}
	}
	else if (state == SETBULLET_MISSILE && Player->aBullet[2] == false)
	{ // �~�T�C������ʏ�e��
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_fBullet -= 0.5f;
			g_fBulletPlus[1] -= 1.0f / CUT_UI_TEXTURE;
		}
	}
	else if (state == SETBULLET_REVERSEWAVE)
	{ // ���o�[�X�E�F�[�u����ʏ�e��
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_fBullet -= 0.5f;
			g_fBulletPlus[2] -= 1.0f / CUT_UI_TEXTURE;
		}
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBulletUi->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f + g_fBullet, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + g_fBullet, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.5f + g_fBullet, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f + g_fBullet, 0.0f);

	for (int nCnt = 0; nCnt < BULLET_PLUS; nCnt++,pVtx += 4)
	{
		pVtx[4].tex = D3DXVECTOR2(0.0f + g_fBulletPlus[nCnt], 1.0f);
		pVtx[5].tex = D3DXVECTOR2(0.0f + g_fBulletPlus[nCnt], 0.0f);
		pVtx[6].tex = D3DXVECTOR2(1.0f / 3.0f + g_fBulletPlus[nCnt], 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f / 3.0f + g_fBulletPlus[nCnt], 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBulletUi->Unlock();
}

//==============================================================================
// �e���[�U�[�C���^�[�t�F�[�X�̕`�揈��
//==============================================================================
void DrawBulletUI(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffBulletUi,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < BULLET_UI_POLYGON; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureBulletUi[nCnt]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
			nCnt * 4,				// �`����J�n���钸�_�C���f�b�N�X
			2);						// �`�悷��v���~�e�B�u��
	}
}