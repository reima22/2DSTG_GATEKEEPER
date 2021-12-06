//=============================================================================
//
// �e�L�X�g�f�[�^�Ǘ� [textdata.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTDATA_H_
#define _TEXTDATA_H_

#include "main.h"

// �}�N����`
#define TEXT_NUM	(128)	// �e�L�X�g�f�[�^�o�C�g��

class CTextDataMeshfield;

//==============================================================================
// �e�L�X�g�Ǘ��N���X
//==============================================================================
class CTextData
{
public:
	// �e�L�X�g�f�[�^�̎��
	typedef enum
	{
		TEXTDATA_RANKING = 0,	// �����L���O�̃f�[�^
		TEXTDATA_SYSTEM,		// �Q�[�����[���f�[�^
		TEXTDATA_PLAYER,		// �v���C���[�̃f�[�^
		TEXTDATA_ENEMY,			// �G�f�[�^
		TEXTDATA_MESHFIELD,		// ���b�V���f�[�^
		TEXTDATA_MAX
	}TEXTDATA;

	CTextData();
	~CTextData();

	static void LoadTextAll(void);
	static void UnloadTextAll(void);

	static CTextDataMeshfield *GetMeshfield(void) { return m_pDataMeshfield; }

protected:
	static char *m_pFileName[TEXTDATA_MAX];	// �ǂݍ��ރt�@�C�����p�X

private:
	static CTextDataMeshfield *m_pDataMeshfield;
};

#endif