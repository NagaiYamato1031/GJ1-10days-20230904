#pragma once

#include "Scenes/IScene.h"

class SceneResult : public IScene {
public:
	// コンストクラタ
	SceneResult();

	// デストラクタ
	~SceneResult();

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

private:
};
