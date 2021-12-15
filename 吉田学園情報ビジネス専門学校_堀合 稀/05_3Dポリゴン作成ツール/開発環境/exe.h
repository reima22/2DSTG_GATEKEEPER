//=============================================================================
//
// 実行処理 [exe.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _EXE_H_
#define _EXE_H_

#include "main.h"
#include "scene.h"

// 前方宣言
class CMeshfield;	// メッシュフィールドクラス

//==============================================================================
// 実行クラス
//==============================================================================
class CExe : public CScene
{
public:
	CExe();
	~CExe();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 生成処理
	static CExe *Create(void);

	static CMeshfield *GetMeshField(void) { return m_pMeshField; }

private:
	int m_nNumObject;						// オブジェクト数
	static CMeshfield *m_pMeshField;		// オリジナルメッシュ
};

#endif