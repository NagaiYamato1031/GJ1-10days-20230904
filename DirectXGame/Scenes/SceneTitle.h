#pragma once
#include "IScene.h"

#include <memory>
#include "TextureManager.h"
#include "Sprite.h"

class SceneTitle : public IScene {
public:

	~SceneTitle() override;

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

private:
	

	std::unique_ptr<Sprite> backGround_;
	std::unique_ptr<Sprite> titleLogo_;

};
