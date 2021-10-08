//=============================================================================
//
// ���f���Ǘ� [model.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//==============================================================================
// �e�N�X�`���Ǘ��N���X
//==============================================================================
class CModel
{
public:
	// �e�N�X�`���̎��
	typedef enum
	{
		MODELTYPE_METEOR = 0,	// 覐�
		MODELTYPE_DEBRI_0,		// �F���S�~0
		MODELTYPE_DEBRI_1,		// �F���S�~1
		MODELTYPE_MAX			// �ő吔
	}MODELTYPE;

	CModel();
	~CModel();

	static char *GetFileName(int nType) { return m_pFileName[nType]; }
	static void UnloadAll(void);

private:
	static char *m_pFileName[MODELTYPE_MAX];	// �ǂݍ��ރt�@�C�����p�X
};

#endif