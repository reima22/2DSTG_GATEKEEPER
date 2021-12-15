//=============================================================================
//
// �e�N�X�`���Ǘ� [texture.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

//==============================================================================
// �e�N�X�`���Ǘ��N���X
//==============================================================================
class CTexture
{
public:
	CTexture();
	~CTexture();

	HRESULT LoadAll(void);
	void UnloadAll(void);

	static CTexture *Create(void);

	LPDIRECT3DTEXTURE9 GetAddress(int nIdx) { return *(LPDIRECT3DTEXTURE9*)&m_pTexture[nIdx]; }

	// 3D���f���ւ̃e�N�X�`�����蓖��
	LPDIRECT3DTEXTURE9 GetAddress3D(char *pTexFilename);

	int GetTexNum(void) { return m_nTexMax; }

private:
	char **m_pFileName;				// �ۊǗp�|�C���^
	LPDIRECT3DTEXTURE9 m_pTexture;	// �|�C���^�z��i�[�A�h���X
	int m_nTexMax;					// �e�N�X�`���̍ő吔
};

#endif
