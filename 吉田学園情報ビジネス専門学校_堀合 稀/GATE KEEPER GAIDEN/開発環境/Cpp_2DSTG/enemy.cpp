//==============================================================================
//
// 敵描画処理〔enemy.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "enemy.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "renderer.h"
#include "texture.h"
#include "item.h"
#include "score.h"
#include "defeat.h"
#include "effect.h"
#include "effect_on.h"
#include "game.h"
#include "textdata_enemy.h"
#include "caution.h"
#include "sound.h"

// 静的メンバ変数宣言
int CEnemy::m_nNumEnemy = 0;						// 総数
int CEnemy::m_nSetEnemyCnt = 0;						// 自動生成設定カウント
CEnemy::BULLET_PATTERN CEnemy::m_patternSet = 
BULLET_PATTERN_NORMAL;								// 弾の発射パターン

int CEnemyFormation::m_nFormNumInterval = 0;		// 隊列生成の個別間の間隔
int CEnemyFormation::m_nFormSetInterval = 0;		// 隊列そのものの生成間隔
bool CEnemyFormation::m_bFormSetInt = false;		// 隊列を生成するか
D3DXVECTOR3 CEnemyFormation::m_pos;					// 隊列配置開始位置
D3DXVECTOR3 CEnemyFormation::m_moveRot;				// 隊列配置移動方向
float CEnemyFormation::m_fMove;						// 隊列移動力
CTexture::TEXTYPE CEnemyFormation::m_type;			// 隊列の種類
int CEnemyFormation::m_nInterval = 0;				// 生成する感覚
bool CEnemyFormation::m_bSetParam = false;			// パラメータを設定するか
bool CEnemyFormation::m_bLock = false;

CEnemyMother::MOTHER_STATE CEnemyMother::m_motherState = MOTHER_STATE_NONE;	// 母艦の状態
CEnemyMother::MOTHER_PHASE CEnemyMother::m_phase = MOTHER_PHASE_0;			// 母艦戦のフェーズ
int CEnemyMother::m_nMotherStartLife;										// 母艦の最大体力

//==============================================================================
// コンストラクタ
//==============================================================================
CEnemy::CEnemy()
{
	m_nNumEnemy++;		// 総数の加算
	m_nBulletCnt = 0;
	m_nBulletSetDelay = 0;
}

//==============================================================================
// デストラクタ
//==============================================================================
CEnemy::~CEnemy()
{

}

//==============================================================================
// 敵の初期化処理
//==============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY,int nBulletSetDelay)
{
	// 初期化設定
	CScene2D::Init(pos);
	CScene2D::SetSize(fSizeX, fSizeY);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	CScene2D::SetObjType(objType);

	// 弾発射パターンの設定
	SetBulletPattern();

	// 発射間隔の設定
	m_nBulletSetDelay = nBulletSetDelay;

	// 状態設定
	m_state = ENEMYSTATE_NORMAL;

	return S_OK;
}

//==============================================================================
// 敵の終了処理
//==============================================================================
void CEnemy::Uninit(void)
{
	// 総数の減少
	m_nNumEnemy--;

	CScene2D::Uninit();
}

//==============================================================================
// 敵の更新処理
//==============================================================================
void CEnemy::Update(void)
{
	// キャラ情報の取得
	D3DXVECTOR3 posEnemy = CScene2D::GetPosition();
	float fSizeCharaX = CScene2D::GetSize().x;
	float fSizeCharaY = CScene2D::GetSize().y;

	// 移動処理
	posEnemy += m_Move;

	if (m_type == ENEMY_TYPE_MOTHER)
	{
		CEnemy::MoveMother(posEnemy);
	}

	CEnemyMother::MoveParts();

	// 弾発射カウント
	m_nBulletCnt++;

	// 弾の自動発射
	if ( m_nBulletSetDelay != 0 && m_nBulletCnt % m_nBulletSetDelay == 0)
	{	
		AutoSetBullet(posEnemy, m_pattern);
	}

	// 弾との当たり判定
	CollisionBullet(posEnemy, fSizeCharaX, fSizeCharaY);

	// 画面横端接触による移動の折り返し	
	if (m_bUnderSet == false)
	{// X軸の判定有効
		if (posEnemy.x < fSizeCharaX || posEnemy.x > SCREEN_WIDTH - fSizeCharaX)
		{
			m_Move.x *= -1;
		}

		if (CGame::GetMotherEnemy() == true)
		{
			if (m_type == ENEMY_TYPE_1)
			{
				if (posEnemy.x < fSizeCharaX + 10.0f || posEnemy.x > SCREEN_WIDTH - fSizeCharaX - 10.0f)
				{
					m_Move.x = 0.0f;
					m_Move.y = 1.0f;
				}
			}
		}
	}
	else
	{// 折り返し無効の場合、特定位置で降下
		if (m_Move.x > 0 && posEnemy.x >= 1240.0f)
		{// 右移動の場合
			m_Move = D3DXVECTOR3(0.0f, 4.0f, 0.0f);
		}
		else if (m_Move.x < 0 && posEnemy.x <= 40.0f)
		{// 左移動の場合
			m_Move = D3DXVECTOR3(0.0f, 4.0f, 0.0f);
		}		
	}

	// ステータス状態
	EnemyState();

	// 位置の設定
	CScene2D::SetPosition(posEnemy);

	// 更新
	CScene2D::Update();
}

//==============================================================================
// 敵の描画処理
//==============================================================================
void CEnemy::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// 敵の生成処理
//==============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY, CTexture::TEXTYPE type, D3DXVECTOR3 move,int nLife,int nBulletSetDelay,BULLET_PATTERN pattern,bool bUnder,ENEMY_TYPE enemyType)
{
	// ローカル変数宣言
	CEnemy *pEnemy;
	pEnemy = new CEnemy;

	// 弾発射パターンの一時保存
	m_patternSet = pattern;

	// 初期化
	if (pEnemy != NULL)
	{
		pEnemy->Init(pos, objType, fSizeX, fSizeY,nBulletSetDelay);
	}

	// テクスチャ割り当て
	pEnemy->BindTexture(type);

	// キャラクターパラメータ
	pEnemy->SetMove(move);
	pEnemy->SetLife(CHARA_TYPE_ENEMY,nLife);
	pEnemy->CharaSetType(CHARA_TYPE_ENEMY);

	pEnemy->m_bUnderSet = bUnder;
	pEnemy->m_type = enemyType;

	return pEnemy;
}

//==============================================================================
// 個別自動生成処理
//==============================================================================
void CEnemy::AutoSetCreate(void)
{
	// ランダム設置カウント基準値
	int nSetMin = 0;
	int nSetDif = 0;

	float fSpeed = 0.0f;

	switch (CTitleDifficulty::GetDifficulty())
	{
	case CTitleDifficulty::DIFFICULTY_EASY:
		nSetMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_MIN_EASY);
		nSetDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_DIFFERENCE_EASY);
		fSpeed = CTextDataEnemy::SetSpeed(CTitleDifficulty::DIFFICULTY_EASY);
		break;

	case CTitleDifficulty::DIFFICULTY_NORMAL:
		nSetMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_MIN_NORMAL);
		nSetDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_DIFFERENCE_NORMAL);
		fSpeed = CTextDataEnemy::SetSpeed(CTitleDifficulty::DIFFICULTY_NORMAL);
		break;

	case CTitleDifficulty::DIFFICULTY_HARD:
		nSetMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_MIN_HARD);
		nSetDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_DIFFERENCE_HARD);
		fSpeed = CTextDataEnemy::SetSpeed(CTitleDifficulty::DIFFICULTY_HARD);
		break;
	}

	// 設置カウントの増加
	m_nSetEnemyCnt++;

	// 敵の自動生成
	if (m_nSetEnemyCnt % nSetMin == 0)
	{
		// 配置するX軸の設定
		float fRandX = (float)(rand() % 1200 + 60);

		// 生成する敵のサイズ
		float fSizeX;
		float fSizeY;

		ENEMY_TYPE enemyType;

		// 敵の種類設定
		int nType = rand() % 4;

		fSpeed -= (float)nType / 10.0f;

		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeed, 0.0f);

		// 弾ランダム生成カウント基準値
		int nSetBulletMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_MIN);
		int nSetBulletDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_DIFFERENCE);

		// 弾の発射間隔
		int nBulletCntSet = rand() % nSetBulletDif + nSetBulletMin;

		// 敵のクリエイト
		switch (nType)
		{// サイズの分類
		case 0:
			fSizeX = ENEMYX_0;
			fSizeY = ENEMYY_0;
			enemyType = ENEMY_TYPE_0;
			break;

		case 1:
			fSizeX = ENEMYX_1;
			fSizeY = ENEMYY_1;
			enemyType = ENEMY_TYPE_1;
			break;

		case 2:
			fSizeX = ENEMYX_2;
			fSizeY = ENEMYY_2;
			enemyType = ENEMY_TYPE_2;
			break;

		case 3:
			fSizeX = ENEMYX_3;
			fSizeY = ENEMYY_3;
			enemyType = ENEMY_TYPE_3;
			break;
		}

		// 敵の生成
		CEnemy::Create(D3DXVECTOR3(fRandX, ENEMY_SET_POS_Y, 0.0f),
			CScene::OBJTYPE_ENEMY,
			fSizeX, fSizeY, (CTexture::TEXTYPE)(CTexture::TEXTYPE_ENEMY_0 + nType),
			move, nType + 1, nBulletCntSet, BULLET_PATTERN_NORMAL, false,enemyType);

	}
}

//==============================================================================
// 被ダメージの処理
//==============================================================================
void CEnemy::DamageLife(int nDamage,D3DXVECTOR3 pos)
{
	// ダメージ計算
	m_nLife[CHARA_TYPE_ENEMY] -= nDamage;

	// 結果
	if (m_nLife[CHARA_TYPE_ENEMY] <= 0)
	{// 撃破
		// 撃破ステータスへ
		SetState(ENEMYSTATE_DEATH);
	}
	else
	{// 被ダメージ
		// サウンド再生
		CSound::Play(CSound::SOUND_LABEL_SE_EXPLOSION000);

		// 被ダメージステータスへ
		SetState(ENEMYSTATE_DAMAGE);
	}
}

//==============================================================================
// STATE毎の処理
//==============================================================================
void CEnemy::EnemyState(void)
{
	// 敵の位置
	D3DXVECTOR3 posEnemy = GetPosition();

	m_nStateCnt++;

	switch (m_state)
	{
	case ENEMYSTATE_NORMAL:	// 通常時
		CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_nStateCnt = 0;
		break;

	case ENEMYSTATE_DAMAGE:	// 被ダメージ
		if (m_nStateCnt <= 9)
		{
			CScene2D::SetCol(D3DXCOLOR(0.9f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			m_state = ENEMYSTATE_NORMAL;
			m_nStateCnt = 0;
		}
		break;

	case ENEMYSTATE_DEATH:	// ライフ喪失		
		m_nStateCnt = 0;

		// 爆発音
		CSound::Play(CSound::SOUND_LABEL_SE_EXPLOSION000);

		// スコアの加算
		if (m_type == ENEMY_TYPE_MOTHER)
		{// 母艦の撃破
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{
				CEffect::Create(posEnemy, 0.08f, D3DXCOLOR(1.0f, 0.0f, 0.5f, 1.0f), 20.0f, 0.01f, 0);
			}

			CScore::AddScore(30000);
			
			CEnemyMother::SetMotherState(CEnemyMother::MOTHER_STATE_END);
		}
		else
		{// 他敵の撃破
			CScore::AddScore(10000);

			// 撃破数の加算
			CDefeat::AddDefeat();

			// アイテム生成
			CItem::RandCreate(posEnemy);

			for (int nCnt = 0; nCnt < 50; nCnt++)
			{
				CEffect::Create(posEnemy, 0.05f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, 0.02f, 0);
			}
		}
		
		Uninit();
		break;

	default:
		break;
	}
}

//==============================================================================
// 敵の弾自動発射
//==============================================================================
void CEnemy::AutoSetBullet(D3DXVECTOR3 pos,BULLET_PATTERN pattern)
{
	// 発射する方向
	switch (pattern)
	{
	case BULLET_PATTERN_NONE:	//	撃たない
		break;

	case BULLET_PATTERN_NORMAL:	// 真下へ撃つ
		CBullet::Create(pos, CScene::OBJTYPE_BULLET, 
			BULLET_A_X, BULLET_A_Y, D3DXVECTOR3(0.0f, 5.0f, 0.0f), 
			CTexture::TEXTYPE_BULLET_ENEMY, CBullet::BULLET_SIDE_ENEMY,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
		break;

	case BULLET_PATTERN_AIM:	// プレイヤーを狙う
		// プレイヤーの位置取得
		D3DXVECTOR3 posPlayer = CScene::GetScene(PRIORITY_CHARA)->GetPosition();

		// プレイヤーへの方向
		D3DXVECTOR3 vecPlayer = posPlayer - pos;

		// 長さを求める
		float fLength = (float)sqrt((vecPlayer.x * vecPlayer.x + vecPlayer.y * vecPlayer.y));

		// 角度
		float fAngle = atan2f(vecPlayer.x, vecPlayer.y);

		// 長さを調整
		vecPlayer.x = vecPlayer.x / fLength;
		vecPlayer.y = vecPlayer.y / fLength;

		// 速さを指定
		vecPlayer.x *= BULLET_AIM_SPEED;
		vecPlayer.y *= BULLET_AIM_SPEED;

		CBullet::Create(pos, CScene::OBJTYPE_BULLET,
			BULLET_A_X, BULLET_A_Y, vecPlayer,
			CTexture::TEXTYPE_BULLET_ENEMY, CBullet::BULLET_SIDE_ENEMY,
			D3DXVECTOR3(0.0f, 0.0f, fAngle), 1);
		break;
	}
}

//==============================================================================
// 残存する敵の全破棄
//==============================================================================
void CEnemy::UninitAll(void)
{
	// ローカル変数宣言
	CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
	CScene *pSceneNext = NULL;

	// 全ての敵の死亡フラグ建て
	while (pScene)
	{
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene->GetObjType() == OBJTYPE_ENEMY)
		{
			pScene->Uninit();
		}

		pScene = pSceneNext;
	}
}

//==============================================================================
// 母艦の挙動
//==============================================================================
void CEnemy::MoveMother(D3DXVECTOR3 pos)
{
	// 母艦の出現状態
	CEnemyMother::MOTHER_STATE state = CEnemyMother::GetMotherState();

	// 状態により変化
	switch (state)
	{
	case CEnemyMother::MOTHER_STATE_NONE:
		// 警告の終了
		if (CCaution::GetCautionState() == CCaution::CAUTION_STATE_ALERT_END)
		{
			// 状態の変化
			CEnemyMother::SetMotherState(CEnemyMother::MOTHER_STATE_START);
		}

		break;

	case CEnemyMother::MOTHER_STATE_START:
		// 移動力の設定
		m_Move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		if (pos.y >= MOTHER_POS_Y)
		{
			// 状態の変化
			m_Move = D3DXVECTOR3(MOTHER_MOVE_0, 0.0f, 0.0f);			
			CEnemyFormation::SetParamReset();	// 隊列情報のリセット
			CEnemyMother::SetMotherState(CEnemyMother::MOTHER_STATE_ACTIVE);
		}
		break;

	case CEnemyMother::MOTHER_STATE_ACTIVE:
		if (m_nLife[CHARA_TYPE_ENEMY] >= CEnemyMother::GetLife() * 3.0f / 4.0f)
		{
			// 生成インターバル
			CEnemyFormation::SetInterval(100);
			CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_MOTHER_0);
		}
		else if (m_nLife[CHARA_TYPE_ENEMY] >= CEnemyMother::GetLife() / 2.0f)
		{// 体力50%まで
			// フェーズシフト
			if (CEnemyMother::GetPhase() == CEnemyMother::MOTHER_PHASE_0)
			{
				CEnemyMother::SetPhase(CEnemyMother::MOTHER_PHASE_1);

				if (m_Move.x > 0)
				{
					m_Move.x = MOTHER_MOVE_1;
				}
				else
				{
					m_Move.x = -MOTHER_MOVE_1;
				}
			}

			CEnemyFormation::SetInterval(110);
			CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_MOTHER_1);
		}
		else if (m_nLife[CHARA_TYPE_ENEMY] >= CEnemyMother::GetLife() / 4.0f)
		{// 体力25%まで
		 // フェーズシフト
			if (CEnemyMother::GetPhase() == CEnemyMother::MOTHER_PHASE_1)
			{
				CEnemyMother::SetPhase(CEnemyMother::MOTHER_PHASE_2);

				// 移動力の加速
				if (m_Move.x > 0)
				{
					m_Move.x = MOTHER_MOVE_2;
				}
				else
				{
					m_Move.x = -MOTHER_MOVE_2;
				}
			}

			// インターバルの設定
			CEnemyFormation::SetInterval(120);
			CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_MOTHER_2);
		}
		else
		{// 体力25%以降
		 // フェーズシフト
			if (CEnemyMother::GetPhase() == CEnemyMother::MOTHER_PHASE_2)
			{
				CEnemyMother::SetPhase(CEnemyMother::MOTHER_PHASE_3);
			}

			CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
			CScene *pSceneNext = NULL;

			while (pScene)
			{
				pSceneNext = CScene::GetSceneNext(pScene);

				if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{
					CEnemy *pEnemy = (CEnemy*)pScene;

					if (pEnemy->GetEnemyType() == ENEMY_TYPE_MOTHER)
					{
						if (pEnemy->GetPosition().x < 650.0f && pEnemy->GetPosition().x > 630.0f)
						{
							pEnemy->SetPosition(D3DXVECTOR3(640.0f, pEnemy->GetPosition().y, 0.0f));
							pEnemy->m_Move.x = 0.0f;
						}					
					}
				}

				pScene = pSceneNext;
			}

			// インターバルの設定
			CEnemyFormation::SetInterval(130);
			CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_MOTHER_PARTS);
		}

		break;
	}
}

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
void CEnemyFormation::SetFormationEnemy(D3DXVECTOR3 pos, OBJTYPE objType, CTexture::TEXTYPE type, D3DXVECTOR3 moveRot, float fMove, int nSetNum, int nInterval,TEAM_PATTERN team)
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
						CEffect::Create(D3DXVECTOR3(posLeft.x, posLeft.y + 10.0f ,0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_LEFT);
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
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 5, m_nInterval,team);

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
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 3, m_nInterval,team);

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
		m_moveRot = D3DXVECTOR3(sinf(D3DX_PI / 2),cosf(0.0f),0.0f);

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
		0, CEnemy::BULLET_PATTERN_NONE, false,ENEMY_TYPE_MOTHER_PARTS_LEFT);
	
	// 右
	CEnemy::Create(D3DXVECTOR3(pos.x + 120.0f, pos.y, 0.0f), CScene::OBJTYPE_ENEMY, MOTHER_PARTS_SIZE, MOTHER_PARTS_SIZE,
		CTexture::TEXTYPE_MOTHER_PARTS,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		nLifeParts,
		0, CEnemy::BULLET_PATTERN_NONE, false,ENEMY_TYPE_MOTHER_PARTS_RIGHT);

	// 中央
	CEnemy::Create(D3DXVECTOR3(pos.x, pos.y + 80.0f, 0.0f), CScene::OBJTYPE_ENEMY, MOTHER_PARTS_SIZE, MOTHER_PARTS_SIZE,
		CTexture::TEXTYPE_MOTHER_PARTS,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		nLifeParts,
		0, CEnemy::BULLET_PATTERN_NONE, false,ENEMY_TYPE_MOTHER_PARTS_CENTER);
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