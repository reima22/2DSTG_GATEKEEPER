//==============================================================================
//
// 敵編隊生成処理〔enemy_formation.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "game.h"
#include "enemy.h"
#include "enemy_formation.h"
#include "textdata_enemy.h"
#include "sound.h"
#include "effect.h"

// 静的メンバ変数宣言
int CEnemyFormation::m_nFormNumInterval = 0;		// 隊列生成の個別間の間隔
int CEnemyFormation::m_nFormSetInterval = 0;		// 隊列そのものの生成間隔
bool CEnemyFormation::m_bFormSetInt = false;		// 隊列を生成するか
D3DXVECTOR3 CEnemyFormation::m_pos;					// 隊列配置開始位置
D3DXVECTOR3 CEnemyFormation::m_moveRot;				// 隊列配置移動方向
float CEnemyFormation::m_fMove;						// 隊列移動力
CTexture::TEXTYPE CEnemyFormation::m_type;			// 隊列の種類
int CEnemyFormation::m_nInterval = 0;				// 生成する感覚
bool CEnemyFormation::m_bSetParam = false;			// パラメータを設定するか
bool CEnemyFormation::m_bLock = false;				// 生成の中断

//==============================================================================
// コンストラクタ
//==============================================================================
CEnemyFormation::CEnemyFormation()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CEnemyFormation::~CEnemyFormation()
{

}

//==============================================================================
// 敵の編隊状態生成
//==============================================================================
void CEnemyFormation::SetFormationEnemy(D3DXVECTOR3 pos, OBJTYPE objType, CTexture::TEXTYPE type, D3DXVECTOR3 moveRot, float fMove, int nSetNum, int nInterval, TEAM_PATTERN team)
{
	// 移動倍率
	moveRot.x *= fMove;
	moveRot.y *= fMove;

	// 弾ランダム生成カウント基準値
	int nSetBulletMin;
	int nSetBulletDif;

	// 母艦戦時
	if (team == TEAM_PATTERN_MOTHER_0 || team == TEAM_PATTERN_MOTHER_1 || team == TEAM_PATTERN_MOTHER_2)
	{
		nSetBulletMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_MOTHER_MIN);
		nSetBulletDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_MOTHER_DIFFERENCE);
	}
	else
	{// 通常
		nSetBulletMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_MIN);
		nSetBulletDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_DIFFERENCE);
	}

	// 下段移動パターン
	bool bUnder = false;
	if (team == TEAM_PATTERN_UNDER)
	{
		bUnder = true;
	}

	// ローカル変数宣言
	int nNumMax = 1;	// 一度の最大配置数

						// 配置パターンによる設定
	if (team == TEAM_PATTERN_SIDE || team == TEAM_PATTERN_MOTHER_1 || team == TEAM_PATTERN_MOTHER_PARTS)
	{
		nNumMax = 2;
	}
	else if (team == TEAM_PATTERN_MOTHER_2)
	{
		nNumMax = 3;
	}

	m_nFormNumInterval++;

	if (m_nFormNumInterval % nInterval == 0)
	{
		// ローカル変数宣言
		float fSizeX;
		float fSizeY;
		int nLife;
		ENEMY_TYPE enemyType;
		int nBulletDelay = rand() % nSetBulletDif + nSetBulletMin;

		// 各パーツ
		CEnemy *pLeft = NULL;
		CEnemy *pRight = NULL;
		CEnemy *pCenter = NULL;

		// 種類ごとの設定
		switch (type)
		{
		case CTexture::TEXTYPE_ENEMY_0:
			fSizeX = ENEMYX_0;
			fSizeY = ENEMYY_0;
			nLife = 1;
			enemyType = ENEMY_TYPE_0;
			break;

		case CTexture::TEXTYPE_ENEMY_1:
			fSizeX = ENEMYX_1;
			fSizeY = ENEMYY_1;
			nLife = 2;
			enemyType = ENEMY_TYPE_1;
			break;

		case CTexture::TEXTYPE_ENEMY_2:
			fSizeX = ENEMYX_2;
			fSizeY = ENEMYY_2;
			nLife = 3;
			enemyType = ENEMY_TYPE_2;
			break;

		case CTexture::TEXTYPE_ENEMY_3:
			fSizeX = ENEMYX_3;
			fSizeY = ENEMYY_3;
			nLife = 4;
			enemyType = ENEMY_TYPE_3;
			break;
		}

		// ローカル変数宣言
		D3DXVECTOR3 posLeft;
		D3DXVECTOR3 posRight;
		D3DXVECTOR3 posCenter;

		int nLifeLeft = 0;
		int nLifeRight = 0;
		int nLifeCenter = 0;

		// 母艦戦、敵生成エフェクト
		if (team == TEAM_PATTERN_MOTHER_0 || team == TEAM_PATTERN_MOTHER_1 || team == TEAM_PATTERN_MOTHER_2 || team == TEAM_PATTERN_MOTHER_PARTS)
		{
			CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
			CScene *pSceneNext = NULL;

			while (pScene)
			{
				pSceneNext = CScene::GetSceneNext(pScene);

				if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{// 敵である
					CEnemy *pEnemy = (CEnemy*)pScene;

					if (pEnemy->GetEnemyType() == ENEMY_TYPE_MOTHER_PARTS_LEFT)
					{// 左パーツ
					 // 位置の取得
						pLeft = pEnemy;
					}

					if (pEnemy->GetEnemyType() == ENEMY_TYPE_MOTHER_PARTS_RIGHT)
					{// 右パーツ
						pRight = pEnemy;
					}

					if (pEnemy->GetEnemyType() == ENEMY_TYPE_MOTHER_PARTS_CENTER)
					{// 中央パーツ
						pCenter = pEnemy;
					}
				}

				// 次のポインタ
				pScene = pSceneNext;
			}

			if (pLeft != NULL)
			{
				nLifeLeft = pLeft->GetLife(CHARA_TYPE_ENEMY);
				posLeft = pLeft->GetPosition();
			}

			if (pRight != NULL)
			{
				nLifeRight = pRight->GetLife(CHARA_TYPE_ENEMY);
				posRight = pRight->GetPosition();
			}

			if (pCenter != NULL)
			{
				nLifeCenter = pCenter->GetLife(CHARA_TYPE_ENEMY);
				posCenter = pCenter->GetPosition();
			}
		}

		// 複数設定
		if (team == TEAM_PATTERN_SIDE)
		{
			for (int nNum = 0; nNum < nNumMax; nNum++)
			{
				if (nNum < 1)
				{// 1体目
					CEnemy::Create(pos, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
				}
				else
				{// 2体目
					CEnemy::Create(D3DXVECTOR3(pos.x + 1200.0f, pos.y, 0.0f), objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
				}
			}
		}
		else if (team == TEAM_PATTERN_MOTHER_0)
		{// 通常設定
			if (pCenter != NULL)
			{
				// 生成音
				CSound::Play(CSound::SOUND_LABEL_SE_MOTHER_CREATE);

				for (int nCnt = 0; nCnt < 20; nCnt++)
				{
					// 波紋型エフェクト
					CEffect::Create(posCenter, 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
					CEffect::Create(D3DXVECTOR3(posCenter.x + 10.0f, posCenter.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
					CEffect::Create(D3DXVECTOR3(posCenter.x - 10.0f, posCenter.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
				}

				CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
			}
		}
		else if (team == TEAM_PATTERN_MOTHER_1)
		{
			if (pCenter != NULL)
			{
				// 生成音
				CSound::Play(CSound::SOUND_LABEL_SE_MOTHER_CREATE);

				for (int nNum = 0; nNum < nNumMax; nNum++)
				{
					if (nNum < 1)
					{// 1体目
						CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);

						for (int nCnt = 0; nCnt < 20; nCnt++)
						{
							// 波紋型エフェクト
							CEffect::Create(posCenter, 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
							CEffect::Create(D3DXVECTOR3(posCenter.x + 10.0f, posCenter.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
							CEffect::Create(D3DXVECTOR3(posCenter.x - 10.0f, posCenter.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
						}
					}
					else
					{// 2体目

						CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, D3DXVECTOR3(-moveRot.x, moveRot.y, moveRot.z), nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
					}
				}
			}
		}
		else if (team == TEAM_PATTERN_MOTHER_2)
		{
			if (pCenter != NULL)
			{
				// 生成音
				CSound::Play(CSound::SOUND_LABEL_SE_MOTHER_CREATE);

				for (int nCnt = 0; nCnt < 20; nCnt++)
				{
					// 波紋型エフェクト
					CEffect::Create(pos, 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
					CEffect::Create(D3DXVECTOR3(pos.x + 10.0f, pos.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
					CEffect::Create(D3DXVECTOR3(pos.x - 10.0f, pos.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
				}

				for (int nNum = 0; nNum < nNumMax; nNum++)
				{
					if (nNum < 1)
					{// 1体目
						CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
					}
					else if (nNum < 2)
					{// 2体目
						CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, D3DXVECTOR3(0.0f, moveRot.y, moveRot.z), nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
					}
					else
					{// 3体目
						CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, D3DXVECTOR3(-moveRot.x, moveRot.y, moveRot.z), nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
					}
				}
			}
		}
		else if (team == TEAM_PATTERN_MOTHER_PARTS)
		{
			for (int nNum = 0; nNum < nNumMax; nNum++)
			{
				if (nNum < 1 && pLeft != NULL)
				{
					// 生成音
					CSound::Play(CSound::SOUND_LABEL_SE_MOTHER_CREATE);

					CEnemy::Create(posLeft, objType, fSizeX, fSizeY, type, D3DXVECTOR3(-moveRot.x, moveRot.y, 0.0f), nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);

					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						// 波紋型エフェクト
						CEffect::Create(posLeft, 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_LEFT);
						CEffect::Create(D3DXVECTOR3(posLeft.x, posLeft.y + 10.0f, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_LEFT);
						CEffect::Create(D3DXVECTOR3(posLeft.x, posLeft.y - 10.0f, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_LEFT);
					}
				}
				else if (nNum < 2 && pRight != NULL)
				{
					// 生成音
					CSound::Play(CSound::SOUND_LABEL_SE_MOTHER_CREATE);

					CEnemy::Create(posRight, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);

					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						// 波紋型エフェクト
						CEffect::Create(posRight, 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_RIGHT);
						CEffect::Create(D3DXVECTOR3(posRight.x, posRight.y + 10.0f, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_RIGHT);
						CEffect::Create(D3DXVECTOR3(posRight.x, posRight.y - 10.0f, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_RIGHT);

					}
				}
			}
		}
		else
		{// 単体
			CEnemy::Create(pos, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
		}
	}

	if (m_nFormNumInterval >= nInterval * nSetNum || CGame::GetSetEnemy() == false)
	{
		// 編成生成のインターバルへ
		m_bFormSetInt = true;

		// 生成判定を戻す
		m_bSetParam = false;

		// カウントリセット
		m_nFormNumInterval = 0;
	}
}

//==============================================================================
// 敵の編隊生成情報ランダム化
//==============================================================================
void CEnemyFormation::SetFormRandom(TEAM_PATTERN team)
{
	// ローカル変数宣言
	CEnemy *pEnemy = NULL;
	// 母艦の位置取得
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
	CScene *pSceneNext = NULL;

	switch (team)
	{
	case TEAM_PATTERN_RANDOM: // ランダム配置

		if (m_bSetParam == false)
		{
			// 位置の設定
			m_pos = D3DXVECTOR3((float)(rand() % 1200 + 40), ENEMY_SET_POS_Y, 0.0f);

			// 種類の設定
			m_type = (CTexture::TEXTYPE)(rand() % 4 + (int)(CTexture::TEXTYPE_ENEMY_0));

			// 移動方向・力の設定
			float fSinAngle = ((float)(rand() % 314 - 157) / 100.0f);
			m_moveRot = D3DXVECTOR3(sinf(fSinAngle), cosf(0.0f), 0.0f);
			m_fMove = (float)(rand() % 10 + 10) / 10.0f;

			// 生成する間隔
			m_nInterval = 60;

			// 設定完了
			m_bSetParam = true;
		}

		// 編隊配置
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 5, m_nInterval, team);

		break;

	case TEAM_PATTERN_SIDE:	// 両サイド配置

		if (m_bSetParam == false)
		{
			// 位置の設定
			m_pos = D3DXVECTOR3(40.0f, ENEMY_SET_POS_Y, 0.0f);

			// 種類の設定
			m_type = CTexture::TEXTYPE_ENEMY_1;

			// 移動方向・力の設定
			m_moveRot = D3DXVECTOR3(0.0f, cosf(0.0f), 0.0f);
			m_fMove = 1.0f;

			// 生成する間隔
			m_nInterval = 60;

			// 設定完了
			m_bSetParam = true;
		}

		// 編隊配置(左右)
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 3, m_nInterval, team);

		break;

	case TEAM_PATTERN_UNDER:

		if (m_bSetParam == false)
		{
			// 左右のどちらに出るか
			bool bLeft = false;

			int nRand = rand() % 2;

			if (nRand == 0)
			{
				bLeft = true;
			}

			if (bLeft == true)
			{
				// 位置の設定
				m_pos = D3DXVECTOR3(ENEMY_SET_POS_X_UNDER_L, ENEMY_SET_POS_Y_UNDER, 0.0f);

				// 移動方向・力の設定
				m_moveRot = D3DXVECTOR3(sinf(D3DX_PI / 2), 0.0f, 0.0f);
				m_fMove = (float)(rand() % 10 + 10) / 10.0f;
			}
			else
			{
				// 位置の設定
				m_pos = D3DXVECTOR3(ENEMY_SET_POS_X_UNDER_R, ENEMY_SET_POS_Y_UNDER, 0.0f);

				// 移動方向・力の設定
				m_moveRot = D3DXVECTOR3(sinf(-D3DX_PI / 2), 0.0f, 0.0f);
				m_fMove = (float)(rand() % 10 + 10) / 10.0f;
			}

			// 種類の設定
			m_type = (CTexture::TEXTYPE)(rand() % 4 + (int)(CTexture::TEXTYPE_ENEMY_0));

			// 生成する間隔
			m_nInterval = 60;

			// 設定完了
			m_bSetParam = true;
		}

		// 編隊配置
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 5, m_nInterval, team);

		break;

	case TEAM_PATTERN_MOTHER_0:

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

		// 位置の設定
		m_pos = pos;

		if (m_bSetParam == false)
		{
			// 種類の設定
			m_type = (CTexture::TEXTYPE)(rand() % 4 + (int)(CTexture::TEXTYPE_ENEMY_0));

			// 移動方向・力の設定
			float fSinAngle = ((float)(rand() % 314 - 157) / 100.0f);
			m_moveRot = D3DXVECTOR3(sinf(fSinAngle), cosf(0.0f), 0.0f);
			m_fMove = (float)(rand() % 10 + 10) / 10.0f;

			// 設定完了
			m_bSetParam = true;
		}

		// 編隊配置
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 5, m_nInterval, team);
		break;

	case TEAM_PATTERN_MOTHER_1:
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

		// 位置の設定
		m_pos = pos;

		// 種類の設定
		m_type = CTexture::TEXTYPE_ENEMY_3;

		// それぞれの移動量
		m_moveRot = D3DXVECTOR3(sinf(D3DX_PI / 2), cosf(0.0f), 0.0f);

		// 編隊配置
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 1, m_nInterval, team);

		break;

	case TEAM_PATTERN_MOTHER_2:

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

		// 位置の設定
		m_pos = pos;

		// 種類の設定
		m_type = CTexture::TEXTYPE_ENEMY_2;

		// それぞれの移動量
		m_moveRot = D3DXVECTOR3(sinf(D3DX_PI / 2), cosf(0.0f), 0.0f);

		// 編隊配置
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 1, m_nInterval, team);

		break;

	case TEAM_PATTERN_MOTHER_PARTS:

		// 種類の設定
		m_type = CTexture::TEXTYPE_ENEMY_1;

		// それぞれの移動量
		m_moveRot = D3DXVECTOR3(sinf(D3DX_PI / 2) * 4.0f, 0.0f, 0.0f);

		// 編隊配置
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 3, m_nInterval, team);

		break;
	}

}

//==============================================================================
// インターバルカウント
//==============================================================================
void CEnemyFormation::FormInterval(int nIntervalCnt)
{
	// カウント加算
	m_nFormSetInterval++;

	if (m_nFormSetInterval >= nIntervalCnt)
	{
		m_nFormSetInterval = 0;
		m_bFormSetInt = false;
	}
}