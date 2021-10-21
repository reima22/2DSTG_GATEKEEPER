//=============================================================================
//
// キャラクター描画処理 [character.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "main.h"
#include "scene2D.h"
#include "item.h"

// 前方宣言
class CBullet;
class CEnemy;
class CItem;

// キャラクタークラス
class CCharacter : public CScene2D
{
public:
	// キャラクターの種別
	typedef enum
	{
		CHARA_TYPE_PLAYER = 0,
		CHARA_TYPE_ENEMY,
		CHARA_TYPE_MAX
	}CHARA_TYPE;

	CCharacter(int nPriority = PRIORITY_CHARA);
	virtual ~CCharacter() = 0;
	virtual HRESULT Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY,int nBulletSetDelay) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void DamageLife(int nDamage,D3DXVECTOR3 pos) = 0;

	void AddLife(int nLife,CHARA_TYPE type) { m_nLife[type] += nLife; }

	// 弾との当たり判定
	void CollisionBullet(			// 弾との当たり判定
		D3DXVECTOR3 posChara,		// キャラの位置
		float fSizeCharaX,			// キャラの幅
		float fSizeCharaY);			// キャラの高さ

	// プレイヤーと敵の当たり判定
	void CollisionChara(
		D3DXVECTOR3 posChara, 
		float fSizeCharaX, 
		float fSizeCharaY);

	// プレイヤーとアイテムの当たり判定
	void CollisionItem(
		D3DXVECTOR3 posChara,
		float fSizeCharaX,
		float fSizeCharaY);
					
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }						// 移動力の設定
	void SetLife(CHARA_TYPE type,int nLife) { m_nLife[type] = nLife; }		// 体力の設定
	int GetLife(CHARA_TYPE type) { return m_nLife[type]; }					// 体力の取得
	void CharaSetType(CHARA_TYPE charaType) { m_charaType = charaType; }	// キャラの種類の取得

protected:
	int m_nLife[CHARA_TYPE_MAX];	// 体力
	D3DXVECTOR3 m_Move;				// 移動量
	CHARA_TYPE m_charaType;			// キャラの種類

private:
	
};

#endif