//=============================================================================
//
// 弾描画処理 [bullet.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "scene2D.h"

// マクロ定義
#define BULLET_LIFE	(50)
#define BULLET_A_X		(10.0f)		// 弾Aの大きさX
#define BULLET_A_Y		(15.0f)		// 弾Aの大きさY
#define BULLET_B_X		(20.0f)		// 弾Bの大きさX
#define BULLET_B_Y		(5.0f)		// 弾Bの大きさY
#define BULLET_C_X		(10.0f)		// 弾Cの大きさX
#define BULLET_C_Y		(20.0f)		// 弾Cの大きさY
#define BULLET_D_X		(20.0f)		// 弾Dの大きさX
#define BULLET_D_Y		(5.0f)		// 弾Dの大きさY

// 弾クラス
class CBullet : public CScene2D
{
public:
	// 撃つ陣営
	typedef enum
	{
		BULLET_SIDE_PLAYER = 0,
		BULLET_SIDE_ENEMY,
		BULLET_SIDE_MAX
	}BULLET_SIDE;

	// 弾の種類
	typedef enum
	{
		BULLET_TYPE_PLAYER0 = CTexture::TEXTYPE_BULLET_0,	// プレイヤーの弾0
		BULLET_TYPE_PLAYER1,								// プレイヤーの弾1(波型)
		BULLET_TYPE_PLAYER2,								// プレイヤーの弾2(ミサイル)
		BULLET_TYPE_PLAYER3,								// プレイヤーの弾3
		BULLET_TYPE_ENEMY,									// 敵の弾
		BULLET_TYPE_MAX
	}BULLET_TYPE;

	CBullet(int nPriority = PRIORITY_BULLET);
	~CBullet();
	HRESULT Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY, D3DXVECTOR3 move, CTexture::TEXTYPE type,D3DXVECTOR3 rot,int nAttack);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBullet *Create(
		D3DXVECTOR3 pos, 
		OBJTYPE objType, 
		float fSizeX, float fSizeY,
		D3DXVECTOR3 move,
		CTexture::TEXTYPE type,
		BULLET_SIDE side,
		D3DXVECTOR3 rot,
		int nAttack);

	static void CreateType(D3DXVECTOR3 pos,int nUse);		// 使用弾に応じて発射する弾のパターンを変える
	static void SetEffectBullet(D3DXVECTOR3 pos, CTexture::TEXTYPE type);
	static void UninitAll(void);

	BULLET_SIDE GetSide(void) { return m_Side; }

	int GetAttack(void) { return m_aAttack; }

protected:

private:
	D3DXVECTOR3 m_move;						// 移動量
	int m_nLife;							// 弾の寿命
	CTexture::TEXTYPE m_Type;				// 弾のテクスチャの種類
	BULLET_SIDE m_Side;						// 誰が撃った弾か
	BULLET_TYPE m_BulletType;				// 弾の種類
	int m_aAttack;							// 攻撃力 
};

#endif
