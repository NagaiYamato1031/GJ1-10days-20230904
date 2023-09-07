#pragma once

#include "Input.h"

#include "Scenes/IScene.h"


class ScenePlay : public IScene {
public:
	// コンストクラタ
	ScenePlay();

	// デストラクタ
	~ScenePlay();

	// 初期化関数
	void Initialize() override;

	// 更新関数
	void Update(GameScene* gameScene) override;

	// 描画関数
	// 背景スプライト
	void DrawBackdrop() override;
	// 3D
	void Draw3D() override;
	// 前景スプライト
	void DrawOverlay() override;

protected:

	// オブジェクト管理
	ObjectManager* objectManager_ = nullptr;

	Input* input_ = nullptr;

};
