#pragma once

#include "IObject.h"
#include "Vector2.h"

#include "Input.h"

class BlockEffect;

#pragma region 読み込んだテクスチャ

enum BlockTexture {
	kNormal,
	kPixel
};

#pragma endregion

// Plyaerクラス
class Block : public IObject {
public:
	// 仮想関数
	Block();
	~Block() override;

	// 初期化関数
	void Initialize() override;

	// 更新関数
	void Update() override;

	// 描画関数
	void Draw() override;

	// 衝突検知関数
	void OnCollision() override;

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void AddlyGlobalConfigs() override;

	bool IsDead() const { return isDead_; }

	// worldTransformBaseに位置をSet
	void SetPosition(const Vector2 pos);

	// 当たり判定
	void Collider();

private:
	BlockEffect* blockEffect_ = nullptr;
	//BlockEffect* blockEffect_;
	Input* input_ = nullptr;
	bool isDead_ = false;
};
