//=============================================================================
//
// �`��D���G�t�F�N�g�`�揈�� [effect_on.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _EFFECT_ON_H_
#define _EFFECT_ON_H_

#include "main.h"
#include "scene2D.h"

// �`��D���G�t�F�N�g�N���X
class CEffectOn : public CScene2D
{
public:
	// �G�t�F�N�g�̎��
	typedef enum
	{
		EFFECT_TYPE_PARTICLE = 0,			// �g�U�p�[�e�B�N��
		EFFECT_TYPE_PARTICLE_DIRECTION,		// �w�����p�[�e�B�N��
		EFFECT_TYPE_WAVE,					// �g��^
		EFFECT_TYPE_PARTICLE_UNDER,			// �������p�[�e�B�N��
		EFFECT_TYPE_MAX
	}EFFECT_TYPE;

	CEffectOn(int nPriority = PRIORITY_EFFECT_ON);
	~CEffectOn();
	HRESULT Init(D3DXVECTOR3 pos, float fMove, D3DXCOLOR col, float fRadius, float fAlphaDown, int nPattern);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEffectOn *Create(D3DXVECTOR3 pos, float fMove, D3DXCOLOR col, float fRadius, float fAlphaDown, int nPattern);

private:
	D3DXCOLOR m_col;						// �F
	float m_fRadius;						// ���a
	float m_fAlphaDown;						// ���x�ቺ�l
	D3DXVECTOR3 m_Move;						// �ړ���
	EFFECT_TYPE m_effectType;				// �G�t�F�N�g�̎��
};

#endif
