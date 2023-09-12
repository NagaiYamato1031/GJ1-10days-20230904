#pragma once
#include "IScene.h"

#include <memory>
#include "TextureManager.h"
#include "Sprite.h"

#include "Objects/Block.h"
#include "Objects/Player.h"

class SceneSelect : public IScene {
public:

	~SceneSelect() override;

	// 初期化
	void Initialize(GameScene* gameScene) override;
	// 更新
	void Update() override;
	// 描画
	void DrawBackdrop() override;
	void Draw3D() override;
	void DrawOverlay() override;

	// 衝突判定
	void CheckAllCollision() override;

	void AddlyConfigs();

private:
	std::unique_ptr<Sprite> backGround_;

	std::unique_ptr<Block> blockStageOne_;
	std::unique_ptr<Block> blockStageTwo_;
	std::unique_ptr<Block> blockStageThree_;

	std::vector<Block*> blocks_;

	std::unique_ptr<Player> player_;

};
