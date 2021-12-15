//==============================================================================
//
// �e�L�X�g�f�[�^�Ǘ��ktextdata.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include "textdata.h"
#include "textdata_player.h"
#include "textdata_enemy.h"
#include "textdata_system.h"
#include "textdata_meshfield.h"
#include "stdio.h"

// �ÓI�����o�ϐ��錾
char *CTextData::m_pFileName[TEXTDATA_MAX] =
{
	{ "data/TEXT/ranking.txt" },		// �����L���O
	{ "data/TEXT/systemdata.txt" },		// ���[���Ǘ�
	{ "data/TEXT/playerdata.txt" },		// �v���C���[�f�[�^
	{ "data/TEXT/enemydata.txt" },		// �G�f�[�^
	{ "data/TEXT/meshfielddata.txt" },		// ���b�V���f�[�^
};
CTextDataMeshfield *CTextData::m_pDataMeshfield = NULL;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CTextData::CTextData()
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CTextData::~CTextData()
{

}

//==============================================================================
// �e�N�X�`���f�[�^�̑S�ǂݍ���
//==============================================================================
void CTextData::LoadTextAll(void)
{
	// �v���C���[�f�[�^�̓ǂݍ���
	CTextDataPlayer::LoadData();

	// �G�f�[�^�̓ǂݍ���
	CTextDataEnemy::LoadData();

	// �G�f�[�^�̃J�E���g�ݒ�
	CTextDataEnemy::CreateCntSet();

	// ���[���̃f�[�^�ǂݍ���
	CTextDataSystem::LoadData();

	// ���b�V���t�B�[���h
	m_pDataMeshfield = CTextDataMeshfield::Create();
}

//==============================================================================
// �e�N�X�`���f�[�^�̑S�j��
//==============================================================================
void CTextData::UnloadTextAll(void)
{
	// ���b�V���t�B�[���h
	m_pDataMeshfield->UnloadData();
}