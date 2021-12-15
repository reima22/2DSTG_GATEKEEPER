//==============================================================================
//
// �G�̕`��kenemy.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "enemy.h"
#include "player.h"
#include "explosion.h"
#include "score.h"
#include "bullet.h"
#include "item.h"
#include "sound.h"
#include "particle.h"
#include "warning.h"
#include "time.h"
#include "stdlib.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MAX_ENEMY_COUNT			(15000)		// �G�z�u�̃J�E���g�
#define LOW_SCORE				(150)		// �X�R�A�{����̍���
#define MIDDLE_SCORE			(300)		// �X�R�A�{�����̍���
#define HIGH_SCORE				(450)		// �X�R�A�{�����̍���
#define FIRSTWAVE_SHOT_ONE		(20)		// ��1�E�F�[�u�̎ˌ��p�^�[��1
#define FIRSTWAVE_SHOT_TWO		(40)		// ��1�E�F�[�u�̎ˌ��p�^�[��2
#define FIRSTWAVE_SHOT_THREE	(60)		// ��1�E�F�[�u�̎ˌ��p�^�[��3
#define THIRDWAVE_SHOT			(120)		// ��3�E�F�[�u�̎ˌ��p�^�[��
#define FIRSTWAVE_END			(40)		// �Q�[���J�n���̓G�̔z�u���E��1�E�F�[�u�I���̌��j��
#define FIRSTWAVE_ONE			(20)		// ��1�E�F�[�u�̈ړ��p�^�[���ω�1
#define FIRSTWAVE_TWO			(30)		// ��1�E�F�[�u�̈ړ��p�^�[���ω�2
#define SECONDWAVE_END			(80)		// ��2�E�F�[�u�I�����̌��j��
#define THIRDWAVE_END			(120)		// ��3�E�F�[�u�I�����̌��j��
#define BIGENEMY_POSY			(120)		// �{�X�̔z�u���鍂��
#define BIGENEMY_LIFE			(600)		// �{�X�̗̑�
#define BIGENEMY_LOSLIFE		(250)		// �{�X�̏�ԕω�
#define BIGENEMY_SHOT			(60)		// �{�X�̎ˌ��p�^�[��
#define ITEM_PHASE				(2)			// �A�C�e���z�u�̒i�K

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_TYPE_ENEMY] = {};	// �G�e�N�X�`���[�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				// �G�o�b�t�@�ւ̃|�C���^
ENEMY g_aEnemy[MAX_ENEMY];									// �G�̏��
ENEMYINFO info;												// �G�S�̂̏��
int g_aItemRand[ITEM_PHASE][MAX_TYPE_ITEM];					// �A�C�e���̔z�u�ꏊ

//==============================================================================
// �G�̏���������
//==============================================================================
HRESULT InitEnemy(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];

	// �����Z�o���猻���������O
	srand((unsigned int)time(0));

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy000.png",
		&g_apTextureEnemy[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy001.png",
		&g_apTextureEnemy[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy002.png",
		&g_apTextureEnemy[2]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy003.png",
		&g_apTextureEnemy[3]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy004.png",
		&g_apTextureEnemy[4]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy005.png",
		&g_apTextureEnemy[5]);

	// �ϐ��̏�����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		pEnemy->pos = D3DXVECTOR3(0.0f, -30.0f, 0.0f);
		pEnemy->nType = NULL;
		pEnemy->bUse = false;
		pEnemy->state = ENEMYSTATE_NORMAL;
		pEnemy->nCounterState = 0;
		pEnemy->nLife = 0;
		pEnemy->move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
		pEnemy->nRandAngle = (rand() % 315);
		pEnemy->fAngle = ((float)pEnemy->nRandAngle / 100.0f);
		pEnemy->nShotCnt = rand() % 100;
	}
	info.nDestroy = 0;
	info.nVanish = 0;
	info.nShotCnt = 1;
	info.nSetCounter = 0;
	info.nUfoCnt = 0;
	info.nUfoSet = rand() % FIRSTWAVE_END + (FIRSTWAVE_END + 1);
	info.nBigEnemyCnt = 0;
	info.nStartCnt = 0;
	info.boss = BIGENEMY_NORMAL;

	// �A�C�e���̃����_���z�u
	for (int nCnt = 0; nCnt < MAX_TYPE_ITEM; nCnt++)
	{
		while (1)
		{ // ���l�̏d�������O
			g_aItemRand[0][nCnt] = rand() % FIRSTWAVE_END;
			if (g_aItemRand[0][nCnt] != g_aItemRand[0][nCnt - 1] && 
				g_aItemRand[0][nCnt] != g_aItemRand[0][nCnt - 2])
			{
				break;
			}
		}

		while (1)
		{ // ���l�̏d�������O
			g_aItemRand[1][nCnt] = rand() % FIRSTWAVE_END + (FIRSTWAVE_END + 1);
			if (g_aItemRand[1][nCnt] != info.nUfoSet && 
				g_aItemRand[1][nCnt] != g_aItemRand[1][nCnt - 1] &&
				g_aItemRand[1][nCnt] != g_aItemRand[1][nCnt - 2])
			{
				break;
			}
		}
	}

	// ���_���̐ݒ�
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL)))
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// �G�̒��S���W�̐ݒ�
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �G�̒��_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);

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
	g_pVtxBuffEnemy->Unlock();

	return S_OK;
}

//==============================================================================
// �G�̏I������
//==============================================================================
void UninitEnemy(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < MAX_TYPE_ENEMY; nCnt++)
	{
		if (g_apTextureEnemy[nCnt] != NULL)
		{
			g_apTextureEnemy[nCnt]->Release();
			g_apTextureEnemy[nCnt] = NULL;
		}
	}
}

//==============================================================================
// �G�̍X�V����
//==============================================================================
void UpdateEnemy(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;
	PLAYER Player;
	PHASESTATE phase = GetPhase();	// �x����Ԃ̎擾
	int nShot = 0;

	// �G�̎擾
	pEnemy = &g_aEnemy[0];

	// �v���C���[�̎擾
	Player = *GetPlayer();
	
	// �I�[�v�j���O�J�E���g
	if (info.nStartCnt <= OPEN_CNT)
	{
		info.nStartCnt++;
	}

	// �I�[�v�j���O�J�E���g�I����
	if (info.nStartCnt > OPEN_CNT && info.nVanish < FIRSTWAVE_END)
	{
		info.nShotCnt++;	// �V���b�g�J�E���g�J�n
	}

	// ��3�E�F�[�u�ȍ~�̒e���˃J�E���g
	if (info.nDestroy >= SECONDWAVE_END)
	{
		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{
			g_aEnemy[nCnt].nShotCnt++;		// �e���˂̃J�E���g
		}
	}
	
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (info.nVanish >= FIRSTWAVE_END && info.nDestroy < SECONDWAVE_END)
		{ // �G�@��X���̔g�^�ړ�
			g_aEnemy[nCnt].fAngle += 0.06f;

			if (g_aEnemy[nCnt].fAngle >= D3DX_PI)
			{
				g_aEnemy[nCnt].fAngle = -D3DX_PI;
			}
		}

		if (g_aEnemy[nCnt].bUse == true)
		{ // ���C���z���̏���
			if (g_aEnemy[nCnt].pos.y >= WINDOW_LOWEST)
			{ // ���C���z��
				if (info.nVanish < FIRSTWAVE_END)
				{
					info.nVanish++;	// ���ŃJ�E���g
				}
			
				g_aEnemy[nCnt].bUse = false;
			}
		}
	}

	// ��1�E�F�[�u�̒e�̔���
	if (info.nVanish < FIRSTWAVE_END)
	{
		while (1)
		{
			// ������40�@�̂݃����_����1�@���甭��
			nShot = rand() % FIRSTWAVE_END;

			if (g_aEnemy[nShot].bUse == true)
			{
				break;
			}
		}

		if (info.nShotCnt % FIRSTWAVE_SHOT_ONE == 0 && info.nVanish < FIRSTWAVE_ONE)
		{
			// �e�̔���
			if (g_aEnemy[nShot].bUse == true)
			{
				// �e�̐ݒ�
				SetBullet(g_aEnemy[nShot].pos, D3DXVECTOR3(0.0f, 4.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			}
		}
		else if (info.nShotCnt % FIRSTWAVE_SHOT_TWO == 0 && info.nVanish < FIRSTWAVE_TWO && info.nVanish >= FIRSTWAVE_ONE)
		{ // ���E�F�[�u��
			// �e�̔���
			if (g_aEnemy[nShot].bUse == true)
			{
				// �e�̐ݒ�
				SetBullet(g_aEnemy[nShot].pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			}
		}
		else if (info.nShotCnt % FIRSTWAVE_SHOT_THREE == 0 && info.nVanish < FIRSTWAVE_END && info.nVanish >= FIRSTWAVE_TWO)
		{ // ��1�E�F�[�u��
			// �e�̔���
			if (g_aEnemy[nShot].bUse == true)
			{
				// �e�̐ݒ�
				SetBullet(g_aEnemy[nShot].pos, D3DXVECTOR3(0.0f, 6.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			}
		}
	}

	for (int nCntShot = 0; nCntShot < MAX_ENEMY; nCntShot++)
	{
		if (g_aEnemy[nCntShot].nShotCnt % THIRDWAVE_SHOT == 0 && info.nDestroy >= SECONDWAVE_END && info.nDestroy < THIRDWAVE_END && g_aEnemy[nCntShot].nType != 5)
		{ // ��3�E�F�[�u��
			if (g_aEnemy[nCntShot].bUse == true)
			{ // �o������G���ׂĂ��甭��
				// �e�̐ݒ�
				SetBullet(g_aEnemy[nCntShot].pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			}
		}

		// �e�̐ݒ�
		if (g_aEnemy[nCntShot].nShotCnt % BIGENEMY_SHOT == 0 && g_aEnemy[nCntShot].pos.y == BIGENEMY_POSY && g_aEnemy[nCntShot].nType == 5)
		{ // �{�X�̎ˌ�
			SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3((Player.pos.x - g_aEnemy[nShot].pos.x) / 80, (Player.pos.y - g_aEnemy[nShot].pos.y) / 80, 0.0f), 200, BULLETTYPE_ENEMY);
			SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x + 60, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 5.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x - 60, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 5.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x + 90, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 7.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x - 90, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 7.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x + 120, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(5.0f, 7.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x - 120, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(-5.0f, 7.0f, 0.0f), 200, BULLETTYPE_ENEMY);

			if (g_aEnemy[nCntShot].nLife <= BIGENEMY_LOSLIFE)
			{ // �̗͂̌����Œe����
				SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x + 30, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 7.0f, 0.0f), 200, BULLETTYPE_ENEMY);
				SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x - 30, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 7.0f, 0.0f), 200, BULLETTYPE_ENEMY);
				SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x + 180, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 5.0f, 0.0f), 200, BULLETTYPE_ENEMY);
				SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x - 180, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 5.0f, 0.0f), 200, BULLETTYPE_ENEMY);
				SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x + 150, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3((Player.pos.x - g_aEnemy[nShot].pos.x) / 80, (Player.pos.y - g_aEnemy[nShot].pos.y) / 60, 0.0f), 200, BULLETTYPE_ENEMY);
				SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x - 150, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3((Player.pos.x - g_aEnemy[nShot].pos.x) / 80, (Player.pos.y - g_aEnemy[nShot].pos.y) / 60, 0.0f), 200, BULLETTYPE_ENEMY);
			}
		}
	}
	
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++,pVtx = pVtx + 4)
	{
		if (pEnemy->bUse == true)
		{ // �G���g�p����Ă����ꍇ
			// �I�[�v�j���O�̓G�̈ړ�
			if (g_aEnemy[nCntEnemy].nType >= 0 && g_aEnemy[nCntEnemy].nType <= 3)
			{ // �ʏ�̓G�̌��j�����Ƃ̈ړ�
				SetEnemyMoveDes(nCntEnemy);
			}
			else if (g_aEnemy[nCntEnemy].nType == 4)
			{ // UFO�̈ړ�
  				SetEnemyMove(g_aEnemy[nCntEnemy].nType, nCntEnemy);
			}
			else if (g_aEnemy[nCntEnemy].nType == 5)
			{ // ��^�G�̈ړ�
				SetEnemyMove(g_aEnemy[nCntEnemy].nType, nCntEnemy);
			}

			// �ʒu�̍X�V
			pEnemy->pos += pEnemy->move;

			// ���_���W�̍X�V
			SetVertexEnemy(nCntEnemy);

			// �G�̏��
			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_DAMAGE:
				// �J�E���g���s
				g_aEnemy[nCntEnemy].nCounterState--;

				// �J�E���g�I��
				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					// �ʏ��Ԃֈڍs
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

					// �e���_�J���[�̐ݒ�
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				break;

			default:
				break;
			}
		}

		// �G�̌��j�����Ƃ̔z�u
		SetEnemyDes();
	}

	if (info.nDestroy >= BIGENEMY_APPEAR && info.nBigEnemyCnt == 0)
	{ // ��^�G�̔z�u
		SetBigEnemy();
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//==============================================================================
// �G�̕`�揈��
//==============================================================================
void DrawEnemy(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	ENEMY *pEnemy;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �G�̎擾
	pEnemy = &g_aEnemy[0];

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffEnemy,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[pEnemy->nType]);

			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
				nCntEnemy * 4,			// �`����J�n���钸�_�C���f�b�N�X
				2);						// �`�悷��v���~�e�B�u��
		}
	}
}

//==============================================================================
// �G�̐ݒ�
//==============================================================================
void SetEnemy(D3DXVECTOR3 pos, int nType, int nLife)
{
	// ���[�J���ϐ��錾
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			// �ʒu��ݒ�
			pEnemy->pos = pos;

			// ���_���W��ݒ�
			SetVertexEnemy(nCntEnemy);

			// ��ނ̐ݒ�
			pEnemy->nType = nType;

			// �G�̗̑�
			pEnemy->nLife = nLife;

			pEnemy->bUse = true;
			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//==============================================================================
// �G�̌��j�����Ƃ̔z�u�ݒ�
//==============================================================================
void SetEnemyDes(void)
{
	// ���[�J���ϐ��錾
	ENEMY *pEnemy;

	// �G�̎擾
	pEnemy = &g_aEnemy[0];

	// ��2�E�F�[�u�̓G�̔z�u
	if (info.nVanish >= FIRSTWAVE_END && info.nDestroy < SECONDWAVE_END)
	{
		// �����z�u�A�C�e���̖�����
		for (int nCnt = 0; nCnt < MAX_TYPE_ITEM; nCnt++)
		{
			g_aItemRand[0][nCnt] = -1;
		}

		// �G�z�u�J�E���g
		info.nSetCounter++;

		// �G�̔z�u
		if (info.nSetCounter % MAX_ENEMY_COUNT == 0)
		{
			int nPosx = rand() % 1080 + 100;
			int nType = rand() % 4;
			int nLife = 0;

			// �^�C�v���Ƃɑ̗͂̎w��
			if (nType == 0)
			{
				nLife = 4;
			}
			else if (nType == 1)
			{
				nLife = 7;
			}
			else if (nType == 2)
			{
				nLife = 9;
			}
			else if (nType == 3)
			{
				nLife = 12;
			}

			// �G�̐ݒ�
			SetEnemy(D3DXVECTOR3((float)nPosx, -20.0f, 0.0f), nType, nLife);
		}
	}
	else if (info.nDestroy >= SECONDWAVE_END && info.nDestroy < THIRDWAVE_END)
	{ // ��3�E�F�[�u�̓G�̔z�u
		// �G�̐ݒ�J�E���g
		info.nSetCounter++;

		if (info.nSetCounter % MAX_ENEMY_COUNT == 0)
		{
			// �G�̔z�u
			int nPosx = rand() % 1240 + 20;
			int nType = rand() % 4;
			int nLife = 0;

			// �^�C�v���Ƃɑ̗͂̎w��
			if (nType == 0)
			{
				nLife = 4;
			}
			else if (nType == 1)
			{
				nLife = 6;
			}
			else if (nType == 2)
			{
				nLife = 8;
			}
			else if (nType == 3)
			{
				nLife = 10;
			}

			// �G�̐ݒ�
			SetEnemy(D3DXVECTOR3((float)nPosx, -20.0f, 0.0f), nType, nLife);
		}
	}
	else if (info.nDestroy >= THIRDWAVE_END && info.nDestroy < BIGENEMY_APPEAR)
	{ // ��4�E�F�[�u�̓G�̔z�u
	  // �G�̐ݒ�J�E���g
		info.nSetCounter++;

		if (info.nSetCounter % MAX_ENEMY_COUNT == 0)
		{
			// �G�̔z�u
			int nPosx = (rand() % 660 + 300);

			// �G�̐ݒ�
			SetEnemy(D3DXVECTOR3((float)nPosx, -20.0f, 0.0f), 3, 9);
		}
	}

	// UFO�̔z�u
	if (info.nDestroy == info.nUfoSet && info.nUfoCnt == 0)
	{
		// UFO�̐ݒ�
		SetEnemy(D3DXVECTOR3(-20.0f, 567.0f, 0.0f), 4, 5);
		info.nUfoCnt++;
	}
}

//==============================================================================
// �G�̒��_���W�擾����
//==============================================================================
void SetVertexEnemy(int nIdx)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;

	// �G�̎擾
	pEnemy = &g_aEnemy[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�|�C���^��4���i�߂�
	pVtx += nIdx * 4;

	pEnemy += nIdx;

	if (pEnemy->nType != 5)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - ENEMY_SIZEX, g_aEnemy[nIdx].pos.y + ENEMY_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - ENEMY_SIZEX, g_aEnemy[nIdx].pos.y - ENEMY_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + ENEMY_SIZEX, g_aEnemy[nIdx].pos.y + ENEMY_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + ENEMY_SIZEX, g_aEnemy[nIdx].pos.y - ENEMY_SIZEY, 0.0f);
	}
	else
	{ // ��^�̓G�̒��_���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - BIGENEMY_SIZEX, g_aEnemy[nIdx].pos.y + BIGENEMY_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - BIGENEMY_SIZEX, g_aEnemy[nIdx].pos.y - BIGENEMY_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + BIGENEMY_SIZEX, g_aEnemy[nIdx].pos.y + BIGENEMY_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + BIGENEMY_SIZEX, g_aEnemy[nIdx].pos.y - BIGENEMY_SIZEY, 0.0f);
	}
	
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//==============================================================================
// �G�̈ړ��͂̕ω�
//==============================================================================
void SetEnemyMove(int nIdx,int nCnt)
{
	// ���[�J���ϐ��錾
	PHASESTATE phase = GetPhase();	// �x����Ԃ̎擾

	// �^�C�v���ƂɈړ��͂̎w��(��1�E�F�[�u�ȊO)
	if (nIdx == 0)
	{
		g_aEnemy[nCnt].move.y = 3.0f;
	}
	else if (nIdx == 1)
	{
		g_aEnemy[nCnt].move.y = 2.5f;
	}
	else if (nIdx == 2)
	{
		g_aEnemy[nCnt].move.y = 2.0f;
	}
	else if (nIdx == 3)
	{
		g_aEnemy[nCnt].move.y = 1.5f;
	}
	else if (nIdx == 4)
	{ // UFO�̈ړ���
		g_aEnemy[nCnt].move.x = 3.0f;
		g_aEnemy[nCnt].move.y = 0.0f;

		if (g_aEnemy[nCnt].pos.x >= 1300.0f)
		{
			g_aEnemy[nCnt].bUse = false;
		}
	}
	else if (nIdx == 5)
	{ // �{�X�G�̈ړ���
		if (g_aEnemy[nCnt].pos.y < BIGENEMY_POSY && phase != PHASESTATE_WARNING)
		{ // �o�ꉉ�o�ړ�
			g_aEnemy[nCnt].move.x = 0.0f;
			g_aEnemy[nCnt].move.y = 1.0f;
		}
		else if (phase == PHASESTATE_WARNING)
		{ // �x�����̒�~
			g_aEnemy[nCnt].move.y = 0.0f;
		}
		else if (g_aEnemy[nCnt].pos.y > BIGENEMY_POSY)
		{ // ����̈ʒu�Œ�~
			g_aEnemy[nCnt].pos.y = BIGENEMY_POSY;
			g_aEnemy[nCnt].move.x = 3.0f;
			g_aEnemy[nCnt].move.y = 0.0f;
		}

		if (g_aEnemy[nCnt].nLife <= BIGENEMY_LOSLIFE)
		{ // �̗͒ቺ�ɂ�����
			if (g_aEnemy[nCnt].move.x == -3.0f)
			{
				g_aEnemy[nCnt].move.x = -5.0f;
			}
			else if (g_aEnemy[nCnt].move.x == 3.0f)
			{
				g_aEnemy[nCnt].move.x = 5.0f;
			}
		}

		if (g_aEnemy[nCnt].pos.x >= (SCREEN_WIDTH - BIGENEMY_SIZEX) || g_aEnemy[nCnt].pos.x <= BIGENEMY_SIZEX)
		{ // ��ʒ[�Ő܂�Ԃ�
			g_aEnemy[nCnt].pos.x -= g_aEnemy[nCnt].move.x;
			g_aEnemy[nCnt].move.x *= -1;
		}
	}
}

//==============================================================================
// ���j�����Ƃ̓G�̈ړ��͂̕ω�
//==============================================================================
void SetEnemyMoveDes(int nIdx)
{
	if (info.nVanish < 20 && info.nStartCnt == OPEN_CNT)
	{
		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{
			g_aEnemy[nCnt].move.x = 2.0f;
			g_aEnemy[nCnt].move.y = 0.0f;
		}
	}

	// ��1�E�F�[�u�̓G�̈ړ�
	if (info.nVanish < FIRSTWAVE_END)
	{
		if (g_aEnemy[nIdx].pos.x >= (SCREEN_WIDTH - ENEMY_SIZEX) || g_aEnemy[nIdx].pos.x <= ENEMY_SIZEX)
		{ // ��ʒ[�ɓ��B
			for (int nCnt = 0; nCnt < FIRSTWAVE_END; nCnt++)
			{
				g_aEnemy[nCnt].pos.y += 20.0f;
				if (g_aEnemy[nCnt].pos.x >= (SCREEN_WIDTH - ENEMY_SIZEX))
				{
					g_aEnemy[nCnt].pos.x += g_aEnemy[nCnt].move.x * 2;
				}
				g_aEnemy[nCnt].move.x *= -1;
				g_aEnemy[nCnt].pos.x += g_aEnemy[nCnt].move.x * 4;
			}
		}
	}

	if (info.nVanish >= 20 && info.nVanish < 40)
	{ // �����z�u�̔��������j��
	  // �ړ��͂̉���
		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{
			if (g_aEnemy[nCnt].move.x == 2.0f)
			{
				g_aEnemy[nCnt].move.x = 4.0f;
			}
			else if (g_aEnemy[nCnt].move.x == -2.0f)
			{
				g_aEnemy[nCnt].move.x = -4.0f;
			}
		}
	}
	else if (info.nVanish >= FIRSTWAVE_END && info.nDestroy < SECONDWAVE_END)
	{ // ��2�E�F�[�u
		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{ // �G�̈ړ���
		  // ���E�ւ̔g�^�ړ�
			g_aEnemy[nCnt].move.x = sinf(g_aEnemy[nCnt].fAngle) * SPEEDUP;

			// �^�C�v���ƂɈړ��͂̎w��
			SetEnemyMove(g_aEnemy[nCnt].nType, nCnt);
		}
	}
	else if (info.nDestroy >= SECONDWAVE_END && info.nDestroy < THIRDWAVE_END)
	{ // ��3�E�F�[�u�̓G�ړ�
		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{
			if (g_aEnemy[nCnt].move.x != -4.0f)
			{
				// �ړ�����X�����̎w��
				g_aEnemy[nCnt].move.x = 4.0f;
			}
		}

		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{
			if (g_aEnemy[nCnt].pos.x >= (SCREEN_WIDTH - 20))
			{
				g_aEnemy[nCnt].move.x = -4.0f;
			}
			else if (g_aEnemy[nCnt].pos.x <= 20)
			{
				g_aEnemy[nCnt].move.x = 4.0f;
			}
		}
	}
	else if (info.nDestroy >= THIRDWAVE_END && info.nDestroy < BIGENEMY_APPEAR)
	{ // ��4�E�F�[�u�̓G�ړ�
		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{
			g_aEnemy[nCnt].move.x = 0.0f;
			g_aEnemy[nCnt].move.y = 2.5f;
		}
	}
}

//==============================================================================
// �{�X�G�̔z�u
//==============================================================================
void SetBigEnemy(void)
{
	// ���[�J���ϐ��錾
	ENEMY *pEnemy;
	PHASESTATE phase = GetPhase();	// �x����Ԃ̎擾

	// �G�̎擾
	pEnemy = &g_aEnemy[0];

	// �c������G�̔r��
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
	{
		if (pEnemy->bUse == true && pEnemy->nType != 5)
		{
			SetExplosion(pEnemy->pos);
			pEnemy->bUse = false;
		}
	}

	// �T�E���h�̒�~����
	StopSound();

	// �T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_SE_WARNING);

	// �t�F�[�Y��ʏ�ɖ߂��A�{�X�̏o��
	if (phase == PHASESTATE_NORMAL && g_aEnemy[0].bUse == false)
	{
		// ��^�G�̐ݒ�
		SetEnemy(D3DXVECTOR3(640.0f, -60.0f, 0.0f), 5, BIGENEMY_LIFE);
		info.nBigEnemyCnt++;
	}
}

//==============================================================================
// �G�̎擾
//==============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//==============================================================================
// �G�̖����ƃ_���[�W����
//==============================================================================
bool HitEnemy(int nIdx, int nDamage)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;
	int nScore = 0;

	// �G�̎擾
	pEnemy = &g_aEnemy[0];

	// �_���[�W�v�Z
	g_aEnemy[nIdx].nLife -= nDamage;

	// �c��̗͂ɂ�鏈��
	if (g_aEnemy[nIdx].nLife <= 0)
	{
		// �T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_EXPLOSION000);

		// �X�R�A�v�Z
		for (int nCnt = 0; nCnt < MAX_TYPE_ENEMY; nCnt++)
		{
			if (g_aEnemy[nIdx].nType == nCnt)
			{
				if (nCnt == 0)
				{
					// �p�[�e�B�N���̔���
					SetEffect(g_aEnemy[nIdx].pos, 0.05f, D3DXCOLOR(0.9f, 0.4f, 0.1f, 1.0f), 15.0f, 0.015f);

					// �X�R�A�̐ݒ�
					nScore = 1000;
				}
				else if (nCnt == 1)
				{
					// �p�[�e�B�N���̔���
					SetEffect(g_aEnemy[nIdx].pos, 0.05f, D3DXCOLOR(0.8f, 0.3f, 0.8f, 1.0f), 15.0f, 0.015f);

					// �X�R�A�̐ݒ�
					nScore = 2000;
				}
				else if (nCnt == 2)
				{
					// �p�[�e�B�N���̔���
					SetEffect(g_aEnemy[nIdx].pos, 0.05f, D3DXCOLOR(0.5f, 0.9f, 0.4f, 1.0f), 15.0f, 0.015f);

					// �X�R�A�̐ݒ�
					nScore = 3000;
				}
				else if (nCnt == 3)
				{
					// �p�[�e�B�N���̔���
					SetEffect(g_aEnemy[nIdx].pos, 0.05f, D3DXCOLOR(0.2f, 0.9f, 0.2f, 1.0f), 15.0f, 0.015f);

					// �X�R�A�̐ݒ�
					nScore = 4000;
				}
				else if (nCnt == 4)
				{
					// �p�[�e�B�N���̔���
					SetEffect(g_aEnemy[nIdx].pos, 0.05f, D3DXCOLOR(0.8f, 0.2f, 0.3f, 1.0f), 15.0f, 0.015f);

					// �X�R�A�̐ݒ�
					nScore = 12500;
				}
				else if (nCnt == 5)
				{
					// �p�[�e�B�N���̔���
					SetEffect(g_aEnemy[nIdx].pos, 0.2f, D3DXCOLOR(0.9f, 0.2f, 0.2f, 1.0f), 15.0f, 0.015f);

					// �{�X�̏�ԕω�
					info.boss = BIGENEMY_DEATH;

					// �X�R�A�̐ݒ�
					nScore = 80000;
				}
			}
		}

		// ���j�����ʒu�̍����ɂ��X�R�A�̔{��
		if (pEnemy->pos.y >= LOW_SCORE && pEnemy->pos.y < MIDDLE_SCORE)
		{
			nScore *= 2;
		}
		else if (pEnemy->pos.y >= MIDDLE_SCORE && pEnemy->pos.y < HIGH_SCORE)
		{
			nScore *= 3;
		}
		else if (pEnemy->pos.y >= HIGH_SCORE && pEnemy->pos.y < WINDOW_LOWEST)
		{
			nScore *= 4;
		}

		// ���j���E���Ő��̉��Z
		info.nDestroy++;
		if (info.nVanish < 40)
		{
			info.nVanish++;
		}
		
		// �X�R�A�̉��Z
		AddScore(nScore);

		// �G�̔j��
		g_aEnemy[nIdx].bUse = false;

		// ��1�E�F�[�u�̃A�C�e���z�u
		for (int nCnt = 0; nCnt < MAX_TYPE_ITEM; nCnt++)
		{
			if (nIdx == g_aItemRand[0][nCnt])
			{
				SetItem(g_aEnemy[nIdx].pos, nCnt);
			}
		}

		// ��2�E�F�[�u�̃A�C�e���z�u
		for (int nCnt = 0; nCnt < MAX_TYPE_ITEM; nCnt++)
		{
			if (g_aItemRand[1][nCnt] == info.nDestroy)
			{
				SetItem(g_aEnemy[nIdx].pos, nCnt);
			}
		}

		return true;
	}
	else
	{
		// �G�̏�Ԃ̕ω�
		g_aEnemy[nIdx].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nIdx].nCounterState = 9;

		// �T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_EXPLOSION000);

		// �p�[�e�B�N���̔���
		SetEffect(g_aEnemy[nIdx].pos, 0.01f, D3DXCOLOR(0.9f, 0.1f, 0.1f, 1.0f), 3.0f, 0.015f);

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nIdx * 4;

		// �e���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffEnemy->Unlock();

		return false;
	}
}

//==============================================================================
// �G�S�̏��̎擾
//==============================================================================
ENEMYINFO GetInfo(void)
{
	return info;
}