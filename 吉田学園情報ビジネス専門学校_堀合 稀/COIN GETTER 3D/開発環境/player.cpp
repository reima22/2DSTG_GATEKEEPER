//==============================================================================
//
// 3D�v���C���[�̕`��kplayer.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "object.h"
#include "item.h"
#include "sound.h"
#include "enemy.h"
#include "gamepad.h"
#include "math.h"
#include <stdio.h>

//==============================================================================
// �}�N����`
//==============================================================================
#define PLAYER_MOVE		(1.0f)
#define PLAYER_TURN		(0.1f)
#define VTX_MINP		(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))		// �����_�ŏ��l
#define	VTX_MAXP		(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))	// �����_�ő�l
#define TEXT_LENGTH		(128)	// �e�L�X�g�ǂݍ��݂̍ő吔
#define DATA_LENGTH		(64)	// �f�[�^�ǂݍ��݂̍ő吔
#define MOVE_MIN		(0.5f)	// �ړ����[�V�������̍ŏ��ړ���

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================

//LPD3DXMESH g_pMeshPlayer = NULL;						// ���b�V��(���_���)�ւ̃|�C���^
//LPD3DXBUFFER g_pBuffMatPlayer = NULL;					// �}�e���A��(�ގ����)�ւ̃|�C���^
//LPDIRECT3DTEXTURE9 g_apTextureModel[TEX_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
//DWORD g_nNumMatPlayer = 0;							// �}�e���A���̐�
D3DXVECTOR3 g_vtxMinPlayer, g_vtxMaxPlayer;	// ���f���̊e���W�̍ő�l�E�ŏ��l
Player g_player;							// �v���C���[�̍\����
bool g_bJump;

//==============================================================================
// ����������
//==============================================================================
HRESULT InitPlayer(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�̎擾

	LoadMotion();
	
	// X�t�@�C���̓ǂݍ���
	for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
	{
		D3DXLoadMeshFromX(
			g_player.aModel[nCnt].aFileName,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[nCnt].pBuffMatModel,
			NULL,
			&g_player.aModel[nCnt].nNumMatModel,
			&g_player.aModel[nCnt].pMeshModel);
	}

	// ���[�J���ϐ��錾
	int nNumVtx;			// ���_��
	DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�����擾
	nNumVtx = g_player.aModel[0].pMeshModel->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y�̎擾
	sizeFVF = D3DXGetFVFVertexSize(g_player.aModel[0].pMeshModel->GetFVF());

	// �ϐ��̏�����
	g_player.pos = D3DXVECTOR3(0.0f, 200.0f, 0.0f);
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	g_player.rotDest = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.state = PLAYERSTATE_APPEAR;
	g_player.nLife = MAX_LIFE;
	g_player.motionType = MOTIONTYPE_NEUTRAL;
	g_player.bLoopMotion = false;
	g_player.bOnBlock = false;
	g_player.nNumKey = 0;
	g_player.nKey = 0;
	g_player.nCounterMotion = 0;
	g_player.nCntState = 120;
	g_player.fInRadius = 0.0f;
	g_vtxMinPlayer = VTX_MINP;
	g_vtxMaxPlayer = VTX_MAXP;
	g_bJump = true;

	// �e�̐ݒ�
	g_player.nIdx = SetShadow(D3DXVECTOR3(g_player.pos.x, 0.0f, g_player.pos.z), 10.0f, 10.0f);

	// ���_�o�b�t�@�����b�N
	g_player.aModel[0].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// ���_���W�̔�r�ƍX�V
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

		// �e���W�̍ő�l�̔�r
		if (g_vtxMaxPlayer.x < vtx.x)
		{			
			g_vtxMaxPlayer.x = vtx.x;
		}			
		if (g_vtxMaxPlayer.y < vtx.y)
		{			
			g_vtxMaxPlayer.y = vtx.y;
		}			
		if (g_vtxMaxPlayer.z < vtx.z)
		{			
			g_vtxMaxPlayer.z = vtx.z;
		}

		// �e���W�̍ŏ��l�̔�r
		if (g_vtxMinPlayer.x > vtx.x)
		{			
			g_vtxMinPlayer.x = vtx.x;
		}			
		if (g_vtxMinPlayer.y > vtx.y)
		{			
			g_vtxMinPlayer.y = vtx.y;
		}			
		if (g_vtxMinPlayer.z > vtx.z)
		{			
			g_vtxMinPlayer.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_player.aModel[0].pMeshModel->UnlockVertexBuffer();

	// �����a�̌v�Z
	if (g_player.fInRadius < g_vtxMaxPlayer.x)
	{// X���ő�l
		g_player.fInRadius = g_vtxMaxPlayer.x;
	}
	if (g_player.fInRadius < g_vtxMaxPlayer.z)
	{// Z���ő�l
		g_player.fInRadius = g_vtxMaxPlayer.z;
	}
	if (g_player.fInRadius < -g_vtxMinPlayer.x)
	{// X���ŏ��l
		g_player.fInRadius = -g_vtxMinPlayer.x;
	}
	if (g_player.fInRadius < -g_vtxMinPlayer.z)
	{// Z���ŏ��l
		g_player.fInRadius = -g_vtxMinPlayer.z;
	}

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
	{
		if (g_player.aModel[nCnt].pMeshModel != NULL)
		{// ���b�V���̔j��
			g_player.aModel[nCnt].pMeshModel->Release();
			g_player.aModel[nCnt].pMeshModel = NULL;
		}
		
		if (g_player.aModel[nCnt].pBuffMatModel != NULL)
		{// �o�b�t�@�̔j��
			g_player.aModel[nCnt].pBuffMatModel->Release();
			g_player.aModel[nCnt].pBuffMatModel = NULL;
		}
	}
}

//==============================================================================
// �X�V����
//==============================================================================
void UpdatePlayer(void)
{
	// �J�����̎擾
	Camera camera = *GetCamera();
	//bool bJump = true;
	StateChange();

	// ���O�̈ʒu�̕ۑ�
	g_player.posOld = g_player.pos;

	// �d�͂̔���
	if (g_player.pos.y > 0.0f && g_player.bOnBlock == false)
	{
		if (GetKeyboardPress(KEYINFO_JUMP) != true || IsButtonPush(KEYINFO::KEYINFO_OK) != true)
		{
			g_player.move.y -= PLUS_GRAVITY;
		}
	}

	if (g_player.pos.y < 0.0f)
	{
		g_player.bOnBlock = true;
		g_player.nKey = 0;
		g_player.nCounterMotion = 0;
		g_player.motionType = MOTIONTYPE_LANDING;
	}

	if (g_player.bOnBlock == true)
	{
		if (g_player.pos.y < 0.0f)
		{
			g_player.pos.y = 0.0f;
		}
		g_bJump = false;
		g_player.move.y = 0.0f;

		//g_player.nJumpCnt = 0;
		//if (g_player.bOnBlock == false)
		//{
		//	g_player.bOnBlock = true;
		//}
	}

	// �ʒu�̃��Z�b�g
	if (GetKeyboardTrigger(KEYINFO_PRESET) == true)
	{
		g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// �W�����v
	//if (GetKeyboardTrigger(KEYINFO_JUMP) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
	//{
	//	//SetBullet(
	//		//D3DXVECTOR3(g_player.pos.x + sinf(D3DX_PI - g_player.rot.y) * -10.0f, g_player.pos.y + 50.0f, g_player.pos.z + cosf(D3DX_PI - g_player.rot.y) * 10.0f)
	//	//	/*D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 30.0f, g_player.pos.z)*/,
	//	//	D3DXVECTOR3(sinf(D3DX_PI - g_player.rot.y) * -5.0f, 0.0f, cosf(D3DX_PI - g_player.rot.y) * 5.0f),
	//	//	100);
	//	if (g_player.motionType == MOTIONTYPE_MOVE || g_player.motionType == MOTIONTYPE_NEUTRAL)
	//	{
	//		g_player.bOnBlock = false;
	//		PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_player.move.y = 10.0f;
	//		g_player.nKey = 0;
	//		g_player.nCounterMotion = 0;
	//		g_player.motionType = MOTIONTYPE_JUMP;
	//	}
	//}

	// �W�����v
	if (GetKeyboardTrigger(KEYINFO_JUMP) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
	{
		if (g_player.bOnBlock == true)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_player.nKey = 0;
			g_player.nCounterMotion = 0;
			
			g_player.motionType = MOTIONTYPE_JUMP;
		}
		g_player.bOnBlock = false;
	}
	if (GetKeyboardPress(KEYINFO_JUMP) == true || IsButtonPush(KEYINFO::KEYINFO_OK) == true)
	{
		if (g_bJump == false)
		{
			if (g_player.nJumpCnt == 1)
			{
				g_player.move.y = 5.0f;
			}
			if (g_player.nJumpCnt < 7)
			{
				g_player.nJumpCnt++;
				g_player.move.y += 1.0f;
			}
		}
	}
	if (GetKeyboardRelease(KEYINFO_JUMP) == true || IsButtonUp(KEYINFO::KEYINFO_OK) == true)
	{
		//if (g_player.bOnBlock == true)
		{
			g_player.nJumpCnt = 0;
			g_bJump = true;
		}
	}

	//if (GetKeyboardTrigger(KEYINFO_ATTACK) == true && g_player.motionType == MOTIONTYPE_NEUTRAL)
	//{
	//	g_player.nKey = 0;
	//	g_player.nCounterMotion = 0;
	//	g_player.motionType = MOTIONTYPE_ACTION;
	//}

	// ���[�V�����̍X�V
	UpdateMotion(g_player.motionType);

	// �ړ��ʂ̉��Z
	g_player.pos += g_player.move;

	// �e�̒Ǐ]
	SetPositionShadow(g_player.nIdx, D3DXVECTOR3(g_player.pos.x, 0.0f, g_player.pos.z));

	// ���f���̈ړ�
	if (GetKeyboardPress(KEYINFO_UP) == true || IsButtonPush(KEYINFO::KEYINFO_UP) == true)
	{
		if (g_player.motionType != MOTIONTYPE_JUMP)
		{
			g_player.motionType = MOTIONTYPE_MOVE;
		}
		if (GetKeyboardPress(KEYINFO_LEFT) == true || IsButtonPush(KEYINFO::KEYINFO_LEFT) == true)
		{// �������
			g_player.move.x -= cosf(camera.rot.y + D3DX_PI / 4) * PLAYER_MOVE;
			g_player.move.z += sinf(camera.rot.y + D3DX_PI / 4) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y + (D3DX_PI * 3 / 4);
		}
		else if (GetKeyboardPress(KEYINFO_RIGHT) == true || IsButtonPush(KEYINFO::KEYINFO_RIGHT) == true)
		{// �E�����
			g_player.move.x += cosf(camera.rot.y - D3DX_PI / 4) * PLAYER_MOVE;
			g_player.move.z -= sinf(camera.rot.y - D3DX_PI / 4) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y - (D3DX_PI * 3 / 4);
		}
		else
		{// �����
			g_player.move.x += sinf(camera.rot.y) * PLAYER_MOVE;
			g_player.move.z += cosf(camera.rot.y) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y + D3DX_PI;
		}
	}
	else if (GetKeyboardPress(KEYINFO_DOWN) == true || IsButtonPush(KEYINFO::KEYINFO_DOWN) == true)
	{
		if (g_player.motionType != MOTIONTYPE_JUMP)
		{
			g_player.motionType = MOTIONTYPE_MOVE;
		}

		if (GetKeyboardPress(KEYINFO_LEFT) == true || IsButtonPush(KEYINFO::KEYINFO_LEFT) == true)
		{// ��������
			g_player.move.x += cosf(camera.rot.y + D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_player.move.z -= sinf(camera.rot.y + D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y + D3DX_PI / 4;
		}
		else if (GetKeyboardPress(KEYINFO_RIGHT) == true || IsButtonPush(KEYINFO::KEYINFO_RIGHT) == true)
		{// �E������
			g_player.move.x -= cosf(camera.rot.y - D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_player.move.z += sinf(camera.rot.y - D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y + D3DX_PI / -4;
		}
		else
		{// ������
			g_player.move.x += sinf(camera.rot.y - D3DX_PI) * PLAYER_MOVE;
			g_player.move.z += cosf(camera.rot.y - D3DX_PI) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y;
		}
	}
	else if (GetKeyboardPress(KEYINFO_LEFT) == true || IsButtonPush(KEYINFO::KEYINFO_LEFT) == true)
	{// ������
		g_player.move.x -= cosf(camera.rot.y) * PLAYER_MOVE;
		g_player.move.z += sinf(camera.rot.y) * PLAYER_MOVE;
		g_player.rotDest.y = camera.rot.y + (D3DX_PI / 2);
		if (g_player.motionType != MOTIONTYPE_JUMP)
		{
			g_player.motionType = MOTIONTYPE_MOVE;
		}
	}
	else if (GetKeyboardPress(KEYINFO_RIGHT) == true || IsButtonPush(KEYINFO::KEYINFO_RIGHT) == true)
	{// �E����
		g_player.move.x += cosf(camera.rot.y) * PLAYER_MOVE;
		g_player.move.z -= sinf(camera.rot.y) * PLAYER_MOVE;
		g_player.rotDest.y = camera.rot.y + (D3DX_PI / -2);
		if (g_player.motionType != MOTIONTYPE_JUMP)
		{
			g_player.motionType = MOTIONTYPE_MOVE;
		}
	}

	if (GetKeyboardTrigger(KEYINFO_UP) == true || GetKeyboardTrigger(KEYINFO_DOWN) == true || GetKeyboardTrigger(KEYINFO_RIGHT) == true || GetKeyboardTrigger(KEYINFO_LEFT) == true
		|| IsButtonDown(KEYINFO::KEYINFO_UP) == true || IsButtonDown(KEYINFO::KEYINFO_LEFT) == true || IsButtonDown(KEYINFO::KEYINFO_RIGHT) == true || IsButtonDown(KEYINFO::KEYINFO_DOWN) == true)
	{// ���[�V�����J�E���g�ƈʒu�̏C��
		g_player.nCounterMotion = 0;
		g_player.aModel[0].pos.y = 20.0f;
	}

	if (GetKeyboardRelease(KEYINFO_UP) == true || GetKeyboardRelease(KEYINFO_DOWN) == true || GetKeyboardRelease(KEYINFO_RIGHT) == true || GetKeyboardRelease(KEYINFO_LEFT) == true
		|| IsButtonUp(KEYINFO::KEYINFO_UP) == true || IsButtonUp(KEYINFO::KEYINFO_LEFT) == true || IsButtonUp(KEYINFO::KEYINFO_RIGHT) == true || IsButtonUp(KEYINFO::KEYINFO_DOWN) == true)
	{// �ړ��I�����̈ʒu�␳
		g_player.aModel[0].pos.y = 20.0f;
	}

	// �ړ����[�V��������ҋ@���[�V������
	if (g_player.motionType == MOTIONTYPE_MOVE && g_player.move.x <= MOVE_MIN && g_player.move.x >= -MOVE_MIN && g_player.move.z <= MOVE_MIN && g_player.move.z >= -MOVE_MIN)
	{
		g_player.nCounterMotion = 0;
		g_player.nKey = 0;
		g_player.motionType = MOTIONTYPE_NEUTRAL;
	}

	// ���f���̏㏸�E���~
	//if (GetKeyboardPress(KEYINFO_UP_MODEL) == true)
	//{
	//	g_player.pos.y += 2;
	//}
	//else if (GetKeyboardPress(KEYINFO_DOWN_MODEL) == true)
	//{
	//	g_player.pos.y += -2;
	//}

	// ���f���̐���
	//if (GetKeyboardPress(KEYINFO_TURN_LEFT_MODEL) == true)
	//{
	//	g_player.rotDest.y += D3DX_PI / 36;
	//}
	//if (GetKeyboardPress(KEYINFO_TURN_RIGHT_MODEL) == true)
	//{
	//	g_player.rotDest.y -= D3DX_PI / 36;
	//}

	// �ړI�̉�]�p�̏��
	if (g_player.rotDest.y - g_player.rot.y < -D3DX_PI)
	{
		g_player.rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rotDest.y - g_player.rot.y > D3DX_PI)
	{
		g_player.rotDest.y -= D3DX_PI * 2.0f;
	}

	// �����̍X�V
	g_player.rot.y += (g_player.rotDest.y - g_player.rot.y) * 0.1f;

	// ���݂̉�]�p�̏��
	if (g_player.rot.y < -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rot.y > D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}

	// ������̌�������
	g_player.move.x += (0.0f - g_player.move.x) * SPEEDDOWN;
	g_player.move.z += (0.0f - g_player.move.z) * SPEEDDOWN;

	// �ړ����EX��
	if (g_player.pos.x >= 500.0f - g_vtxMaxPlayer.x)
	{
		g_player.pos.x = 500.0f - g_vtxMaxPlayer.x;
	}
	else if (g_player.pos.x <= -500.0f - g_vtxMinPlayer.x)
	{
		g_player.pos.x = -500.0f - g_vtxMinPlayer.x;
	}

	// �ړ����EZ��
	if (g_player.pos.z >= 500.0f - g_vtxMaxPlayer.z)
	{
		g_player.pos.z = 500.0f - g_vtxMaxPlayer.z;
	}
	else if (g_player.pos.z <= -500.0f - g_vtxMinPlayer.z)
	{
		g_player.pos.z = -500.0f - g_vtxMinPlayer.z;
	}

	// �����蔻��
	g_player.bOnBlock = CollisionVec(&g_player.pos, &g_player.posOld, &g_player.move, g_player.fInRadius,g_player.fHeight);
	TouchItem(&g_player.pos, g_player.fInRadius,g_player.fHeight);
	g_player.bOnBlock = TouchEnemy(&g_player.pos, &g_player.posOld ,&g_player.move,g_vtxMaxPlayer.x, g_vtxMinPlayer.x, g_vtxMaxPlayer.z, g_vtxMinPlayer.z, g_vtxMaxPlayer.y, g_vtxMinPlayer.y);
}

//==============================================================================
// �`�揈��
//==============================================================================
void DrawPlayer(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							// �}�e���A���f�[�^�ւ̃|�C���^

												// �v���C���[�̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_player.mtxWorld);

	// �v���C���[�̌����̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	// �v���C���[�̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	// �v���C���[�̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel;	// �v�Z�p�}�g���b�N�X
		D3DXMATRIX mtxParent;					// �e�̃}�g���b�N�X

		// �e�p�[�c�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_player.aModel[nCntModel].mtxWorld);

		// �e�p�[�c�̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.aModel[nCntModel].rot.y, g_player.aModel[nCntModel].rot.x, g_player.aModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxRotModel);

		// �e�p�[�c�̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTransModel, g_player.aModel[nCntModel].pos.x, g_player.aModel[nCntModel].pos.y, g_player.aModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxTransModel);

		// �e�p�[�c�̐e�}�g���b�N�X��ݒ�
		if (g_player.aModel[nCntModel].nIdxModelParent != -1)
		{
			mtxParent = g_player.aModel[g_player.aModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{
			mtxParent = g_player.mtxWorld;	// �v���C���[�̃}�g���b�N�X��ݒ�
		}

		// �Z�o�����e�p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxParent);

		// �e�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_player.aModel[nCntModel].mtxWorld);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMatModel->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMatModel; nCntMat++)
		{
			if (g_player.state == PLAYERSTATE_APPEAR && g_player.nCntState % 2 != 0)
			{
				pMat[nCntMat].MatD3D.Diffuse.a = 0.0f;
			}
			else
			{
				pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;
			}

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			// ���f��(�p�[�c)�̕`��
			g_player.aModel[nCntModel].pMeshModel->DrawSubset(nCntMat);
		}
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

}

//==============================================================================
// ���f���̎擾
//==============================================================================
Player *GetPlayer(void)
{
	return &g_player;
}

//==============================================================================
// ���[�V�����̍X�V
//==============================================================================
void UpdateMotion(MOTIONTYPE motionType)
{
	// �L�[���ƃ��[�v���̑��
	g_player.nNumKey = g_player.aMotionInfo[motionType].nNumKey;
	g_player.bLoopMotion = g_player.aMotionInfo[motionType].bLoop;

	// ���[�V�����J�E���^�[�̉��Z
	g_player.nCounterMotion++;

	// ���f���p�[�c���Ƃ̃��[�V�����A�j���[�V����
	for (int nCnt = 0; nCnt < MODEL_PARTS; nCnt++)
	{
		if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == true)
		{// ���[�v���čŏ��ɖ߂�Ƃ�
			g_player.aModel[nCnt].pos.x += (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosX) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.y += (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosY) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.z += (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosZ) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].rot.x = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.y = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.z = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
		}
		else
		{// �ʏ펞
			g_player.aModel[nCnt].pos.x += (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fPosX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosX) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.y += (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fPosY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosY) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.z += (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fPosZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosZ) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].rot.x = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fRotX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.y = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fRotY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.z = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fRotZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
		}

		if (g_player.nCounterMotion == g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame)
		{// �L�[�t���[���I����
			g_player.nCounterMotion = 0;

			if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == true)
			{// ���[�v����Ƃ�
				g_player.nKey = 0;
			}
			else if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == false)
			{// ���[�v�����I������Ƃ�
				if (g_player.motionTypeOld != MOTIONTYPE_NEUTRAL)
				{// ���쒼�O���j���[�g�����ȊO�̎�
					g_player.motionType = MOTIONTYPE_NEUTRAL;	// �j���[�g�����ֈڍs
					g_player.nKey = 0;							// �L�[�̃��Z�b�g
					g_player.aModel[0].pos.y = 20.0f;			// �ʒu�̃��Z�b�g
					break;
				}
			}
			else if (g_player.nKey < g_player.nNumKey)
			{// �L�[�̃J�E���g
				g_player.nKey++;
			}
		}

		if (g_player.nKey == 0 && g_player.bLoopMotion == false && g_player.nCounterMotion == 1)
		{// ���[�V�����J�n���̈ʒu�C��
			g_player.aModel[nCnt].pos.x += g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosX;
			g_player.aModel[nCnt].pos.y += g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosY;
			g_player.aModel[nCnt].pos.z += g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosZ;
		}
	}

	// ���O�̃��[�V������Ԃ̕ۑ�
	g_player.motionTypeOld = g_player.motionType;
}

//==============================================================================
// .txt�t�H���_�̓ǂݍ���
//==============================================================================
void LoadMotion(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile;
	char aEqual[2] = { NULL };					// �u=�v�ǂݎ��p�ϐ�
	int nCntFile = 0;							// X�t�@�C���̐�
	int nIdxParts = 0;							// �p�[�c��
	int nMotion = 0;							// ���[�V�����̐�
	bool bComment = false;						// �R�����g�A�E�g���邩
	char aText[TEXT_LENGTH] = {NULL};			// �e�L�X�g�ǂݎ��p�ϐ�
	char aCharaSet[DATA_LENGTH] = { NULL };		// �L�����f�[�^�ǂݎ��p�ϐ�
	char aPartsSet[DATA_LENGTH] = { NULL };		// �p�[�c�f�[�^�ǂݎ��p�ϐ�
	char aMotionSet[DATA_LENGTH] = { NULL };	// ���[�V�����ǂݎ��p�ϐ�
	char aKeySet[DATA_LENGTH] = { NULL };		// �L�[�ݒ���ǂݎ��p�ϐ�
	char aKey[DATA_LENGTH] = { NULL };			// �L�[���ǂݎ��p�ϐ�

	// strcmp�ǂݎ��p�|�C���^
	char *pText = &aText[0];					
	char *pCharaSet = &aCharaSet[0];
	char *pPartsSet = &aPartsSet[0];
	char *pMotionSet = &aMotionSet[0];
	char *pKeySet = &aKeySet[0];
	char *pKey = &aKey[0];

	// �f�[�^�̓ǂݍ���
	pFile = fopen("data/TEXT/player_horiai.txt", "r");
	if (pFile != NULL)
	{ //�t�@�C���W�J�\
		while (strcmp("END_SCRIPT",pText) != 0)
		{
			aText[0] = {NULL};
			if (bComment == false)
			{// �R�����g�A�E�g���Ă��Ȃ�
				fscanf(pFile, "%s", &aText[0]);

				if (aText[0] == '#')
				{// ������̐擪���u#�v�Ȃ�΃R�����g�A�E�g��
					bComment = true;
				}
				else
				{// �ʏ펞
					if (strcmp("NUM_MODEL", pText) == 0)
					{// ���f���̐�
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%d", &g_player.nNumModel);
					}
					else if (strcmp("MODEL_FILENAME", pText) == 0)
					{// X�t�@�C�����̓ǂݍ���
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%s", &g_player.aModel[nCntFile].aFileName[0]);
						nCntFile++;
					}
					else if (strcmp("CHARACTERSET", pText) == 0)
					{// �L�����̏��
						while (strcmp("END_CHARACTERSET", pCharaSet) != 0)
						{// �L�������̓ǂݎ��
							fscanf(pFile, "%s", &aCharaSet[0]);
							if (strcmp("NUM_PARTS", pCharaSet) == 0)
							{

							}
							else if (strcmp("RADIUS", pCharaSet) == 0)
							{// �����蔻��
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%f", &g_player.fOutRadius);
							}
							else if (strcmp("HEIGHT", pCharaSet) == 0)
							{// �����蔻��
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%f", &g_player.fHeight);
							}
							else if (strcmp("PARTSSET", pCharaSet) == 0)
							{
								aPartsSet[0] = { NULL };

								while (strcmp("END_PARTSSET", pPartsSet) != 0)
								{
									fscanf(pFile, "%s", &aPartsSet[0]);
									if (strcmp("INDEX", pPartsSet) == 0)
									{// �p�[�c���̓ǂݎ��
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &nIdxParts);
									}
									else if (strcmp("PARENT", pPartsSet) == 0)
									{// �p�[�c�̐e���
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &g_player.aModel[nIdxParts].nIdxModelParent);
									}
									else if (strcmp("POS", pPartsSet) == 0)
									{// �p�[�c�̈ʒu���
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &g_player.aModel[nIdxParts].pos.x);
										fscanf(pFile, "%f", &g_player.aModel[nIdxParts].pos.y);
										fscanf(pFile, "%f", &g_player.aModel[nIdxParts].pos.z);
									}
									else if (strcmp("ROT", pPartsSet) == 0)
									{// �p�[�c�̊p�x���
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &g_player.aModel[nIdxParts].rot.x);
										fscanf(pFile, "%f", &g_player.aModel[nIdxParts].rot.y);
										fscanf(pFile, "%f", &g_player.aModel[nIdxParts].rot.z);
									}
								}
							}
						}	
					}
					else if (strcmp("MOTIONSET", pText) == 0)
					{
						// �ϐ��̏�����
						aMotionSet[0] = { NULL };

						// ���[�J���ϐ��錾
						int nLoop = 0;
						int nKeySet = 0;

						while (strcmp("END_MOTIONSET", pMotionSet) != 0)
						{// ���[�V�����̓ǂݍ���
							fscanf(pFile, "%s", &aMotionSet[0]);
							if (strcmp("LOOP", pMotionSet) == 0)
							{// ���[�v���邩
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &nLoop);
								if (nLoop == 1)
								{// �P�̏ꍇ���[�v����
									g_player.bLoopMotion = true;
									g_player.aMotionInfo[nMotion].bLoop = true;
								}
							}
							else if (strcmp("NUM_KEY", pMotionSet) == 0)
							{// �L�[���̓ǂݍ���
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &g_player.nNumKey);
							}

							else if (strcmp("KEYSET",pMotionSet) == 0)
							{// �L�[�̓ǂݍ���
								// ���[�J���ϐ��錾
								int nKey = 0;
								// �ϐ��̏�����
								aKeySet[0] = { NULL };

								while (strcmp("END_KEYSET", pKeySet) != 0)
								{// �L�[��񍀖ڂ̓ǂݍ���
									fscanf(pFile, "%s", &aKeySet[0]);

									if (aKeySet[0] == '#')
									{// �R�����g�A�E�g
										// ���[�J���ϐ��錾
										char c = NULL;
										char d = NULL;
										fscanf(pFile, "%c", &c);
										while (c != '\n' && d != '\n')
										{
											fscanf(pFile, "%s", &aKey[0]);
											fscanf(pFile, "%c", &d);
										}
									}

									if (strcmp("KEY", pKeySet) == 0)
									{// �L�[���̓ǂݍ���
										aKey[0] = { NULL };
										while (strcmp("END_KEY", pKey) != 0)
										{// �ʒu�E�p�x�̓ǂݍ���
											fscanf(pFile, "%s", &aKey[0]);
											if (strcmp("POS", pKey) == 0)
											{// �ʒu���̓ǂݍ���
												fscanf(pFile, "%s", &aEqual[0]);
												fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosX);
												fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosY);
												fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosZ);
											}
											else if (strcmp("ROT", pKey) == 0)
											{// �p�x���̓ǂݍ���
												fscanf(pFile, "%s", &aEqual[0]);
												fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fRotX);
												fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fRotY);
												fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fRotZ);
											}	
										}
										nKey++;
									}
									else if (strcmp("FRAME", pKeySet) == 0)
									{// �L�[�t���[�����̓ǂݍ���
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].nFrame);
									}
								}
								nKeySet++;
							}
						}
						g_player.aMotionInfo[nMotion].nNumKey = nKeySet;	// �L�[���̐ݒ�
						nMotion++;
					}
				}
			}
			else if (bComment == true)
			{// �R�����g�A�E�g����
				// ���[�J���ϐ��錾
				char a = NULL;
				char b = NULL;
				fscanf(pFile, "%c", &a);
				while (a != '\n' && b != '\n')
				{
					fscanf(pFile, "%s", &aText[0]);
					fscanf(pFile, "%c", &b);
				}
				bComment = false;
			}
		}
		fclose(pFile);
	}
	else
	{ // �t�@�C���W�J�s��
	}
}

//==============================================================================
// �v���C���[�̏�ԕω�
//==============================================================================
void StateChange(void)
{
	// ���[�J���ϐ��錾
	Player *pPlayer = &g_player;

	// �v���C���[�̏�Ԃ��Ƃ̏���
	switch (pPlayer->state)
	{
	case PLAYERSTATE_DAMAGE:	// ��_���[�W
		g_player.bOnBlock = false;					// �n�ʂ��痣���
		PlaySound(SOUND_LABEL_SE_HIT000);			// �������̍Đ�
		g_player.move.x *= -1;						// �ړ��͂𔽓]
		g_player.move.z *= -1;
		g_player.move.y = 5.0f;						// �v���C���[���΂�
		g_bJump = true;
		g_player.nKey = 0;							// �L�[�J�E���g�̃��Z�b�g
		g_player.nCounterMotion = 0;				// ���[�V�����J�E���g�̃��Z�b�g
		pPlayer->motionType = MOTIONTYPE_DAMAGE;	// �_���[�W���[�V�����̎��s

		// �̗͂��c��ꍇ
		if (pPlayer->nLife > 0)
		{
			pPlayer->nCntState--;					// ��ԃJ�E���g�̌��Z
			if (pPlayer->nCntState <= 0)
			{// ���̏�Ԃֈڍs
				pPlayer->nCntState = 120;
				pPlayer->state = PLAYERSTATE_APPEAR;
			}
		}
		else
		{// �̗͂�0�̎�
			pPlayer->state = PLAYERSTATE_GAMEOVER;		// �Q�[���[�I�[�o�[
		}

		break;

	case PLAYERSTATE_APPEAR:	// �o����
		pPlayer->nCntState--;							// ��ԃJ�E���g�̌��Z

		if (pPlayer->nCntState <= 0)
		{// �ʏ��Ԃւ̈ڍs
			pPlayer->state = PLAYERSTATE_NORMAL;		// �ʏ���
		}
		break;
	}
}