//==============================================================================
//
// �^�C�g����ʂ̕`��ktitle.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "gamepad.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define TITLE_POLYGON		(3)		// �^�C�g����ʂ̃|���S����
#define TITLE_LOGOX			(640)	// �^�C�g�����S��X���W
#define TITLE_FIRST_LOGOY	(-50)	// �^�C�g�����S�̍ŏ���Y���W
#define TITLE_LAST_LOGOY	(200)	// �^�C�g�����S�̍ŏIY���W
#define TITLE_LOGO_SIZEX	(480)	// �^�C�g�����S�̕�
#define TITLE_LOGO_SIZEY	(80)	// �^�C�g�����S�̍���
#define TITLE_ENTERX		(640)	// �G���^�[�T�C����X���W
#define TITLE_ENTERY		(550)	// �G���^�[�T�C����Y���W
#define TITLE_ENTER_SIZEX	(240)	// �G���^�[�T�C���̕�
#define TITLE_ENTER_SIZEY	(40)	// �G���^�[�T�C���̍���

//==============================================================================
// �G���^�[�T�C���̏��
//==============================================================================
typedef enum
{
	ENTERSIGN_NORMAL = 0,	// �ʏ���
	ENTERSIGN_NONE,			// ��\�����
	ENTERSIGN_ACTIVE,		// �G���^�[�L�[���͎��̓_��
	ENTERSIGN_MAX
} ENTERSIGN;

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLE_POLYGON] = {};		// �^�C�g����ʃe�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;				// �o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posTitle;										// �^�C�g����ʂ̈ʒu
D3DXVECTOR3 g_posTitlelogo;									// �^�C�g�����S�̈ʒu
D3DXVECTOR3 g_posTitleEnter;								// �G���^�[�T�C���̈ʒu
int g_nCntMoveLogo;											// ���S�̈ړ��J�E���^�[
ENTERSIGN Sign;												// �G���^�[�T�C���̏��
int g_nSignCounter;											// �G���^�[�T�C���̓_�ŃJ�E���^�[
int g_nShiftCount;											// ������ʑJ�ڂ̃J�E���^�[

//==============================================================================
// �^�C�g����ʂ̏���������
//==============================================================================
HRESULT InitTitle(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();
	
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bg104.png",
		&g_pTextureTitle[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/title000.png",
		&g_pTextureTitle[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/press_enter.png",
		&g_pTextureTitle[2]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TITLE_POLYGON,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL)))
	{
		return E_FAIL;
	}

	// �G���^�[�T�C���̏�����
	Sign = ENTERSIGN_NONE;
	g_nSignCounter = 0;
	g_nShiftCount = 0;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// �^�C�g����ʂ̒��S���W
	g_posTitle = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	// �|���S���̊e���_���W
	pVtx[0].pos = D3DXVECTOR3(g_posTitle.x - (SCREEN_WIDTH / 2), g_posTitle.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posTitle.x - (SCREEN_WIDTH / 2), g_posTitle.y - (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posTitle.x + (SCREEN_WIDTH / 2), g_posTitle.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posTitle.x + (SCREEN_WIDTH / 2), g_posTitle.y - (SCREEN_HEIGHT / 2), 0.0f);

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
	g_pVtxBuffTitle->Unlock();


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// �^�C�g����ʂ̒��S���W
	g_posTitlelogo = D3DXVECTOR3(TITLE_LOGOX, TITLE_FIRST_LOGOY, 0.0f);

	// �|���S���̊e���_���W
	pVtx[4].pos = D3DXVECTOR3(g_posTitlelogo.x - TITLE_LOGO_SIZEX, g_posTitlelogo.y + TITLE_LOGO_SIZEY, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posTitlelogo.x - TITLE_LOGO_SIZEX, g_posTitlelogo.y - TITLE_LOGO_SIZEY, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posTitlelogo.x + TITLE_LOGO_SIZEX, g_posTitlelogo.y + TITLE_LOGO_SIZEY, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posTitlelogo.x + TITLE_LOGO_SIZEX, g_posTitlelogo.y - TITLE_LOGO_SIZEY, 0.0f);

	// rhw�̐ݒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// �e���_�J���[�̐ݒ�
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// ���_���W�̐ݒ�
	pVtx[4].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[5].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[6].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[7].tex = D3DXVECTOR2(1.0, 0.0);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// �G���^�[�T�C���̒��S���W
	g_posTitleEnter = D3DXVECTOR3(TITLE_ENTERX, TITLE_ENTERY, 0.0f);

	// �|���S���̊e���_���W
	pVtx[8].pos = D3DXVECTOR3(g_posTitleEnter.x - TITLE_ENTER_SIZEX, g_posTitleEnter.y + TITLE_ENTER_SIZEY, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(g_posTitleEnter.x - TITLE_ENTER_SIZEX, g_posTitleEnter.y - TITLE_ENTER_SIZEY, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(g_posTitleEnter.x + TITLE_ENTER_SIZEX, g_posTitleEnter.y + TITLE_ENTER_SIZEY, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(g_posTitleEnter.x + TITLE_ENTER_SIZEX, g_posTitleEnter.y - TITLE_ENTER_SIZEY, 0.0f);

	// rhw�̐ݒ�
	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	// �e���_�J���[�̐ݒ�
	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// ���_���W�̐ݒ�
	pVtx[8].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[9].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[10].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[11].tex = D3DXVECTOR2(1.0, 0.0);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();
	
	// BGM�̍Đ�
	PlaySound(SOUND_LABEL_BGM000);

	return S_OK;
}

//==============================================================================
// �^�C�g����ʂ̏I������
//==============================================================================
void UninitTitle(void)
{
	// �T�E���h�̒�~����
	StopSound();

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < TITLE_POLYGON; nCnt++)
	{
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}
}

//==============================================================================
// �^�C�g����ʂ̍X�V����
//==============================================================================
void UpdateTitle(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	FADE fade;
	int nSign = 255;

	// �t�F�[�h�̎擾
	fade = GetFade();

	// �^�C�g�����S�̈ړ�
	if (g_posTitlelogo.y != TITLE_LAST_LOGOY)
	{
		g_nCntMoveLogo++;
		if (g_nCntMoveLogo % 2 == 0)
		{
			g_posTitlelogo.y += 2;
			if (g_posTitlelogo.y >= TITLE_LAST_LOGOY)
			{
				g_posTitlelogo.y = TITLE_LAST_LOGOY;
			}
		}
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// �|���S���̊e���_���W
	pVtx[4].pos = D3DXVECTOR3(g_posTitlelogo.x - TITLE_LOGO_SIZEX, g_posTitlelogo.y + TITLE_LOGO_SIZEY, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posTitlelogo.x - TITLE_LOGO_SIZEX, g_posTitlelogo.y - TITLE_LOGO_SIZEY, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posTitlelogo.x + TITLE_LOGO_SIZEX, g_posTitlelogo.y + TITLE_LOGO_SIZEY, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posTitlelogo.x + TITLE_LOGO_SIZEX, g_posTitlelogo.y - TITLE_LOGO_SIZEY, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();
	

	if (g_posTitlelogo.y != TITLE_LAST_LOGOY)
	{
		if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
		{
			g_posTitlelogo.y = TITLE_LAST_LOGOY - 1;
		}
	}

	if (g_posTitlelogo.y == TITLE_LAST_LOGOY)
	{
		g_nSignCounter++;	// �G���^�[�T�C���J�E���^�[�̑���
		g_nShiftCount++;	// ������ʑJ�ڃJ�E���^�[�̑���
		if (g_nShiftCount >= 300)
		{ // �����J�ڎ��s
			// �t�F�[�h�A�E�g
			SetFade(FADE_OUT, MODE_RANKING);
		}
	}

	// �G���^�[�L�[���͂ŃQ�[����ʂ֑J��
	if (g_posTitlelogo.y == TITLE_LAST_LOGOY && g_nShiftCount < 300)
	{
		if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
		{
			if (Sign != ENTERSIGN_ACTIVE)
			{
				// �T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_START000);
			}

			// �t�F�[�h�A�E�g
			SetFade(FADE_OUT, MODE_TUTORIAL);

			// �G���^�[�T�C���̏�ԕω�
			Sign = ENTERSIGN_ACTIVE;
		}
	}

	// �G���^�[�T�C���̏��
	switch (Sign)
	{
	case ENTERSIGN_NORMAL:
		if (g_nSignCounter % 60 == 0)
		{
			// �����x0��
			nSign = 0;

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// �e���_�J���[�̐ݒ�
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffTitle->Unlock();
		}
		else if (g_nSignCounter % 30 == 0 && g_nSignCounter % 60 != 0)
		{
			// �����x�ő��
			nSign = 255;

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// �e���_�J���[�̐ݒ�
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffTitle->Unlock();
		}

		break;

	case ENTERSIGN_NONE:
		// �����x�ŏ���
		nSign = 0;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

		// �e���_�J���[�̐ݒ�
		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffTitle->Unlock();

		// �ʏ펞�ւ̈ڍs
		if (g_posTitlelogo.y == TITLE_LAST_LOGOY)
		{
			Sign = ENTERSIGN_NORMAL;
		}

		break;

	case ENTERSIGN_ACTIVE:
		if (g_nSignCounter % 4 == 0)
		{
			// �����x0��
			nSign = 255;

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// �e���_�J���[�̐ݒ�
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffTitle->Unlock();
		}
		else if (g_nSignCounter % 2 == 0 && g_nSignCounter % 4 != 0)
		{
			// �����x�ő��
			nSign = 0;

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// �e���_�J���[�̐ݒ�
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffTitle->Unlock();
		}

		break;

	default:
		break;
	}
}

//==============================================================================
// �^�C�g����ʂ̕`�揈��
//==============================================================================
void DrawTitle(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffTitle,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCnt = 0; nCnt < TITLE_POLYGON; nCnt++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[nCnt]);
		
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
			nCnt * 4,				// �`����J�n���钸�_�C���f�b�N�X
			2);						// �`�悷��v���~�e�B�u��
	}
}

