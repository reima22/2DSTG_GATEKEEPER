//=============================================================================
//
// 当たり判定管理 [collision.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"
#include "model.h"
#include "player.h"
#include "meshfield.h"

//==============================================================================
// 当たり判定管理クラス
//==============================================================================
class CCollision
{
public:
	CCollision();
	~CCollision();

	static CCollision *Create(void);

	// 3Dモデル間の当たり判定
	bool Collision3D(CPlayer *pPlayer, CModel *pObject);
	//bool Collision3DMove(CPlayer *pPlayer, CModel *pObject);

	// メッシュフィールドの判定
	bool CollisionMesh(CPlayer *pPlayer, CMeshfield *pMesh);

	bool GetCollisionBool(void) { return m_bCollision; }
	void SetCollisionBool(bool bCollision) { m_bCollision = bCollision; }

	bool GetOnObject(void) { return m_bOnObject; }

private:
	bool m_bCollision;			// 触れている判定
	bool m_bOnObject;			// オブジェクトよりも上にある
	bool m_bUnderObject;		// オブジェクトよりも下にいる
};

#endif