#pragma once

#include "Scenes/IScene.h"

#include <vector>
#include "TextureManager.h"
#include "Sprite.h"


#include "Objects/Player.h"
#include "Objects/Block.h"
#include "Objects/BlockEffect.h"
#include "Objects/Score.h"

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

	void SetLoadFileName(const std::string& name) { fileName_ = name; }

private:
	std::unique_ptr<Sprite> backGround_;
	std::unique_ptr<Player> player_;
	std::vector<std::unique_ptr<BlockEffect>> blockEffects_;
	Score* score_;

	std::string fileName_;
};
