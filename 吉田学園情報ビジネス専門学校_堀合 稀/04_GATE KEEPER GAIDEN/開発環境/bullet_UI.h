//=============================================================================
//
// �eUI�`�揈�� [bullet_UI.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _bullet_UI_H_
#define _bullet_UI_H_

#include "main.h"
#include "scene.h"
#include "item.h"

// �}�N����`�@
#define BUI_SIZE_X	(32.0f)	// UI�傫��
#define BUI_SIZE_Y	(34.0f)	// UI�傫��

// �O���錾
class CPolygon;

//==============================================================================
// �e��UI�N���X
//==============================================================================
class CBulletUI : public CScene2D
{
public:
	typedef enum
	{
		BUITYPE_NORMAL = 0,
		BUITYPE_WAVE,
		BUITYPE_MISSILE,
		BUITYPE_REVERSE,
		BUITYPE_MAX
	}BUITYPE;

	CBulletUI(int nPriority = PRIORITY_UI);
	~CBulletUI();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBulletUI *Create(D3DXVECTOR3 pos);
	static int GetUseNum(void) { return m_nUse; }
	void BulletChangeRight(void);
	void BulletChangeLeft(void);
	void BulletStatus(void);

	static void AddBullet(CItem::ITEM_TYPE type);

	static bool GetTakeBullet(int nIdx) { return m_bTake[nIdx]; }

private:
	static CScene2D *m_apScene2D[BUITYPE_MAX];	// Scene2D�N���X�|�C���^
	int m_aPattern[BUITYPE_MAX];				// �e�N�X�`�����W���
	static bool m_bTake[BUITYPE_MAX];			// �e�̏��L���
	static int m_nUse;							// �g�p���Ă���e

	static D3DXVECTOR2 m_size[BUITYPE_MAX];		// �A�C�R���̃T�C�Y
};
#endif