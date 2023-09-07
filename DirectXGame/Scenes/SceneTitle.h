#pragma once
#include "IScene.h"

#include "Input.h"

#include "./Objects/ObjectManager.h"

class SceneTitle : public IScene {
public:

	~SceneTitle() override;

	// 初期化
	void Initialize() override;
	// 更新
	void Update(GameScene* gameScene) override;
	// 描画
	void DrawBackdrop() override;
	void Draw3D() override;
	void DrawOverlay() override;

private:
	// シーン内の変数
	Input* input_ = nullptr;

	
};
