//=============================================================================
//
// 弾UI描画処理 [bullet_UI.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _bullet_UI_H_
#define _bullet_UI_H_

#include "main.h"
#include "scene.h"
#include "item.h"

// マクロ定義　
#define BUI_SIZE_X	(32.0f)	// UI大きさ
#define BUI_SIZE_Y	(34.0f)	// UI大きさ

// 前方宣言
class CPolygon;

//==============================================================================
// 弾のUIクラス
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
	static CScene2D *m_apScene2D[BUITYPE_MAX];	// Scene2Dクラスポインタ
	int m_aPattern[BUITYPE_MAX];				// テクスチャ座標情報
	static bool m_bTake[BUITYPE_MAX];			// 弾の所有状態
	static int m_nUse;							// 使用している弾

	static D3DXVECTOR2 m_size[BUITYPE_MAX];		// アイコンのサイズ
};
#endif