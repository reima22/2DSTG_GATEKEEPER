//=============================================================================
//
// �A�C�e���`�揈�� [item.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"
#include "scene2D.h"

// �}�N����`
#define ITEM_X			(20.0f)	// �A�C�e���̑傫��X��
#define ITEM_Y			(20.0f)	// �A�C�e���̑傫��Y��
#define ITEM_BLINK		(0.02f)	// ���l�̕ω�����l
#define ITEM_BLINK_MIN	(0.5f)	// ���l�̍ŏ�
#define ITEM_BLINK_MAX	(1.0f)	// ���l�̍ő�
#define ITEM_CHASE_AREA (50.0f)	// �Ǐ]����͈�

// �A�C�e���N���X
class CItem : public CScene2D
{
public:
	// �A�C�e���̎��
	typedef enum
	{
		ITEM_TYPE_0 = CTexture::TEXTYPE_ITEM_0,	// �E�F�[�u�擾
		ITEM_TYPE_1,							// �~�T�C���擾
		ITEM_TYPE_2,							// ���o�[�X�E�F�[�u�擾
		ITEM_TYPE_3,							// �񕜃A�C�e��
		ITEM_TYPE_MAX
	}ITEM_TYPE;

	CItem(int nPriority = PRIORITY_ITEM);
	~CItem();
	HRESULT Init(D3DXVECTOR3 pos, 
		OBJTYPE objType, 
		float fSizeX, 
		float fSizeY, 
		D3DXVECTOR3 move, 
		CTexture::TEXTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CItem *Create(
		D3DXVECTOR3 pos,
		OBJTYPE objType,
		float fSizeX, float fSizeY,
		D3DXVECTOR3 move,
		CTexture::TEXTYPE type);

	static void RandCreate(D3DXVECTOR3 pos);

	void SetType(CTexture::TEXTYPE texType);

	void SetBlink(void);

	void SetChaseMove(D3DXVECTOR3 posItem);

	CItem::ITEM_TYPE GetType(void) { return m_Type; }

	// �Ǐ]�J�n
	void SetChase(void) { m_bChase = true; }

private:
	D3DXVECTOR3 m_move;					// �ړ���
	ITEM_TYPE m_Type;					// �A�C�e���̎��
	D3DXCOLOR m_col;					// �F
	int m_nBlinkCnt;					// �_�ŃJ�E���g
	static int m_nSetHealCnt;			// �񕜃A�C�e�������J�E���g
	bool m_bBlinkUp;					// �_�Ńt�F�C�Y
	bool m_bChase;						// �A�C�e���̒ǔ�����
};

#endif
