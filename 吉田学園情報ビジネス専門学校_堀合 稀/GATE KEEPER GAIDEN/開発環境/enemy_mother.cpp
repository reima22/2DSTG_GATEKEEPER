//==============================================================================
//
// 敵母艦処理〔enemy_mother.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "game.h"
#include "enemy.h"
#include "enemy_mother.h"
#include "textdata_enemy.h"
#include "sound.h"
#include "effect.h"
#include "chara3D.h"

// 静的メンバ変数宣言
CEnemyMother::MOTHER_STATE CEnemyMother::m_motherState = MOTHER_STATE_NONE;	// 母艦の状態
CEnemyMother::MOTHER_PHASE CEnemyMother::m_phase = MOTHER_PHASE_0;			// 母艦戦のフェーズ
int CEnemyMother::m_nMotherStartLife;										// 母艦の最大体力
CChara3D *CEnemyMother::m_pMother3D = NULL;									// 本体の3Dモデル
CChara3D *CEnemyMother::m_pSubParts[MOTHER_PARTS_MAX] = {};					// パーツの3Dモデル

//==============================================================================
// コンストラクタ
//==============================================================================
CEnemyMother::CEnemyMother()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CEnemyMother::~CEnemyMother()
{

}

//==============================================================================
// 母艦の生成
//==============================================================================
void CEnemyMother::MotherSetCreate(void)
{
	// 難易度ごとの母艦体力
	switch (CTitleDifficulty::GetDifficulty())
	{
	case CTitleDifficulty::DIFFICULTY_EASY:
		m_nMotherStartLife = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_MOTHER_LIFE_EASY);
		break;

	case CTitleDifficulty::DIFFICULTY_NORMAL:
		m_nMotherStartLife = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_MOTHER_LIFE_NORMAL);
		break;

	case CTitleDifficulty::DIFFICULTY_HARD:
		m_nMotherStartLife = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_MOTHER_LIFE_HARD);
		break;
	}

	D3DXVECTOR3 pos = MOTHER_SET_POS;

	// パーツの体力
	int nLifeParts = 50;

	// 本体
	CEnemy::Create(pos, CScene::OBJTYPE_ENEMY, MOTHER_ENEMY_X, MOTHER_ENEMY_Y,
		CTexture::TEXTYPE_ENEMY_4_MOTHER,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		m_nMotherStartLife,
		0, CEnemy::BULLET_PATTERN_NONE, false, ENEMY_TYPE_MOTHER);

	// 左
	CEnemy::Create(D3DXVECTOR3(pos.x - 120.0f, pos.y, 0.0f), CScene::OBJTYPE_ENEMY, MOTHER_PARTS_SIZE, MOTHER_PARTS_SIZE,
		CTexture::TEXTYPE_MOTHER_PARTS,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		nLifeParts,
		0, CEnemy::BULLET_PATTERN_NONE, false, ENEMY_TYPE_MOTHER_PARTS_LEFT);

	// 右
	CEnemy::Create(D3DXVECTOR3(pos.x + 120.0f, pos.y, 0.0f), CScene::OBJTYPE_ENEMY, MOTHER_PARTS_SIZE, MOTHER_PARTS_SIZE,
		CTexture::TEXTYPE_MOTHER_PARTS,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		nLifeParts,
		0, CEnemy::BULLET_PATTERN_NONE, false, ENEMY_TYPE_MOTHER_PARTS_RIGHT);

	// 中央
	CEnemy::Create(D3DXVECTOR3(pos.x, pos.y + 80.0f, 0.0f), CScene::OBJTYPE_ENEMY, MOTHER_PARTS_SIZE, MOTHER_PARTS_SIZE,
		CTexture::TEXTYPE_MOTHER_PARTS,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		nLifeParts,
		0, CEnemy::BULLET_PATTERN_NONE, false, ENEMY_TYPE_MOTHER_PARTS_CENTER);

	// 3Dモデル
	//m_pMother3D = CChara3D::Create(D3DXVECTOR3(0.0f, 90.0f, 0.0f),D3DXVECTOR3(0.0f,-0.2f,0.0f),0);

	//m_pSubParts[MOTHER_PARTS_LEFT] = CChara3D::Create(D3DXVECTOR3(-30.0f, 90.0f, 0.0f), D3DXVECTOR3(0.0f, -0.2f, 0.0f), 0);
	//m_pSubParts[MOTHER_PARTS_RIGHT] = CChara3D::Create(D3DXVECTOR3(30.0f, 90.0f, 0.0f), D3DXVECTOR3(0.0f, -0.2f, 0.0f), 0);
	//m_pSubParts[MOTHER_PARTS_CENTER] = CChara3D::Create(D3DXVECTOR3(0.0f, 70.0f, 0.0f), D3DXVECTOR3(0.0f, -0.2f, 0.0f), 0);
}

//==============================================================================
// 母艦パーツの挙動
//==============================================================================
void CEnemyMother::MoveParts(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 posLeft = MOTHER_PARTS_POS_0;
	D3DXVECTOR3 posRight = MOTHER_PARTS_POS_1;
	D3DXVECTOR3 posCenter = MOTHER_PARTS_POS_2;

	// 母艦の情報
	// 母艦の位置取得
	D3DXVECTOR3 pos;
	CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
	CScene *pSceneNext = NULL;

	while (pScene)
	{
		// 次のポインタ
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene->GetObjType() == OBJTYPE_ENEMY)
		{// 敵である
			CEnemy *pEnemy = (CEnemy*)pScene;

			if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOTHER)
			{// 母艦である
			 // 位置の取得
				pos = pEnemy->GetPosition();
				break;
			}
		}

		// 次のポインタ
		pScene = pSceneNext;
	}

	// ポインタ修正
	pScene = CScene::GetScene(PRIORITY_CHARA);
	pSceneNext = NULL;

	while (pScene)
	{
		// 次のポインタ
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene->GetObjType() == OBJTYPE_ENEMY)
		{// 敵である
			CEnemy *pEnemy = (CEnemy*)pScene;

			if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOTHER_PARTS_LEFT)
			{
				posLeft = pos + posLeft;
				pEnemy->SetPosition(posLeft);
			}
			else if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOTHER_PARTS_RIGHT)
			{
				posRight = pos + posRight;
				pEnemy->SetPosition(posRight);
			}
			else if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOTHER_PARTS_CENTER)
			{
				posCenter = pos + posCenter;
				pEnemy->SetPosition(posCenter);
			}
		}

		// 次のポインタ
		pScene = pSceneNext;
	}
}