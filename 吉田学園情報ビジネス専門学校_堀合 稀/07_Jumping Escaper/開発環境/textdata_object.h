//=============================================================================
//
// テキストデータ（3Dオブジェクト）管理 [textdata_object.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTDATA_OBJECT_H_
#define _TEXTDATA_OBJECT_H_

#include "main.h"
#include "textdata.h"
#include "object.h"

//==============================================================================
// 配置オブジェクト管理クラス
//==============================================================================
class CTextDataObject : public CTextData
{
public:
	CTextDataObject();
	~CTextDataObject();

	void LoadData(STAGEINFO stage);
	void Unload(void);

	static CTextDataObject *Create(STAGEINFO stage);	// 生成

	char *GetFileName(int nType) { return m_pFileObjectName[nType]; }					// ファイル名の取得
	int GetNumObject(void) { return m_nNumObject; }										// オブジェクト数の取得
	int GetTypeObject(int nIdx) { return m_pType[nIdx]; }								// 種類の取得

	D3DXVECTOR3 GetPosition(int nIdx) { return m_pPos[nIdx]; }							// 位置
	D3DXVECTOR3 GetRotation(int nIdx) { return m_pRot[nIdx]; }							// 角度
	D3DXVECTOR2 GetShadowSize(int nIdx) { return m_pShadowSize[nIdx]; }					// 影のサイズ

	CObject::OBJATTRIBUTE GetObjAttribute(int nIdx) { return m_pObjAttribute[nIdx]; }	// オブジェクトの属性

	CObject::MOVETYPE GetMoveType(int nIdx) { return m_pMoveType[nIdx]; }				// 移動パターン
	float GetMovePower(int nIdx) { return m_pMovePower[nIdx]; }

private:
	int m_nNumFile;							// ファイル名の総数
	char **m_pFileObjectName;				// ファイル名ポインタ
	char **m_pFileObjectNameLoad;			// 読み取り用ファイル名変数

	int *m_pType;							// 種類
	D3DXVECTOR3 *m_pPos;					// 位置
	D3DXVECTOR3 *m_pRot;					// 角度
	D3DXVECTOR2 *m_pShadowSize;				// 影のサイズ

	CObject::OBJATTRIBUTE *m_pObjAttribute;	// オブジェクトの種類

	CObject::MOVETYPE *m_pMoveType;			// 移動方法
	float *m_pMovePower;					// 移動力の割合

	int m_nNumObject;						// オブジェクトの総数
};
#endif