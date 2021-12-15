//=============================================================================
//
// モデル管理 [model.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//==============================================================================
// テクスチャ管理クラス
//==============================================================================
class CModel
{
public:
	// テクスチャの種類
	typedef enum
	{
		MODELTYPE_METEOR = 0,	// 隕石
		MODELTYPE_DEBRI_0,		// 宇宙ゴミ0
		MODELTYPE_DEBRI_1,		// 宇宙ゴミ1
		MODELTYPE_MAX			// 最大数
	}MODELTYPE;

	CModel();
	~CModel();

	static char *GetFileName(int nType) { return m_pFileName[nType]; }
	static void UnloadAll(void);

private:
	static char *m_pFileName[MODELTYPE_MAX];	// 読み込むファイル名パス
};

#endif