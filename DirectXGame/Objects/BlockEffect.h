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

	void ChangeState(BlockState behavior);

	bool GetFlage() { return spawnFlage_; }

	void SetIsUse(uint16_t isUse) { sprites_[kNormal][0]->isUse_ = isUse; }

	void SetIsActive(bool isActive) { isActive_ = isActive; }

private:
	struct BlockPixel {
		Vector2 position;
		Vector2 velocity;
	};
	// 振る舞い
	BlockState blockEffectBehavior_ = BlockState::kSpawn;
	// 振る舞いリクエスト
	std::optional<BlockState> blockEffectBehaviorRequest_ = std::nullopt;

	uint32_t effectTimer = 0;
	BlockPixel pixel_[6];
	float effectSize_ = 0.0f;
	float degree_ = 0.0f;
	const float rotateSpeed_ = 2.25f;
	bool isActive_ = true;
	bool spawnFlage_ = false; 

};
