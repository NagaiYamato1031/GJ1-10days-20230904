#pragma once

#include "IObject.h"

#include "Vector2.h"

#include "Input.h"

#include <optional>

#include "Block.h"


enum class BlockState { 
	kNone,
	kSpawn,
	kBreak,
	kVanish
};

// Plyaerクラス
class BlockEffect : public IObject {
public:
	// 仮想関数
	BlockEffect();
	~BlockEffect() override;

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

	// worldTransformBaseに位置をSet
	void SetPosition(const Vector2 pos);

	// 出現のエフェクト
	void SpawnEffectInitialize();
	void SpawnEffect();

	// 破壊のエフェクト
	void BreakEffectInitialize();
	void BreakEffect();

	// 消滅のエフェクト
	void VanishEffectInitialize();
	void VanishEffect();

	// 振る舞いを変える
	void ChangeState(BlockState behavior);

	void SetSpritePosition(const Vector2& position);

	bool GetCom() { return com_; }

private:
	struct BlockPixel {
		Vector2 position;
		Vector2 velocity;
	};
	// 振る舞い
	BlockState blockEffectBehavior_ = BlockState::kNone;
	// 振る舞いリクエスト
	std::optional<BlockState> blockEffectBehaviorRequest_ = std::nullopt;

	// 破壊された時の破片
	BlockPixel pixel_[6];

	float effectSize_ = 0.0f;
	float degree_ = 0.0f;
	const float rotateSpeed_ = 2.25f;
	bool com_ = false;
};
