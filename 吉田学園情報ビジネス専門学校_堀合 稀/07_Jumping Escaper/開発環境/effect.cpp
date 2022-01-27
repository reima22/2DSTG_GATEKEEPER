//==============================================================================
//
// エフェクト描画処理〔effect.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CEffect::CEffect(int nPriority) : CScene(nPriority)
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CEffect::~CEffect()
{

}

//==============================================================================
// エフェクトの初期化処理
//==============================================================================
HRESULT CEffect::Init(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 各頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// テクスチャ頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================
// エフェクトの描画の終了処理
//==============================================================================
void CEffect::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	Release();
}

//==============================================================================
// エフェクトの描画の更新処理
//==============================================================================
void CEffect::Update(void)
{  
	// 頂点情報の設定
	SetVertex();

	// エフェクトの挙動
	m_col.a -= m_fAlphaDown;
	m_pos += m_move;
}

//==============================================================================
// エフェクトの描画の描画処理
//==============================================================================
void CEffect::Draw(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイスのポインタ
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxTrans;	// 計算用マトリックス

	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// フォグを無効化
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// 加算合成によるアルファブレンディング
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zテストの更新
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,						// 描画を開始する頂点インデックス
		2);						// 描画するプリミティブ数

	// 通常のアルファブレンディング
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 元に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// フォグを無効化
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
}

//==============================================================================
// エフェクトの生成
//==============================================================================
CEffect *CEffect::Create(void)
{
	// ローカル変数宣言
	CEffect *pEffect;
	pEffect = new CEffect;

	// 初期化
	if (pEffect != NULL)
	{
		pEffect->Init();
	}

	// 返し値
	return pEffect;
}

//==============================================================================
// 頂点情報の設定
//==============================================================================
void CEffect::SetVertex(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-m_size.x, +m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(+m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, +m_size.y, 0.0f);

	// 各頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==============================================================================
// テクスチャの情報設定
//==============================================================================
void CEffect::SetTexture(char *pText)
{
	// ローカル変数宣言
	CTexture *pTexture = CManager::GetTexture();

	// テクスチャの割り当て
	m_pTexture = pTexture->GetAddress(pText);
}

/*砂煙エフェクト*/

//==============================================================================
// コンストラクタ
//==============================================================================
CEffectDetail::CEffectDetail(int nPriority) : CEffect(nPriority)
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CEffectDetail::~CEffectDetail()
{

}

//==============================================================================
// エフェクト詳細の初期化処理
//==============================================================================
HRESULT CEffectDetail::Init(void)
{
	// 生成数
	switch (m_effectType)
	{
	case EFFECTTYPE_SANDSMOKE:

		m_nNumEffect = EFFECT_NUM_SANDSMOKE;

		break;

	case EFFECTTYPE_MAXJUMP:

		m_nNumEffect = EFFECT_NUM_SANDSMOKE;

		break;

	case EFFECTTYPE_LANDING:
		
		m_nNumEffect = EFFECT_NUM_LANDING;

		break;

	case EFFECTTYPE_DIVE_WATER:
		
		m_nNumEffect = EFFECT_NUM_WATER;

		break;

	case EFFECTTYPE_DIVE_LAVA:

		m_nNumEffect = EFFECT_NUM_LAVA;

		break;
	}

	// 動的確保
	if (m_ppEffect == NULL)
	{
		m_ppEffect = new CEffect*[m_nNumEffect];
	}

	D3DXVECTOR3 move = VECTOR3_NULL;

	// エフェクト生成
	switch (m_effectType)
	{
	case EFFECTTYPE_SANDSMOKE:

		// エフェクト生成
		m_ppEffect[0] = CEffect::Create();
		m_ppEffect[0]->SetPosition(m_posMain);
		m_ppEffect[0]->SetMove(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		m_ppEffect[0]->SetSize(D3DXVECTOR2(1.0f, 1.0f));
		m_ppEffect[0]->SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		m_ppEffect[0]->SetTexture("SMOKE");
		m_ppEffect[0]->SetAlphaDown(0.02f);

		break;

	case EFFECTTYPE_LANDING:

		// エフェクト生成
		for (int nCntEffect = 0; nCntEffect < m_nNumEffect; nCntEffect++)
		{
			m_ppEffect[nCntEffect] = CEffect::Create();
			m_ppEffect[nCntEffect]->SetPosition(m_posMain);

			move.x = sinf((D3DX_PI * 2.0f / m_nNumEffect) * nCntEffect) * 5.0f;
			move.z = cosf((D3DX_PI * 2.0f / m_nNumEffect) * nCntEffect) * 5.0f;

			m_ppEffect[nCntEffect]->SetMove(move);
			m_ppEffect[nCntEffect]->SetSize(D3DXVECTOR2(2.0f, 2.0f));
			m_ppEffect[nCntEffect]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_ppEffect[nCntEffect]->SetTexture("SMOKE");
			m_ppEffect[nCntEffect]->SetAlphaDown(0.04f);
		}

		break;

	case EFFECTTYPE_DIVE_WATER:

		// エフェクト生成
		for (int nCntEffect = 0; nCntEffect < m_nNumEffect; nCntEffect++)
		{
			m_ppEffect[nCntEffect] = CEffect::Create();
			m_ppEffect[nCntEffect]->SetPosition(m_posMain);

			move.x = sinf((D3DX_PI * 2.0f / m_nNumEffect) * nCntEffect) * (rand() % 5 + 1);
			move.z = cosf((D3DX_PI * 2.0f / m_nNumEffect) * nCntEffect) * (rand() % 5 + 1);

			move.y = (float)(rand() % 50 + 1) / 10;

			m_ppEffect[nCntEffect]->SetMove(move);
			m_ppEffect[nCntEffect]->SetSize(D3DXVECTOR2(10.0f, 10.0f));
			m_ppEffect[nCntEffect]->SetColor(D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f));
			m_ppEffect[nCntEffect]->SetTexture("EFFECT");
			m_ppEffect[nCntEffect]->SetAlphaDown(0.03f);
		}
		break;

	case EFFECTTYPE_DIVE_LAVA:
		
		// エフェクト生成
		m_ppEffect[0] = CEffect::Create();
		m_ppEffect[0]->SetPosition(m_posMain);
		m_ppEffect[0]->SetMove(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		m_ppEffect[0]->SetSize(D3DXVECTOR2(20.0f, 20.0f));
		m_ppEffect[0]->SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		m_ppEffect[0]->SetTexture("SMOKE");
		m_ppEffect[0]->SetAlphaDown(0.01f);

		break;

	case EFFECTTYPE_MAXJUMP:

		// エフェクト生成
		m_ppEffect[0] = CEffect::Create();
		m_ppEffect[0]->SetPosition(m_posMain);
		m_ppEffect[0]->SetMove(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		m_ppEffect[0]->SetSize(D3DXVECTOR2(1.0f, 1.0f));
		m_ppEffect[0]->SetColor(D3DXCOLOR(0.1f, 1.0f, 0.1f, 1.0f));
		m_ppEffect[0]->SetTexture("EFFECT");
		m_ppEffect[0]->SetAlphaDown(0.02f);

		break;

	case EFFECTTYPE_GAMECLEAR:

		// エフェクト生成
		m_pClearEffect = CScene2D::Create();
		float fX = (float)(rand() % 1200 + 40);

		m_pClearEffect->SetPosition(D3DXVECTOR3(fX, -50.0f, 0.0f));
		m_pClearEffect->SetSize(D3DXVECTOR2(30.0f, 30.0f));
		m_pClearEffect->BindTexture("EFFECT");
		m_pClearEffect->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
		m_pClearEffect->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		m_pClearEffect->AlphaBlend();
		m_pClearEffect->ZbuffBlend();

		break;


	}

	return S_OK;
}

//==============================================================================
// エフェクト詳細の終了処理
//==============================================================================
void CEffectDetail::Uninit(void)
{
	// エフェクトの破棄・開放
	for (int nCntEffect = 0; nCntEffect < m_nNumEffect; nCntEffect++)
	{
		if (m_ppEffect[nCntEffect] != NULL)
		{
			m_ppEffect[nCntEffect]->Uninit();
		}
	}

	// 確保の開放
	if (m_nNumEffect > 1)
	{
		delete[] m_ppEffect;
	}
	else
	{
		delete m_ppEffect;
	}

	// 破棄
	m_ppEffect = NULL;

	// クリアエフェクト
	if (m_pClearEffect != NULL)
	{
		m_pClearEffect->Uninit();
		m_pClearEffect = NULL;
	}

	Release();
}

//==============================================================================
// エフェクト詳細の更新処理
//==============================================================================
void CEffectDetail::Update(void)
{
	// クリアエフェクトの破棄
	if (m_pClearEffect != NULL && m_effectType == EFFECTTYPE_GAMECLEAR)
	{
		D3DXVECTOR3 pos = m_pClearEffect->GetPosition();
		pos += D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		m_pClearEffect->SetPosition(pos);

		if (m_pClearEffect->GetPosition().y >= SCREEN_HEIGHT)
		{
			Uninit();
		}
	}

	// 破棄条件
	for (int nCntEffect = 0; nCntEffect < m_nNumEffect; nCntEffect++)
	{
		if (m_ppEffect[nCntEffect] != NULL)
		{
			// 破棄条件
			if (m_ppEffect[nCntEffect]->GetColor().a <= 0.0f)
			{
				Uninit();
				return;
			}
		}
	}

	// 各エフェクトの仕様
	if (m_effectType == EFFECTTYPE_DIVE_WATER)
	{
		D3DXVECTOR3 move = VECTOR3_NULL;

		for (int nCntEffect = 0; nCntEffect < m_nNumEffect; nCntEffect++)
		{
			if (m_ppEffect[nCntEffect] != NULL)
			{
				move = m_ppEffect[nCntEffect]->GetMove();

				move.y -= 0.2f;

				m_ppEffect[nCntEffect]->SetMove(move);
			}
		}
	}
	else if(m_effectType == EFFECTTYPE_LANDING || m_effectType == EFFECTTYPE_SANDSMOKE || m_effectType == EFFECTTYPE_MAXJUMP)
	{
		D3DXVECTOR2 size = VECTOR2_NULL;

		for (int nCntEffect = 0; nCntEffect < m_nNumEffect; nCntEffect++)
		{
			if (m_ppEffect[nCntEffect] != NULL)
			{
				size = m_ppEffect[nCntEffect]->GetSize();

				size += D3DXVECTOR2(0.5f, 0.5f);

				m_ppEffect[nCntEffect]->SetSize(size);
			}
		}
	}
	else if (m_effectType == EFFECTTYPE_DIVE_LAVA)
	{
		D3DXVECTOR2 size = VECTOR2_NULL;

		size = m_ppEffect[0]->GetSize();

		size += D3DXVECTOR2(0.5f, 0.5f);

		m_ppEffect[0]->SetSize(size);
	}

	// 着地エフェクト
	if (m_effectType == EFFECTTYPE_LANDING)
	{
		for (int nCntEffect = 0; nCntEffect < m_nNumEffect; nCntEffect++)
		{
			D3DXVECTOR3 move = m_ppEffect[nCntEffect]->GetMove();

			// 減速処理
			move.x += (0.0f - move.x) * EFFECT_SPEED_DOWN;
			move.z += (0.0f - move.z) * EFFECT_SPEED_DOWN;

			m_ppEffect[nCntEffect]->SetMove(move);
		}		
	}
}

//==============================================================================
// エフェクト詳細の描画処理
//==============================================================================
void CEffectDetail::Draw(void)
{
}

//==============================================================================
// エフェクト詳細の生成
//==============================================================================
CEffectDetail *CEffectDetail::Create(EFFECTTYPE effectType, D3DXVECTOR3 pos)
{
	// ローカル変数宣言
	CEffectDetail *pEffectDetail;
	pEffectDetail = new CEffectDetail;

	// 初期化
	if (pEffectDetail != NULL)
	{
		pEffectDetail->SetType(effectType);
		pEffectDetail->SetPositionMain(D3DXVECTOR3(pos.x,pos.y + 10.0f,pos.z));
		pEffectDetail->Init();
	}

	// 返し値
	return pEffectDetail;
}