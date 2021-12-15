//=============================================================================
//
// カメラの設定 [camera.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

#define CAMERA_TURN	(D3DX_PI / 100.0f)	// カメラ旋回速度
#define CAMERA_SPDOWN	(0.1f)			// カメラの減衰係数
#define CAMERA_MOVE		(5.0f)

//==============================================================================
// カメラクラス
//==============================================================================
class CCamera
{
public:
	//typedef enum
	//{
	//	CAMERATYPE_EDIT = 0,	// 編集モード
	//	CAMERATYPE_PREVIEW,		// プレビュー
	//	//CAMERATYPE_FPS,		// キャラクターの1人称視点
	//	CAMERATYPE_MAX
	//}CAMERATYPE;

	CCamera();
	~CCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);

	static CCamera *Create(void);

	void SetCamera(void);
	D3DXVECTOR3 GetRotation(void) { return m_rot; }	// 向きの取得

	D3DXVECTOR3 GetPositionR(void) { return m_posR; }	// 注視点
	D3DXVECTOR3 GetPositionV(void) { return m_posV; }	// 視点

	float GetLength(void) { return m_fLength; }
	float GetGroundLength(void) { return m_fGroundLength; }

	void SetInit(void);

	void ControlCamera(void);

private:
	D3DXMATRIX m_mtxView;		// ビューマトリックス
	D3DXMATRIX m_mtxProjection;	// プロジェクションマトリックス

	D3DXVECTOR3 m_posV;			// 視点位置
	D3DXVECTOR3 m_posR;			// 注視点位置
	D3DXVECTOR3 m_vecU;			// 上方向ベクトル
	
	D3DXVECTOR3 m_rot;			// 向き

	float m_fLength;			// 視点間の距離
	float m_fGroundLength;		// 視点(高さ0)と注視点間の距離

	float m_fFront;				// キャラの前方距離

	float m_fHeightR;			// 注視点高さ
	float m_fHeightV;			// 視点高さ

	D3DXVECTOR3 m_posVDest;		// 目的の視点
	D3DXVECTOR3 m_posRDest;		// 目的の注視点

	D3DXVECTOR3 m_vecInterval;	// 視点、注視点のベクトル
};

#endif