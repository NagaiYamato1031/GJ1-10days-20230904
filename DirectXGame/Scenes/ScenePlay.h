#pragma once

#include "Scenes/IScene.h"

#include <vector>
#include "TextureManager.h"
#include "Sprite.h"


#include "Objects/Player.h"
#include "Objects/Block.h"
#include "Objects/BlockEffect.h"
#include "Objects/Score.h"

#include <optional>

#include "Objects/Result.h"

enum class ScenePlayState { 
	kPlay,
	kResult,
};

class ScenePlay : public IScene {
public:
	// コンストクラタ
	ScenePlay();

	// デストラクタ
	~ScenePlay();

	// 初期化関数
	void Initialize(GameScene* gameScene) override;

	// 更新関数
	void Update() override;

	// 描画関数
	// 背景スプライト
	void DrawBackdrop() override;
	// 3D
	void Draw3D() override;
	// 前景スプライト
	void DrawOverlay() override;

	// 衝突判定
	void CheckAllCollision() override;

	void BlockSqawn();

	void AddlyConfigs() override;


private:
	std::unique_ptr<Sprite> backGround_;
	std::unique_ptr<Player> player_;
	std::vector<std::unique_ptr<BlockEffect>> blockEffects_;
	Score* score_;

	int32_t kLimitTimeFrame_ = 1800;
	int32_t limitTimeFrame_ = 0;

	// フェーズ変更
	int32_t phaseChangeAudioHandle_ = 0;

	std::unique_ptr<Result> result_;
	std::unique_ptr<Block> resultBlock_;

	// 振る舞い
	ScenePlayState ScenePlayBehavior_ = ScenePlayState::kPlay;
	// 振る舞いリクエスト
	std::optional<ScenePlayState> ScenePlayBehaviorRequest_ = std::nullopt;
};
