//==============================================================================
//
// 3Dオブジェクト管理〔object.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "camera.h"
#include "game.h"
#include "main.h"
#include "manager.h"
#include "model.h"
#include "object.h"
#include "player.h"
#include "renderer.h"
#include "select.h"
#include "shadow.h"
#include "textdata.h"
#include "textdata_object.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CObject::CObject()
{
	m_bDropOut = false;
	m_nDropCnt = 0;
}

//==============================================================================
// デストラクタ
//==============================================================================
CObject::~CObject()
{

}

//==============================================================================
// 3Dオブジェクトの初期化処理
//==============================================================================
HRESULT CObject::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// オブジェクトタイプ
	SetObjType(OBJTYPE_OBJECT);

	// モデルの生成
	m_pModel = CModel::Create(m_pFileName, m_pos, m_rot);

	// 一度踏んだ判定
	m_bBonus = false;

	// 影の生成
	if (m_shadowSize == VECTOR2_NULL)
	{
		m_pShadow = CShadow::Create(m_shadowSize);
	}
	else
	{
		m_pShadow = NULL;
	}

	return S_OK;
}

//==============================================================================
// 3Dオブジェクトの終了処理
//==============================================================================
void CObject::Uninit(void)
{
	// モデル情報の破棄
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		m_pModel = NULL;
	}

	// ファイル名の破棄
	if (m_pFileName != NULL)
	{
		m_pFileName = NULL;
	}
}

//==============================================================================
// 3Dオブジェクトの更新処理
//==============================================================================
void CObject::Update(void)
{	
	// データテキストの取得
	CStageSelect::STAGE_SELECT select = CStageSelect::GetSelectingStage();

	// 読み取りデータポインタ
	CTextDataObject *pDataObject = CTextData::GetDataObject(select);

	// 幻影オブジェクトの設定
	PhantomObject();

	// モデルポインタ
	if (m_pModel != NULL)
	{
		// 移動処理
		if (m_bDropOut == false)
		{
			if (m_moveType != MOVETYPE_NONE)
			{
				m_pModel->Update();

				// 移動処理
				MoveObject();
			}
		}
		else
		{
			m_pModel->Update();

			if (m_nDropCnt < CNT_ONE_SECOND)
			{
				// 移動処理
				MoveObject();
			}

			DropObject();
		}
	}

	// 挙動処理
	m_pos += m_move;
	m_pModel->SetPosition(m_pos);
	m_pModel->SetMove(m_move);

	// ゲームオーバー時、全ての落下
	if (CGame::GetOverPhase() != CGame::GAMEOVERPHASE_NONE && (m_objAttribute == OBJATTRIBUTE_SCAFFORD || m_objAttribute == OBJATTRIBUTE_SAVE || m_objAttribute == OBJATTRIBUTE_SCAFFORD_NODROP))
	{
		m_bDropOut = true;
	}

	// チュートリアル足場の処理、落ちた足場は自動で復活する
	if (m_objAttribute == OBJATTRIBUTE_SCAFFORD_TUTO && m_bDropOut == true && m_pos.y < -1000.0f)
	{
		m_bDropOut = false;
		m_nDropCnt = 0;
		m_pos = pDataObject->GetPosition(m_nIdx);
		m_move = VECTOR3_NULL;
	}

	// 影の追従
	if (m_pShadow != NULL)
	{
		m_pShadow->SetPosition(D3DXVECTOR3(m_pos.x, m_pos.y + 1.0f, m_pos.z));
	}
}

//==============================================================================
// 3Dオブジェクトの描画処理
//==============================================================================
void CObject::Draw(void)
{
	D3DXVECTOR3 posPlayer;
	posPlayer = CGame::GetPlayer()->GetPosition();

	// 距離
	D3DXVECTOR3 posDistance = m_pos - posPlayer;

	// 描画距離の補正
	if (posDistance.x < 0)
	{
		posDistance.x *= -1;
	}

	if (posDistance.z < 0)
	{
		posDistance.z *= -1;
	}

	//if (posDistance.x < 500.0f || posDistance.z < 500.0f)
	{
		// ローカル変数宣言
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
		D3DXMATRIX mtxRot, mtxTrans;										// 計算用マトリックス
		D3DMATERIAL9 matDef;												// 現在のマテリアル保存用

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// モデルの描画
		if (m_pModel != NULL)
		{
			m_pModel->Draw();
		}

		// 影の追従
		if (m_pShadow != NULL)
		{
			m_pShadow->Draw();
		}
	}
}

//==============================================================================
// 生成処理
//==============================================================================
CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pFileName, D3DXVECTOR2 shadowSize, MOVETYPE moveType, float fMovePower, OBJATTRIBUTE objAttribute)
{
	// ローカル変数宣言
	CObject *pObject;
	pObject = new CObject;	// インスタンス

	// 初期化
	if (pObject != NULL)
	{
		// 情報の設定
		pObject->SetPosition(pos);				// 位置
		pObject->SetRotation(rot);				// 角度
		pObject->SetFileName(pFileName);		// 読み込むファイル名
		pObject->SetShadowSize(shadowSize);		// 影の大きさ
		pObject->SetMoveType(moveType);			// 移動タイプ
		pObject->SetMovePower(fMovePower);		// 移動力
		pObject->SetObjAttribute(objAttribute);	// オブジェクトの属性

		pObject->Init();
	}

	return pObject;
}

//==============================================================================
// 3Dオブジェクトの初期化処理(テキストデータ)
//==============================================================================
HRESULT CObject::InitFromData(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// オブジェクトタイプ
	SetObjType(OBJTYPE_OBJECT);

	// データテキストの取得
	CStageSelect::STAGE_SELECT select = CStageSelect::GetSelectingStage();

	// 読み取りデータポインタ
	CTextDataObject *pDataObject = CTextData::GetDataObject(select);

	// データの取得
	m_pos = pDataObject->GetPosition(m_nIdx);				// 位置
	m_rot = pDataObject->GetRotation(m_nIdx);				// 角度
	m_nType = pDataObject->GetTypeObject(m_nIdx);			// オブジェクトの種類
	m_shadowSize = pDataObject->GetShadowSize(m_nIdx);		// 影のサイズ
	m_moveType = pDataObject->GetMoveType(m_nIdx);			// 移動の方法
	m_fMovePower = pDataObject->GetMovePower(m_nIdx);		// 移動力
	m_pFileName = pDataObject->GetFileName(m_nType);		// オブジェクトのファイル名
	m_objAttribute = pDataObject->GetObjAttribute(m_nIdx);	// オブジェクトの属性

	// モデルの生成
	m_pModel = CModel::Create(m_pFileName, m_pos, m_rot);

	// 影の生成
	if (m_shadowSize == VECTOR2_NULL)
	{
		m_pShadow = CShadow::Create(m_shadowSize);
	}
	else
	{
		m_pShadow = NULL;
	}

	return S_OK;
}

//==============================================================================
// テキストデータから生成
//==============================================================================
CObject *CObject::CreateFromData(int nIdx)
{
	// ローカル変数宣言
	CObject *pObject;
	pObject = new CObject;	// インスタンス

	// 初期化
	if (pObject != NULL)
	{
		// 情報の設定
		pObject->SetIdx(nIdx);		// インデックス

		pObject->InitFromData();
	}

	return pObject;
}

//==============================================================================
// オブジェクトの移動
//==============================================================================
void CObject::MoveObject(void)
{
	// 移動の種類
	if (m_moveType != MOVETYPE_NONE)
	{
		switch (m_moveType)
		{
		case MOVETYPE_SIDE:

			m_moveRot.y += 0.01f;

			if (m_moveRot.y > D3DX_PI)
			{
				m_moveRot.y -= PI_DEFAULT;
			}

			m_move = D3DXVECTOR3(cosf(m_moveRot.y) * m_fMovePower, 0.0f, 0.0f);

			break;

		case MOVETYPE_DEPTH:	// 前後移動
			m_moveRot.y += 0.01f;

			if (m_moveRot.y > D3DX_PI)
			{
				m_moveRot.y -= PI_DEFAULT;
			}

			m_move = D3DXVECTOR3(0.0f, 0.0f, cosf(m_moveRot.y) * m_fMovePower);

			break;

		case MOVETYPE_UPDOWN:	// 前後移動
			m_moveRot.y += 0.01f;

			if (m_moveRot.y > D3DX_PI)
			{
				m_moveRot.y -= PI_DEFAULT;
			}

			m_move = D3DXVECTOR3(0.0f, cosf(m_moveRot.y) * m_fMovePower, 0.0f);

			break;
		}
	}
}

//==============================================================================
// オブジェクトの落下
//==============================================================================
void CObject::DropObject(void)
{
	// SEの再生
	if (m_nDropCnt <= 0)
	{
		m_pSound->Play(CSound::SOUND_LABEL_SE_EARTHQUAKE);
	}
	else if (m_nDropCnt == CNT_ONE_SECOND / 2)
	{
		m_pSound->Play(CSound::SOUND_LABEL_SE_FALL_SCCAFORD);
	}

	// カウント
	m_nDropCnt++;

	// 落下挙動
	if (m_nDropCnt > 0 && m_nDropCnt < CNT_ONE_SECOND / 2)
	{
		// 振動
		float fRandom = (float)(rand() % 628 - 314) / 100;

		if (m_moveType == MOVETYPE_NONE)
		{
			m_move.x = sinf(fRandom);
			m_move.z = cosf(fRandom);
		}
		else
		{
			m_move.x += sinf(fRandom);
			m_move.z += cosf(fRandom);
		}
	}
	else
	{
		// 落下
		m_move.x = 0.0f;
		m_move.z = 0.0f;
		m_move.y -= 0.05f;
	}
}

//==============================================================================
// オブジェクトの復活
//==============================================================================
void CObject::ResetObject(void)
{
	// データテキストの取得
	CStageSelect::STAGE_SELECT select = CStageSelect::GetSelectingStage();

	// 読み取りデータポインタ
	CTextDataObject *pDataObject = CTextData::GetDataObject(select);

	// 落下したものの復活
	if (m_bDropOut == true || pDataObject->GetMovePower(m_nIdx) != 0)
	{
		m_pos = pDataObject->GetPosition(m_nIdx);
		m_move = VECTOR3_NULL;
		m_pModel->SetPosition(m_pos);
		m_pModel->SetMove(m_move);
		m_nDropCnt = 0;
		m_bDropOut = false;
		m_moveRot = VECTOR3_NULL;
	}
}

//==============================================================================
// 幻影オブジェクトの設定
//==============================================================================
void CObject::PhantomObject(void)
{
	// 影オブジェクトの半透明化
	if (m_objAttribute == OBJATTRIBUTE_PHANTOM && m_pModel != NULL)
	{
		m_pModel->SetCol(PHANTOM_OBJ);
	}
}