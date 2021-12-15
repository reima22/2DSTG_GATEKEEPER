//=============================================================================
//
// �N�����`�揈�� [invasion.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _INVASION_H_
#define _INVASION_H_

#include "main.h"
#include "scene.h"

// �}�N����`
#define INVASION_NUM_MAX	(2)	// �ő包��
#define INV_SIZE	(D3DXVECTOR2(10.0f, 20.0f))

// �O���錾
class CNumber;

//==============================================================================
// �N�����N���X
//==============================================================================
class CInvasion : public CScene2D
{
public:
	CInvasion(int nPriority = PRIORITY_UI);
	~CInvasion();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CInvasion *Create(D3DXVECTOR3 pos);
	void SetInvasion(int nDefeat);
	static void AddInvasion(void);
	static int GetInvasion(void) { return m_nInvasion; }

private:
	static CNumber *m_apNumber[INVASION_NUM_MAX];	// �i���o�[�N���X�|�C���^
	static int m_nInvasion;					// �N�����l
};

#endif
