//==============================================================================
//
// �Q�[���J�n�����̕`��kstart.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "start.h"
#include "fade.h"
#include "sound.h"

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
Start start;

//==============================================================================
// �X�^�[�g�̏���������
//==============================================================================
HRESULT InitStart(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/three.png",
		&start.pTexture[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/two.png",
		&start.pTexture[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/one.png",
		&start.pTexture[2]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/go.png",
		&start.pTexture[3]);

	// ���_���̐ݒ�
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * START_POLY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&start.pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// �ϐ��̏�����
	start.nStartCnt = 0;
	start.bStart = false;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	start.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < START_POLY; nCnt++)
	{
		start.startInfo[nCnt].pos = D3DXVECTOR3(SCREEN_CENTERX, SCREEN_CENTERY, 0.0f);
		start.startInfo[nCnt].bUse = false;

		//�X�R�A�̒��_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(start.startInfo[nCnt].pos.x - 60.0f, start.startInfo[nCnt].pos.y + 60.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(start.startInfo[nCnt].pos.x - 60.0f, start.startInfo[nCnt].pos.y - 60.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(start.startInfo[nCnt].pos.x + 60.0f, start.startInfo[nCnt].pos.y + 60.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(start.startInfo[nCnt].pos.x + 60.0f, start.startInfo[nCnt].pos.y - 60.0f, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// �e���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		// �e�N�X�`�����_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;	// ���_�|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	start.pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================
// �X�^�[�g�̏I������
//==============================================================================
void UninitStart(void)
{
	// ���_�o�b�t�@�̊J��
	if (start.pVtxBuff != NULL)
	{
		start.pVtxBuff->Release();
		start.pVtxBuff = NULL;
	}

	for (int nCnt = 0; nCnt < START_POLY; nCnt++)
	{
		// �e�N�X�`���̊J��
		if (start.pTexture[nCnt] != NULL)
		{
			start.pTexture[nCnt]->Release();
			start.pTexture[nCnt] = NULL;
		}
	}
}

//==============================================================================
// �X�^�[�g�̍X�V����
//==============================================================================
void UpdateStart(void)
{
	// ���[�J���ϐ��錾
	FADE fade = GetFade();

	if (fade == FADE_NONE)
	{
		if (start.nStartCnt < 240)
		{
			start.nStartCnt++;
		}

		if (start.nStartCnt == 1)
		{
			start.startInfo[0].bUse = true;
			start.startInfo[1].bUse = false;
			start.startInfo[2].bUse = false;
			start.startInfo[3].bUse = false;
			PlaySound(SOUND_LABEL_SE_COUNTDOWN);
		}
		else if (start.nStartCnt == 60)
		{
			start.startInfo[0].bUse = false;
			start.startInfo[1].bUse = true;
			start.startInfo[2].bUse = false;
			start.startInfo[3].bUse = false;
			PlaySound(SOUND_LABEL_SE_COUNTDOWN);
		}
		else if (start.nStartCnt == 120)
		{
			start.startInfo[0].bUse = false;
			start.startInfo[1].bUse = false;
			start.startInfo[2].bUse = true;
			start.startInfo[3].bUse = false;
			PlaySound(SOUND_LABEL_SE_COUNTDOWN);
		}
		else if (start.nStartCnt == 180)
		{
			start.startInfo[0].bUse = false;
			start.startInfo[1].bUse = false;
			start.startInfo[2].bUse = false;
			start.startInfo[3].bUse = true;
			PlaySound(SOUND_LABEL_SE_GAMESTART);
		}
		else if (start.nStartCnt == 240)
		{
			start.startInfo[3].bUse = false;
			start.bStart = true;
		}
	}
}

//==============================================================================
// �X�^�[�g�̕`�揈��
//==============================================================================
void DrawStart(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, start.pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCntTexture = 0; nCntTexture < START_POLY; nCntTexture++)
	{
		pDevice->SetTexture(0, start.pTexture[nCntTexture]);

		if (start.startInfo[nCntTexture].bUse == true)
		{
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				nCntTexture * 4,
				2);
		}
	}
}

//==============================================================================
// �X�^�[�g���̎擾
//==============================================================================
Start GetStart(void)
{
	return start;
}