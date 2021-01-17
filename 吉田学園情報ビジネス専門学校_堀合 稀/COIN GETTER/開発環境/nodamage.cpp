//==============================================================================
//
// �m�[�_���[�W�{�[�i�X�̕`��knodamage.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "nodamage.h"
#include "player.h"
#include "score.h"	
#include "sound.h"
#include "game.h"
#include "input.h"
#include "gamepad.h"

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureNodamage[TEX_BONUS] = {};	// �e�N�X�`���[�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffNodamage = NULL;		// �o�b�t�@�ւ̃|�C���^
BONUS Bonus;											// �{�[�i�X�̍\����

//==============================================================================
// �m�[�_���[�W�{�[�i�X�̏���������
//==============================================================================
HRESULT InitNodamage(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	int nStage = GetStage();

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/nodamage_logo.png",
		&g_pTextureNodamage[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureNodamage[1]);

	// ���_���̐ݒ�
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_POLY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffNodamage,
		NULL)))
	{
		return E_FAIL;
	}

	// �ϐ��̏�����
	Bonus.colNodamage = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	Bonus.nBonusCnt = 0;
	if (nStage == 0)
	{
		Bonus.nBonus = BONUS_STAGE1;
	}
	else if (nStage == 1)
	{
		Bonus.nBonus = BONUS_STAGE2;
	}
	else if (nStage == 2)
	{
		Bonus.nBonus = BONUS_STAGE3;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffNodamage->Lock(0, 0, (void**)&pVtx, 0);

	// �e�̒��S���W�̐ݒ�
	Bonus.posNodamage = D3DXVECTOR3(LOGO_POSX, LOGO_POSY, 0.0f);
	

	// �e�̒��_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(Bonus.posNodamage.x - LOGO_SIZEX, Bonus.posNodamage.y + LOGO_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Bonus.posNodamage.x - LOGO_SIZEX, Bonus.posNodamage.y - LOGO_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(Bonus.posNodamage.x + LOGO_SIZEX, Bonus.posNodamage.y + LOGO_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Bonus.posNodamage.x + LOGO_SIZEX, Bonus.posNodamage.y - LOGO_SIZEY, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// �e���_�J���[�̐ݒ�
	pVtx[0].col = Bonus.colNodamage;
	pVtx[1].col = Bonus.colNodamage;
	pVtx[2].col = Bonus.colNodamage;
	pVtx[3].col = Bonus.colNodamage;

	// �e�N�X�`�����_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffNodamage->Unlock();


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffNodamage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BONUS; nCnt++,pVtx += 4)
	{
		// �e�̒��S���W�̐ݒ�
		Bonus.posBonus[nCnt] = D3DXVECTOR3(BONUS_POSX + (nCnt * 30.0f), BONUS_POSY, 0.0f);

		// �e�̒��_���W�̐ݒ�
		pVtx[4].pos = D3DXVECTOR3(Bonus.posBonus[nCnt].x - BONUS_SIZEX, Bonus.posBonus[nCnt].y + BONUS_SIZEY, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(Bonus.posBonus[nCnt].x - BONUS_SIZEX, Bonus.posBonus[nCnt].y - BONUS_SIZEY, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(Bonus.posBonus[nCnt].x + BONUS_SIZEX, Bonus.posBonus[nCnt].y + BONUS_SIZEY, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(Bonus.posBonus[nCnt].x + BONUS_SIZEX, Bonus.posBonus[nCnt].y - BONUS_SIZEY, 0.0f);

		// rhw�̐ݒ�
		pVtx[4].rhw = 1.0f;
		pVtx[5].rhw = 1.0f;
		pVtx[6].rhw = 1.0f;
		pVtx[7].rhw = 1.0f;

		// �e���_�J���[�̐ݒ�
		pVtx[4].col = Bonus.colNodamage;
		pVtx[5].col = Bonus.colNodamage;
		pVtx[6].col = Bonus.colNodamage;
		pVtx[7].col = Bonus.colNodamage;

		// �e�N�X�`�����_���̐ݒ�
		pVtx[4].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(0.1f, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffNodamage->Unlock();

	return S_OK;
}

//==============================================================================
// �m�[�_���[�W�{�[�i�X�̏I������
//==============================================================================
void UninitNodamage(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffNodamage != NULL)
	{
		g_pVtxBuffNodamage->Release();
		g_pVtxBuffNodamage = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < TEX_BONUS; nCnt++)
	{
		if (g_pTextureNodamage[nCnt] != NULL)
		{
			g_pTextureNodamage[nCnt]->Release();
			g_pTextureNodamage[nCnt] = NULL;
		}
	}
}

//==============================================================================
// �m�[�_���[�W�{�[�i�X�̍X�V����
//==============================================================================
void UpdateNodamage(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	int aNumber[MAX_BONUS];
	int nData = (int)pow(10, (MAX_BONUS - 1));

	AddScoreBonus();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffNodamage->Lock(0, 0, (void**)&pVtx, 0);

	// �e���̃e�N�X�`���̐؂�ւ�
	for (int nCnt = 0; nCnt < MAX_BONUS; nCnt++)
	{
		aNumber[nCnt] = (Bonus.nBonus / nData) % 10;
		nData /= 10;

		// �e�N�X�`���̒��_���W�̍X�V
		pVtx[4].tex = D3DXVECTOR2(0.0f + ((float)aNumber[nCnt] / 10), 1.0f);
		pVtx[5].tex = D3DXVECTOR2(0.0f + ((float)aNumber[nCnt] / 10), 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.1f + ((float)aNumber[nCnt] / 10), 1.0f);
		pVtx[7].tex = D3DXVECTOR2(0.1f + ((float)aNumber[nCnt] / 10), 0.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffNodamage->Unlock();
}

//==============================================================================
// �m�[�_���[�W�{�[�i�X�̕`�揈��
//==============================================================================
void DrawNodamage(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffNodamage,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCnt = 0; nCnt < MAX_POLY; nCnt++)
	{
		if (nCnt == 0)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureNodamage[0]);
		}
		else
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureNodamage[1]);
		}

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
			nCnt * 4,				// �`����J�n���钸�_�C���f�b�N�X
			2);						// �`�悷��v���~�e�B�u��
	}
}

//==============================================================================
// �{�[�i�X���Z�̏���
//==============================================================================
void AddScoreBonus(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	PLAYER *Player;
	// �v���C���[�̎擾
	Player = GetPlayer();

	// �m�[�_���[�W�N���A����
	if (Player->state == PLAYERSTATE_CLEAR && Player->nLife == MAX_LIFE)
	{
		Bonus.nBonusCnt++;	// �J�E���g�̉��Z
		if (Bonus.nBonusCnt == 40)
		{
			PlaySound(SOUND_LABEL_SE_NODAMAGE);	// �T�E���h�Đ�
		}

		if (Bonus.nBonusCnt < 180)
		{// ��3�b��
			if (Bonus.nBonusCnt % 20 == 0 && Bonus.nBonusCnt % 40 != 0)
			{// 3����1�b���Ƃɓ_��
				Bonus.colNodamage.a = 0.0f;
			}
			else if (Bonus.nBonusCnt % 40 == 0)
			{
				Bonus.colNodamage.a = 1.0f;
			}
		}
		else if (Bonus.nBonusCnt >= 180)
		{// ��3�b�o�ߌ�A�X�R�A�ւ̊��Z
			Bonus.colNodamage.a = 1.0f;	// �����x����

			if (Bonus.nBonus > 0)
			{// �X�R�A���Z�����i�{�[�i�X��0�ɂȂ�܂Łj
				Bonus.nBonus -= 2000;				// �{�[�i�X��������
				AddScore(2000);						// �X�R�A�̉��Z
				PlaySound(SOUND_LABEL_SE_SCORE_UP);	// �T�E���h�Đ�

				if (GetKeyboardTrigger(KEYINFO_OK) == true || GetKeyboardTrigger(KEYINFO_JUMP) == true || 
					IsButtonDown(KEYINFO::KEYINFO_JUMP) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
				{// �{�^�����͂ŏȗ�
					AddScore(Bonus.nBonus);
					Bonus.nBonus -= Bonus.nBonus;
					PlaySound(SOUND_LABEL_SE_SCORE_UP);	// �T�E���h�Đ�
				}

				if (Bonus.nBonus <= 0)
				{
					Bonus.nBonus = 0;
				}
			}
		}
	}
	else if(Player->state == PLAYERSTATE_CLEAR && Player->nLife != MAX_LIFE)
	{// ���s
		Bonus.nBonus = 0;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffNodamage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_POLY; nCnt++, pVtx += 4)
	{
		// �e���_�J���[�̐ݒ�
		pVtx[0].col = Bonus.colNodamage;
		pVtx[1].col = Bonus.colNodamage;
		pVtx[2].col = Bonus.colNodamage;
		pVtx[3].col = Bonus.colNodamage;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffNodamage->Unlock();
}

//==============================================================================
// �{�[�i�X�̎擾
//==============================================================================
BONUS GetBonus(void)
{
	return Bonus;
}