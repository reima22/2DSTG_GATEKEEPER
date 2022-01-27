//=============================================================================
//
// 3Dオブジェクト管理 [object.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "scene.h"
//#include "scene3D.h"

// マクロ定義
#define PHANTOM_OBJ	(D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.5f))		// 幻影オブジェクトの色

// 前方宣言
class CModel;			// モデルクラス
class CShadow;			// 影クラス
class CObjectSccaford;	// 足場オブジェクトクラス

//==============================================================================
// オブジェクト管理クラス
//==============================================================================
class CObject : public CScene
{
public:
	// オブジェクトの属性
	typedef enum
	{
		OBJATTRIBUTE_NONE = 0,			// 指定なし
		OBJATTRIBUTE_SCAFFORD,			// 足場
		OBJATTRIBUTE_SAVE,				// 中間地点
		OBJATTRIBUTE_START,				// スタート地点
		OBJATTRIBUTE_GOAL,				// ゴール地点
		OBJATTRIBUTE_PHANTOM,			// 当たり判定無し(幻影)
		OBJATTRIBUTE_SCAFFORD_NODROP,	// 落下しない足場
		OBJATTRIBUTE_SCAFFORD_TUTO,		// チュートリアル用落ちても復活する
		OBJATTRIBUTE_MAX
	}OBJATTRIBUTE;

	// オブジェクトの移動の種類
	typedef enum
	{
		MOVETYPE_NONE = 0,	// 移動無し
		MOVETYPE_SIDE,		// 横移動
		MOVETYPE_DEPTH,		// 前後移動
		MOVETYPE_UPDOWN,	// 上下移動
		MOVETYPE_MAX
	}MOVETYPE;

	CObject();
	~CObject();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CObject *Create(
		D3DXVECTOR3 pos,
		D3DXVECTOR3 rot,
		char *pFileName,
		D3DXVECTOR2 shadowSize,
		MOVETYPE moveType,
		float movePower,
		OBJATTRIBUTE objAttribute);

	// テキストデータからの読み込み
	HRESULT InitFromData(void);
	static CObject *CreateFromData(int nIdx);

	void MoveObject(void);		// オブジェクトの移動
	void DropObject(void);		// オブジェクトの落下
	void ResetObject(void);		// オブジェクトの再配置

	void PhantomObject(void);	// オブジェクトの幻影化

	// インデックスの取得・設定
	int GetIdx(void) { return m_nIdx; }
	void SetIdx(int nIdx) { m_nIdx = nIdx; }

	// 位置の取得・設定
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }

	// 角度の取得・設定
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }

	// 種類の取得・設定
	int GetType(void) { return m_nType; }
	void SetType(int nType) { m_nType = nType; }

	// 影のサイズ設定
	void SetShadowSize(D3DXVECTOR2 shadowSize) { m_shadowSize = shadowSize; }

	// モデルの取得
	CModel *GetModel(void) { return m_pModel; }

	// ファイル名パスの取得・設定
	char *GetFileName(void) { return m_pFileName; }
	void SetFileName(char *pFileName) { m_pFileName = pFileName; }
	
	// オブジェクトの属性の取得・設定
	OBJATTRIBUTE GetObjAttribute(void) { return m_objAttribute; }
	void SetObjAttribute(OBJATTRIBUTE objAttribute) { m_objAttribute = objAttribute; }

	// 移動パターンの取得設定
	MOVETYPE GetMoveType(void) { return m_moveType; }
	void SetMoveType(MOVETYPE moveType) { m_moveType = moveType; }

	// 移動方向の取得・設定
	D3DXVECTOR3 GetMoveRot(void) { return m_moveRot; }
	void SetMoveRot(D3DXVECTOR3 moveRot) { m_moveRot = moveRot; }

	// 移動力の取得・設定
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }

	// 移動力の割合の取得・設定
	float SetMovePower(void) { return m_fMovePower; }
	void SetMovePower(float fMovePower) { m_fMovePower = fMovePower; }

	// 落下判定
	bool GetDropOut(void) { return m_bDropOut; }
	void SetDropOut(bool bDropOut) { m_bDropOut = bDropOut; }

	// 一度踏んだ判定
	bool GetBonus(void) { return m_bBonus; }
	void SetBonus(bool bBonus) { m_bBonus = bBonus; }

private:
	int m_nIdx;						// オブジェクトのインデックス
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3 m_pos;				// 位置
	D3DXVECTOR3 m_rot;				// 向き

	int m_nType;					// 種類

	char *m_pFileName;				// ファイル名パスのポインタ
	CModel *m_pModel;				// モデルポインタ

	MOVETYPE m_moveType;			// 移動パターン
	D3DXVECTOR3 m_move;				// 移動力
	D3DXVECTOR3 m_moveRot;			// 移動力の角度
	float m_fMovePower;				// 移動力の割合

	OBJATTRIBUTE m_objAttribute;	// オブジェクトの種類

	CShadow *m_pShadow;				// 影ポインタ
	D3DXVECTOR2 m_shadowSize;		// 影の大きさ

	bool m_bDropOut;				// 落下判定
	int m_nDropCnt;					// 落下までのカウント

	bool m_bBonus;					// ボーナス取得用踏んだ判定
};
#endif