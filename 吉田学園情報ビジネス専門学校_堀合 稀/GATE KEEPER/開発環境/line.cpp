//==============================================================================
//
// �Z�[�t�e�B���C���̕`��kline.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "line.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define LINE_POSX		(640)	// �Z�[�t�e�B���C����X���W
#define LINE_POSY		(620)	// �Z�[�t�e�B���C����Y���W
#define LINE_SIZEX		(640)	// �Z�[�t�e�B���C���̕�
#define LINE_SIZEY		(12)	// �Z�[�t�e�B���C���̍���
#define LINEOVER		(30)	// ���C���z���̃e�N�X�`���X�V�J�E���g
#define ALERT_COUNTA	(20)	// �x�����C���̃J�E���gA
#define ALERT_COUNTB	(40)	// �x�����C���̃J�E���gB
#define LINE_ALERT		(7)		// ���C���z���̌x�����C��

//==============================================================================
// �Z�[�t�e�B���C���̏��
//==============================================================================
typedef enum
{
	LINESTATE_NORMAL = 0,	// �ʏ���
	LINESTATE_OVER,			// �ʉߏ��
	LINESTATE_ALERT,		// �x�����
	LINESTATE_MAX
} LINESTATE;

//==============================================================================
// �Z�[�t�e�B���C���̍\����
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXCOLOR col;			// �F�E�����x
	LINESTATE linestate;	// �Z�[�t�e�B���C���̏��
	int nLineCnt;			// ���C���z���̕ω����̃J�E���g
	int nAlertLine;			// �x�����̃J�E���g
} LINE;

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureLine = NULL;		// ���[�U�[�C���^�[�t�F�[�X�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLine = NULL;	// �o�b�t�@�ւ̃|�C���^
LINE line;										// �Z�[�t�e�B���C���̍\����

//==============================================================================
// �Z�[�t�e�B���C���̏���������
//==============================================================================
HRESULT InitLine(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/safetyline000.png",
		&g_pTextureLine);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffLine,
		NULL)))
	{
		return E_FAIL;
	}

	//�@�ϐ��̏�����
	line.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	line.linestate = LINESTATE_NORMAL;
	line.nLineCnt = 0;
	line.nAlertLine = 0;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);

	// ���[�U�[�C���^�[�t�F�[�X�̒��S���W
	line.pos = D3DXVECTOR3(LINE_POSX, LINE_POSY, 0.0f);

	// �|���S���̊e���_���W
	pVtx[0].pos = D3DXVECTOR3(line.pos.x - LINE_SIZEX, line.pos.y + LINE_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(line.pos.x - LINE_SIZEX, line.pos.y - LINE_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(line.pos.x + LINE_SIZEX, line.pos.y + LINE_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(line.pos.x + LINE_SIZEX, line.pos.y - LINE_SIZEY, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// �e���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLine->Unlock();

	return S_OK;
}

//==============================================================================
// �Z�[�t�e�B���C���̏I������
//==============================================================================
void UninitLine(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffLine != NULL)
	{
		g_pVtxBuffLine->Release();
		g_pVtxBuffLine = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureLine != NULL)
	{
		g_pTextureLine->Release();
		g_pTextureLine = NULL;
	}
}

//==============================================================================
// �Z�[�t�e�B���C���̍X�V����
//==============================================================================
void UpdateLine(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	PLAYER *Player;
	ENEMY *pEnemy;

	// �v���C���[�̎擾
	Player = GetPlayer();

	// �G�̎擾
	pEnemy = GetEnemy();

	// �Z�[�t�e�B���C���̏��
	switch (line.linestate)
	{
	case LINESTATE_NORMAL:
		// �J�E���g�E�z�F�̏�����
		line.nLineCnt = 0;
		line.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		break;

	case LINESTATE_OVER:
		// �z�F�̕ω��ƍX�V�J�E���g����
		line.col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		line.nLineCnt++;

		// �X�e�[�^�X�̕ω�
		if (line.nLineCnt >= LINEOVER && Player->nLineOver < LINE_ALERT)
		{
			line.linestate = LINESTATE_NORMAL;
		}
		else if (line.nLineCnt >= LINEOVER && Player->nLineOver >= LINE_ALERT)
		{
			line.linestate = LINESTATE_ALERT;
		}

		break;

	case LINESTATE_ALERT:
		line.nLineCnt = 0;	// �X�V�J�E���g�̏�����
		line.nAlertLine++;	// �A���[�g�J�E���g�̑���

		// �A���[�g���̔z�F�̕ω�
		if (line.nAlertLine % ALERT_COUNTA == 0 && line.nAlertLine % ALERT_COUNTB != 0)
		{
			line.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (line.nAlertLine % ALERT_COUNTB == 0)
		{
			line.col = D3DXCOLOR(1.0f, 0.3f, 0.9f, 1.0f);
		}

		break;

	default:
		break;
	}

	// ���C���z���̃X�e�[�^�X�ω�
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{ // �G���L�����
			if (pEnemy->pos.y >= WINDOW_LOWEST)
			{ // ���C���z��
				line.linestate = LINESTATE_OVER;
			}
		}
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍X�V
	pVtx[0].col = line.col;
	pVtx[1].col = line.col;
	pVtx[2].col = line.col;
	pVtx[3].col = line.col;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLine->Unlock();
}

//==============================================================================
// �Z�[�t�e�B���C���̕`�揈��
//==============================================================================
void DrawLine(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffLine,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureLine);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,						// �`����J�n���钸�_�C���f�b�N�X
		2);						// �`�悷��v���~�e�B�u��
}