////==============================================================================
////
//// �����̕`��kexplosion.cpp�l
//// AUTHOR : MARE HORIAI
////
////==============================================================================
//#include "effect.h"
//#include "enemy.h"
//#include "player.h"
//#include "sound.h"
//
////==============================================================================
//// �����̍\����
////==============================================================================
//typedef struct
//{
//	D3DXVECTOR3 pos;	// �ʒu
//	D3DXCOLOR color;	// �F�E�����x
//	float fAlphaDown;	// ������
//	int nSizeX;			// ��
//	int nSizeY;			// ����
//	bool bUse;			// �g�p���Ă��邩�ǂ���
//} EFFECT;
//
////==============================================================================
//// �O���[�o���ϐ�
////==============================================================================
//LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;			// �G�t�F�N�g�e�N�X�`���[�ւ̃|�C���^
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// �o�b�t�@�ւ̃|�C���^
//EFFECT g_aEffect[MAX_EFFECT];
//
////==============================================================================
//// �G�t�F�N�g�̏���������
////==============================================================================
//HRESULT InitEffect(void)
//{
//	// ���[�J���ϐ��錾
//	VERTEX_2D *pVtx;
//	LPDIRECT3DDEVICE9 pDevice;
//
//	// �f�o�C�X�̎擾
//	pDevice = GetDevice();
//
//	// �e�N�X�`���̓ǂݍ���
//	D3DXCreateTextureFromFile(
//		pDevice,
//		"data/TEXTURE/effect000.jpg",
//		&g_pTextureEffect);
//
//	// g_aExplosion�̏�����
//	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
//	{
//		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_aEffect[nCntEffect].color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
//		g_aEffect[nCntEffect].fAlphaDown = 0.0f;
//		g_aEffect[nCntEffect].nSizeX = 0;
//		g_aEffect[nCntEffect].nSizeY = 0;
//		g_aEffect[nCntEffect].bUse = false;
//	}
//
//	// ���_���̐ݒ�
//	if (FAILED(pDevice->CreateVertexBuffer(
//		sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_2D,
//		D3DPOOL_MANAGED,
//		&g_pVtxBuffEffect,
//		NULL)))
//	{
//		return E_FAIL;
//	}
//
//	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
//	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
//	{
//		// rhw�̐ݒ�
//		pVtx[0].rhw = 1.0f;
//		pVtx[1].rhw = 1.0f;
//		pVtx[2].rhw = 1.0f;
//		pVtx[3].rhw = 1.0f;
//
//		// �e���_�J���[�̐ݒ�
//		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
//
//		// �e�N�X�`�����_���̐ݒ�
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
//
//		pVtx += 4;	// ���_�|�C���^��4���i�߂�
//	}
//
//	// ���_�o�b�t�@���A�����b�N����
//	g_pVtxBuffEffect->Unlock();
//
//	return S_OK;
//}
//
////==============================================================================
//// �G�t�F�N�g�̏I������
////==============================================================================
//void UninitEffect(void)
//{
//	// ���_�o�b�t�@�̊J��
//	if (g_pVtxBuffEffect != NULL)
//	{
//		g_pVtxBuffEffect->Release();
//		g_pVtxBuffEffect = NULL;
//	}
//
//	// �e�N�X�`���̊J��
//	if (g_pTextureEffect != NULL)
//	{
//		g_pTextureEffect->Release();
//		g_pTextureEffect = NULL;
//	}
//}
//
////==============================================================================
//// �G�t�F�N�g�̍X�V����
////==============================================================================
//void UpdateEffect(void)
//{
//	// ���[�J���ϐ��錾
//	EFFECT *pEffect;
//	pEffect = &g_aEffect[0];
//	VERTEX_2D *pVtx;
//
//	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
//	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4, pEffect++)
//	{
//		if (pEffect->bUse == true)
//		{ // �G�t�F�N�g���g�p����Ă����ꍇ
//
//			g_aEffect[nCntEffect].color.a -= g_aEffect[nCntEffect].fAlphaDown;
//
//			if (g_aEffect[nCntEffect].color.a <= 0)
//			{
//				g_aEffect[nCntEffect].color.a = 0.0f;
//				g_aEffect[nCntEffect].bUse = false;
//			}
//		}
//	}
//
//	// ���_�o�b�t�@���A�����b�N����
//	g_pVtxBuffEffect->Unlock();
//}
//
////==============================================================================
//// �G�t�F�N�g�̕`�揈��
////==============================================================================
//void DrawEffect(void)
//{
//	// ���[�J���ϐ��錾
//	LPDIRECT3DDEVICE9 pDevice;
//
//	// �f�o�C�X�̎擾
//	pDevice = GetDevice();
//
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
//
//	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
//	pDevice->SetStreamSource(
//		0,
//		g_pVtxBuffEffect,
//		0,
//		sizeof(VERTEX_2D));
//
//	// ���_�t�H�[�}�b�g�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	// �e�N�X�`���̐ݒ�
//	pDevice->SetTexture(0, g_pTextureEffect);
//
//	// �|���S���̕`��
//	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
//	{
//		if (g_aEffect[nCntEffect].bUse == true)
//		{
//			pDevice->DrawPrimitive(
//				D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
//				nCntEffect * 4,			// �`����J�n���钸�_�C���f�b�N�X
//				2);						// �`�悷��v���~�e�B�u��
//		}
//	}
//
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//}
//
////==============================================================================
//// �G�t�F�N�g�̐ݒ�
////==============================================================================
//void SetEffect(D3DXVECTOR3 pos,int nType)
//{
//	// ���[�J���ϐ��錾
//	VERTEX_2D *pVtx;
//	EFFECT *pEffect;
//	pEffect = &g_aEffect[0];
//
//	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
//	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
//	{
//		if (pEffect->bUse == false)
//		{ // �g�p����Ă��Ȃ��ꍇ
//			// �T�E���h�̍Đ�
//			//PlaySound(SOUND_LABEL_SE_EXPLOSION000);
//
//			// �ʒu��ݒ�
//			pEffect->pos = pos;
//
//			// �G�t�F�N�g�̓����x�ݒ�
// 			pEffect->color.a = 1.0f;
//
//			// �G�t�F�N�g�̒��_���W�ݒ�
//			SetVertexEffect(nCntEffect, nType);
//
//			// �A�j���[�V�����̗L����
//			pEffect->bUse = true;
//			break;
//		}
//	}
//
//	// ���_�o�b�t�@���A�����b�N����
//	g_pVtxBuffEffect->Unlock();
//}
//
////==============================================================================
//// �G�t�F�N�g�̒��_���W�ݒ�
////==============================================================================
//void SetVertexEffect(int nIdx,int nType)
//{
//	// ���[�J���ϐ��錾
//	VERTEX_2D *pVtx;
//	EFFECT *pEffect;
//	pEffect = &g_aEffect[0];
//
//	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
//	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
//
//	// ���_�|�C���^��4���i�߂�
//	pVtx += 4 * nIdx;
//
//	// �o���b�g�|�C���^��i�߂�
//	pEffect += nIdx;
//
//	if (nType == 0)
//	{ // �eA(�ʏ�e)�̏ꍇ
//	  // ���_���W�̐ݒ�
//		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 10, g_aEffect[nIdx].pos.y + 10, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 10, g_aEffect[nIdx].pos.y - 10, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 10, g_aEffect[nIdx].pos.y + 10, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 10, g_aEffect[nIdx].pos.y - 10, 0.0f);
//
//		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aEffect[nIdx].color.a);
//		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aEffect[nIdx].color.a);
//		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aEffect[nIdx].color.a);
//		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aEffect[nIdx].color.a);
//
//		g_aEffect[nIdx].fAlphaDown = 0.15f;
//	}
//	else if (nType == 1)
//	{ // �eB(�E�F�[�u)�̏ꍇ
//		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 30, g_aEffect[nIdx].pos.y + 10, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 30, g_aEffect[nIdx].pos.y - 10, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 30, g_aEffect[nIdx].pos.y + 10, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 30, g_aEffect[nIdx].pos.y - 10, 0.0f);
//
//		pVtx[0].col = D3DXCOLOR(1.0f, 0.3f, 0.0f, g_aEffect[nIdx].color.a);
//		pVtx[1].col = D3DXCOLOR(1.0f, 0.3f, 0.0f, g_aEffect[nIdx].color.a);
//		pVtx[2].col = D3DXCOLOR(1.0f, 0.3f, 0.0f, g_aEffect[nIdx].color.a);
//		pVtx[3].col = D3DXCOLOR(1.0f, 0.3f, 0.0f, g_aEffect[nIdx].color.a);
//
//		g_aEffect[nIdx].fAlphaDown = 0.09f;
//	}
//	else if (nType == 2)
//	{ // �eC(�~�T�C��)�̏ꍇ
//	  // �~�T�C���̊e���_���W
//		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 5, g_aEffect[nIdx].pos.y + 5, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 5, g_aEffect[nIdx].pos.y - 5, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 5, g_aEffect[nIdx].pos.y + 5, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 5, g_aEffect[nIdx].pos.y - 5, 0.0f);
//
//		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEffect[nIdx].color.a);
//		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEffect[nIdx].color.a);
//		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEffect[nIdx].color.a);
//		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEffect[nIdx].color.a);
//
//		g_aEffect[nIdx].fAlphaDown = 0.15f;
//	}
//	else if (nType == 3)
//	{ // �eD(���o�[�X�E�F�[�u)�̏ꍇ
//		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 30, g_aEffect[nIdx].pos.y + 10, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 30, g_aEffect[nIdx].pos.y - 10, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 30, g_aEffect[nIdx].pos.y + 10, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 30, g_aEffect[nIdx].pos.y - 10, 0.0f);
//
//		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, g_aEffect[nIdx].color.a);
//		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, g_aEffect[nIdx].color.a);
//		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, g_aEffect[nIdx].color.a);
//		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, g_aEffect[nIdx].color.a);
//
//		g_aEffect[nIdx].fAlphaDown = 0.09f;
//	}
//
//	// ���_�o�b�t�@���A�����b�N����
//	g_pVtxBuffEffect->Unlock();
//}